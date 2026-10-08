from pathlib import Path
import sys

root = Path(__file__).resolve().parent.parent
outdir = Path(sys.argv[1]) if len(sys.argv) > 1 else root / 'build'
outdir.mkdir(parents=True, exist_ok=True)
FONT = lambda c: ord(c) + 96 if 32 <= ord(c) <= 126 else 128
EROW, VIEW, KROW0 = 3, 16, 20

keys = ["abcdefghijklm", "nopqrstuvwxyz", "0123456789.,\"", "+-*/%=<>!&|_'", "(){}[];:?#$"]
special = [["SPACE", 0], ["ENTER", 3], ["ABC", 6], ["let", 8], ["func", 10]], [["else", 0], ["for", 3], ["while", 5], ["ret", 8], ["log", 10]]

def kpos(c): return 3 + c * 2
def attr(editor):
    a = [0] * 64
    for y in range(15):
        for x in range(16):
            pal = 0
            if y == 0: pal = 1
            elif editor and y <= 9 and x == 0: pal = 1
            elif editor and 10 <= y <= 13: pal = 2
            elif y >= 13: pal = 3 if editor else 0
            a[(y >> 1) * 8 + (x >> 1)] |= pal << (((y & 1) << 2) | ((x & 1) << 1))
    return a

def text(nt, x, y, s):
    for i, c in enumerate(s): nt[y * 32 + x + i] = FONT(c)

ed = [128] * 960
for i, t in enumerate([225, 226, 227, 228, 229, 227]): ed[32 + 2 + i] = t

for r in range(VIEW):
    row = EROW + r
    for c in range(2, 32): ed[row * 32 + c] = (64 if row & 1 else 0) + c

for r, k in enumerate(keys):
    for c, ch in enumerate(k): ed[(KROW0 + r) * 32 + kpos(c)] = FONT(ch)
text(ed, kpos(11), KROW0 + 4, "if")

for r, row in enumerate(special):
    for label, c in row: text(ed, kpos(c), KROW0 + 5 + r, label)

for c in range(23, 32): ed[32 + c] = 64 + c
for c in range(32): ed[28 * 32 + c] = c
for c in range(32): ed[29 * 32 + c] = 96 + c

ed += attr(True)
lg = [128] * 960
for row in range(1, 29):
    for c in range(32): lg[row * 32 + c] = (64 if row & 1 else 0) + 32 + c
lg += [0] * 64

def arr(name, data, seg):
    body = ",".join(map(str, data))
    return f'#pragma rodata-name(push, "{seg}")\nstatic const unsigned char {name}[1024] = {{{body}}};\n#pragma rodata-name(pop)\n'

(outdir / "screens_edit.h").write_text(arr("editor_nt", ed, "ERODATA"))
(outdir / "screens_log.h").write_text(arr("log_nt", lg, "RODATA"))
print("ok", len(ed), len(lg))
