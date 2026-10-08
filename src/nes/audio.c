void beep(unsigned int period) {
  if (period < 8) period = 8;
  APU(0) = 0x9F;
  APU(1) = 0x08;
  APU(2) = period & 0xFF;
  APU(3) = ((period >> 8) & 7) | (2 << 3);
}
