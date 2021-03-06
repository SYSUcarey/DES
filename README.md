# DES算法程序设计和实现实验报告
## （2018年秋季学期）
| 课程名称 | Web安全技术 | 任课老师 | 蔡国扬 |
| :------------: | :-------------: | :------------: | :-------------: |
| 年级 | 大三 | 专业（方向） | 计应 |
| 学号 | 16340015 | 姓名 | 陈彬彬 |
| 电话 | 13590883387 | Email | 944131226@qq.com |
| 开始日期 | 2018/11/3 | 完成日期 |2018/11/3|

---

[TOC]

## 一、算法原理概述

### 1.1 关于算法

DES是一种使用64位密钥的块加密算法,它以64位为分组长度，64位一组的明文作为算法的输入，通过一系列涉及密钥的复杂的换位、置换操作，输出同样64位长度的密文。

### 1.2 关于密钥

DES 采用64位密钥，但由于每8位中的最后1位用于奇偶校验，实际有效密钥长度为56位。密钥可以是任意的56位的数，且可随时改变。其中极少量的数被认为是弱密钥，但能容易地避开它们。所有的保密性依赖于密钥。

---

## 二、总体结构

### 2.1 算法输入输出

算法输入：

- 64位明文分组：$M = m_1m_2···m_{64} ,  m_i \epsilon\{0, 1\},  i = 1, ··· ,64$
- 64位密钥K(有效56位)：$$K = k_1k_2 ··· k_{64} , k_i \epsilon\{0, 1\}, i = 1, ··· ,64$$

算法输出：

- 64位密文分组：$$C = c_1c_2 ··· c_{64} , c_i \epsilon\{0, 1\}, i = 1 ,··· ,64$$

### 2.2 算法加密解密过程

#### 2.2.1加密过程

$C = E_k(M) = IP^{-1} · W · T_{16} · T_{15} · ... · T_1 · IP(M) $    

变量意义解析：

- $C$ 为加密算法输出的64位密文块

- $E_k$ 描述以K为密钥的加密函数

- $M$ 为加密算法输入的64位明文块

加密流程解析

- 第一步：对输入 $M$ 进行 $IP$ 置换。
- 第二步：对第一步结果进行16次 $T_i， i=1,2,...,16$ 迭代运算
- 第三步：对第二步结果进行64位高低位交换 $W$ 置换
- 第四步：对第三步结果进行 $IP$ 逆置换得到输出 $C$ 

#### 2.2.2解密过程

$M = D_k(C) = IP^{-1} · W · T_{1} · T_{2} · ... · T_{16} · IP(C) $     

变量意义解析：

- $C$ 为解密算法输入的64位密文块
- $D_k$ 描述以K为密钥的解密函数
- $M$ 为解密算法输出的64位明文块

加密流程解析

- 第一步：对输入 $C$ 进行 $IP$ 置换。
- 第二步：对第一步结果进行16次 $T_i， i=16,15,...,2,1$ 迭代运算
- 第三步：对第二步结果进行64位高低位交换 $W$ 置换
- 第四步：对第三步结果进行 $IP$ 逆置换得到输出 $M$ 

***

## 三、模块分解

### 3.1 IP置换

64位输入M，按照IP置换表，置换位置，输出结果仍放在M中

```
58,50,42,34,26,18,10,2,
60,52,44,36,28,20,12,4,
62,54,46,38,30,22,14,6,
64,56,48,40,32,24,16,8,
57,49,41,33,25,17, 9,1,
59,51,43,35,27,19,11,3,
61,53,45,37,29,21,13,5,
63,55,47,39,31,23,15,7
```

注意：这里的数字表示的是原数据的位置，不是数据。即将输入的64位明文的第1位置换到第40位，第2位置换到第8位，第3位置换到第48位。以此类推，最后一位是原来的第7位。置换规则是规定的。 

### 3.2 $T_i$ 迭代运算​

64位输入M，连续进行16轮迭代过程，得到64位输出W

每轮的迭代过程可以表示如下：

$$L_n = R_{n - 1}，n=1,2,...,16$$

$$R_n = L_{n - 1}⊕f(R_{n-1},K_{n})，n=1,2,...,16$$

其中：

-  $L_0、R_0$ 是64位输入M的左右两半部分，各32位。
- $⊕$ 是异或运算
- $K_{n}$是子密钥
- $f$ 是Feistel轮函数
- 经过16轮迭代，我们得到 $L_{16}、R_{16}$ ，各32位，再左右合成为64位输出结果W

#### 3.2.1 子密钥获取

64位输入K，经过PC-1置换、轮左移操作、PC-2置换得到16个48位子密钥$K_n，n=1,2,...,16$ 

流程：

- 第一步：64位输入K进行PC-1置换得到56位 $T_0$，其左右两部分记为$C_0$，$D_0$ （28bit）。

- 第二步：进行16轮左移操作，得到 $$C_n、D_n，n=1,2,...,16$$ ，左右合并成$$T_n，n=1,2,...,16$$ （56bit）。

- 第三步：对 $$T_n，n=1,2,...,16$$ 进行PC-2置换得到16个48位子密钥$K_n，n=1,2,...,16$ 

##### 3.2.1.1 PC-1置换

64bit输入K，按照PC-1置换表，得到结果56bitT

```
57,49,41,33,25,17,9,1,
58,50,42,34,26,18,10,2,
59,51,43,35,27,19,11,3,
60,52,44,36,63,55,47,39,
31,23,15,7,62,54,46,38,
30,22,14,6,61,53,45,37,
29,21,13,5,28,20,12,4
```

##### 3.2.1.2 左移操作

28bit输入，按照子密钥获取过程的轮数，左移1或2位。

每轮移动的位数表：

```
轮数：1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
位数：1, 1, 2, 2, 2, 2, 2, 2, 1,  2,  2,  2,  2,  2,  2,  1
```

##### 3.2.1.3 PC-2置换

56bit输入 $T_n$ ，按照PC-2置换表，得到结果48bit $K_n$ 

```
14,17,11,24,1,5,
3,28,15,6,21,10,
23,19,12,4,26,8,
16,7,27,20,13,2,
41,52,31,37,47,55,
30,40,51,45,33,48,
44,49,39,56,34,53,
46,42,50,36,29,32
```

#### 3.2.2 Feistel轮函数

32bit输入$R_{n-1}$ ，48bit输入 $K_n$ ，32bit输出 $F_{n}$

 流程：

- 第一步：32位$R_{n-1}$ 进行E拓展置换，得到48位 $E_{n-1}$

- 第二步：48位 $E_{n-1}$ 与 48位 $K_1$ 进行异或运算，结果仍放在$E_{n-1}$

- 第三步：第二步48位 $E_{n-1}$进行S-盒置换得到32位$S_{n-1}$

- 第四步：32位 $S_{n-1}$ 进行P-置换得到 32位 $F_{n}$

##### 3.2.2.1 E拓展置换

32位$R_{n-1}$ 按照E拓展置换表进行置换，得到48位 $E_{n-1}$

```
32,1,2,3,4,5,
4,5,6,7,8,9,
8,9,10,11,12,13,
12,13,14,15,16,17,
16,17,18,19,20,21,
20,21,22,23,24,25,
24,25,26,27,28,29,
28,29,30,31,32,1

```

##### 3.2.2.2 S-盒置换

48位 $E_{n-1}$拆成8个6bit分组，按照8个S-盒置换表进行8个S-盒置换，得到8个4bit分组，再组合成输出32位$S_{n-1}$

代替运算由8个不同的代替盒(S盒)完成。每个S-盒有6位输入，4位输出。

每个S-和是4行16列的格式，因为二进制4位是0~15。8个S-盒的值如下：

S-盒1：

```
14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,
```

S-盒2：
```
15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,
```
S-盒3：
```
10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
```
S-盒4：
```
7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
```
S-盒5：
```
2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,
```
S-盒6：
```
12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
```
S-盒7：
```
4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
```
S-盒8：
```
13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11,
```
S-盒计算过程
以S-盒8为例子，


假设S-盒8的输入(即异或函数的第43~48位)为110011。

第1位和最后一位组合形成了11(二进制)，对应S-盒8的第3行。中间的4位组成形成1001(二进制),对应S-盒8的第9列。所以对应S-盒8第3行第9列值是12。则S-盒输出是1100(二进制)。

##### 3.2.2.3 P-置换

32位 $S_{n-1}$ 按照P-置换表进行置换得到 32位 $F_{n}$

```
16,7,20,21,
29,12,28,17,
1,15,23,26,
5,18,31,10,
2,8,24,14,
32,27,3,9,
19,13,30,6,
22,11,4,25,
```

### 3.3 高低位交换W置换

64bit输入W，64bit输出结果仍放在W中

将W的高32bit和低32bit置换

### 3.4 IP逆置换

64bit输入W，按照IP逆置换表进行置换，得到64bit输出C

```
40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,
38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,
36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,
34,2,42,10,50,18,58 26,33,1,41, 9,49,17,57,25,
```

---

## 四、数据结构
- **main.cpp**：测试主函数，终端输入加密明文和密钥，输出加密解密后结果。
- **Table.hpp**：存放DES算法中使用到的置换表格
  - `IP_Table` : IP置换表
  - `IP_Inverse_Table` : IP逆置换表
  - `E_Extend_Table` ： E拓展置换表
  - `S_Box_Table` : 8个S沙盒置换表
  - `P_Table` ： P置换表
  - `PC_1_Table` ： PC1置换表
  - `PC_2_Table` ： PC2置换表
- **transform.cpp**：存放DES算法中使用到的置换函数
  - `void IP_Transform(char* M)` : IP置换(初始置换) (64bit -> 64bit) 
  - `void IP_Inverse_Transform(char* M)` : IP逆置换 (64bit -> 64bit) 
  - `void E_Extend_Transform(char* R, char* E)` : E拓展置换 (32bit -> 48bit) 
  - `void S_Box_Transform(char (*IN)[6], char (*S)[4])` : S盒置换 (8*6bit -> 8*4bit)
  - `void P_Transform(char* IN, char* P)` : P置换 (32bit -> 32bit)
  - `void PC_1_Transform(char* KEY_IN, char* KEY_OUT)` : PC-1置换 (64bit -> 56bit)
  - `void PC_2_Transform(char* KEY_IN, char* KEY_OUT)` : PC-2置换 (56bit -> 48bit)
- **iteration.cpp**：DES算法中使用到的16层迭代函数
  - `void iteration(char* M, char SUB_KEY[16][49], bool isEncrypt, char* W)`
- **feistel.cpp**：DES算法中再16层迭代中用到的**feistel**函数
  - `void feistel(char* R, char* K, char* F)`
- **des.cpp**：DES算法主函数以及辅助函数
  - `void stringToBit(const char *in, char *out)`：将8字符的字符串转化成64位的01数据
  - `void bitToString(const char* in, char* out)`：将64位的01数据转化成8字符的字符串
  - `void leftShift(char* key56)`：左移操作函数，将64位01数据分成左右两部分，分别左移1位
  - `void getSubKey(string KEY, char (*SUBKEY)[49])`：生成子密钥函数
  - `void des(string input, string& output, bool isEncrypt, string key)`：DES算法主函数，根据**isEncrypt：bool**分成加密解密两种情况。

---

## 五、C++语言源代码

- 源码已经在上传作业包下的code文件夹内
- 或者查看[我的github仓库](https://github.com/SYSUcarey/DES)

***

## 六、编译运行结果

在code目录下：

- 终端执行命令编译算法测试程序：

```bash
g++ main.cpp -o main
```

- 终端执行命令运行程序：

```bash
./main
```

- 测试DES算法程序：

  输入明文：

  ```
  16级软工一班
  ```

  输入8字符密钥：

  ```
  16340015
  ```

  程序运行结果截图：

  ![Img_result](https://chenbb-1257745007.cos.ap-guangzhou.myqcloud.com/blog/20181104125321.png)

  

  
