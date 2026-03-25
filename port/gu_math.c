/**
 * gu_math.c - N64 SDK math library implementations for PC port.
 *
 * Combines implementations from Ghostship (SM64 recomp) and Paper Mario decomp.
 * These replace the MIPS assembly (.s) and decomp C files from src/os/ that we
 * excluded from the build.
 *
 * Paper Mario uses GBI_FLOATS=1, so Mtx == MtxF (float[4][4]).
 */

#include "common.h"
#include <math.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Matrix identity / conversion                                        */
/* ------------------------------------------------------------------ */

void guMtxIdentF(float mf[4][4]) {
    int r, c;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            mf[r][c] = (r == c) ? 1.0f : 0.0f;
        }
    }
}

void guMtxIdent(Mtx *m) {
#ifdef GBI_FLOATS
    guMtxIdentF(m->mf);
#else
    float mf[4][4];
    guMtxIdentF(mf);
    guMtxF2L(mf, m);
#endif
}

#ifdef GBI_FLOATS
void guMtxF2L(float mf[4][4], Mtx *m) {
    memcpy(m, mf, sizeof(Mtx));
}
#else
void guMtxF2L(float mf[4][4], Mtx *m) {
    int r, c;
    s32 tmp1, tmp2;
    s32 *m1 = &m->m[0][0];
    s32 *m2 = &m->m[2][0];
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 2; c++) {
            tmp1 = mf[r][2 * c] * 65536.0f;
            tmp2 = mf[r][2 * c + 1] * 65536.0f;
            *m1++ = (tmp1 & 0xffff0000) | ((tmp2 >> 16) & 0xffff);
            *m2++ = ((tmp1 << 16) & 0xffff0000) | (tmp2 & 0xffff);
        }
    }
}

void guMtxL2F(float mf[4][4], Mtx *m) {
    int r, c;
    u32 tmp1, tmp2;
    u32 *m1, *m2;
    s32 stmp1, stmp2;
    m1 = (u32 *)&m->m[0][0];
    m2 = (u32 *)&m->m[2][0];
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 2; c++) {
            tmp1 = (*m1 & 0xffff0000) | ((*m2 >> 16) & 0xffff);
            tmp2 = ((*m1++ << 16) & 0xffff0000) | (*m2++ & 0xffff);
            stmp1 = *(s32 *)&tmp1;
            stmp2 = *(s32 *)&tmp2;
            mf[r][c * 2 + 0] = stmp1 / 65536.0f;
            mf[r][c * 2 + 1] = stmp2 / 65536.0f;
        }
    }
}
#endif

/* GBI_FLOATS: guMtxL2F is a no-op (Mtx already IS float) */
#ifdef GBI_FLOATS
void guMtxL2F(float mf[4][4], Mtx *m) {
    memcpy(mf, m, sizeof(Mtx));
}

/* Convert N64 fixed-point matrix (MtxS) to float.
 * Used by entity fragment code where data is declared as MtxS but
 * with GBI_FLOATS the extern is Mtx (=MtxF). guMtxL2F would just
 * memcpy the raw int bits, so we need the real conversion. */
void guMtxFixedToFloat(float mf[4][4], void *fixedMtx) {
    int r, c;
    u32 tmp1, tmp2;
    u32 *m1, *m2;
    s32 stmp1, stmp2;
    u32 *raw = (u32 *)fixedMtx;
    m1 = &raw[0];   /* integer part rows */
    m2 = &raw[8];   /* fractional part rows */
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 2; c++) {
            tmp1 = (*m1 & 0xffff0000) | ((*m2 >> 16) & 0xffff);
            tmp2 = ((*m1++ << 16) & 0xffff0000) | (*m2++ & 0xffff);
            stmp1 = *(s32 *)&tmp1;
            stmp2 = *(s32 *)&tmp2;
            mf[r][c * 2 + 0] = stmp1 / 65536.0f;
            mf[r][c * 2 + 1] = stmp2 / 65536.0f;
        }
    }
}
#endif

/* ------------------------------------------------------------------ */
/* Matrix concatenation                                                */
/* ------------------------------------------------------------------ */

void guMtxCatF(float m[4][4], float n[4][4], float res[4][4]) {
    float temp[4][4];
    int i, j, k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp[i][j] = 0.0f;
            for (k = 0; k < 4; k++) {
                temp[i][j] += m[i][k] * n[k][j];
            }
        }
    }
    memcpy(res, temp, sizeof(temp));
}

void guMtxCatL(Mtx *m, Mtx *n, Mtx *res) {
    float mf[4][4], nf[4][4], resf[4][4];
    guMtxL2F(mf, m);
    guMtxL2F(nf, n);
    guMtxCatF(mf, nf, resf);
    guMtxF2L(resf, res);
}

/* ------------------------------------------------------------------ */
/* Normalize                                                           */
/* ------------------------------------------------------------------ */

void guNormalize(f32 *x, f32 *y, f32 *z) {
    f32 tmp = 1.0f / sqrtf(*x * *x + *y * *y + *z * *z);
    *x *= tmp;
    *y *= tmp;
    *z *= tmp;
}

/* ------------------------------------------------------------------ */
/* Rotation                                                            */
/* ------------------------------------------------------------------ */

void guRotateF(float m[4][4], float a, float x, float y, float z) {
    float sin_a, cos_a;
    float sp2c, sp28, sp24;
    float xx, yy, zz;
    static float dtor = GU_PI / 180.0f;

    guNormalize(&x, &y, &z);
    a *= dtor;
    sin_a = sinf(a);
    cos_a = cosf(a);
    sp2c = x * y * (1 - cos_a);
    sp28 = y * z * (1 - cos_a);
    sp24 = z * x * (1 - cos_a);

    guMtxIdentF(m);
    xx = x * x;
    m[0][0] = (1 - xx) * cos_a + xx;
    m[2][1] = sp28 - x * sin_a;
    m[1][2] = sp28 + x * sin_a;
    yy = y * y;
    m[1][1] = (1 - yy) * cos_a + yy;
    m[2][0] = sp24 + y * sin_a;
    m[0][2] = sp24 - y * sin_a;
    zz = z * z;
    m[2][2] = (1 - zz) * cos_a + zz;
    m[1][0] = sp2c - z * sin_a;
    m[0][1] = sp2c + z * sin_a;
}

void guRotate(Mtx *m, float a, float x, float y, float z) {
    float mf[4][4];
    guRotateF(mf, a, x, y, z);
    guMtxF2L(mf, m);
}

/* ------------------------------------------------------------------ */
/* Scale                                                               */
/* ------------------------------------------------------------------ */

void guScaleF(float mf[4][4], float x, float y, float z) {
    guMtxIdentF(mf);
    mf[0][0] = x;
    mf[1][1] = y;
    mf[2][2] = z;
    mf[3][3] = 1.0f;
}

void guScale(Mtx *m, float x, float y, float z) {
    float mf[4][4];
    guScaleF(mf, x, y, z);
    guMtxF2L(mf, m);
}

/* ------------------------------------------------------------------ */
/* Translate                                                           */
/* ------------------------------------------------------------------ */

void guTranslateF(float m[4][4], float x, float y, float z) {
    guMtxIdentF(m);
    m[3][0] = x;
    m[3][1] = y;
    m[3][2] = z;
}

void guTranslate(Mtx *m, float x, float y, float z) {
    float mf[4][4];
    guTranslateF(mf, x, y, z);
    guMtxF2L(mf, m);
}

/* ------------------------------------------------------------------ */
/* Ortho                                                               */
/* ------------------------------------------------------------------ */

void guOrthoF(float m[4][4], float left, float right, float bottom,
              float top, float near, float far, float scale) {
    int r, c;
    guMtxIdentF(m);
    m[0][0] = 2.0f / (right - left);
    m[1][1] = 2.0f / (top - bottom);
    m[2][2] = -2.0f / (far - near);
    m[3][0] = -(right + left) / (right - left);
    m[3][1] = -(top + bottom) / (top - bottom);
    m[3][2] = -(far + near) / (far - near);
    m[3][3] = 1.0f;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            m[r][c] *= scale;
        }
    }
}

void guOrtho(Mtx *m, float left, float right, float bottom,
             float top, float near, float far, float scale) {
    float mf[4][4];
    guOrthoF(mf, left, right, bottom, top, near, far, scale);
    guMtxF2L(mf, m);
}

/* ------------------------------------------------------------------ */
/* Perspective                                                         */
/* ------------------------------------------------------------------ */

void guPerspectiveF(float mf[4][4], u16 *perspNorm, float fovy,
                    float aspect, float near, float far, float scale) {
    float yscale;
    int r, c;
    guMtxIdentF(mf);
    fovy *= GU_PI / 180.0f;
    yscale = cosf(fovy / 2.0f) / sinf(fovy / 2.0f);
    mf[0][0] = yscale / aspect;
    mf[1][1] = yscale;
    mf[2][2] = (near + far) / (near - far);
    mf[2][3] = -1.0f;
    mf[3][2] = 2.0f * near * far / (near - far);
    mf[3][3] = 0.0f;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            mf[r][c] *= scale;
        }
    }
    if (perspNorm != NULL) {
        if (near + far <= 2.0f) {
            *perspNorm = 65535;
        } else {
            *perspNorm = (double)(1 << 17) / (near + far);
            if (*perspNorm <= 0) {
                *perspNorm = 1;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Frustum                                                             */
/* ------------------------------------------------------------------ */

void guFrustumF(f32 mf[4][4], f32 l, f32 r, f32 b, f32 t,
                f32 n, f32 f, f32 scale) {
    int i, j;
    guMtxIdentF(mf);
    mf[0][0] = 2 * n / (r - l);
    mf[1][1] = 2 * n / (t - b);
    mf[2][0] = (r + l) / (r - l);
    mf[2][1] = (t + b) / (t - b);
    mf[2][2] = -(f + n) / (f - n);
    mf[2][3] = -1;
    mf[3][2] = -2 * f * n / (f - n);
    mf[3][3] = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mf[i][j] *= scale;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Position (euler angles)                                             */
/* ------------------------------------------------------------------ */

void guPositionF(f32 mf[4][4], f32 r, f32 p, f32 h, f32 s,
                 f32 x, f32 y, f32 z) {
    static f32 dtor = 3.1415926f / 180.0f;
    f32 sinr, sinp, sinh_val;
    f32 cosr, cosp, cosh_val;

    r *= dtor;
    p *= dtor;
    h *= dtor;
    sinr = sinf(r);
    cosr = cosf(r);
    sinp = sinf(p);
    cosp = cosf(p);
    sinh_val = sinf(h);
    cosh_val = cosf(h);

    mf[0][0] = (cosp * cosh_val) * s;
    mf[0][1] = (cosp * sinh_val) * s;
    mf[0][2] = (-sinp) * s;
    mf[0][3] = 0.0f;
    mf[1][0] = (sinr * sinp * cosh_val - cosr * sinh_val) * s;
    mf[1][1] = (sinr * sinp * sinh_val + cosr * cosh_val) * s;
    mf[1][2] = (sinr * cosp) * s;
    mf[1][3] = 0.0f;
    mf[2][0] = (cosr * sinp * cosh_val + sinr * sinh_val) * s;
    mf[2][1] = (cosr * sinp * sinh_val - sinr * cosh_val) * s;
    mf[2][2] = (cosr * cosp) * s;
    mf[2][3] = 0.0f;
    mf[3][0] = x;
    mf[3][1] = y;
    mf[3][2] = z;
    mf[3][3] = 1.0f;
}

void guPosition(Mtx *m, f32 r, f32 p, f32 h, f32 s,
                f32 x, f32 y, f32 z) {
    f32 mf[4][4];
    guPositionF(mf, r, p, h, s, x, y, z);
    guMtxF2L(mf, m);
}

/* ------------------------------------------------------------------ */
/* RotateRPY (roll/pitch/yaw)                                          */
/* ------------------------------------------------------------------ */

void guRotateRPYF(float mf[4][4], f32 r, f32 p, f32 h) {
    static f32 dtor = 3.1415926f / 180.0f;
    f32 sinr, sinp, sinh_val;
    f32 cosr, cosp, cosh_val;

    r *= dtor;
    p *= dtor;
    h *= dtor;
    sinr = sinf(r);
    cosr = cosf(r);
    sinp = sinf(p);
    cosp = cosf(p);
    sinh_val = sinf(h);
    cosh_val = cosf(h);

    guMtxIdentF(mf);
    mf[0][0] = cosp * cosh_val;
    mf[0][1] = cosp * sinh_val;
    mf[0][2] = -sinp;
    mf[1][0] = sinr * sinp * cosh_val - cosr * sinh_val;
    mf[1][1] = sinr * sinp * sinh_val + cosr * cosh_val;
    mf[1][2] = sinr * cosp;
    mf[2][0] = cosr * sinp * cosh_val + sinr * sinh_val;
    mf[2][1] = cosr * sinp * sinh_val - sinr * cosh_val;
    mf[2][2] = cosr * cosp;
}

/* ------------------------------------------------------------------ */
/* Matrix × vector transforms                                          */
/* ------------------------------------------------------------------ */

void guMtxXFMF(f32 mf[4][4], f32 x, f32 y, f32 z,
               f32 *ox, f32 *oy, f32 *oz) {
    *ox = mf[0][0] * x + mf[1][0] * y + mf[2][0] * z + mf[3][0];
    *oy = mf[0][1] * x + mf[1][1] * y + mf[2][1] * z + mf[3][1];
    *oz = mf[0][2] * x + mf[1][2] * y + mf[2][2] * z + mf[3][2];
}

void guMtxXFML(Mtx *m, float x, float y, float z,
               float *ox, float *oy, float *oz) {
    float mf[4][4];
    guMtxL2F(mf, m);
    guMtxXFMF(mf, x, y, z, ox, oy, oz);
}

/* ------------------------------------------------------------------ */
/* LookAt                                                              */
/* ------------------------------------------------------------------ */

void guLookAtF(float mf[4][4], float xEye, float yEye, float zEye,
               float xAt, float yAt, float zAt,
               float xUp, float yUp, float zUp) {
    float len, xLook, yLook, zLook, xRight, yRight, zRight;

    guMtxIdentF(mf);
    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;
    len = -1.0f / sqrtf(xLook * xLook + yLook * yLook + zLook * zLook);
    xLook *= len;
    yLook *= len;
    zLook *= len;

    xRight = yUp * zLook - zUp * yLook;
    yRight = zUp * xLook - xUp * zLook;
    zRight = xUp * yLook - yUp * xLook;
    len = 1.0f / sqrtf(xRight * xRight + yRight * yRight + zRight * zRight);
    xRight *= len;
    yRight *= len;
    zRight *= len;

    xUp = yLook * zRight - zLook * yRight;
    yUp = zLook * xRight - xLook * zRight;
    zUp = xLook * yRight - yLook * xRight;
    len = 1.0f / sqrtf(xUp * xUp + yUp * yUp + zUp * zUp);
    xUp *= len;
    yUp *= len;
    zUp *= len;

    mf[0][0] = xRight;
    mf[1][0] = yRight;
    mf[2][0] = zRight;
    mf[3][0] = -(xEye * xRight + yEye * yRight + zEye * zRight);
    mf[0][1] = xUp;
    mf[1][1] = yUp;
    mf[2][1] = zUp;
    mf[3][1] = -(xEye * xUp + yEye * yUp + zEye * zUp);
    mf[0][2] = xLook;
    mf[1][2] = yLook;
    mf[2][2] = zLook;
    mf[3][2] = -(xEye * xLook + yEye * yLook + zEye * zLook);
    mf[0][3] = 0;
    mf[1][3] = 0;
    mf[2][3] = 0;
    mf[3][3] = 1;
}

#define FTOFRAC8(x) ((int)((x) * 128.0f > 127.0f ? 127.0f : (x) * 128.0f) & 0xff)

void guLookAtReflectF(float mf[4][4], LookAt *l,
                      float xEye, float yEye, float zEye,
                      float xAt, float yAt, float zAt,
                      float xUp, float yUp, float zUp) {
    float len, xLook, yLook, zLook, xRight, yRight, zRight;

    guMtxIdentF(mf);
    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;
    len = -1.0f / sqrtf(xLook * xLook + yLook * yLook + zLook * zLook);
    xLook *= len;
    yLook *= len;
    zLook *= len;

    xRight = yUp * zLook - zUp * yLook;
    yRight = zUp * xLook - xUp * zLook;
    zRight = xUp * yLook - yUp * xLook;
    len = 1.0f / sqrtf(xRight * xRight + yRight * yRight + zRight * zRight);
    xRight *= len;
    yRight *= len;
    zRight *= len;

    xUp = yLook * zRight - zLook * yRight;
    yUp = zLook * xRight - xLook * zRight;
    zUp = xLook * yRight - yLook * xRight;
    len = 1.0f / sqrtf(xUp * xUp + yUp * yUp + zUp * zUp);
    xUp *= len;
    yUp *= len;
    zUp *= len;

    l->l[0].l.dir[0] = FTOFRAC8(xRight);
    l->l[0].l.dir[1] = FTOFRAC8(yRight);
    l->l[0].l.dir[2] = FTOFRAC8(zRight);
    l->l[1].l.dir[0] = FTOFRAC8(xUp);
    l->l[1].l.dir[1] = FTOFRAC8(yUp);
    l->l[1].l.dir[2] = FTOFRAC8(zUp);
    l->l[0].l.col[0] = 0x00;
    l->l[0].l.col[1] = 0x00;
    l->l[0].l.col[2] = 0x00;
    l->l[0].l.pad1 = 0x00;
    l->l[0].l.colc[0] = 0x00;
    l->l[0].l.colc[1] = 0x00;
    l->l[0].l.colc[2] = 0x00;
    l->l[0].l.pad2 = 0x00;
    l->l[1].l.col[0] = 0x00;
    l->l[1].l.col[1] = 0x80;
    l->l[1].l.col[2] = 0x00;
    l->l[1].l.pad1 = 0x00;
    l->l[1].l.colc[0] = 0x00;
    l->l[1].l.colc[1] = 0x80;
    l->l[1].l.colc[2] = 0x00;
    l->l[1].l.pad2 = 0x00;

    mf[0][0] = xRight;
    mf[1][0] = yRight;
    mf[2][0] = zRight;
    mf[3][0] = -(xEye * xRight + yEye * yRight + zEye * zRight);
    mf[0][1] = xUp;
    mf[1][1] = yUp;
    mf[2][1] = zUp;
    mf[3][1] = -(xEye * xUp + yEye * yUp + zEye * zUp);
    mf[0][2] = xLook;
    mf[1][2] = yLook;
    mf[2][2] = zLook;
    mf[3][2] = -(xEye * xLook + yEye * yLook + zEye * zLook);
    mf[0][3] = 0;
    mf[1][3] = 0;
    mf[2][3] = 0;
    mf[3][3] = 1;
}

void guLookAtHiliteF(float mf[4][4], LookAt *l, Hilite *h,
                     float xEye, float yEye, float zEye,
                     float xAt, float yAt, float zAt,
                     float xUp, float yUp, float zUp,
                     float xl1, float yl1, float zl1,
                     float xl2, float yl2, float zl2,
                     int twidth, int theight) {
    /* The hilite function is the same as LookAtReflect plus hilite calculation */
    float len, xLook, yLook, zLook, xRight, yRight, zRight;
    float nx, ny, nz;

    guLookAtReflectF(mf, l, xEye, yEye, zEye, xAt, yAt, zAt, xUp, yUp, zUp);

    /* Hilite calculation */
    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;
    len = 1.0f / sqrtf(xLook * xLook + yLook * yLook + zLook * zLook);
    xLook *= len;
    yLook *= len;
    zLook *= len;

    /* Light 1 hilite */
    len = 1.0f / sqrtf(xl1 * xl1 + yl1 * yl1 + zl1 * zl1);
    nx = xLook + xl1 * len;
    ny = yLook + yl1 * len;
    nz = zLook + zl1 * len;
    len = 1.0f / sqrtf(nx * nx + ny * ny + nz * nz);
    nx *= len;
    ny *= len;
    nz *= len;

    h->h.x1 = (int)(twidth * 4 *
        (1.0f + (l->l[0].l.dir[0] / 127.0f * nx +
                 l->l[0].l.dir[1] / 127.0f * ny +
                 l->l[0].l.dir[2] / 127.0f * nz)));
    h->h.y1 = (int)(theight * 4 *
        (1.0f + (l->l[1].l.dir[0] / 127.0f * nx +
                 l->l[1].l.dir[1] / 127.0f * ny +
                 l->l[1].l.dir[2] / 127.0f * nz)));

    /* Light 2 hilite */
    len = 1.0f / sqrtf(xl2 * xl2 + yl2 * yl2 + zl2 * zl2);
    nx = xLook + xl2 * len;
    ny = yLook + yl2 * len;
    nz = zLook + zl2 * len;
    len = 1.0f / sqrtf(nx * nx + ny * ny + nz * nz);
    nx *= len;
    ny *= len;
    nz *= len;

    h->h.x2 = (int)(twidth * 4 *
        (1.0f + (l->l[0].l.dir[0] / 127.0f * nx +
                 l->l[0].l.dir[1] / 127.0f * ny +
                 l->l[0].l.dir[2] / 127.0f * nz)));
    h->h.y2 = (int)(theight * 4 *
        (1.0f + (l->l[1].l.dir[0] / 127.0f * nx +
                 l->l[1].l.dir[1] / 127.0f * ny +
                 l->l[1].l.dir[2] / 127.0f * nz)));
}
