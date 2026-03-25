/**
 * audio_mixer.c - Software RSP audio mixer for Paper Mario PC port
 *
 * Replaces N64 RSP audio microcode with CPU-based implementations.
 * Ported from Ghostship (SM64 PC port) mixer.c and adapted for
 * Paper Mario's n_abi audio command format.
 *
 * Key differences from Ghostship:
 * - PM64 uses n_* prefixed macros that pack all parameters into single commands
 *   (no aSetBuffer calls to set in/out/nbytes)
 * - PM64's n_aSetVolume has different flag semantics than SM64
 * - PM64's n_aEnvMixer takes cvolR parameter directly
 * - PM64's n_aInterleave uses hardcoded DMEM addresses
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Audio constants from Paper Mario */
#define AUDIO_SAMPLES 184

/* DMEM address layout for Paper Mario */
#define N_AL_DECODER_IN     0x000
#define N_AL_RESAMPLER_OUT  0x000
#define N_AL_TEMP_0         0x000
#define N_AL_DECODER_OUT    0x170
#define N_AL_TEMP_1         0x170
#define N_AL_TEMP_2         0x2E0
#define N_AL_MAIN_L_OUT     0x4E0
#define N_AL_MAIN_R_OUT     0x650
#define N_AL_AUX_L_OUT      0x7C0
#define N_AL_AUX_R_OUT      0x930

/* ABI flag constants */
#define A_INIT      0x01
#define A_CONTINUE  0x00
#define A_LOOP      0x02
#define A_LEFT      0x02
#define A_RIGHT     0x00
#define A_VOL       0x04
#define A_RATE      0x00
#define A_AUX       0x08

/* NEON SIMD support for Apple Silicon */
#ifdef __ARM_NEON
#include <arm_neon.h>
/* NEON disabled: n_aResampleImpl uses vqrdmulhq_s16 (int16 saturating multiply)
 * instead of int32 accumulation like the scalar path, causing premature clipping.
 * aMixImpl_pm64 NEON uses a different formula than scalar (out + round(in*gain/32768)
 * vs (out*0x7FFF + in*gain) >> 15). ADPCM decoder NEON is correct but disabled
 * for consistency. TODO: Fix NEON paths to match scalar precision. */
#define HAS_NEON 0
#else
#define HAS_NEON 0
#endif

#ifdef __SSE4_1__
#include <immintrin.h>
#define HAS_SSE41 1
#else
#define HAS_SSE41 0
#endif

#pragma GCC optimize ("unroll-loops")

#define ROUND_UP_64(v) (((v) + 63) & ~63)
#define ROUND_UP_32(v) (((v) + 31) & ~31)
#define ROUND_UP_16(v) (((v) + 15) & ~15)
#define ROUND_UP_8(v)  (((v) + 7) & ~7)
#define ROUND_DOWN_16(v) ((v) & ~0xf)

/* ================================================================
 * Simulated RSP DMEM buffer
 * ================================================================ */

#define DMEM_SIZE 3072  /* 0xC00 — enough for PM64's DMEM layout */

#define BUF_U8(a)  (rspa.buf.as_u8 + (a))
#define BUF_S16(a) (rspa.buf.as_s16 + (a) / sizeof(int16_t))

static struct {
    /* Volume/envelope state (set by n_aSetVolume, used by n_aEnvMixer) */
    int16_t vol[2];         /* Current volume: [0]=left, [1]=right */
    int16_t target[2];      /* Target volume: [0]=left, [1]=right */
    int32_t rate[2];        /* Volume ramp rate: [0]=left, [1]=right */
    int16_t vol_dry;        /* Dry mix amount */
    int16_t vol_wet;        /* Wet mix amount */

    /* ADPCM state */
    int16_t *adpcm_loop_state;
    int16_t adpcm_table[8][2][8];

    /* Simulated DMEM */
    union {
        int16_t as_s16[DMEM_SIZE / sizeof(int16_t)];
        uint8_t as_u8[DMEM_SIZE];
    } buf;
} rspa;

/* ================================================================
 * Resample table (copied from Ghostship / N64 SDK)
 * ================================================================ */

static int16_t resample_table[64][4] = {
    {0x0c39, 0x66ad, 0x0d46, 0xffdf}, {0x0b39, 0x6696, 0x0e5f, 0xffd8},
    {0x0a44, 0x6669, 0x0f83, 0xffd0}, {0x095a, 0x6626, 0x10b4, 0xffc8},
    {0x087d, 0x65cd, 0x11f0, 0xffbf}, {0x07ab, 0x655e, 0x1338, 0xffb6},
    {0x06e4, 0x64d9, 0x148c, 0xffac}, {0x0628, 0x643f, 0x15eb, 0xffa1},
    {0x0577, 0x638f, 0x1756, 0xff96}, {0x04d1, 0x62cb, 0x18cb, 0xff8a},
    {0x0435, 0x61f3, 0x1a4c, 0xff7e}, {0x03a4, 0x6106, 0x1bd7, 0xff71},
    {0x031c, 0x6007, 0x1d6c, 0xff64}, {0x029f, 0x5ef5, 0x1f0b, 0xff56},
    {0x022a, 0x5dd0, 0x20b3, 0xff48}, {0x01be, 0x5c9a, 0x2264, 0xff3a},
    {0x015b, 0x5b53, 0x241e, 0xff2c}, {0x0101, 0x59fc, 0x25e0, 0xff1e},
    {0x00ae, 0x5896, 0x27a9, 0xff10}, {0x0063, 0x5720, 0x297a, 0xff02},
    {0x001f, 0x559d, 0x2b50, 0xfef4}, {0xffe2, 0x540d, 0x2d2c, 0xfee8},
    {0xffac, 0x5270, 0x2f0d, 0xfedb}, {0xff7c, 0x50c7, 0x30f3, 0xfed0},
    {0xff53, 0x4f14, 0x32dc, 0xfec6}, {0xff2e, 0x4d57, 0x34c8, 0xfebd},
    {0xff0f, 0x4b91, 0x36b6, 0xfeb6}, {0xfef5, 0x49c2, 0x38a5, 0xfeb0},
    {0xfedf, 0x47ed, 0x3a95, 0xfeac}, {0xfece, 0x4611, 0x3c85, 0xfeab},
    {0xfec0, 0x4430, 0x3e74, 0xfeac}, {0xfeb6, 0x424a, 0x4060, 0xfeaf},
    {0xfeaf, 0x4060, 0x424a, 0xfeb6}, {0xfeac, 0x3e74, 0x4430, 0xfec0},
    {0xfeab, 0x3c85, 0x4611, 0xfece}, {0xfeac, 0x3a95, 0x47ed, 0xfedf},
    {0xfeb0, 0x38a5, 0x49c2, 0xfef5}, {0xfeb6, 0x36b6, 0x4b91, 0xff0f},
    {0xfebd, 0x34c8, 0x4d57, 0xff2e}, {0xfec6, 0x32dc, 0x4f14, 0xff53},
    {0xfed0, 0x30f3, 0x50c7, 0xff7c}, {0xfedb, 0x2f0d, 0x5270, 0xffac},
    {0xfee8, 0x2d2c, 0x540d, 0xffe2}, {0xfef4, 0x2b50, 0x559d, 0x001f},
    {0xff02, 0x297a, 0x5720, 0x0063}, {0xff10, 0x27a9, 0x5896, 0x00ae},
    {0xff1e, 0x25e0, 0x59fc, 0x0101}, {0xff2c, 0x241e, 0x5b53, 0x015b},
    {0xff3a, 0x2264, 0x5c9a, 0x01be}, {0xff48, 0x20b3, 0x5dd0, 0x022a},
    {0xff56, 0x1f0b, 0x5ef5, 0x029f}, {0xff64, 0x1d6c, 0x6007, 0x031c},
    {0xff71, 0x1bd7, 0x6106, 0x03a4}, {0xff7e, 0x1a4c, 0x61f3, 0x0435},
    {0xff8a, 0x18cb, 0x62cb, 0x04d1}, {0xff96, 0x1756, 0x638f, 0x0577},
    {0xffa1, 0x15eb, 0x643f, 0x0628}, {0xffac, 0x148c, 0x64d9, 0x06e4},
    {0xffb6, 0x1338, 0x655e, 0x07ab}, {0xffbf, 0x11f0, 0x65cd, 0x087d},
    {0xffc8, 0x10b4, 0x6626, 0x095a}, {0xffd0, 0x0f83, 0x6669, 0x0a44},
    {0xffd8, 0x0e5f, 0x6696, 0x0b39}, {0xffdf, 0x0d46, 0x66ad, 0x0c39}
};

/* ================================================================
 * Utility functions
 * ================================================================ */

static inline int16_t clamp16(int32_t v) {
    if (v < -0x8000) return -0x8000;
    if (v > 0x7fff) return 0x7fff;
    return (int16_t)v;
}

static inline int32_t clamp32(int64_t v) {
    if (v < -0x7fffffff - 1) return -0x7fffffff - 1;
    if (v > 0x7fffffff) return 0x7fffffff;
    return (int32_t)v;
}

/* ================================================================
 * Standard audio command implementations
 * ================================================================ */

void aClearBufferImpl(uint16_t addr, int nbytes) {
    nbytes = ROUND_UP_16(nbytes);
    if (addr + nbytes <= DMEM_SIZE) {
        memset(BUF_U8(addr), 0, nbytes);
    }
}

void aLoadADPCMImpl(int num_entries_times_16, const int16_t *book_source_addr) {
    if (num_entries_times_16 > (int)sizeof(rspa.adpcm_table)) {
        num_entries_times_16 = sizeof(rspa.adpcm_table);
    }
    memcpy(rspa.adpcm_table, book_source_addr, num_entries_times_16);
    /* Predictor data is already swapped to native order during BK bank loading
     * (bulk swap in au_load_BK_to_bank). No additional swap needed here. */
}

void aDMEMMoveImpl(uint16_t in_addr, uint16_t out_addr, int nbytes) {
    nbytes = ROUND_UP_16(nbytes);
    if (in_addr + nbytes <= DMEM_SIZE && out_addr + nbytes <= DMEM_SIZE) {
        memmove(BUF_U8(out_addr), BUF_U8(in_addr), nbytes);
    }
}

void aSetLoopImpl(int16_t *adpcm_loop_state) {
    /* Loop state is already swapped to native order during BK bank loading
     * (bulk swap in au_load_BK_to_bank). Just store the pointer directly. */
    rspa.adpcm_loop_state = adpcm_loop_state;
}

void aMixImpl_pm64(int16_t gain, uint16_t in_addr, uint16_t out_addr) {
    /* Process exactly AUDIO_SAMPLES — do NOT round up, as that overflows into adjacent
     * DMEM buffers (e.g., L aMix overflows 8 samples into R buffer).
     *
     * N64 RSP formula (confirmed via mupen64plus-rsp-hle naudio MIXER handler):
     *   *dst = clamp_s16(*dst + ((*src * gain) >> 15))
     *
     * This is a pure additive accumulation — the destination is preserved exactly,
     * and only the scaled source is added. The previous Ghostship-derived formula
     * (*out * 0x7FFF + *in * gain + 0x4000) >> 15 differs in two ways:
     *   1. It scales the existing output by 0x7FFF/0x8000 (~0.99997) per call
     *   2. It adds a +0.5 rounding bias
     * These differences are negligible for SM64's simple single-stage reverb, but
     * cause audible crackling in PM64's multi-tap allpass reverb because:
     *   - The lowpass filter in the reverb feedback loop relies on exact accumulation
     *   - The rounding bias injects DC offset into the feedback loop
     *   - The allpass structure's unity-gain property is broken by the scaling
     */
    int count = AUDIO_SAMPLES;
    int16_t *in = BUF_S16(in_addr);
    int16_t *out = BUF_S16(out_addr);
    int i;
    int32_t sample;

    if (gain == -0x8000) {
        for (i = 0; i < count; i++) {
            sample = *out - *in++;
            *out++ = clamp16(sample);
        }
        return;
    }

#if HAS_NEON
    /* Process 8 samples at a time with NEON */
    for (i = 0; i < count - 7; i += 8) {
        int16x8_t ov = vld1q_s16(out);
        int16x8_t iv = vld1q_s16(in);
        ov = vqaddq_s16(ov, vqrdmulhq_n_s16(iv, gain));
        vst1q_s16(out, ov);
        out += 8;
        in += 8;
    }
    /* Handle remaining samples */
    for (; i < count; i++) {
        sample = *out + ((*in++ * gain) >> 15);
        *out++ = clamp16(sample);
    }
#else
    for (i = 0; i < count; i++) {
        sample = *out + ((*in++ * gain) >> 15);
        *out++ = clamp16(sample);
    }
#endif

}

/* ================================================================
 * PM64 n_abi command implementations
 * ================================================================ */

/**
 * n_aLoadBuffer - Load data from DRAM to simulated DMEM
 */
void n_aLoadBufferImpl(uint16_t count, uint16_t dmemDest, const void *dramSrc) {
    count = ROUND_UP_8(count);
    if (dmemDest + count <= DMEM_SIZE && dramSrc != NULL) {
        memcpy(BUF_U8(dmemDest), dramSrc, count);
    }
}

/**
 * n_aSaveBuffer - Save data from simulated DMEM to DRAM
 */
void n_aSaveBufferImpl(uint16_t count, uint16_t dmemSrc, void *dramDest) {
    count = ROUND_UP_8(count);
    if (dmemSrc + count <= DMEM_SIZE && dramDest != NULL) {
        memcpy(dramDest, BUF_U8(dmemSrc), count);
    }
}

/**
 * n_aADPCMdec - Decode ADPCM compressed audio
 *
 * Parameters (from n_abi.h packing):
 *   state          - ADPCM decoder state (DRAM pointer)
 *   flags          - A_INIT (first frame) or A_LOOP (loop point)
 *   sampleByteCount - Number of output PCM bytes (tsam * 2)
 *   alignOffset    - DMA alignment offset into compressed data
 *   output         - DMEM destination for decoded PCM samples
 *
 * Compressed ADPCM data was previously loaded to DMEM[0] by n_aLoadBuffer.
 */
void n_aADPCMdecImpl(void *state_addr, uint8_t flags, uint16_t sampleByteCount,
                      uint8_t alignOffset, uint16_t output)
{
    int16_t *state = (int16_t *)state_addr;
    uint8_t *in = BUF_U8(alignOffset);  /* Skip DMA alignment padding */
    int16_t *out = BUF_S16(output);
    int nbytes = ROUND_UP_32(sampleByteCount);

    if (flags & A_INIT) {
        memset(out, 0, 16 * sizeof(int16_t));
    } else if (flags & A_LOOP) {
        memcpy(out, rspa.adpcm_loop_state, 16 * sizeof(int16_t));
    } else {
        memcpy(out, state, 16 * sizeof(int16_t));
    }
    out += 16;

#if HAS_NEON
    {
        static const int8_t pos0_data[] = {-1, 0, -1, 0, -1, 1, -1, 1, -1, 2, -1, 2, -1, 3, -1, 3};
        static const int8_t pos1_data[] = {-1, 4, -1, 4, -1, 5, -1, 5, -1, 6, -1, 6, -1, 7, -1, 7};
        static const int16_t mult_data[] = {0x01, 0x10, 0x01, 0x10, 0x01, 0x10, 0x01, 0x10};
        static const int16_t table_prefix_data[] = {0, 0, 0, 0, 0, 0, 0, 1 << 11};
        const int8x16_t pos0 = vld1q_s8(pos0_data);
        const int8x16_t pos1 = vld1q_s8(pos1_data);
        const int16x8_t mult = vld1q_s16(mult_data);
        const int16x8_t mask = vdupq_n_s16((int16_t)0xf000);
        const int16x8_t table_prefix = vld1q_s16(table_prefix_data);
        int16x8_t result = vld1q_s16(out - 8);

        while (nbytes > 0) {
            int shift = *in >> 4;
            int table_index = *in++ & 0xf;
            int16_t (*tbl)[8] = rspa.adpcm_table[table_index];
            int i;

            int8x8_t inv = vld1_s8((int8_t *)in);
            int16x8_t tblvec[2] = {vld1q_s16(tbl[0]), vld1q_s16(tbl[1])};
            int16x8_t invec[2] = {
                vreinterpretq_s16_s8(vcombine_s8(vtbl1_s8(inv, vget_low_s8(pos0)),
                                                  vtbl1_s8(inv, vget_high_s8(pos0)))),
                vreinterpretq_s16_s8(vcombine_s8(vtbl1_s8(inv, vget_low_s8(pos1)),
                                                  vtbl1_s8(inv, vget_high_s8(pos1))))
            };
            int16x8_t shiftcount = vdupq_n_s16(shift - 12);
            int16x8_t tblvec1[8];

            in += 8;
            tblvec1[0] = vextq_s16(table_prefix, tblvec[1], 7);
            invec[0] = vmulq_s16(invec[0], mult);
            tblvec1[1] = vextq_s16(table_prefix, tblvec[1], 6);
            invec[1] = vmulq_s16(invec[1], mult);
            tblvec1[2] = vextq_s16(table_prefix, tblvec[1], 5);
            tblvec1[3] = vextq_s16(table_prefix, tblvec[1], 4);
            invec[0] = vandq_s16(invec[0], mask);
            tblvec1[4] = vextq_s16(table_prefix, tblvec[1], 3);
            invec[1] = vandq_s16(invec[1], mask);
            tblvec1[5] = vextq_s16(table_prefix, tblvec[1], 2);
            tblvec1[6] = vextq_s16(table_prefix, tblvec[1], 1);
            invec[0] = vqshlq_s16(invec[0], shiftcount);
            invec[1] = vqshlq_s16(invec[1], shiftcount);
            tblvec1[7] = table_prefix;

            for (i = 0; i < 2; i++) {
                int32x4_t acc0, acc1;

                acc1 = vmull_lane_s16(vget_high_s16(tblvec[0]), vget_high_s16(result), 2);
                acc1 = vmlal_lane_s16(acc1, vget_high_s16(tblvec[1]), vget_high_s16(result), 3);
                acc0 = vmull_lane_s16(vget_low_s16(tblvec[0]), vget_high_s16(result), 2);
                acc0 = vmlal_lane_s16(acc0, vget_low_s16(tblvec[1]), vget_high_s16(result), 3);

                acc0 = vmlal_lane_s16(acc0, vget_low_s16(tblvec1[0]), vget_low_s16(invec[i]), 0);
                acc0 = vmlal_lane_s16(acc0, vget_low_s16(tblvec1[1]), vget_low_s16(invec[i]), 1);
                acc0 = vmlal_lane_s16(acc0, vget_low_s16(tblvec1[2]), vget_low_s16(invec[i]), 2);
                acc0 = vmlal_lane_s16(acc0, vget_low_s16(tblvec1[3]), vget_low_s16(invec[i]), 3);

                acc1 = vmlal_lane_s16(acc1, vget_high_s16(tblvec1[0]), vget_low_s16(invec[i]), 0);
                acc1 = vmlal_lane_s16(acc1, vget_high_s16(tblvec1[1]), vget_low_s16(invec[i]), 1);
                acc1 = vmlal_lane_s16(acc1, vget_high_s16(tblvec1[2]), vget_low_s16(invec[i]), 2);
                acc1 = vmlal_lane_s16(acc1, vget_high_s16(tblvec1[3]), vget_low_s16(invec[i]), 3);
                acc1 = vmlal_lane_s16(acc1, vget_high_s16(tblvec1[4]), vget_high_s16(invec[i]), 0);
                acc1 = vmlal_lane_s16(acc1, vget_high_s16(tblvec1[5]), vget_high_s16(invec[i]), 1);
                acc1 = vmlal_lane_s16(acc1, vget_high_s16(tblvec1[6]), vget_high_s16(invec[i]), 2);
                acc1 = vmlal_lane_s16(acc1, vget_high_s16(tblvec1[7]), vget_high_s16(invec[i]), 3);

                result = vcombine_s16(vqshrn_n_s32(acc0, 11), vqshrn_n_s32(acc1, 11));
                vst1q_s16(out, result);
                out += 8;
            }
            nbytes -= 16 * sizeof(int16_t);
        }
    }
#else
    /* Scalar fallback */
    while (nbytes > 0) {
        int shift = *in >> 4;
        int table_index = *in++ & 0xf;
        int16_t (*tbl)[8] = rspa.adpcm_table[table_index];
        int i, j, k;

        for (i = 0; i < 2; i++) {
            int16_t ins[8];
            int16_t prev1 = out[-1];
            int16_t prev2 = out[-2];
            /* Match mupen64plus adpcm_predict_frame_4bits exactly:
             * nibble is placed at bit 15 of int16, then arithmetic right-shifted.
             * For scale 0-11: rshift = 12-scale (effective left shift = scale).
             * For scale 12-15: rshift = 0 (max magnitude capped at ±2^12).
             * Our old code (nibble << shift) overflowed int16 for scale 13-15. */
            {
                int rshift = (shift < 12) ? 12 - shift : 0;
                for (j = 0; j < 4; j++) {
                    ins[j * 2]     = (int16_t)((uint16_t)(*in & 0xf0) << 8) >> rshift;
                    ins[j * 2 + 1] = (int16_t)((uint16_t)(*in++ & 0x0f) << 12) >> rshift;
                }
            }
            for (j = 0; j < 8; j++) {
                int32_t acc = tbl[0][j] * prev2 + tbl[1][j] * prev1 + (ins[j] << 11);
                for (k = 0; k < j; k++) {
                    acc += tbl[1][((j - k) - 1)] * ins[k];
                }
                acc >>= 11;
                *out++ = clamp16(acc);
            }
        }
        nbytes -= 16 * sizeof(int16_t);
    }
#endif
    if (state != NULL) {
        memcpy(state, out - 16, 16 * sizeof(int16_t));
    }

}

/**
 * n_aResample - Pitch-shift resample audio
 *
 * Reads from DMEM[input], writes to DMEM output buffer.
 * Produces AUDIO_SAMPLES (184) output samples.
 *
 * outputOffset is a packed buffer index (from N64 Acmd command packing where
 * the caller does address >> 8). Standard voice path passes 0 (DMEM[0]).
 * Reverb chorus/flange path passes 1 (= outputTapBuffer at DMEM[0x170]).
 * The N64 RSP reconstructs: address = outputOffset * 2 * AUDIO_SAMPLES.
 */
void n_aResampleImpl(void *state_addr, uint8_t flags, uint16_t pitch,
                      uint16_t input, uint8_t outputOffset)
{
    int16_t *state = (int16_t *)state_addr;
    int16_t tmp[16] = {0};
    int16_t *in_initial = BUF_S16(input);
    int16_t *in = in_initial;
    int16_t *out = BUF_S16(outputOffset * 2 * AUDIO_SAMPLES);
    int nbytes = ROUND_UP_16(AUDIO_SAMPLES * 2);
    uint32_t pitch_accumulator = 0;

    if (flags & A_INIT) {
        memset(tmp, 0, 5 * sizeof(int16_t));
    } else {
        memcpy(tmp, state, 16 * sizeof(int16_t));
    }
    if (flags & 2) {
        memcpy(in - 8, tmp + 8, 8 * sizeof(int16_t));
        in -= tmp[5] / sizeof(int16_t);
    }
    in -= 4;
    pitch_accumulator = (uint16_t)tmp[4];
    memcpy(in, tmp, 4 * sizeof(int16_t));

#if HAS_NEON
    {
        static const uint16_t multiples_data[8] = {0, 2, 4, 6, 8, 10, 12, 14};
        uint16x8_t multiples = vld1q_u16(multiples_data);
        uint32x4_t pitchvec_8_steps = vdupq_n_u32((pitch << 1) * 8);
        uint32x4_t pitchacclo_vec = vdupq_n_u32((uint16_t)pitch_accumulator);
        uint32x4_t acc_a = vmlal_n_u16(pitchacclo_vec, vget_low_u16(multiples), pitch);
        uint32x4_t acc_b = vmlal_n_u16(pitchacclo_vec, vget_high_u16(multiples), pitch);

        do {
            uint16x8x2_t unzipped = vuzpq_u16(vreinterpretq_u16_u32(acc_a), vreinterpretq_u16_u32(acc_b));
            uint16x8_t tbl_positions = vshrq_n_u16(unzipped.val[0], 10);
            uint16x8_t in_positions = unzipped.val[1];
            int16x8_t tbl_entries[4];
            int16x8_t samples[4];
            int16x8x2_t unzipped1, unzipped2;

            tbl_entries[0] = vcombine_s16(vld1_s16(resample_table[vgetq_lane_u16(tbl_positions, 0)]), vld1_s16(resample_table[vgetq_lane_u16(tbl_positions, 1)]));
            tbl_entries[1] = vcombine_s16(vld1_s16(resample_table[vgetq_lane_u16(tbl_positions, 2)]), vld1_s16(resample_table[vgetq_lane_u16(tbl_positions, 3)]));
            tbl_entries[2] = vcombine_s16(vld1_s16(resample_table[vgetq_lane_u16(tbl_positions, 4)]), vld1_s16(resample_table[vgetq_lane_u16(tbl_positions, 5)]));
            tbl_entries[3] = vcombine_s16(vld1_s16(resample_table[vgetq_lane_u16(tbl_positions, 6)]), vld1_s16(resample_table[vgetq_lane_u16(tbl_positions, 7)]));
            samples[0] = vcombine_s16(vld1_s16(&in[vgetq_lane_u16(in_positions, 0)]), vld1_s16(&in[vgetq_lane_u16(in_positions, 1)]));
            samples[1] = vcombine_s16(vld1_s16(&in[vgetq_lane_u16(in_positions, 2)]), vld1_s16(&in[vgetq_lane_u16(in_positions, 3)]));
            samples[2] = vcombine_s16(vld1_s16(&in[vgetq_lane_u16(in_positions, 4)]), vld1_s16(&in[vgetq_lane_u16(in_positions, 5)]));
            samples[3] = vcombine_s16(vld1_s16(&in[vgetq_lane_u16(in_positions, 6)]), vld1_s16(&in[vgetq_lane_u16(in_positions, 7)]));
            samples[0] = vqrdmulhq_s16(samples[0], tbl_entries[0]);
            samples[1] = vqrdmulhq_s16(samples[1], tbl_entries[1]);
            samples[2] = vqrdmulhq_s16(samples[2], tbl_entries[2]);
            samples[3] = vqrdmulhq_s16(samples[3], tbl_entries[3]);

            unzipped1 = vuzpq_s16(samples[0], samples[1]);
            unzipped2 = vuzpq_s16(samples[2], samples[3]);
            samples[0] = vqaddq_s16(unzipped1.val[0], unzipped1.val[1]);
            samples[1] = vqaddq_s16(unzipped2.val[0], unzipped2.val[1]);
            unzipped1 = vuzpq_s16(samples[0], samples[1]);
            samples[0] = vqaddq_s16(unzipped1.val[0], unzipped1.val[1]);

            vst1q_s16(out, samples[0]);

            acc_a = vaddq_u32(acc_a, pitchvec_8_steps);
            acc_b = vaddq_u32(acc_b, pitchvec_8_steps);
            out += 8;
            nbytes -= 8 * sizeof(int16_t);
        } while (nbytes > 0);
        in += vgetq_lane_u16(vreinterpretq_u16_u32(acc_a), 1);
        pitch_accumulator = vgetq_lane_u16(vreinterpretq_u16_u32(acc_a), 0);
    }
#else
    /* Scalar fallback */
    {
        int16_t *tbl;
        int32_t sample;
        int i;
        do {
            for (i = 0; i < 8; i++) {
                tbl = resample_table[pitch_accumulator * 64 >> 16];
                /* N64 RSP accumulates all 4 taps as int32 then shifts once.
                 * Per-tap rounding ((tap + 0x4000) >> 15) introduces error. */
                sample = ((int32_t)in[0] * tbl[0] +
                          (int32_t)in[1] * tbl[1] +
                          (int32_t)in[2] * tbl[2] +
                          (int32_t)in[3] * tbl[3]) >> 15;
                *out++ = clamp16(sample);

                pitch_accumulator += (pitch << 1);
                in += pitch_accumulator >> 16;
                pitch_accumulator %= 0x10000;
            }
            nbytes -= 8 * sizeof(int16_t);
        } while (nbytes > 0);
    }
#endif

    state[4] = (int16_t)pitch_accumulator;
    memcpy(state, in, 4 * sizeof(int16_t));
    {
        int i = (in - in_initial + 4) & 7;
        in -= i;
        if (i != 0) {
            i = -8 - i;
        }
        state[5] = i;
        memcpy(state + 8, in, 8 * sizeof(int16_t));
    }

}

/**
 * n_aSetVolume - Set envelope mixer volume parameters
 *
 * PM64 flag semantics (different from SM64!):
 *   A_RATE (0x00):           Set LEFT target and LEFT ramp rate
 *   A_VOL|A_LEFT (0x06):    Set LEFT current volume, dry amount, wet amount
 *   A_VOL|A_RIGHT (0x04):   Set RIGHT target and RIGHT ramp rate
 */
void n_aSetVolumeImpl(uint8_t flags, int16_t v, int16_t t, int16_t r) {
    if (flags == A_RATE) {
        /* Set left target and left ramp rate */
        rspa.target[0] = v;
        rspa.rate[0] = (int32_t)((uint16_t)t << 16 | (uint16_t)r);
    } else if ((flags & A_VOL) && (flags & A_LEFT)) {
        /* Set left current volume, dry/wet amounts */
        rspa.vol[0] = v;
        rspa.vol_dry = t;
        rspa.vol_wet = r;
    } else if (flags & A_VOL) {
        /* Set right target and right ramp rate */
        rspa.target[1] = v;
        rspa.rate[1] = (int32_t)((uint16_t)t << 16 | (uint16_t)r);
    }
}

/**
 * n_aEnvMixer - Envelope mixer with volume ramping
 *
 * Reads resampled audio from DMEM[0], mixes into:
 *   - Dry: N_AL_MAIN_L_OUT, N_AL_MAIN_R_OUT
 *   - Wet: N_AL_AUX_L_OUT, N_AL_AUX_R_OUT
 *
 * cvolR parameter provides the right channel current volume (on A_INIT).
 *
 * This is a direct 1:1 port of mupen64plus-rsp-hle alist_envmix_lin().
 * PM64 uses the naudio RSP ucode which has LINEAR volume ramping
 * (vol += step per sample), NOT the exponential ramping (vol *= rate)
 * used by SM64's audio ucode.
 *
 * State layout matches mupen64plus (int16_t[40] = 80 bytes):
 *   [0]: wet, [2]: dry, [4]: target_L, [6]: target_R,
 *   [8..9]: step_L (int32), [10..11]: step_R (int32),
 *   [16..17]: value_L (int32), [18..19]: value_R (int32)
 */
void n_aEnvMixerImpl(uint8_t flags, int16_t cvolR, void *state_addr) {
    int16_t *save = (int16_t *)state_addr;
    int16_t *in = BUF_S16(0);  /* N_AL_RESAMPLER_OUT */
    int16_t *dl = BUF_S16(N_AL_MAIN_L_OUT);
    int16_t *dr = BUF_S16(N_AL_MAIN_R_OUT);
    int16_t *wl = BUF_S16(N_AL_AUX_L_OUT);
    int16_t *wr = BUF_S16(N_AL_AUX_R_OUT);
    int count = ROUND_UP_16(AUDIO_SAMPLES * 2) >> 1; /* bytes to samples */

    int16_t vol_dry, vol_wet;
    int64_t ramp_value[2], ramp_step[2], ramp_target[2];
    int k;

    if (flags & A_INIT) {
        rspa.vol[1] = cvolR;

        ramp_step[0]   = rspa.rate[0] / 8;
        ramp_value[0]  = (int64_t)rspa.vol[0] << 16;
        ramp_target[0] = (int64_t)rspa.target[0] << 16;

        ramp_step[1]   = rspa.rate[1] / 8;
        ramp_value[1]  = (int64_t)rspa.vol[1] << 16;
        ramp_target[1] = (int64_t)rspa.target[1] << 16;

        vol_dry = rspa.vol_dry;
        vol_wet = rspa.vol_wet;
    } else {
        vol_wet        = *(int16_t *)(save +  0);
        vol_dry        = *(int16_t *)(save +  2);
        ramp_target[0] = (int64_t)*(int16_t *)(save +  4) << 16;
        ramp_target[1] = (int64_t)*(int16_t *)(save +  6) << 16;
        ramp_step[0]   = *(int32_t *)(save +  8);
        ramp_step[1]   = *(int32_t *)(save + 10);
        ramp_value[0]  = *(int32_t *)(save + 16);
        ramp_value[1]  = *(int32_t *)(save + 18);
    }

    for (k = 0; k < count; ++k) {
        int16_t gains[4];
        int16_t l_vol, r_vol;
        int c;

        /* ramp_step for left channel */
        ramp_value[0] += ramp_step[0];
        if (ramp_step[0] <= 0) {
            if (ramp_value[0] <= ramp_target[0]) {
                ramp_value[0] = ramp_target[0];
                ramp_step[0] = 0;
            }
        } else {
            if (ramp_value[0] >= ramp_target[0]) {
                ramp_value[0] = ramp_target[0];
                ramp_step[0] = 0;
            }
        }
        l_vol = (int16_t)(ramp_value[0] >> 16);

        /* ramp_step for right channel */
        ramp_value[1] += ramp_step[1];
        if (ramp_step[1] <= 0) {
            if (ramp_value[1] <= ramp_target[1]) {
                ramp_value[1] = ramp_target[1];
                ramp_step[1] = 0;
            }
        } else {
            if (ramp_value[1] >= ramp_target[1]) {
                ramp_value[1] = ramp_target[1];
                ramp_step[1] = 0;
            }
        }
        r_vol = (int16_t)(ramp_value[1] >> 16);

        /* Compute per-channel gains: vol * dry/wet with rounding */
        gains[0] = clamp16((l_vol * vol_dry + 0x4000) >> 15);
        gains[1] = clamp16((r_vol * vol_dry + 0x4000) >> 15);
        gains[2] = clamp16((l_vol * vol_wet + 0x4000) >> 15);
        gains[3] = clamp16((r_vol * vol_wet + 0x4000) >> 15);

        /* Mix input sample into all 4 output buffers */
        dl[k] = clamp16(dl[k] + ((in[k] * gains[0]) >> 15));
        dr[k] = clamp16(dr[k] + ((in[k] * gains[1]) >> 15));
        wl[k] = clamp16(wl[k] + ((in[k] * gains[2]) >> 15));
        wr[k] = clamp16(wr[k] + ((in[k] * gains[3]) >> 15));
    }

    /* Save state — matches mupen64plus layout exactly */
    *(int16_t *)(save +  0) = vol_wet;
    *(int16_t *)(save +  2) = vol_dry;
    *(int16_t *)(save +  4) = (int16_t)(ramp_target[0] >> 16);
    *(int16_t *)(save +  6) = (int16_t)(ramp_target[1] >> 16);
    *(int32_t *)(save +  8) = (int32_t)ramp_step[0];
    *(int32_t *)(save + 10) = (int32_t)ramp_step[1];
    *(int32_t *)(save + 16) = (int32_t)ramp_value[0];
    *(int32_t *)(save + 18) = (int32_t)ramp_value[1];
}

/**
 * n_aInterleave - Interleave L and R channels for stereo output
 *
 * PM64 uses hardcoded addresses:
 *   Input L: N_AL_MAIN_L_OUT (0x4E0)
 *   Input R: N_AL_MAIN_R_OUT (0x650)
 *   Output:  DMEM[0] (interleaved LRLRLR...)
 */
void n_aInterleaveImpl(void) {
    int16_t *l = BUF_S16(N_AL_MAIN_L_OUT);
    int16_t *r = BUF_S16(N_AL_MAIN_R_OUT);
    int16_t *d = BUF_S16(0);
    int count = AUDIO_SAMPLES;
    int i;

    for (i = 0; i < count; i++) {
        *d++ = *l++;
        *d++ = *r++;
    }
}

/**
 * n_aPoleFilter - Multi-tap FIR/IIR lowpass filter (used in reverb)
 *
 * Direct port of mupen64plus-rsp-hle alist_polef().
 * This is NOT a simple 1st-order IIR — it's an 8-tap filter with
 * coefficient tables (h1, h2) and 2nd-order IIR feedback (l1, l2).
 *
 * The coefficient table is loaded into rspa.adpcm_table by the
 * n_aLoadADPCM call that precedes each n_aPoleFilter call in reverb.c.
 *   h1 = table[0..7]  (FIR coefficients for l1 feedback)
 *   h2 = table[8..15]  (FIR coefficients for l2 feedback + scaled convolution)
 *
 * For PM64's lowpass filter, h1 is all zeros and h2 contains an exponentially
 * decaying sequence derived from the cutoff frequency.
 *
 * dmemAddr arrives as the reconstructed DMEM byte address (the macro in
 * audio_mixer.h already converts the packed buffer index to a byte offset
 * by multiplying by 2 * AUDIO_SAMPLES).
 *
 * State layout (POLEF_STATE = int16_t[4]):
 *   [0..1]: last output samples (not used for feedback directly)
 *   [2]: l1 (2nd-to-last output of previous 8-sample block)
 *   [3]: l2 (last output of previous 8-sample block)
 */
void n_aPoleFilterImpl(uint8_t first, int16_t gain, uint16_t dmemAddr, void *state_addr) {
    int16_t *buf = BUF_S16(dmemAddr);
    int16_t *state = (int16_t *)state_addr;
    int count = ROUND_UP_16(AUDIO_SAMPLES * 2); /* byte count, aligned */

    /* h1 = table[0..7], h2 = table[8..15] from adpcm_table loaded by n_aLoadADPCM */
    const int16_t *h1 = (const int16_t *)rspa.adpcm_table;
    int16_t *h2 = (int16_t *)rspa.adpcm_table + 8;

    int16_t l1, l2;
    int16_t h2_before[8];
    unsigned i;

    if (first) {
        l1 = 0;
        l2 = 0;
    } else {
        l1 = state[2];
        l2 = state[3];
    }

    /* Scale h2 by gain, saving originals for l2 feedback term */
    for (i = 0; i < 8; i++) {
        h2_before[i] = h2[i];
        h2[i] = (int16_t)(((int32_t)h2[i] * gain) >> 14);
    }

    /* Process 8 samples at a time */
    do {
        int16_t frame[8];

        for (i = 0; i < 8; i++)
            frame[i] = buf[i];

        for (i = 0; i < 8; i++) {
            int32_t accu = (int32_t)frame[i] * gain;
            unsigned k;
            accu += (int32_t)h1[i] * l1 + (int32_t)h2_before[i] * l2;
            /* rdot(i, h2, frame) — convolution of scaled h2 with previous frame samples */
            for (k = 0; k < i; k++) {
                accu += (int32_t)h2[k] * frame[i - 1 - k];
            }
            buf[i] = clamp16(accu >> 14);
        }

        l1 = buf[6];
        l2 = buf[7];
        buf += 8;
        count -= 16; /* 8 samples × 2 bytes */
    } while (count != 0);

    /* Save last 4 output samples to state */
    state[0] = *(buf - 4);
    state[1] = *(buf - 3);
    state[2] = *(buf - 2); /* l1 for next call */
    state[3] = *(buf - 1); /* l2 for next call */
}
