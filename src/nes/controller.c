unsigned char pad, pad_old, pad_new, rpt;

unsigned char raw_pad(void) {
  unsigned char i, q = 0;
  JOY1 = 1;
  JOY1 = 0;
  for (i = 0; i < 8; ++i)
    q = (q << 1) | (JOY1 & 1);
  return q;
}

void read_pad(void) {
  static unsigned char lastf;
  unsigned char d = frame_count - lastf;
  lastf = frame_count;
  pad_old = pad;
  pad = raw_pad();
  pad_new = pad & ~pad_old;
  if (pad & 0x0F) {
    if (pad_new & 0x0F) rpt = 18;
    else if (rpt <= d) {
      rpt = 4;
      pad_new |= pad & 0x0F;
    } else rpt -= d;
  }
}
