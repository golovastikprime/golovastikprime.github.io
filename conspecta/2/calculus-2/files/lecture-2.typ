#import "../preambule.typ": *

#lecture[
  #corollary[
    $ f,g : [a,b]->RR, f in cal(R) and g = f|_([a,y]\/Y) , Y={y_i}_(i=1)^m \ => exists ing g(x) d x = ing f(x) d x $
  ]
  #let tl = $tau, xi$
  #proof[
    $tau$ - дробление $[a,b]$, $xi$ - замещение $tau$

    $ sigma(g, tl) - sigma(f, tl) = sum_(i=1)^m (g(xi_i) - f(xi_i))mu(Delta_i) + sum_(i:Delta_i inter Y != emptyset) + sum_(i : Delta_i inter Y = emptyset ) $

    $ f in cal(R) => |f| <= C_f => |g|<= dfrac(
      max(C_f, |g(Y)|), C_g
    )\
    |sigma(g, tl) - sigma(f, tl)| <+ sum (|g(xi_i)| + |f(xi_i)|) mu(Delta_i) <= (C_f + C_g) |tau| sum_(i : Delta_i inter Y = emptyset ) <= 2m (C_f + C_g)|tau| + epsilon/2 \

    frl epsilon > 0 exists delta > 0 : frl tau, xi |tau| < delta => |sigma(f, tl) - I| < epsilon/2 \
    2m(C_f + C_g)|tau| < epsilon/2
    $

    Как только $|tau| < dfrac(epsilon, 4m(C_f + C_g))=>$
    В качестве $delta$ берем $delta' = min(delta, dfrac(epsilon, 4m(C_f+C_g)))$
  ]

  #corollary[$f in cal(R)[a,b], [alpha, beta] subset [a,b] => f in cal(R)[alpha, beta] $]
  #proof[$ f in B[a,b] and f in cal(R)[a,b] <=> frl epsilon exists tau: omega(f, tau) < epsilon and omega(f, tau) = sum_i^n omega(f, Delta_i)  mu(Delta_i) $
  #let tba = $tau_(a b)$
  #let tab = $tau_(alpha beta)$
  
  $ frl epsilon >0 : quad &exists tau_(a b) : omega(f, tba) < epsilon\
              &exists tab : omega(f, tab) < epsilon $

  Построим $tba' = tba union {alpha, beta} => omega(f, tba') <= omega(f, tba) < epsilon$

  $ tba' : a = x_0' < x_1' < ... < x_n' = b $

  Найдутся такие $k, m$, что выполнено условие $alpha = x'_k, beta = x'_m$

  $ tab : alpha = x'_k < ... < x'_m = beta \ 
    omega(f, tab) = sum_(i=k)^m omega(f, Delta_i) mu(Delta_i) <= sum_(i=1)^n omega(f, Delta_i) mu(Delta_i) < epsilon
  $]

#pagebreak()

  
  #align(
  box([
    Давайте введем соглашение

    
    - $a, b in RR$
    - $[a,b] = {x in RR : min(a,b) <= x <= max(a,b)}$
    - $f in cal(R) [2,1] <=> f in cal(R)[1,2]$
    - $f in cal(R)[a,a]$
    - $ ing f(x) d x = cases(ing f ", "a<b,0 ", a=b" , -ing  "if" b<a, ) $
    - $ integral.dash_a^b f(x) d x = integral_min(a,b)^max(a,b) f(x) d x, space integral.dash_a^a f(x)d x = 0 $
  ], stroke: black, inset: 1em), center)
  #align(center)[
  #pagebreak()
  #text(size: 1.8em)[Аддитивность интеграла по отрезку]

  #corollary[$ Let c in RR, f in cal(R)[a,b] and f cal(R)[b, c] => f in cal(R)[a,c] and ing + integral_b^c = integral_a^c $]
  #proof[
    #let tab = $tau_(a b)$
    #let tbc = $tau_(b c)$
    $ f in B[a, b] and f in B[b, c] => f in B[a, c] $
    $ f in cal(R)[a,c] <=> I^*(a,c) = I_*(a,c) $
    $ frl epsilon >0space &exists tau_(a b) : s (f, tab) > I_*(a,b) - epsilon/2 \ 
    &exists tbc : s(f, tbc) > I_*(b,c) - epsilon/2 $

    Давайте рассмотрим следующие разбиение $tau_(a c) := tab union tbc$
  #let tac = $tau_(a c)$
    $ s(f, tac ) = s(f, tab) + s(f, tbc) > I(a,b) + I(b,c) - epsilon 
    \ => I_*(a, c) = sup_tau s(f, tau) >= s(f, tac) 
    \ => I_*(a, c) >= I(a, b) + I(b, c) $

    Аналогично для $I* => I_*(a, c) >= I(a,b) + I(b,c) >= I^*(a,c)$

    Но $I^* >= I_* => I(a,c) = I(a,b)+I(b,c)$
  ]

  #theorem(name: "О среднем")[
    $f,g in cal(R)[a,b], g >=0 =>$

    $ exists mu in [m_f, M_f] : ing f(x)g(x) d x = mu ing g(x) d x $
  ]
  #proof[
    $m_f <= f(x) <= M_f => m_f g(x) <= f(x) g(x) <= M_f g(x)$

    $ ing m_f g <= ing f g ing M_f g $
    
    $ "if" I_g != 0 =>  mu = dfrac(ing f g, I_g)," otherwise all good" $
  ]
  
]

#pagebreak()

#align([#text("Интеграл с верхним и нижним пределом", size: 19pt)], center)




]




