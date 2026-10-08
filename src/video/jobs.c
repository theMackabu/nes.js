static unsigned char job_base, job_slot = 0xFF, job_half, job_gen, job_line, job_len, job_x0, job_clsk;
static char jobbuf[LOGW + 1];
static unsigned char jobcls[LOGW + 1];
static const unsigned char *job_cls;

void job_render(void) {
  r_text = (const unsigned char *)jobbuf;
  r_len = job_len;
  r_x0 = job_x0;
  r_half = job_half;
  r_cls = job_cls;
  r_clsk = job_clsk;
  render_half();
}

#define INK_NONE 0xF0
#pragma bss-name(push, "LOGTXT")
static unsigned char s_valid[NSLOT], s_dirty[NSLOT], s_gen[NSLOT];
#pragma bss-name(pop)
static unsigned char s_cls[NSLOT], s_line[NSLOT];

unsigned char ink_range(unsigned char h) {
  unsigned char i, xs, xe, t0, t1, lo = 0xFF, hi = 0, base = h ? 128 : 0;
  for (i = 0; i < job_len; ++i) {
    if ((unsigned char)jobbuf[i] <= ' ') continue;
    xs = job_x0 + i * 5;
    xe = xs + 3;
    if (xe < base || xs > base + 127) continue;
    t0 = xs < base ? 0 : (xs - base) >> 3;
    t1 = (xe - base) >> 3;
    if (t1 > 15) t1 = 15;
    if (t0 < lo) lo = t0;
    if (t1 > hi) hi = t1;
  }
  return lo == 0xFF ? INK_NONE : (lo << 4) | hi;
}

unsigned char job_step(void) {
  unsigned char nw = ink_range(job_half), old = s_ink[job_base + job_half][job_slot], lo, hi;
  if (nw == INK_NONE && old == INK_NONE) return 0;
  s_ink[job_base + job_half][job_slot] = nw;
  if (nw == INK_NONE) {
    lo = old >> 4;
    hi = old & 15;
  } else if (old == INK_NONE) {
    lo = nw >> 4;
    hi = nw & 15;
  } else {
    lo = nw >> 4;
    if ((old >> 4) < lo) lo = old >> 4;
    hi = nw & 15;
    if ((old & 15) > hi) hi = old & 15;
  }
  lo &= 0xFE;
  hi |= 1;
  job_render();
  up_bank = SLOTBANK(job_slot);
  up_hi = job_base + job_half;
  up_off = lo << 4;
  stage_busy = 1;
  up_left = (hi - lo + 1) >> 1;
  return 1;
}

void static_line(unsigned char bank, unsigned char base, const char *text) {
  job_len = strlen(text);
  memcpy(jobbuf, text, job_len);
  job_x0 = 3;
  job_cls = 0;
  job_clsk = 1;
  for (job_half = 0; job_half < 2; ++job_half) {
    job_render();
    up_hi = base + job_half;
    upload_now(bank);
  }
}

void job_now(void) {
  for (job_half = 0; job_half < 2; ++job_half) {
    s_ink[job_base + job_half][job_slot] = ink_range(job_half);
    job_render();
    up_hi = job_base + job_half;
    upload_now(SLOTBANK(job_slot));
  }
  job_slot = 0xFF;
}
