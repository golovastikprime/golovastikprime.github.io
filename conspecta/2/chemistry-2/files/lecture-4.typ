#import "../preambule.typ": *


#lecture[
== 1. Фазовое пространство

#definition[
Фазовое пространство:
$
Gamma = {(q_1,..,q_n,p_1,dots,p_n)\}  RR^{2n}
$
]

#definition[
Обобщённые импульсы:
$
p_i = frac( partial L, partial dot q_i)
$
]

#remark[
В квантовой механике:
$
Delta q Delta p >= frac(planck,2)
$
]

== 2. Гильбертово пространство

#definition[
Состояние:
$
psi in L^2(RR^n), quad |psi|=1
$
]

#definition[
Скалярное произведение:
$
chevron.l psi,phi chevron.r = integral psi^* phi dx
$
]

== 3. Эрмитовы операторы

#definition[
$
chevron.l psi, hat(A) phi chevron.r = chevron.l hat(A) psi, phi chevron.r
$
]

#proposition[
Собственные значения вещественны.
]

#proof[
$
hat(A) psi = lambda psi
$
$
chevron.l psi,hat(A) psi chevron.r = lambda chevron.l psi,psi chevron.r
$
$
chevron.l hat(A) psi,psi chevron.r = lambda^* chevron.l psi,psi chevron.r
$
⇒ $lambda = lambda^*$
]

#proposition[
Собственные функции ортогональны.
]

== 4. Коммутаторы

#definition[
$
[hat(A),hat(B)]=hat(A)hat(B)- hat(B)hat(A)
$
]

#example[
$
[hat(x),hat(p)]=i planck
$
]

#theorem[
Если $[hat(A),hat(B)]=0$, существует общий базис.
]

== 5. Спектральная теория

#definition[
$
hat(A) psi = lambda psi
$
]

#definition[
Спектр — множество допустимых $lambda$.
]

#proposition[
Разложение:
$
psi = sum c_n psi_n
$
]

== 6. Дифференциальные операторы

#definition[
$
nabla = (partial_x,partial_y,partial_z)
$
]

#definition[
$
Delta = nabla^2
$
]

#proposition[
$
hat(T) = -frac(planck^2, 2m)Delta
$
]

== 7. Соответствие Борна–Дирака

#definition[
$
hat(x) = x,quad hat(p) = -i planck partial_x
$
]

#definition[
Среднее:
$
chevron.l A chevron.r = chevron.l psi,hat(A) psi chevron.r
$
]

== 8. Измерения

#theorem[
Результаты измерения — спектр оператора.
]

#theorem[
$
P(lambda_n)=|c_n|^2
$
]

#remark[
Коллапс:
$
psi -> psi_n
$
]
]

