// walk the @ around with the d-pad
const UP = 8, DOWN = 4, LEFT = 2, RIGHT = 1
const B = 64
let x = 20, y = 12

cls()
put(0, 0, "d-pad: move  b: fast  select: stop")
while (true) {
  let p = pad()
  let nx = x, ny = y
  if (p & UP) ny--
  if (p & DOWN) ny++
  if (p & LEFT) nx--
  if (p & RIGHT) nx++
  nx = Math.min(49, Math.max(0, nx))
  ny = Math.min(27, Math.max(2, ny))
  if (nx != x || ny != y) {
    put(x, y, ".")
    x = nx
    y = ny
  }
  put(x, y, "@")
  wait(p & B ? 2 : 6)
}
