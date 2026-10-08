// game of life: a glider on a 12x10 world
const W = 12, H = 10
let g = Array(W * H)
let n = Array(W * H)
g[1] = 1
g[W + 2] = 1
g[W * 2] = 1
g[W * 2 + 1] = 1
g[W * 2 + 2] = 1

cls()
for (let gen = 0; gen < 40; gen++) {
  put(0, 0, "generation " + gen)
  draw()
  step()
}

function draw() {
  for (let y = 0; y < H; y++) {
    let row = ""
    for (let x = 0; x < W; x++) {
      row += g[y * W + x] ? "#" : "."
    }
    put(0, y + 2, row)
  }
}

function step() {
  for (let i = 0; i < W * H; i++) n[i] = 0
  for (let y = 0; y < H; y++) {
    for (let x = 0; x < W; x++) {
      if (g[y * W + x]) bump(x, y)
    }
  }
  for (let i = 0; i < W * H; i++) {
    let c = n[i]
    g[i] = c == 3 || (c == 2 && g[i])
  }
}

function bump(x, y) {
  for (let dy = -1; dy <= 1; dy++) {
    let row = (y + dy + H) % H * W
    for (let dx = -1; dx <= 1; dx++) {
      if (dx || dy) n[row + (x + dx + W) % W]++
    }
  }
}
