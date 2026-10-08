void key_span(unsigned char *s, unsigned char *e) {
  unsigned char k = keys[ky][kx];
  *s = *e = kx;
  if (k < 0x80) return;
  while (*s && keys[ky][*s - 1] == k)
    --*s;
  while (*e < 12 && keys[ky][*e + 1] == k)
    ++*e;
}

void update_sprites(void) {
  unsigned char s, e, k, n, i, x, y;
  unsigned char *o;
  key_span(&s, &e);
  k = keys[ky][kx];
  n = (k >= 0x80) ? strlen(klabel[k - 0x80]) : 1;
  x = kpos(s) * 8;
  y = (KROW0 + ky) * 8 - 1;
  for (i = 0; i < 6; ++i) {
    o = OAM + i * 4;
    if (i < n) {
      o[0] = y;
      o[1] = TILE_BLOCK;
      o[2] = 0x20;
      o[3] = x + i * 8;
    } else o[0] = 0xFF;
  }
  ++blink;
  o = OAM + 6 * 4;
  if (blink & 0x10) o[0] = 0xFF;
  else {
    o[0] = (EROW + cy - top) * 8 - 1;
    o[1] = TILE_CURSOR;
    o[2] = 0x21;
    o[3] = CODE_X0 + cx * 5;
  }
}

void press_key(void) {
  unsigned char k = keys[ky][kx], i;
  const char *l;
  char one[2];
  if (k == K_ENTER) {
    do_enter();
    return;
  }
  if (k == K_SHIFT) {
    shift ^= 1;
    draw_keyboard_letters();
    return;
  }
  if (k >= 0x80) insert_text(kinsert[k - 0x80]);
  else {
    if (k == '}') {
      l = line_ptr(cy);
      for (i = 0; i < cx && l[i] == ' '; ++i)
        ;
      if (i == cx && cx >= 2) delete_before(2);
    }
    if (k >= 'a' && k <= 'z' && shift) k -= 32;
    one[0] = k;
    one[1] = 0;
    insert_text(one);
  }
  fix_view();
}
