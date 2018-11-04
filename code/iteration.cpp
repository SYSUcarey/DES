/*
 * DES算法中运用到的16层的迭代
 * void iteration(char* M, char SUB_KEY[16][49], bool isEncrypt)
 * M(64bit)为原始明文初始IP置换后得到的64位结果
 * SUB_KEY(16*48bit)是16个48位子密钥
 * isEncrypt(bool)表示加解密方式: true 表示 加密, false 表示 解密
 * W(64bit)为16轮迭代并高低交换位后的输出
 */
#include "feistel.cpp"

void iteration(char* M, char SUB_KEY[16][49], bool isEncrypt, char* W) {
    // 第一步 从M(64bit)分离得到左右部分L(32bit), R(32bit)
    char L[33], R[33];
    for (int i = 1; i <= 32; i++) {
        L[i] = M[i];
        R[i] = M[i + 32];
    }

    // 第二步，进行16次迭代运算
    char tmp[33];
    char F[33];
    for (int i = 0; i < 16; i++) {
        // 每一轮L赋值上一轮的R
        for (int j = 1; j <= 32; j++) {
            tmp[j] = L[j];
            L[j] = R[j];
        }
        // 获得每一轮feistel输出F
        if (isEncrypt == true) {
            feistel(R, SUB_KEY[i], F);
        }
        else {
            feistel(R, SUB_KEY[15 - i], F);
        }
        // 每一轮R赋值L与F的或
        for (int j = 1; j <= 32; j++) {
            R[j] = tmp[j] ^ F[j];
        }


    }
    // 第三步 得到结果W(64bit)
    for (int i = 1; i <= 32; i++) {
        W[i] = L[i];
        W[i + 32] = R[i];
    }
}