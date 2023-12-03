## decrypt

+ Category: **Crypto**
+ Difficulty: ★★
+ Solved: 1
+ Tag: **LCG, Math**

## Description

I forgot my password, please help me find it back.

## Details

六个LCG，输出位是后两个LCG决定的前四个LCG中某一个状态的中间位，给定2048比特，需要还原所有LCG的初始状态。

## Solution

很难根据输出直接还原出原始状态，一个直接的想法爆破初始状态，但是爆破所有初始状态的复杂度较高，这里需要利用一点概率与统计的想法。

由于前四个LCG每轮被选到的概率是 $\frac{1}{4}$ ，若只考虑爆破一个初始状态，如果是正确的初始状态，则该LCG被选中时，其输出与最终比特流对应位一定相同，否则以 $\frac{1}{2}$ 的概率相同。

综上爆破正确的初始状态其与最终输出的拟合度为 $\frac{1}{4}+\frac{3}{4}\cdot \frac{1}{2}=\frac{5}{8}$

筛选出拟合度接近 $\frac{5}{8}$ 的初始状态（可能不止一个满足），在还原出前四个LCG的状态后，爆破后两者，也是直接爆破其中一个，这样每次选择的LCG有两种可能，只需判断输出是否包含于这两个LCG的输出流即可。

