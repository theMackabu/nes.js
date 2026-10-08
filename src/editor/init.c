void editor_init(void) {
  cache_ok = 0;
  if (memcmp(sig, "NJ03", 4)) {
    prog[0] = 0;
    modified = 0;
    memcpy(sig, "NJ03", 4);
  }
  prog_index();
}
