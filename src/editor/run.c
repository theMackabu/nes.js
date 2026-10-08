void run_program_x(void);

void run_and_view(void) {
  run_program_x();
  for (;;) {
    wait_frame();
    video_service();
    read_pad();
    if (pad_new & P_START) run_program_x();
    if (pad_new & P_B) break;
  }
}
