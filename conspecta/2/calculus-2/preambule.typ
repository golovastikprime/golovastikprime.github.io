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
  remark, example,
  rules: thm-rules
) = default-theorems(
  "thm-group",
  thm-numbering: thm-numbering-heading.with(max-heading-level: 2),
  lang: "ru"
  //thm-styling: my-theorem-style
)

#let (
  proof,
  definition,proposition,
  rules: my-rules
) = new-theorems(
  "my-group",
  (
    proposition : "Утв",
    proof: "Док-во:",
    definition: "Опр"
  ),
)
#let proof = proof.with(numbering: none)

#let theme(txt) = align(center)[== #txt

 #v(1.5em)
]

#let lecture(body) = {
  set page(
  paper: "a4",
  margin: (x: 1.7cm, y: 1.5cm))
  set text( size: 11pt)
  show: thm-rules
  show: my-rules
  
  lecture-counter.step()
  let lecture-heading = heading(level: 1, numbering: "1")[Лекция]
  lecture-heading
  body
  pagebreak()
}