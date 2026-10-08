#define L_STALE 1
#define L_CHANGED 2
#define SLOT_NONE 0xFF
#define SLOT_EMPTY 0xFE
#define SLOT_BUSY 0xFE

#define K_SPACE 0x80
#define K_ENTER 0x81
#define K_SHIFT 0x82
#define K_LET 0x83
#define K_FUNC 0x84
#define K_IF 0x85
#define K_ELSE 0x86
#define K_FOR 0x87
#define K_WHILE 0x88
#define K_RET 0x89
#define K_LOG 0x8A

#define KROW0 20
#define KROWS 7
#define CODE_X0 19
#define STATUS_JOB 0xFD
#define PREFETCH 10
static unsigned char scroll_up;
#define PF_BELOW (scroll_up ? 1 : PREFETCH - 1)
#define PF_ABOVE (scroll_up ? PREFETCH - 1 : 1)
static char status_txt[8];
static unsigned char status_dirty;
void status_prepare(void);

static const unsigned char keys[KROWS][13] = {
  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm'},
  {'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'},
  {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', '"'},
  {'+', '-', '*', '/', '%', '=', '<', '>', '!', '&', '|', '_', '\''},
  {'(', ')', '{', '}', '[', ']', ';', ':', '?', '#', '$', K_IF, K_IF},
  {K_SPACE, K_SPACE, K_SPACE, K_ENTER, K_ENTER, K_ENTER, K_SHIFT, K_SHIFT, K_LET, K_LET, K_FUNC, K_FUNC, K_FUNC},
  {K_ELSE, K_ELSE, K_ELSE, K_FOR, K_FOR, K_WHILE, K_WHILE, K_WHILE, K_RET, K_RET, K_LOG, K_LOG, K_LOG}
};
static const char *const klabel[] =
  {"SPACE", "ENTER", "ABC", "let", "func", "if", "else", "for", "while", "ret", "log"};
static const char *const kinsert[] =
  {" ", "", "", "let ", "function ", "if (", "else ", "for (", "while (", "return ", "print("};

static unsigned char cx, cy, top, kx, ky, shift, blink;
#include "screens_edit.h"
