#import "@preview/lemmify:0.1.8": *
#import "@preview/chemformula:0.1.2": *

#set heading(numbering: "1.1")

#let theme(txt) = align(center)[== #txt
#v(2em)
]

#let lecture-counter = counter("lecture")

#let dfrac(a, b) = $display(frac(#a, #b))$
#let dsum = $display(sum)$

#let frl = $forall$
#let ing = $display(integral_a^b)$
#let Let = "Пусть"
#let undertext(a, b) = $op(#a, limits: #true)_(#b)$
#let hbar = text(font: "Linux Libertine", style: "italic")[\u{0127}]

#let dx = $dif x$
#let dy = $dif y$
#let dz = $dif z$
#let dV = $dif V$
#let dr = $dif r$
#let pm = $plus.minus$

#let create_image(img, cap) = figure(
  img,
  caption: [#cap],
  supplement: "Рис."
)

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

#let Be = "Be"
#let Ga = "Ga"
#let Ge = "Ge"
#let As = "As"
#let Se = "Se"
#let Br = "Br"
#let Kr = "Kr"
#let Tl = "Tl"
#let Pb = "Pb"
#let Bi = "Bi"
#let Po = "Po"
#let At = "At"
#let Rn = "Rt"
#let Zn = "Zn"
#let Zr = "Zr"
#let Hf = "Hf"
#let Sn = "Sn"
#let ns = "ns"
#let np = "np"
#let Cl = "Cl"
#let NH = "NH"
#let BF = "BF"
#let sp = "sp"
#let dsp = "dsp"