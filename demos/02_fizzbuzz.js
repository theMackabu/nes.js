// the classic fizzbuzz
for (let i = 1; i <= 30; i++) {
  let out = ""
  if (i % 3 == 0) out += "fizz"
  if (i % 5 == 0) out += "buzz"
  if (out == "") out = i
  print(out)
}
