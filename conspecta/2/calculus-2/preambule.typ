#import "@preview/lemmify:0.1.8": *

#set heading(numbering: "1.1")

#let lecture-counter = counter("lecture")

#let dfrac(a, b) = $display(frac(#a, #b))$
#let dsum = $display(sum)$

#let frl = $forall$
#let ing = $display(integral_a^b)$
#let Let = "Пусть"
#let undertext(a, b) = $op(#a, limits: #true)_(#b)$

#let dx = $dif x$
#let dt = $dif t$
#let CR = $cal(R)$
#let int(a,b) = $display(integral_#a^#b)$
#let evaluated(expr, size: 100%) = $lr(#expr|, size: #size)$
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

#let theme(txt) = align(center)[== #txt

 #v(3em)
]

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