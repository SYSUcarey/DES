/*
 * DES算法主函数以及辅助函数
 * - void stringToBit(const char *in, char *out)：将8字符的字符串转化成64位的01数据
 * void bitToString(const char* in, char* out)将64位的01数据转化成8字符的字符串
 * void leftShift(char* key56)左移操作函数，将64位01数据分成左右两部分，分别左移1位
 * - void getSubKey(string KEY, char (*SUBKEY)[49])：生成子密钥函数
 * - void des(string input, string& output, bool isEncrypt, string key)：DES算法主函数，根据isEncrypt：bool分成加密解密两种情况。
 */
#include "transform.cpp"
#include "iteration.cpp"
#include <string>
using namespace std;

// 辅助函数，将8字符字符串转成64位数据
void stringToBit(const char *in, char *out) {
  for (int i = 0; i < 8; i++) {
    char c = in[i];
    unsigned char mask = 0x80;//mask = 10000000
    for (int j = 0; j < 8; j++) {
      out[i * 8 + j + 1] = (mask & c) ? 1 : 0;
      mask >>= 1;
    }
  }
}

// 辅助函数，将64位数据转成8字符字符串
void bitToString(const char* in, char* out) {
    for (int i = 0; i < 8; i++) {
        unsigned char mask = 0;
        for (int j = 0; j < 8; j++) {
            char c = in[i * 8 + j + 1];
            mask <<= 1;
            mask |= int(c);
        }
        out[i] = mask;
    }
}

// 辅助函数, key56(56bit)分成左右两部分，分别左移1位
void leftShift(char* key56) {
    char c_left = key56[1], d_left = key56[29];
    for (int i = 1; i < 28; i++) {
        key56[i] = key56[i + 1];
        key56[i + 28] = key56[i + 29];
    }
    key56[28] = c_left;
    key56[56] = d_left;
}

// 根据密钥获得16个子密钥(16*48bit)
void getSubKey(string KEY, char (*SUBKEY)[49]) {
	// 第一步 8位字符密钥转成64位数据key64(64bit)
    char key64[65];
    stringToBit(KEY.c_str(), key64);

    // 第二步 key64(64bit)PC1置换得到key56(56bit)
    char key56[57];
    PC_1_Transform(key64, key56);

    // 第三步 重复算法生成16个子密钥SUBKEY(16*48bit)
    for (int i = 1; i <= 16; i++) {
        if (i != 1 && i != 2 && i != 9 && i != 16) leftShift(key56);
        leftShift(key56);
        PC_2_Transform(key56, SUBKEY[i - 1]);
    }
}

void des(string input, string& output, bool isEncrypt, string key) {
	// 第一步 获得16个48位密钥
	char K[16][49];
	getSubKey(key, K);

	// 第二步 输入原始明文分组成8个字符一组，再将每个分组转化成64位数据，进行加密（DES：64bit加密）
	int count = (input.length() % 8) ? (input.length() / 8 + 1) : (input.length() / 8);
	string m[count];
	char M[count][65];
	char W[count][65];
	for(int i = 0; i < count; i++) {
		for(int j = 0; j < 8; j++) {
			if(input[i*8+j] == '\0') break;
			m[i] += input[i*8+j];
		}
		// 不够8个字符的，用'\0'凑齐
		while(m[i].length() < 8) m[i] += '\0';
		// 将8字符一组的明文，转化成64bit的数据
		stringToBit(m[i].c_str(), M[i]);

		// 第三步 对于每个64bit明文分组进行加密
		// 1. IP置换明文M(64bit)，结果还放在M中
		IP_Transform(M[i]);
		// 2. 进行16次迭代（涉及feistel函数）
		iteration(M[i], K, isEncrypt, W[i]);
		// 3. W(64bit)交换置换，结果仍放在W
        char tmp[65];
        memcpy(tmp, W[i], 65);
        for (int j = 1; j <= 32; ++j) {
            W[i][j] = tmp[j + 32];
            W[i][j + 32] = tmp[j];
        }
        // 4. W(64bit)IP逆置，结果仍放在W
        IP_Inverse_Transform(W[i]);
        // 第四步，加密后的64位数据再转成字符串
        char after[9] = {'\0'}; // 最后一个是终止符
        bitToString(W[i], after);
        output += after;
	}
}

