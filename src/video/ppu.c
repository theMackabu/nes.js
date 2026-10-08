static const unsigned char palette[32] = {0x0F, 0x30, 0x27, 0x3A, 0x0F, 0x00, 0x28, 0x10, 0x0F, 0x21, 0x10,
                                          0x31, 0x0F, 0x2A, 0x10, 0x00, 0x0F, 0x12, 0x10, 0x00, 0x0F, 0x06,
                                          0x10, 0x00, 0x0F, 0x30, 0x10, 0x00, 0x0F, 0x30, 0x10, 0x00};

#define FONT(c) ((unsigned char)((c) >= 32 && (c) <= 126 ? (c) + 96 : 128))
#define TILE_BLOCK 95
#define TILE_CURSOR 96

static unsigned char ppu_is_on, vqn;
static unsigned char nl;

void wait_frame(void) {
  unsigned char f;
  if (vqn) {
    vq[vqn] = 0;
    vq_ready = 1;
  }
  f = frame_count;
  while (f == frame_count)
    ;
  vqn = 0;
}

void vaddr(unsigned int a) {
  PPUADDR = a >> 8;
  PPUADDR = a & 0xFF;
}

void nt_write(unsigned int addr, const unsigned char *d, unsigned char len, unsigned char flags) {
  unsigned char i;
  if (!ppu_is_on) {
    PPUCTRL = flags & 0x40 ? 0x04 : 0;
    vaddr(addr);
    for (i = 0; i < len; ++i)
      PPUDATA = flags & 0x80 ? d[0] : d[i];
    PPUCTRL = 0;
    return;
  }
  if (vqn + (flags & 0x80 ? 1 : len) + 4 > sizeof(vq)) wait_frame();
  vq[vqn++] = addr >> 8;
  vq[vqn++] = addr & 0xFF;
  vq[vqn++] = len | flags;
  if (flags & 0x80) vq[vqn++] = d[0];
  else {
    memcpy(vq + vqn, d, len);
    vqn += len;
  }
}

void nt_text(unsigned char x, unsigned char y, const char *s) {
  unsigned char b[32], n = 0;
  while (*s && n < 32 - x) {
    b[n++] = FONT(*s);
    ++s;
  }
  nt_write(0x2000 + y * 32 + x, b, n, 0);
}

void hide_sprites(void) {
  unsigned char i = 0;
  do {
    OAM[i] = 0xFF;
    i += 4;
  } while (i);
}

void ppu_off(void) {
  if (ppu_is_on) wait_frame();
  ppu_ctrl = 0;
  PPUCTRL = 0;
  ppu_mask = 0;
  PPUMASK = 0;
  MMC3_IRQ_OFF = 0;
  ppu_is_on = 0;
}

void ppu_on(void) {
  ppu_ctrl = 0x88;
  ppu_mask = 0x1E;
  (void)PPUSTATUS;
  ppu_is_on = 1;
  PPUCTRL = 0x88;
  wait_frame();
}

void screen_begin(const unsigned char *nt) {
  unsigned char i;
  ppu_off();
  hide_sprites();
  memset(rowbank, 0, 30);
  vaddr(0x3F00);
  for (i = 0; i < 32; ++i)
    PPUDATA = palette[i];
  nt_blit(nt);
}

void bitmap_row(unsigned char r, unsigned char from) {
  unsigned char b[32], c, base = (r & 1) ? 64 : 0;
  for (c = from; c < 32; ++c)
    b[c] = base + c;
  nt_write(0x2000 + r * 32 + from, b + from, 32 - from, 0);
}
