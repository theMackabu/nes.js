// recursion + a stopwatch
function fib(n) {
  if (n < 2) return n
  return fib(n - 1) + fib(n - 2)
}

let start = frames()
for (let i = 0; i <= 14; i++) {
  print("fib(" + i + ") =", fib(i))
}
print("took", frames() - start, "frames")
