/**
 * audio_mixer.h - Software RSP audio mixer for Paper Mario PC port
 *
 * Replaces N64 RSP audio commands with CPU-based software implementations.
 * Ported from Ghostship (SM64 PC port) and adapted for Paper Mario's
 * n_abi audio command format.
 *
 * This header redefines the RSP audio macros from PR/abi.h and PR/n_abi.h
 * to call software implementations instead of building Acmd command lists.
 */

#ifndef AUDIO_MIXER_H
#define AUDIO_MIXER_H

#include <stdint.h>
#include <stdbool.h>

/* ================================================================
 * Forward declarations for software mixer implementations
 * ================================================================ */

/* Standard audio commands (same interface as Ghostship) */
void aClearBufferImpl(uint16_t addr, int nbytes);
void aLoadADPCMImpl(int num_entries_times_16, const int16_t *book_source_addr);
void aDMEMMoveImpl(uint16_t in_addr, uint16_t out_addr, int nbytes);
void aSetLoopImpl(int16_t *adpcm_loop_state);
void aMixImpl_pm64(int16_t gain, uint16_t in_addr, uint16_t out_addr);

/* PM64 n_abi-specific audio commands */
void n_aADPCMdecImpl(void *state, uint8_t flags, uint16_t sampleByteCount, uint8_t alignOffset, uint16_t output);
void n_aLoadBufferImpl(uint16_t count, uint16_t dmemDest, const void *dramSrc);
void n_aSaveBufferImpl(uint16_t count, uint16_t dmemSrc, void *dramDest);
void n_aResampleImpl(void *state, uint8_t flags, uint16_t pitch, uint16_t input, uint8_t outputOffset);
void n_aSetVolumeImpl(uint8_t flags, int16_t v, int16_t t, int16_t r);
void n_aEnvMixerImpl(uint8_t flags, int16_t cvolR, void *state);
void n_aInterleaveImpl(void);
void n_aPoleFilterImpl(uint8_t first, int16_t gain, uint16_t dmemAddr, void *state);

/* ================================================================
 * Fix K0_TO_PHYS for 64-bit — the libultraship version truncates
 * to u32, but audio code passes pointers through it.
 * ================================================================ */
#undef K0_TO_PHYS
#define K0_TO_PHYS(x) ((uintptr_t)(x))

/* ================================================================
 * Undefine original macros from PR/abi.h and PR/n_abi.h
 * ================================================================ */

/* Standard macros from libultra/abi.h */
#undef aClearBuffer
#undef aDMEMMove
#undef aMix
#undef aLoadADPCM
#undef aSetLoop

/* n_abi macros from PR/n_abi.h */
#undef n_aADPCMdec
#undef n_aLoadBuffer
#undef n_aSaveBuffer
#undef n_aResample
#undef n_aSetVolume
#undef n_aEnvMixer
#undef n_aInterleave
#undef n_aLoadADPCM
#undef n_aPoleFilter

/* ================================================================
 * Redefine macros to call software implementations
 *
 * The (pkt) parameter is accepted but ignored — the software mixer
 * executes immediately instead of building a command list.
 * The pkt++ in calling code just advances a dummy pointer.
 * ================================================================ */

/* Standard macros */
#define aClearBuffer(pkt, d, c)         aClearBufferImpl(d, c)
#define aDMEMMove(pkt, i, o, c)         aDMEMMoveImpl(i, o, c)
#define aMix(pkt, f, g, i, o)           aMixImpl_pm64(g, i, o)
#define aLoadADPCM(pkt, c, d)           aLoadADPCMImpl(c, (const int16_t*)(d))
#define aSetLoop(pkt, a)                aSetLoopImpl((int16_t*)(a))

/* PM64 n_abi macros — parameters passed directly instead of packed into Acmd */
#define n_aADPCMdec(pkt, s, f, c, a, d)    n_aADPCMdecImpl((void*)(uintptr_t)(s), f, c, a, d)
#define n_aLoadBuffer(pkt, c, d, s)         n_aLoadBufferImpl(c, d, (const void*)(uintptr_t)(s))
#define n_aSaveBuffer(pkt, c, d, s)         n_aSaveBufferImpl(c, d, (void*)(uintptr_t)(s))
#define n_aResample(pkt, s, f, p, i, o)     n_aResampleImpl((void*)(uintptr_t)(s), f, p, i, o)
#define n_aSetVolume(pkt, f, v, t, r)       n_aSetVolumeImpl(f, v, t, r)
#define n_aEnvMixer(pkt, f, t, s)           n_aEnvMixerImpl(f, t, (void*)(uintptr_t)(s))
#define n_aInterleave(pkt)                  n_aInterleaveImpl()
#define n_aLoadADPCM(pkt, c, d)             aLoadADPCMImpl(c, (const int16_t*)(uintptr_t)(d))
/* n_aPoleFilter DMEM address reconstruction:
 * The 't' parameter is a packed 8-bit index (original DMEM address >> 8).
 * On the N64 RSP, the microcode reconstructs the DMEM byte address as
 * t * (2 * AUDIO_SAMPLES), where AUDIO_SAMPLES = 184 and 2*184 = 0x170.
 * Example: outputTapBuffer=0x170 -> t=0x170>>8=1 -> 1*0x170=0x170 (correct).
 * Without this reconstruction, the filter operates on DMEM address 0 or 1
 * instead of the intended outputTapBuffer, causing the reverb lowpass filter
 * to process the wrong data and producing audio crackling. */
#define n_aPoleFilter(pkt, f, g, t, s)      n_aPoleFilterImpl(f, g, (uint16_t)((t) * (2 * 184)), (void*)(uintptr_t)(s))

#endif /* AUDIO_MIXER_H */
