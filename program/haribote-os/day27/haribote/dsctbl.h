#ifndef __DSCTBL_H__
#define __DSCTBL_H__

void init_gdtidt (void);
void set_segmdesc (struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc (struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

void asm_hrb_api (char a, int b);

void asm_inthandler0c (void);
void asm_inthandler0d (void);
void asm_inthandler20 (void);
void asm_inthandler21 (void);
void asm_inthandler27 (void);
void asm_inthandler2c (void);

#endif // __DSCTBL_H__


