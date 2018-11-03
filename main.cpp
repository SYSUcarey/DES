#include "des.cpp"
#include <iostream>
using namespace std;

int main () {
    string input = "";
    string key = "";
    string encryptText = "";
    string decryptText = "";

    cout << "输入想要加密的明文：" << endl;
    cin >> input;
    cout << "输入8字符密钥：" << endl;
    cin >> key;

    if (input == "") {
        cout << "null input error!" << endl;
        exit(1);
    }
    if (key.length() != 8) {
        cout << "key length must be 8!" << endl;
        exit(2);
    }

    cout << "加密前:" + input << endl;
    des(input, encryptText, true, key);
    cout << "加密后:" + encryptText << endl;
    des(encryptText, decryptText, false, key);
    cout << "解密后:" + decryptText << endl;
    return 0;
}