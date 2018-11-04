/*
 * DES算法中运用到的feistel函数实现
 * void feistel(char* R, char* K, char* F)
 * R为循环明文Rn-1的后半部分(32bit)， K为循环子密钥Kn(48bit)，F为循环中feistel输出Fn-1(32bit)
 */
#include <iostream>
#include "Table.hpp"
using namespace std;
void feistel(char* R, char* K, char* F) {
	// 第一步 R(32bit)拓展E置换, 得到E(48bit)
	char E[49];
	E_Extend_Transform(R, E);

	// 第二步 E(48bit)和K(48bit)异或运算, 结果保存在E
	for (int i = 1; i <= 48; i++) E[i] ^= K[i];

	// 第三步 E(48bit)分组成S_IN(8*6bit)
	char S_IN[8][6];
	for(int i = 0; i < 8; i++) {
		for (int j = 0; j < 6; j++) {
			S_IN[i][j] = E[i*6+j+1];
		}
	}

	// 第四步 S_IN(8*6bit)进行沙盒置换得到S_OUT(8*4bit)
	char S_OUT[8][4];
	S_Box_Transform(S_IN, S_OUT);

	// 第五步 分组S_OUT(8*4bit)合并成C(32bit)
	char C[33];
	C[0] = 0;
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 4; j++) {
			C[4*i+j+1] = S_OUT[i][j];
		}
	}

	// 第六步 C(32bit)P置换得到F(32bit)
	P_Transform(C, F);
}