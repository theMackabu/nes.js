// guess the number game
let secret = rand(100) + 1
let tries = 0, guess = 0
print("i'm thinking of a number, 1 to 100.")
print("d-pad up/down: +-1, left/right: +-10")
print("a: submit")
while (guess != secret) {
  guess = input("your guess")
  tries++
  if (guess < secret) print("  higher!")
  else if (guess > secret) print("  lower!")
}
print("got it in", tries, "tries!")
beep(200)
