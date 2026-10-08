static unsigned char lrs[LROWS + 1];
static unsigned char lrow, lcol;
static char *lptr;
static unsigned char log_any, log_hint = 0xFF, log_hint2 = 0xFF;

void map_log(void) {
  unsigned char r, s;
  for (r = 1; r <= LROWS; ++r) {
    s = lrs[r];
    rowbank[r] = s_valid[s] ? SLOTBANK(s) : 0;
  }
}

void slot_clear(unsigned char s) {
  memset(logtxt[s], 0, LOGW);
  s_valid[s] = 0;
  s_dirty[s] = 0;
  s_cls[s] = 1;
  ++s_gen[s];
}

void log_cls(void) {
  unsigned char r;
  for (r = 1; r <= LROWS; ++r) {
    lrs[r] = r - 1;
    slot_clear(r - 1);
  }
  lrow = 1;
  lcol = 0;
  lptr = logtxt[lrs[1]];
  map_log();
}

#include "screens_log.h"

void log_screen(void) {
  screen_begin(log_nt);
  vmode = 2;
  job_slot = 0xFF;
  log_cls();
  ppu_on();
}
