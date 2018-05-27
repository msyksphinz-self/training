#ifndef __TEK_H__
#define __TEK_H__

#include "bootpack.h"
#include <setjmp.h>
#define NULL		0

typedef unsigned char UCHAR;
typedef unsigned int UINT32;
typedef UINT32 tek_TPRB;

struct tek_STR_BITMODEL {
	UCHAR t, m, s, dmy;
	UINT32 prb0, prb1, tmsk, ntm, lt, lt0, dmy4;
};

struct tek_STR_PRB {
	struct tek_STR_PRB_PB {
		struct tek_STR_PRB_PBST {
			tek_TPRB mch, rep0l1;
		} st[12];
		tek_TPRB lenlow[2][8], lenmid[2][8];
	} pb[16];
	struct tek_STR_PRB_ST {
		tek_TPRB rep, repg0, repg1, repg2;
	} st[12];
	tek_TPRB lensel[2][2], lenhigh[2][256], pslot[4][64], algn[64];
	tek_TPRB spdis[2][2+4+8+16+32], lenext[2+4+8+16+32];
	tek_TPRB repg3, fchgprm[2 * 32], tbmt[16], tbmm[16], fchglt;
	tek_TPRB lit[1];
};

struct tek_STR_RNGDEC {
	UCHAR *p;
	UINT32 range, code, rmsk;
	jmp_buf errjmp;
	struct tek_STR_BITMODEL bm[32], *ptbm[16];
	struct tek_STR_PRB probs;
};

int tek_getsize (unsigned char *p);
int tek_decomp (unsigned char *p, char *q, int size);

#endif // __TEK_H__
