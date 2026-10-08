// twinkle twinkle little star
let scale = [427, 380, 338, 319,
             284, 253, 226, 213]
let names = "CDEFGABc"
let song = [0, 0, 4, 4, 5, 5, 4,
            3, 3, 2, 2, 1, 1, 0]
let tune = ""

cls()
put(2, 1, "now playing:")
let i = 0
for (let note of song) {
  beep(scale[note])
  tune += names[note] + " "
  put(2, 3, tune)
  i++
  wait(i % 7 == 0 ? 30 : 14)
}
put(2, 5, "the end!")
