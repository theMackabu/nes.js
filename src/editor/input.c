void editor(void) {
  unsigned char s, e;
  if (cy >= nl) cy = nl - 1;
  top = cy >= VIEW ? cy - VIEW + 1 : 0;
  enter_editor();
  fix_view();
  for (;;) {
    read_pad();
    if (pad & P_SELECT) {
      if ((pad_new & P_UP) && cy) {
        --cy;
        fix_view();
      }
      if ((pad_new & P_DOWN) && cy + 1 < nl) {
        ++cy;
        fix_view();
      }
      if (pad_new & P_LEFT) {
        if (cx) --cx;
        else if (cy) {
          --cy;
          cx = LW;
        }
        fix_view();
      }
      if (pad_new & P_RIGHT) {
        if (cx < line_len(cy)) ++cx;
        else if (cy + 1 < nl) {
          ++cy;
          cx = 0;
        }
        fix_view();
      }
      if (pad_new & P_B) {
        delete_line(cy);
        fix_view();
      }
      if (pad_new & P_START) return;
    } else {
      key_span(&s, &e);
      if (pad_new & P_UP) ky = ky ? ky - 1 : KROWS - 1;
      if (pad_new & P_DOWN) ky = ky < KROWS - 1 ? ky + 1 : 0;
      if (pad_new & P_LEFT) kx = s ? s - 1 : 12;
      if (pad_new & P_RIGHT) kx = e < 12 ? e + 1 : 0;
      if (pad_new & P_A) press_key();
      if (pad_new & P_B) {
        do_backspace();
        fix_view();
      }
      if (pad_new & P_START) {
        run_and_view();
        enter_editor();
        fix_view();
        continue;
      }
    }
    update_sprites();
    video_service();
    wait_frame();
  }
}
