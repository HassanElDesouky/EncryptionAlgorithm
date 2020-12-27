//
// Created by Hassan El Desouky on 23/11/2020.
//

#include "SubstitutionCipher.h"
#include <string>
#include <map>
#include <set>

using namespace std;

string CaesarCipher::cipher() {
    string cipheredText;
    for (char curChar: plaintext)
        cipheredText += char(int(curChar + shift - 'a') % 26 + 'a');

    return cipheredText;
}

string CaesarCipher::decipher() {
    int tempShift = shift;
    shift = 26 - tempShift;
    string decipheredText = cipher();
    return decipheredText;
}

string MonoalphabeticCipher::cipher() {
    string cipheredText;
    for (char curChar: plaintext) {
        int j;
        for (int j = 0; j < (int) ALPHA.length(); ++j) {
            if (curChar == ALPHA[j]) {
                cipheredText += cipheredAlphabet[j];
                break;
            }
        }
        if (j == (int) ALPHA.length())
            cipheredText += curChar;
    }
    return cipheredText;
}

string MonoalphabeticCipher::decipher() {
    string decipheredText;
    for (char curChar: plaintext) {
        int j;
        for (j = 0; j < (int) cipheredAlphabet.length(); j++) {
            if (curChar == cipheredAlphabet[j]) {
                decipheredText += ALPHA[j];
                break;
            }
        }
        if (j == cipheredAlphabet.length())
            decipheredText += curChar;
    }
    return decipheredText;
}

void PlayfairCipher::processPlaintext() {
    bool group = false;
    for (int i = 0; i < plaintext.size(); ++i) {
        if (i & 1) {
            group = true;
        } else {
            group = false;
        }

        if (group) {
            if (plaintext[i-1] == plaintext[i]) {
                plaintext.insert(plaintext.begin()+i, 'x');
            }
        }
    }
    if (plaintext.size() & 1) plaintext += 'x';
}

void PlayfairCipher::processDecipheredText(string &decipheredText) {
    for (int i = 1; i < (int) decipheredText.size(); i+=2) {
        if (decipheredText[i] == 'x' && i+1 < (int)decipheredText.size()-1) {
            if (decipheredText[i-1] == decipheredText[i+1]) {
                decipheredText.erase(decipheredText.begin()+i);
            }
        }
    }
}

void PlayfairCipher::fillMaps(map<char, pair<int, int>> &grid, map<pair<int, int>, char> &coordinates) {
    set<char> alphabet(ALPHA.begin(), ALPHA.end());
    int row = 0, col = 0;
    for (char curChar: keyword) {
        if (curChar == 'i') curChar = 'j'; // I/J case
        if (alphabet.count(curChar)) {
            if (col > 4) {
                row++;
                col = 0;
            }
            grid[curChar] = {row,col};
            coordinates[{row, col}] = curChar;
            alphabet.erase(curChar);
            col++;
        }
    }

    while (!alphabet.empty()) {
        char curChar = *alphabet.begin();
        if(alphabet.count(curChar)){
            if (col > 4) {
                row++;
                col = 0;
            }
            grid[curChar] = {row,col};
            coordinates[{row, col}] = curChar;
            alphabet.erase(curChar);
            col++;
        }
    }
}

string PlayfairCipher::cipher() {
    map<char, pair<int,int>> grid;
    map<pair<int,int> , char> coordinates;
    fillMaps(grid, coordinates);

    processPlaintext();

    string cipheredText;

    for (int i =0; i < plaintext.size()-1; i+=2) {
        char a = plaintext[i];
        char b = plaintext[i+1];
        if (a == 'i') a = 'j';
        if (b == 'i') b = 'j';

        int x1 = grid[a].first ,  x2 = grid[b].first;
        int y1 = grid[a].second , y2 = grid[b].second;

        if (x1 == x2) {
            y1 = (y1+1)%5;
            y2 = (y2+1)%5;
        } else if(y1 == y2) {
            x1 = (x1+1)%5;
            x2 = (x2+1)%5;
        } else {
            swap(y1,y2);
        }

        cipheredText += coordinates[{x1,y1}];
        cipheredText += coordinates[{x2, y2}];
    }

    return cipheredText;
}

string PlayfairCipher::decipher() {
    map<char, pair<int,int>> grid;
    map<pair<int,int> , char> coordinates;
    fillMaps(grid, coordinates);

    processPlaintext();

    string decipheredText;

    for (int i =0; i < plaintext.size()-1; i+=2) {
        char a = plaintext[i];
        char b = plaintext[i+1];
        if (a == 'i') a = 'j';
        if (b == 'i') b = 'j';

        int x1 = grid[a].first ,  x2 = grid[b].first;
        int y1 = grid[a].second , y2 = grid[b].second;

        if (x1 == x2) {
            y1 = (y1-1)%5;
            y2 = (y2-1)%5;
        } else if (y1 == y2) {
            x1 = (x1-1)%5;
            x2 = (x2-1)%5;
        } else {
            swap(y1,y2);
        }

        decipheredText += coordinates[{x1,y1}];
        decipheredText += coordinates[{x2, y2}];
    }

    processDecipheredText(decipheredText);

    return decipheredText;
}

string VigenereCipher::generateKey(string keyword) {
    int plaintextSize = (int) plaintext.size();

    for (int i = 0; ; ++i) {
        if (plaintextSize == i) i = 0;
        if (keyword.size() == plaintext.size()) break;
        keyword.push_back(keyword[i]);
    }

    return keyword;
}

string VigenereCipher::cipher() {
    // The plaintext(P) and key(K) are added modulo 26.
    // Ei = (Pi + Ki) mod 26
    string cipheredText;
    for (int i = 0; i < plaintext.size(); i++) {
        // converting in range 0-25
        char x = (plaintext[i] + key[i] - (2 * 'a')) % 26;
        // convert into alphabets(ASCII)
        x += 'a';
        cipheredText.push_back(x);
    }
    return cipheredText;
}

string VigenereCipher::decipher() {
    // Di = (Ei - Ki + 26) mod 26
    string decipheredText;
    for (int i = 0 ; i < plaintext.size(); i++) {
        // converting in range 0-25
        char x = (plaintext[i] - key[i] + 26) % 26;
        // convert into alphabets(ASCII)
        x += 'a';
        decipheredText.push_back(x);
    }
    return decipheredText;
}

// ---------------------------------------------------------------------------------------------------------------------

vector<vector<char>> RailFence::getRail() {
    // create the matrix to cipher plain text
    // key = rows , length(text) = columns
    vector<vector<char>> rail = vector<vector<char>>(plaintext.length(), vector<char>(level));
    // filling the rail matrix to distinguish filled
    // spaces from blank ones
    for (int i=0; i < level; i++)
        for (int j = 0; j < plaintext.length(); j++)
            rail[i][j] = '\n';

    return rail;
}

string RailFence::cipher() {
    vector<vector<char>> rail = getRail();

    // to find the direction
    bool dir_down = false;
    int row = 0, col = 0;

    for (int i=0; i < plaintext.length(); i++) {
        // check the direction of flow
        // reverse the direction if we've just
        // filled the top or bottom rail
        if (row == 0 || row == level-1)
            dir_down = !dir_down;

        // fill the corresponding alphabet
        rail[row][col++] = plaintext[i];

        // find the next row using direction flag
        dir_down?row++ : row--;
    }

    //now we can construct the cipher using the rail matrix
    string result;
    for (int i=0; i < level; i++)
        for (int j=0; j < plaintext.length(); j++)
            if (rail[i][j]!='\n')
                result.push_back(rail[i][j]);

    return result;
}

string RailFence::decipher() {
    vector<vector<char>> rail = getRail();

    // to find the direction
    bool dir_down;

    int row = 0, col = 0;

    // mark the places with '*'
    for (int i=0; i < plaintext.length(); i++) {
        // check the direction of flow
        if (row == 0)
            dir_down = true;
        if (row == level-1)
            dir_down = false;

        // place the marker
        rail[row][col++] = '*';

        // find the next row using direction flag
        dir_down?row++ : row--;
    }

    // now we can construct the fill the rail matrix
    int index = 0;
    for (int i=0; i<level; i++)
        for (int j=0; j<plaintext.length(); j++)
            if (rail[i][j] == '*' && index<plaintext.length())
                rail[i][j] = plaintext[index++];


    // now read the matrix in zig-zag manner to construct
    // the resultant text
    string result;

    row = 0, col = 0;
    for (int i=0; i< plaintext.length(); i++) {
        // check the direction of flow
        if (row == 0)
            dir_down = true;
        if (row == level-1)
            dir_down = false;

        // place the marker
        if (rail[row][col] != '*')
            result.push_back(rail[row][col++]);

        // find the next row using direction flag
        dir_down?row++: row--;
    }
    return result;
}