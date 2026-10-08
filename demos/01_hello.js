// a quick tour of nes.js
let lang = "nes"
lang += ".js"
console.log("hello from", lang)

let a = 6, b = 7
print("a * b =", a * b, "  big?", a * b > 40)

let s = "8-bit"
print(s, "is", s.length, "chars, starts", s[0])

function shout(text) {
  return text.length > 3 ? text + "!!" : text
}
print(shout("yay"), shout("hooray"))

let list = [3, 1, 4, 1, 5]
list.push(9)
print("list:", list, " length:", list.length)

for (let i = 1; i <= 3; i++) {
  let kind = i % 2 ? "odd" : "even"
  print(i, "is", kind)
}
