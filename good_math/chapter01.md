第1章 自然数
=============

数とは何であるか
- 意味論的に
 - 基数(cardinal number) としての数。ものを数えるときに使う
 - 順序数(ordinal number)としての数。集団の中で特定のものがどの位置にあるかを数える
- 公理的に
- 構成的に

# 1.1 自然数を公理的に語る

## ペアノ算術(Peano arithmetic)について

- 初期値の規則(Initial value rule) 0は自然数である。
- 後者の規則(Successor Rule) すべての自然数について、後者(successor)が1つ存在する。
- 前者の規則(Predecessor Rule) 0以外のすべての自然数について、前者(predecessor)と呼ばれる自然数の後者になる。
- 一意性の規則(Uniqueness Rule) どの異なる2つの自然数も同じ後者を持たない
- 等価性の規則(Equality Rules)
 - 等価性は反射的(reflexive)である。すべての数は自分自身と等しい(？)
 - 等価性は対照的(symmetric)である。自然数aとbについて、a=bならばb=a
 - 等価性は推移的(transitive)である。a=bかつb=cならばa=c
- 帰納法の規則(Induction Rule)
 1. Pは0について真である
 2. Pが自然数nについて真であると仮定したとき、Pがnの後者s(n)についても真であると証明できる

## 例: 足し算の定義について

形式的には以下の規則で定義される。
- 可換性(Commutativity) どのような自然数についても、a=bならばb=a
- 単位元(Identity) どのような自然数nについても n+0=0+n=nが成り立つ。この性質を持つ0を単位元と呼ぶ
- 再帰(Recursion)どんな自然数mとnについても、m+s(n)=s(m+n)が成り立つ

# 1.2 ペアノの帰納法を使う

一般的な数学的帰納法の話
```math
\left(0+1+2+3+\cdots +n+(n+1)\right)=\dfrac{(n+1)(n+2)}{2}
```

