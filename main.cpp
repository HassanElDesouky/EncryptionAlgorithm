#include <iostream>
#include <memory>
#include "SubstitutionCipher.h"
using namespace std;

int main() {
    unique_ptr<SubstitutionCipher> substitutionCipher;

    {
        // MARK: Caesar Cipher
        string plaintext = "balloon";
        int shift = 5;
        substitutionCipher = make_unique<CaesarCipher>(plaintext, shift);
        string cipheredText = substitutionCipher->cipher();
        string decipheredText = substitutionCipher->decipher();
//        cout << cipheredText << endl;
//        cout << decipheredText << endl;
    }

    {
        // MARK: Monoalphabetic Cipher
        string plaintext = "jyyv jy muvyi vay vofm hmivl";
        string key = "msxnyufaewbkjqohgitvdzrplc";
        substitutionCipher = make_unique<MonoalphabeticCipher>(plaintext, key);
        string cipheredText = substitutionCipher->cipher();
        string decipheredText = substitutionCipher->decipher();
//        cout << cipheredText << endl;
//        cout << decipheredText << endl;
    }

    {
        // MARK: Vigenere Cipher
        string plaintext = "welcometokfsuniversity";
        string key = "item";
        substitutionCipher = make_unique<VigenereCipher>(plaintext, key);
        string cipheredText = substitutionCipher->cipher();
        string decipheredText = substitutionCipher->decipher();
//        cout << cipheredText << endl;
//        cout << decipheredText << endl;
    }


    {
        // MARK: Playfair Cipher
        string keyword = "monarchy";
        string plaintext = "welcometokfsuniversity";
        substitutionCipher = make_unique<PlayfairCipher>(plaintext, keyword);
        string cipheredText = substitutionCipher->cipher(); // balloon
        string decipheredText = substitutionCipher->decipher(); // cbsuqpmq
        cout << cipheredText << endl;
//        cout << decipheredText << endl;
    }

    unique_ptr<TranspositionCipher> transpositionCipher;

    {
        // MARK: Rail Fence
        string plaintext = "welcome to kfs university";
        int level = 2;
        transpositionCipher = make_unique<RailFence>(plaintext, level);
        string cipheredText = transpositionCipher->cipher();
        string decipheredText = transpositionCipher->decipher();
//        cout << cipheredText << endl;
//        cout << decipheredText << endl;
    }

    return 0;
}
