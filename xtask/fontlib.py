import re
from pathlib import Path
src=(Path(__file__).resolve().parent.parent / 'assets' / 'fonts' / 'font8x8_basic.h').read_text()
rows=re.findall(r'\{\s*((?:0x[0-9A-Fa-f]{2},?\s*){8})\}', src)
IBM=[[int(x,16) for x in re.findall(r'0x[0-9A-Fa-f]{2}', r)] for r in rows]
def thin_row(r):
    out = 0; x = 0
    while x < 8:
        if r >> x & 1:
            s = x
            while x < 8 and r >> x & 1: x += 1
            n = x - s
            if n == 2: out |= 1 << s
            else:
                for k in range(s, x): out |= 1 << k
        else: x += 1
    return out
def thin(g): return [thin_row(r) for r in g]
