void fix_view(void) {
  unsigned char l, ot = top;
  if (cy >= nl) cy = nl - 1;
  if (cy < top) {
    top = cy;
    scroll_up = 1;
  } else if (cy >= top + VIEW) {
    top = cy - VIEW + 1;
    scroll_up = 0;
  }
  if (top != ot) {
    map_editor();
    draw_gutter();
  }
  l = line_len(cy);
  if (cx > l) cx = l;
  draw_status();
  blink = 0;
}

void touch(void) {
  modified = 1;
}
