static const unsigned char title_tiles[6] = {225, 226, 227, 228, 229, 227};

void draw_gutter(void) {
  unsigned char t[VIEW], o[VIEW], r, n;
  for (r = 0; r < VIEW; ++r) {
    n = top + r + 1;
    if (n > nl) {
      t[r] = o[r] = 128;
      continue;
    }
    t[r] = 230 + n / 10;
    o[r] = 240 + n % 10;
  }
  nt_write(0x2000 + EROW * 32, t, VIEW, 0x40);
  nt_write(0x2000 + EROW * 32 + 1, o, VIEW, 0x40);
}

void draw_status(void) {
  char *s = status_txt;
  s[0] = 'L';
  s[1] = '0' + (cy + 1) / 10;
  s[2] = '0' + (cy + 1) % 10;
  s[3] = ' ';
  s[4] = 'C';
  s[5] = '0' + (cx + 1) / 10;
  s[6] = '0' + (cx + 1) % 10;
  s[7] = 0;
  status_dirty = 1;
}

void status_prepare(void) {
  memcpy(jobbuf, status_txt, 7);
  job_len = 7;
  job_x0 = 216;
  job_cls = 0;
  job_clsk = 1;
  job_base = 0;
  job_slot = STATUS_SLOT;
  job_line = STATUS_JOB;
  status_dirty = 0;
}
