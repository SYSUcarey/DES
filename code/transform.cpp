/*
 * DES算法中需要用的所有置换函数
 * 1. void IP_Transform(char* M) : IP置换(初始置换) (64bit -> 64bit) 
 * 2. void IP_Inverse_Transform(char* M) : IP逆置换 (64bit -> 64bit) 
 * 3. void E_Extend_Transform(char* R, char* E) : E拓展置换 (32bit -> 48bit) 
 * 4. void S_Box_Transform(char (*IN)[6], char (*S)[4]) : S盒置换 (8*6bit -> 8*4bit)
 * 5. void P_Transform(char* IN, char* P) : P置换 (32bit -> 32bit)
 * 6. void PC_1_Transform(char* KEY_IN, char* KEY_OUT) : PC-1置换 (64bit -> 56bit)
 * 7. void PC_2_Transform(char* KEY_IN, char* KEY_OUT) : PC-2置换 (56bit -> 48bit)
 */
#include <cstring>
#include "Table.hpp"
// IP置换(初始置换) (64bit -> 64bit) 
void IP_Transform(char* M) {
	char copy[65];
	memcpy(copy, M, 65);
	for (int i = 1; i <= 64; i++) {
		M[i] = copy[IP_Table[i]];
	}
}

// IP逆置换 (64bit -> 64bit) 
void IP_Inverse_Transform(char* M) {
	char copy[65];
	memcpy(copy, M, 65);
	for (int i = 1; i <= 64; i++) {
		M[i] = copy[IP_Inverse_Table[i]];
	}
}

// E拓展置换 (32bit -> 48bit) 
void E_Extend_Transform(char* R, char* E) {
	for(int i = 1; i <= 48; i++) {
		E[i] = R[E_Extend_Table[i]];
	}
}


// S盒置换 (8*6bit -> 8*4bit)
void S_Box_Transform(char (*IN)[6], char (*S)[4]) {
    for (int i = 0; i < 8; i++) {
        char n = IN[i][0] * 2 + IN[i][5]; //得到行 0~3
        char m = IN[i][1] * 8 + IN[i][2] * 4 + IN[i][3] * 2 + IN[i][4]; //得到列 0~15
        char result = S_Box_Table[i][n][m];
        unsigned char mask = 0x8; //00001000
        for (int j = 0; j < 4; ++j) {
            S[i][j] = (result & mask) ? 1 : 0;
            mask >>= 1;
        }
    }
}


// P置换 (32bit -> 32bit)
void P_Transform(char* IN, char* P) {
	for(int i = 1; i <= 32; i++) {
		P[i] = IN[P_Table[i]];
	}
}

// PC-1置换 (64bit -> 56bit)
void PC_1_Transform(char* KEY_IN, char* KEY_OUT) {
	for(int i = 1; i <= 56; i++) {
		KEY_OUT[i] = KEY_IN[PC_1_Table[i]];
	}
}

// PC-2置换 (56bit -> 48bit)
void PC_2_Transform(char* KEY_IN, char* KEY_OUT) {
	for(int i = 1; i <= 48; i++) {
		KEY_OUT[i] = KEY_IN[PC_2_Table[i]];
	}
}




