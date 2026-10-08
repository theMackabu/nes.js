static unsigned char vmode;
void editor_finish(void);
unsigned char editor_next(void);
void log_finish(void);
unsigned char log_next(void);

void video_service(void) {
  unsigned char q;
  if (stage_busy) return;
  for (;;) {
    if (job_slot == 0xFF) {
      if (!(vmode == 1 ? editor_next() : vmode == 2 ? log_next() : 0)) return;
      job_half = 0;
    } else if (job_half == 2) {
      if (vmode == 1) editor_finish();
      else log_finish();
      job_slot = 0xFF;
    } else {
      q = job_step();
      ++job_half;
      if (q) return;
    }
  }
}
