void __fastcall__ map_code(unsigned char ovl);
#define OVL_INTERP 0
#define OVL_EDIT 1

void run_program(void);
unsigned char menu(void);

void editor(void);
void run_and_view(void);
void editor_init(void);

void run_program_x(void) {
  map_code(OVL_INTERP);
  run_program();
  map_code(OVL_EDIT);
}

void main(void) {
  APU(0x15) = 0x01;
  memset(s_ink, INK_NONE, sizeof(s_ink));

  static_line(HELP_BANK, 0, "B:BKSP  SEL+PAD:MOVE  START:RUN  SEL+START:MENU");
  static_line(HELP_BANK, 2, "SEL+B:DEL LINE   A:TYPE   ABC:CAPS");

  map_code(OVL_EDIT);
  editor_init();

  for (;;) {
    if (menu()) run_and_view();
    editor();
  }
}
