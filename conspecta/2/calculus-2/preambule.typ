#import "@preview/lemmify:0.1.8": *

#set heading(numbering: "1.1")

#let lecture-counter = counter("lecture")

#let dfrac(a, b) = $display(frac(#a, #b))$
#let dsum = $display(sum)$
#let Let = "Пусть"
#let undertext(a, b) = $op(#a, limits: #true)_(#b)$

#box()
#let (
  theorem, lemma, corollary,
  remark, proposition, example,
  definition,
  proof, rules: thm-rules
) = default-theorems(
  "thm-group",
  thm-numbering: thm-numbering-heading.with(max-heading-level: 2),
  //thm-styling: my-theorem-style
)
 
#let lecture(body) = {
  set enum(indent: 2em)
  show math.equation: set pad(left: 2em)
  set math.equation(number-align: left)
  show: thm-rules
  show thm-selector("thm-group", subgroup: "theorem"): it => box(
    it,
    inset: 1em
  )
  
  lecture-counter.step()
  let lecture-heading = heading(level: 1, numbering: "1")[Лекция]
  lecture-heading
  body
  pagebreak()
}