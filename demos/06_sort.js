// bubble sort, pass by pass
let a = []
for (let i = 0; i < 8; i++) a.push(rand(90)+10)
print("start:  ", a)

let swapped = true, pass = 0
while (swapped) {
  swapped = false
  for (let i = 1; i < a.length; i++) {
    if (a[i - 1] > a[i]) {
      let t = a[i]
      a[i] = a[i - 1]
      a[i - 1] = t
      swapped = true
    }
  }
  pass++
  print("pass", pass + ": ", a)
}
print("sorted!")
