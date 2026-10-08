// collatz: which start takes the most steps?
let best = 0
for (let n = 1; n < 200; n++) {
  let x = n, steps = 0
  while (x != 1) {
    if (x % 2) x = 3 * x + 1
    else x = x / 2
    steps++
  }
  if (steps > best) {
    best = steps
    print("new record:", n, "takes", steps)
  }
}
print("done!")
