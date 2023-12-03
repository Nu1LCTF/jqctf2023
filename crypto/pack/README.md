## Pack

+ Category: **Crypto**
+ Difficulty: ★★
+ Solved: 1
+ Tag: **Algebra, Subset Sum**

## Description

It's too noisy around, oh my pack!

## Details

想法来源是 `n1ctf2023 e2D1p`，如果扰动是以加法形式出现那就成了LWE，稍微做了点调整就有了这个题目。题目一开始生成一组SSP的实例，满足 $\sum a_ix_i=0\ (x_i\in \{-1,1\})$，随后转换 $x$ 将 $-1\rightarrow0$，给出 $\sum a_i\cdot(x'_i\oplus e_i)$ 的结果，其中 $e_i$ 为 FLAG 的比特信息

## Solution

#### I. 恢复扰动后的子集和结果

第一步和常规背包问题类似，但是维度上稍微有点高，单纯LLL比较难找到目标向量，可以先进行一次LLL，把满足求和结果的向量拿出来再做一次BKZ，相当于把规约出来的结果再做线性变化，找短向量

#### II. 恢复FLAG

通过第一步我们可以拿到 $x'_i\oplus e_i$ 的值，为了利用 $\sum a_ix_i=0$ 的条件，考虑下面的转换

不妨设 $x'_i\oplus e_i=c_i\ \Rightarrow x'_i=e_i\oplus c_i$

考虑映射 $\Phi:F_2^+\rightarrow A^*$ 满足 $\Phi(x\oplus y)=\Phi(x)\cdot \Phi(y)$

$\Phi(x)=-2\cdot x+1$

$F_2$ 上的规则如下

|  x   |  y   | x $\oplus$ y |
| :--: | :--: | :----------: |
|  0   |  0   |      0       |
|  0   |  1   |      1       |
|  1   |  1   |      0       |

转换后 $A^*$ 规则如下

|  x   |  y   | x $\cdot$ y |
| :--: | :--: | :---------: |
|  1   |  1   |      1      |
|  1   |  -1  |     -1      |
|  -1  |  -1  |      1      |

利用上述代数关系我们有 $\Phi(x'_i)=\Phi(e_i)\cdot \Phi(c_i)$

同时在此映射下，我们保持了 $\sum a_i\Phi(x'_i)=0$

$\Rightarrow \sum (a_i\cdot \Phi(c_i))\cdot \Phi(e_i)=0$

这里同样是个SSP，利用两组结果构造lattice做BKZ求出目标向量后映射回去即可

## Summary

参数没有卡好，导致g6k直接给挂出来了，实际上是不希望能直接求解pack这一层，预期是通过求解epack，构造映射将多组结果combine起来进行求解
