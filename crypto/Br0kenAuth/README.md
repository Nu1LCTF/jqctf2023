## Br0kenAuth

+ Category: **Crypto**
+ Difficulty: ★★★
+ Solved: 0
+ Tag: **HEC, HNP**

## Description

A secure authentication system, but broken nowadays :( .

## Details

题目采用HECDSA算法进行签名，最终需要构造出admin的签名结果，由于超椭圆曲线相关的算法没有普及，并没有标准的曲线参数，为了避免直接PH算法求解HECDLP，出题的时候随机跑出了一条雅各比簇的阶含有较大因子的曲线，曲线的亏格为3，但是并未选择阶为大素数的生成元。

## Solution

#### I. 恢复 Jacobian variety 的阶

这部分可参考 `0CTF2020 Simple Curves` ，还是分解基域上 `Frobenius Polynomial` ，接着利用公式计算扩域上超椭圆曲线雅各比簇的阶

#### II. 求解 HECDLP

通过观察可知选取的生成元的阶含有小素因子，可以利用Pholig-Hellman结合BSGS算法计算出签名时所选随机数 $k$ 的部分信息 $k'$，即 $k=n\cdot x+k'$ 

#### III. 求解 Hidden Number Problem

将第二步所泄漏的k的信息带入到签名的等式，由于计算签名时采用的是有错误的模量，故小阶在等式中可逆，简化后即可转化为HNP，利用LLL进行求解

## References

[An elementary introduction to hyperelliptic curves](https://www.math.uwaterloo.ca/~ajmeneze/publications/hyperelliptic.pdf)

## Summary

一开始对题目难度的评估上有点问题，实际上虽然都是一些陈旧的trick但是组合起来还是比较麻烦的