//
// Created by Hassan El Desouky on 23/11/2020.
//

#ifndef COMPUTERSECURITY_SUBSTITUTIONCIPHER_H
#define COMPUTERSECURITY_SUBSTITUTIONCIPHER_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class SubstitutionCipher {
public:
    virtual string cipher() = 0;
    virtual string decipher() = 0;
};

class CaesarCipher final : public SubstitutionCipher {
    string& plaintext;
    int shift;

public:
    explicit CaesarCipher(string& plaintext, int shift) : plaintext(plaintext), shift(shift) {}

    string cipher() override;
    string decipher() override;
};

class MonoalphabeticCipher final : public SubstitutionCipher {
    const string ALPHA = "abcdefghijklmnopqrstuvwxyz";
    string& plaintext;
    string& cipheredAlphabet;

public:
    explicit MonoalphabeticCipher(string& plaintext, string& cipheredAlphabet) : plaintext(plaintext),
                                                                                 cipheredAlphabet(cipheredAlphabet) {}

    string cipher() override;
    string decipher() override;
};

class PlayfairCipher final : public SubstitutionCipher {
    const string ALPHA = "abcdefghjklmnopqrstuvwxyz"; // without I
    string& plaintext;
    string& keyword;

    /// Process the `plaintext` string in place and adds a character 'x' between every two characters.
    void processPlaintext();

    /// Fills the `grid` and `coordinates` std::maps corresponds to the `plaintext` and `keyword`.
    void fillMaps(map<char, pair<int,int>>& grid, map<pair<int,int> , char>& coordinates);

    /// Process the deciphered string in place and removes any character 'x',
    /// if it's present between two similar characters
    void processDecipheredText(string& decipheredText);

public:
    explicit PlayfairCipher(string& plaintext, string& keyword) : plaintext(plaintext), keyword(keyword) {}

    string cipher() override;
    string decipher() override;
};

class VigenereCipher final : public SubstitutionCipher {
    string& plaintext;
    string key;

    string generateKey(string keyword);

public:
    explicit VigenereCipher(string& plaintext, string& keyword) : plaintext(plaintext), key(generateKey(keyword)) {}

    string cipher() override;
    string decipher() override;
};

// ---------------------------------------------------------------------------------------------------------------------

class TranspositionCipher {
public:
    virtual string cipher() = 0;
    virtual string decipher() = 0;
};

class RailFence final : public TranspositionCipher {
    string& plaintext;
    int level;

    vector<vector<char>> getRail();

public:
    explicit RailFence(string& plaintext, int level) : plaintext(plaintext), level(level) {}

    string cipher() override;
    string decipher() override;
};

#endif //COMPUTERSECURITY_SUBSTITUTIONCIPHER_H
