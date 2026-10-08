enum {
  B_LOG,
  B_PRINT,
  B_INPUT,
  B_STRING,
  B_NUMBER,
  B_PARSEINT,
  B_RAND,
  B_ABS,
  B_MIN,
  B_MAX,
  B_FLOOR,
  B_ROUND,
  B_SQRT,
  B_PAD,
  B_BEEP,
  B_WAIT,
  B_CLS,
  B_PUT,
  B_ARRAY,
  B_FRAMES,
  NB
};
static const char *const bnames[NB] = {"console.log", "print",      "input",     "String",   "Number",
                                       "parseInt",    "rand",       "Math.abs",  "Math.min", "Math.max",
                                       "Math.floor",  "Math.round", "Math.sqrt", "pad",      "beep",
                                       "wait",        "cls",        "put",       "Array",    "frames"};

static jmp_buf errjmp;
unsigned char lcount, fbase;
static unsigned char ctrl, depth, nnames, nlines, ssp, poolidx, lastf;
static unsigned char tokenizing, tline, narr;
static unsigned int heaptop;
static unsigned int rng = 0xACE1;
static const char *nameptr[NB + MAXG];
static unsigned char namelen[NB + MAXG];
static unsigned int linetok[MAXL];
static const unsigned char *funcpos[MAXG];
struct var *gptr[NB + MAXG];
static struct arr arrs[MAXA];
static char pool[NPOOL][SLEN + 1];
static char sstk[128];
static int av[3];
static char errbuf[32];
