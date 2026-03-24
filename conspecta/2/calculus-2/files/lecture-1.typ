// lecture-1.typ
#import "../preambule.typ" : * 



 
#lecture[
  #theorem[$f in B([a,b ]) <=>$
  //text-indent: 3em 

  
    + $f in cal(R)[a,b]$
    + $forall epsilon>0 space exists tau : S(f, tau) - s(f, tau) < epsilon$
    + $I_* = I^*$ (В этом случае $I_*=I^*=I$)
  ]

  #proof[
    $1) => 2) Let epsilon > 0, "выберем" tau : $
    
    $ forall xi space |sigma(f, tau, xi) - I| < dfrac(epsilon, 4) $
    
    $ forall i =1,..,n exists xi_i in Delta_i : f(xi_i) <  undertext(inf, Delta_i)f + dfrac(epsilon, 4(b-a))  \
     => sigma(f, tau, xi) - S(f, tau) = dsum_i (f(xi_i)-inf_Delta_i f) |Delta_i| < dfrac(epsilon, 4(b-a)) dsum_i $

     $ => |s(f,tau) = I) <= |s(f, tau) - I +sigma - sigma| <= |s(t, tau - sigma)| + |sigma - I| < epsilon/4 + epsilon/4 = epsilon/2 $

     Аналогично $|S(f, tau) - I| < dfrac(epsilon, 2) => S(f, tau) -s(f,tau) < epsilon$

    $2)=>3)$

     $  I^* <= S(f, tau) < underbrace(s(f, tau), sup_(tau') s(t, tau')=I_*) + epsilon <= I_*+epsilon $

     Так как $I^* >= I_* => I^*=I_*$

     $3)=>1)$

     $ s(f, tau) <= sigma(f, tau, xi) <= S(f, tau) < I+epsilon $
  ]

  #theorem[$f$ ЧТО-ТО на $[a,b] => f in cal(R)[a,b]$]
  #proof[очевидно, что $f in B[a,b] and f arrow.t : f(a) <= f(x) <= f(b)$
  $ forall epsilon>0 exists tau : S(f, tau) - s(f, tau) < epsilon => sum_i omega(f, Delta_i) |Delta_i| = sum (f(x_i) - f(x_(i-1)))|Delta_i| \
  <= sum_i (f_(x_i) - f(x_(i-1)) ) |tau| = |tau| (f(b) - f(a)) $

  Как только $|tau| < epsilon dfrac(epsilon, f(b) - f(a))$
]

#definition[$Let f : E -> RR, H subset E$

$omega(f,H) = sup|f(x) - f(y)|$ - колебание функции
]
#pagebreak()
#corollary[$ omega(f, H) = sup_H f - inf_H f $]
#proof[в одну сторону очевидно $ |f(y) - f(x)| <= sup - inf => omega(f, H) <= sup_H f - inf_H f $
теперь покажем в другую $omega>= sup-inf$
$ epsilon>0 space &exists x in H : f(x) > sup_H f - epsilon \ 
& exists y in H : f(y) < inf_H f - epsilon $

$ => f(x) - f(y) > sup_H(f) - epsilon - (inf_H f + epsilon) \
=> |f(x)-f(y)|>sup f - inf f - 2 epsilon => sup_(x',y')|f(x') - f(y')| >= |f(x) - f(y)| $
]

#definition[$f:[a, b]->RR, tau - "дробление"$
$ omega(f, tau) :=sum_i w(f, Delta_i)|Delta_i| $

Следствием из утверждение $omega(f, tau)=S(f, tau) - s(f, tau)$
]

#theorem[$Let f in B[a, b]$ $ f in cal(R)[a,b] <=> forall epsilon >0 exists tau : w(f, tau) < epsilon $]

#definition(name: "равномерно непрервыно")[$f:E->RR$ 

$ f - "р.н." <=> forall epsilon > 0 exists delta > 0 : forall x,y in E |x-y|<delta arrow.r.hook |f(x) - f(y)| < epsilon $

Заметим, что из равномерной непрерывности следует обычная непрерывность
]

#theorem(name: "Кантора")[$f in C[a,b] => f "р.н. на " [a, b]$]
#proof[(От противного) $ Let exists epsilon > 0 forall delta > 0 exists x, y in [a, b]: 
cases(|x-y| <delta, |f(x) - f(y)| >= epsilon) $

Для $n in NN : delta := dfrac(1, n) $ получим $x_n, y_n in [a, b]: $

$ |x_n - y_n|<1/n, space |f(x_n) - f(y_n)| >= epsilon $

Заметим, что $|x_n - y_n| -> 0$ По теореме Б-В : $exists{n_k}_(k=1)^(infinity): x_n_k->x in [a,b]$

 $x_n - y_n -> 0 =>  x_n_j - y_n_k -> 0 => y_n_k -> x$

 $f$ непрервыно в точке $x$ => $ cases(f(x_n_k) -> f(x), f(y_n_k)->f(x)) =>f(x_n_k) - f(y_n_k) ->0 $ что есть противоречие.

]


#theorem[$f in C[a, b] => f in cal(R)[a,b]$]
#proof[Пусть $tau$ дробление : $|tau| < delta$ 

$ omega(w, tau)=  sum_i w(f, Delta_i) |Delta_i| ; space omega(f, Delta_i) = sup_Delta_i |f(x) - f(y)| $
$ forall x, y in Delta_i |x-y| <= |Delta_i| <= tau < delta => |f(x) - f(y)) < epsilon => omega(f, tau) <= epsilon dot sum_i |Delta_i|= epsilon(b-a) \ =>forall epsilon'>0 exists tau : w(f, tau) <= epsilon(b-a) < epsilon' $
]

#remark[ $tau prec tau' => omega(f, tau') >=  omega(f, tau)$]


#pagebreak()
#align(center)[
  #set text(size: 14pt)
  Свойства интегральной функции
]



#corollary[$f, g in cal(R)[a, b]$]

$ &alpha, beta in RR => alpha f + beta g in cal(R)[a,b] and integral_a^b  (alpha f(x) + beta g(x)) d x = \
  alpha integral_a^b f(x) dif x + beta integral_a^b g(x) dif x
$

]