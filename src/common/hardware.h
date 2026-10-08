#ifndef NESJS_COMMON_HARDWARE_H
#define NESJS_COMMON_HARDWARE_H

#define PPUCTRL (*(volatile unsigned char *)0x2000)
#define PPUMASK (*(volatile unsigned char *)0x2001)
#define PPUSTATUS (*(volatile unsigned char *)0x2002)
#define PPUADDR (*(volatile unsigned char *)0x2006)
#define PPUDATA (*(volatile unsigned char *)0x2007)
#define JOY1 (*(volatile unsigned char *)0x4016)
#define APU(r) (*(volatile unsigned char *)(0x4000 + (r)))
#define OAM ((unsigned char *)0x0200)
#define MMC3_IRQ_OFF (*(volatile unsigned char *)0xE000)

#define P_A 0x80
#define P_B 0x40
#define P_SELECT 0x20
#define P_START 0x10
#define P_UP 0x08
#define P_DOWN 0x04
#define P_LEFT 0x02
#define P_RIGHT 0x01

extern volatile unsigned char frame_count, ppu_ctrl, ppu_mask, vq_ready, stage_busy, up_bank, up_hi, up_left, up_off;
extern volatile unsigned int frame16;
#pragma zpsym("frame_count")
#pragma zpsym("ppu_ctrl")
#pragma zpsym("ppu_mask")
#pragma zpsym("vq_ready")
#pragma zpsym("stage_busy")
#pragma zpsym("up_bank")
#pragma zpsym("up_hi")
#pragma zpsym("up_left")
#pragma zpsym("up_off")
#pragma zpsym("frame16")
extern unsigned char rowbank[30];
extern unsigned char vq[72];
extern unsigned char stage[256];
extern const unsigned char *r_text, *r_cls, *bc_src;
extern unsigned char r_len, r_x0, r_half, r_clsk, *bc_dst;
extern unsigned int bc_len;
#pragma zpsym("r_text")
#pragma zpsym("r_cls")
#pragma zpsym("r_len")
#pragma zpsym("r_x0")
#pragma zpsym("r_half")
#pragma zpsym("r_clsk")
#pragma zpsym("bc_src")
#pragma zpsym("bc_dst")
#pragma zpsym("bc_len")
void render_half(void);
void __fastcall__ upload_now(unsigned char bank);
void __fastcall__ bank_copy(unsigned char bank);
void __fastcall__ set_r2(unsigned char bank);
void __fastcall__ nt_blit(const unsigned char *src);
extern unsigned char *spill_ptr;
extern unsigned char hw_base;
extern unsigned char spill_area[];
#pragma zpsym("spill_ptr")
#pragma zpsym("hw_base")
void hw_spill(void);
void hw_unspill(void);

#endif
