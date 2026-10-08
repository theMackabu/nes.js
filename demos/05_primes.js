// sieve of eratosthenes
const N = 180
let sieve = Array(N)
let found = []
for (let i = 2; i < N; i++) {
  if (sieve[i]) continue
  found.push(i)
  for (let j = i*2; j < N; j += i) sieve[j] = 1
}
print(found.length, "primes below", N + ":")
let line = ""
for (let p of found) {
  line += p + " "
  if (line.length > 24) {
    print(line)
    line = ""
  }
}
if (line != "") print(line)
