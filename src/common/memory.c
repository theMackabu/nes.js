#pragma bss-name(push, "SAVE")
char sig[4];
unsigned char modified;
char prog[PROGN];
#pragma bss-name(pop)

#pragma bss-name(push, "LOGTXT")
char logtxt[NSLOT][LOGW];
unsigned int loff[MAXL + 1];
unsigned char lslot[MAXL];
unsigned char lflag[MAXL];
#pragma bss-name(pop)

struct var {
  unsigned char t;
  int n;
  char s[SLEN + 1];
};
struct arr {
  unsigned int start;
  unsigned char len, cap;
};

#pragma bss-name(push, "RUNRAM")
unsigned char toks[TOKN];
int heap[HEAPN];
struct var globals[MAXG];
struct var lvar[MAXLOC];
unsigned char lid[MAXLOC];

unsigned char s_ink[4][NSLOT];
#pragma bss-name(pop)

#pragma bss-name(push, "ZEROPAGE")
const unsigned char *p;
unsigned char vt;
int vn;
const char *vs;
#pragma bss-name(pop)
#pragma zpsym("p")
#pragma zpsym("vt")
#pragma zpsym("vn")
#pragma zpsym("vs")
