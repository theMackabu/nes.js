.export __STARTUP__ : absolute = 1
.exportzp _frame_count, _ppu_ctrl, _ppu_mask, _frame16, _spill_ptr, _hw_base
.exportzp _vq_ready, _stage_busy, _up_bank, _up_hi, _up_left, _up_off
.exportzp _r_text, _r_cls, _r_len, _r_x0, _r_half, _r_clsk, _bc_src, _bc_dst, _bc_len
.export _rowbank, _vq, _stage, _glyph5, _shr_tab, _shl_tab
.export _hw_spill, _hw_unspill, _spill_area, _quick_operand
.export _render_half, _upload_now, _bank_copy, _set_r2, _map_code, _nt_blit
.import _lcount, _fbase, _gptr, _lid, _lvar
.importzp _p, _vt, _vn, _vs
.import _main, copydata, zerobss
.importzp c_sp
.import __FONT_LOAD__

DATA_BANK = 5


.segment "HEADER"
  .byte "NES", $1A
  .byte 4           ; 4 x 16KB PRG
  .byte 0           ; CHR-RAM
  .byte $42         ; mapper 4 (low nibble 4), battery
  .byte $08         ; NES 2.0
  .byte 0, 0
  .byte $70         ; 8KB battery-backed PRG-RAM
  .byte $09         ; 32KB CHR-RAM
  .byte 0, 0, 0, 0

.segment "ZEROPAGE"
_frame_count: .res 1
_ppu_ctrl:    .res 1
_ppu_mask:    .res 1
_frame16:     .res 2
_spill_ptr:   .res 2
_hw_base:     .res 1
sp_tmp:       .res 2
sp_cnt:       .res 1
qptr:         .res 2
bank_sel:     .res 1
cur_r6:       .res 1
irq_row:      .res 1
_vq_ready:    .res 1
_stage_busy:  .res 1
_up_bank:     .res 1
_up_hi:       .res 1
_up_left:     .res 1
_up_off:
up_off:       .res 1
nmi_chunks:   .res 1
nmi_tmp:      .res 1
_bc_src:      .res 2
_bc_dst:      .res 2
_bc_len:      .res 2
_r_text:      .res 2
_r_cls:       .res 2
_r_len:       .res 1
_r_x0:        .res 1
_r_half:      .res 1
_r_clsk:      .res 1
r_i:          .res 1
r_x:          .res 1
r_lt:         .res 1
r_tmin:       .res 1
r_bit:        .res 1
r_bo:         .res 1
r_f0:         .res 1
r_f1:         .res 1
r_b0:         .res 1
r_b1:         .res 1
r_cl:         .res 1
r_row:        .res 1
r_v:          .res 1
r_gp:         .res 2

.segment "BSS"
_rowbank:     .res 30
_vq:          .res 72

.segment "STAGE"
_stage:       .res 256

.segment "SPILL"
_spill_area:  .res 1

.segment "FONT"
  .incbin "font.chr"

.segment "STARTUP"
reset:
  sei
  cld
  ldx #$40
  stx $4017
  ldx #$FF
  txs
  inx
  stx $2000
  stx $2001
  stx $4010
  stx $E000             ; MMC3 IRQ off
  lda #$C7  
  sta bank_sel
  sta $8000
  lda #3
  sta $8001
  lda #$C6
  sta bank_sel
  sta $8000
  lda #4
  sta $8001
  sta cur_r6
  lda #$80
  sta $A001             ; enable PRG-RAM
  lda #0
  sta $A000             ; vertical mirroring
  bit $2002
@v1:
  bit $2002
  bpl @v1
  lda #0
  tax
@clr:
  sta $00,x
  sta $0100,x
  sta $0300,x
  sta $0400,x
  sta $0500,x
  sta $0600,x
  sta $0700,x
  inx
  bne @clr
  lda #$FF
@oam:
  sta $0200,x
  inx
  bne @oam
  lda #$C6
  sta bank_sel
  lda #4
  sta cur_r6
@v2:
  bit $2002
  bpl @v2
  ldx #0
@cb:
  lda chr_init,x
  sta $8000
  lda chr_init+1,x
  sta $8001
  inx
  inx
  cpx #12
  bne @cb
  lda bank_sel
  sta $8000
@bank:
  lda nmi_tmp
  jsr _set_r2
  bit $2002
  lda #0
  sta $2006
  sta $2006
  ldx #4
  ldy #0
@b:
  sta $2007
  dey
  bne @b
  dex
  bne @b
  inc nmi_tmp
  lda nmi_tmp
  cmp #32
  bne @bank
  lda #$C6
  sta $8000
  lda #DATA_BANK
  sta $8001
  bit $2002
  lda #$10
  sta $2006
  lda #0
  sta $2006
  lda #<__FONT_LOAD__
  sta qptr
  lda #>__FONT_LOAD__
  sta qptr+1
  ldx #8
  ldy #0
@f:
  lda (qptr),y
  sta $2007
  iny
  bne @f
  inc qptr+1
  dex
  bne @f
  lda cur_r6
  sta $8001
  lda #<$8000
  sta c_sp
  lda #>$8000
  sta c_sp+1
  jsr zerobss
  jsr copydata
  cli
  jmp _main

chr_init:
  .byte $C0, 2, $C1, 2, $C2, 0, $C3, 0, $C4, 2, $C5, 3

_set_r2:
  ldx #$C2
  stx $8000
  sta $8001
  ldx bank_sel
  stx $8000
  rts

_upload_now:
  jsr _set_r2
  bit $2002
  lda _up_hi
  sta $2006
  lda #0
  sta $2006
  tax
@l:
  lda _stage,x
  sta $2007
  inx
  bne @l
  rts

_nt_blit:
  sta qptr
  stx qptr+1
  bit $2002
  lda #$20
  sta $2006
  lda #0
  sta $2006
  ldx #4
  ldy #0
@l:
  lda (qptr),y
  sta $2007
  iny
  bne @l
  inc qptr+1
  dex
  bne @l
  rts

_bank_copy:
  ldx #$C6
  stx bank_sel
  stx $8000
  sta $8001
  ldy #0
@loop:
  lda _bc_len
  ora _bc_len+1
  beq @done
  lda (_bc_src),y
  sta (_bc_dst),y
  inc _bc_src
  bne @s
  inc _bc_src+1
@s:
  inc _bc_dst
  bne @d
  inc _bc_dst+1
@d:
  lda _bc_len
  bne @dl
  dec _bc_len+1
@dl:
  dec _bc_len
  jmp @loop
@done:
  lda #$C6
  sta $8000
  lda cur_r6
  sta $8001
  rts

_map_code:
  asl
  clc
  adc #1
  tax
  lda #$C7
  sta bank_sel
  sta $8000
  stx $8001
  inx
  stx cur_r6
  lda #$C6
  sta bank_sel
  sta $8000
  stx $8001
  rts

nmi:
  pha
  txa
  pha
  tya
  pha
  bit $2002
  lda #0
  sta $2003
  lda #$02
  sta $4014             ; sprite DMA
  lda #5
  sta nmi_chunks
  lda _vq_ready
  beq @novq
  jsr do_vq
  lda #2
  sta nmi_chunks
@novq:
  lda _up_left
  beq @noup
  lda #$C2
  sta $8000
  lda _up_bank
  sta $8001
  lda _up_hi
  sta $2006
  lda up_off
  sta $2006
  ldx up_off
@chunk:
  jsr copy32
  txa
  clc
  adc #32
  tax
  dec _up_left
  beq @updone
  dec nmi_chunks
  bne @chunk
  stx up_off
  jmp @noup
@updone:
  lda #0
  sta up_off
  sta _stage_busy
@noup:
  lda #$C2              ; R2 = window for row 0
  sta $8000
  lda _rowbank
  sta $8001
  lda bank_sel
  sta $8000
  lda #7                ; MMC3 IRQ every 8 scanlines
  sta $C000
  sta $C001
  sta $E001
  lda #0
  sta irq_row
  lda _ppu_ctrl
  sta $2000
  lda #0
  sta $2005
  sta $2005
  lda _ppu_mask
  sta $2001
  inc _frame_count
  inc _frame16
  bne @nohi
  inc _frame16+1
@nohi:
  pla
  tay
  pla
  tax
  pla
  rti

irq:
  pha
  txa
  pha
  sta $E000
  sta $E001
  ldx irq_row
  inx
  stx irq_row
  cpx #30
  bcs @off
  txa
  and #1
  ora #$C2
  sta $8000
  lda _rowbank,x
  sta $8001
  lda bank_sel
  sta $8000
  pla
  tax
  pla
  rti
@off:
  sta $E000
  pla
  tax
  pla
  rti

do_vq:
  ldx #0
@ent:
  lda _vq,x
  beq @done
  sta $2006
  lda _vq+1,x
  sta $2006
  lda _vq+2,x
  sta nmi_tmp
  inx
  inx
  inx
  lda _ppu_ctrl
  bit nmi_tmp
  bvc @h
  ora #$04
  bne @setc
@h:
  and #$FB
@setc:
  sta $2000
  lda nmi_tmp
  and #$3F
  tay
  bit nmi_tmp
  bmi @rle
@cp:
  lda _vq,x
  sta $2007
  inx
  dey
  bne @cp
  jmp @ent
@rle:
  lda _vq,x
  inx
@fill:
  sta $2007
  dey
  bne @fill
  jmp @ent
@done:
  lda _ppu_ctrl
  sta $2000
  lda #0
  sta _vq_ready
  rts

copy32:
  .repeat 32, i
    lda _stage+i,x
    sta $2007
  .endrepeat
  rts

_render_half:
  lda #0
  tax
@clr:
  sta _stage,x
  inx
  bne @clr
  lda _r_half
  asl
  asl
  asl
  asl
  sta r_tmin
  lda _r_x0
  sta r_x
  lda #0
  sta r_i
@char:
  ldy r_i
  cpy _r_len
  bcc @go
  rts
@go:
  lda r_x
  lsr
  lsr
  lsr
  sec
  sbc r_tmin
  sta r_lt
  bmi @neg
  cmp #16
  bcc @inrange
  rts
@neg:
  cmp #$FF
  beq @inrange
  jmp @next
@inrange:
  lda (_r_text),y
  sec
  sbc #33
  cmp #94
  bcc @glyph
  jmp @next
@glyph:
  sta r_gp
  lda #0
  sta r_gp+1
  asl r_gp
  rol r_gp+1
  asl r_gp
  rol r_gp+1
  asl r_gp
  rol r_gp+1
  clc
  lda r_gp
  adc #<_glyph5
  sta r_gp
  lda r_gp+1
  adc #>_glyph5
  sta r_gp+1
  lda _r_cls+1
  beq @kcls
  lda (_r_cls),y
  jmp @scls
@kcls:
  lda _r_clsk
@scls:
  sta r_cl
  lda r_x
  and #7
  sta r_bit
  asl
  asl
  asl
  asl
  asl
  sta r_bo
  lda #0
  sta r_f0
  sta r_f1
  lda r_lt
  bmi @nof0
  asl
  asl
  asl
  asl
  sta r_b0
  inc r_f0
@nof0:
  lda r_bit
  cmp #4
  bcc @nof1
  lda r_lt
  clc
  adc #1
  cmp #16
  bcs @nof1
  asl
  asl
  asl
  asl
  sta r_b1
  inc r_f1
@nof1:
  lda #0
  sta r_row
@row:
  ldy r_row
  lda (r_gp),y
  beq @rnext
  ora r_bo
  tay
  lda r_f0
  beq @t1
  lda _shr_tab,y
  beq @t1
  sta r_v
  lda r_b0
  clc
  adc r_row
  tax
  lda r_cl
  lsr
  bcc @a1
  lda r_v
  ora _stage,x
  sta _stage,x
@a1:
  lda r_cl
  and #2
  beq @t1
  lda r_v
  ora _stage+8,x
  sta _stage+8,x
@t1:
  lda r_f1
  beq @rnext
  lda _shl_tab,y
  beq @rnext
  sta r_v
  lda r_b1
  clc
  adc r_row
  tax
  lda r_cl
  lsr
  bcc @b1
  lda r_v
  ora _stage,x
  sta _stage,x
@b1:
  lda r_cl
  and #2
  beq @rnext
  lda r_v
  ora _stage+8,x
  sta _stage+8,x
@rnext:
  inc r_row
  lda r_row
  cmp #8
  bne @row
@next:
  lda r_x
  clc
  adc #5
  sta r_x
  inc r_i
  jmp @char

.include "font5.inc"

_hw_spill:
  pla
  sta sp_tmp
  pla
  sta sp_tmp+1
  tsx
  txa
  eor #$FF
  sec
  adc _hw_base
  sta sp_cnt
  ldy #0
  cmp #0
  beq @store
@cp:
  inx
  lda $0100,x
  sta (_spill_ptr),y
  iny
  cpy sp_cnt
  bne @cp
@store:
  lda sp_cnt
  sta (_spill_ptr),y
  sec
  lda _spill_ptr
  adc sp_cnt
  sta _spill_ptr
  bcc @nc
  inc _spill_ptr+1
@nc:
  ldx _hw_base
  txs
  lda sp_tmp+1
  pha
  lda sp_tmp
  pha
  rts

_hw_unspill:
  pla
  sta sp_tmp
  pla
  sta sp_tmp+1
  lda _spill_ptr
  bne @nb
  dec _spill_ptr+1
@nb:
  dec _spill_ptr
  ldy #0
  lda (_spill_ptr),y
  sta sp_cnt
  lda _spill_ptr
  sec
  sbc sp_cnt
  sta _spill_ptr
  bcs @nb2
  dec _spill_ptr+1
@nb2:
  lda _hw_base
  sec
  sbc sp_cnt
  tax
  txs
  ldy #0
  lda sp_cnt
  beq @done
@cp:
  inx
  lda (_spill_ptr),y
  sta $0100,x
  iny
  cpy sp_cnt
  bne @cp
@done:
  lda sp_tmp+1
  pha
  lda sp_tmp
  pha
  rts

NB_IDS  = 20
T_NUM   = $80
T_ID    = $82
V_NONE  = 5
_quick_operand:
  ldy #0
  lda (_p),y
  cmp #T_NUM
  bne @notnum
  iny
  lda (_p),y
  sta _vn
  iny
  lda (_p),y
  sta _vn+1
  lda #0
  sta _vt
  lda #3
  bne @adv
@notnum:
  cmp #T_ID
  beq @isid
  jmp @no
@isid:
  ldy #2
  lda (_p),y
  cmp #'('
  beq @no2
  cmp #'['
  beq @no2
  cmp #$B0               ; T_LENGTH..T_POP
  bcc @ok
  cmp #$B3
  bcs @ok
@no2:
  jmp @no
@ok:
  dey
  lda (_p),y             ; A = name id
  ldx _lcount
@loc:
  cpx _fbase
  beq @global
  dex
  cmp _lid,x
  bne @loc
  txa
  lsr
  lsr
  lsr
  sta qptr+1             ; x >> 3
  txa
  asl
  asl
  asl
  asl
  asl
  clc
  adc #<_lvar
  sta qptr
  lda qptr+1
  adc #>_lvar
  sta qptr+1
  jmp @have
@global:
  cmp #NB_IDS
  bcc @no
  asl
  tax
  lda _gptr,x
  sta qptr
  lda _gptr+1,x
  sta qptr+1
@have:
  ldy #0
  lda (qptr),y
  cmp #V_NONE
  beq @no                ; undeclared
  sta _vt
  iny
  lda (qptr),y
  sta _vn
  iny
  lda (qptr),y
  sta _vn+1
  clc
  lda qptr
  adc #3
  sta _vs
  lda qptr+1
  adc #0
  sta _vs+1
  lda #2
@adv:
  clc
  adc _p
  sta _p
  bcc @r1
  inc _p+1
@r1:
  lda #1
  ldx #0
  rts
@no:
  lda #0
  tax
  rts


.segment "VECTORS"
  .word nmi, reset, irq
