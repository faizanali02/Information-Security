#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Function to prepare the key table
void generateKeyTable(string key, char keyTable[5][5]) {
    string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ"; // 'J' is merged with 'I'
    string processedKey = "";
    vector<bool> used(26, false);

    // Process the key
    for (char ch : key) {
        ch = toupper(ch);
        if (ch == 'J') ch = 'I'; // Treat 'J' as 'I'
        if (!used[ch - 'A']) {
            processedKey += ch;
            used[ch - 'A'] = true;
        }
    }

    // Add remaining alphabet letters
    for (char ch : alphabet) {
        if (!used[ch - 'A']) {
            processedKey += ch;
            used[ch - 'A'] = true;
        }
    }

    // Fill the key table
    int idx = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            keyTable[i][j] = processedKey[idx++];
        }
    }
}

// Function to prepare the plaintext
string preparePlaintext(string plaintext) {
    string prepared = "";
    plaintext.erase(remove(plaintext.begin(), plaintext.end(), ' '), plaintext.end());
    for (size_t i = 0; i < plaintext.size(); ++i) {
        char ch = toupper(plaintext[i]);
        if (ch == 'J') ch = 'I'; // Treat 'J' as 'I'
        prepared += ch;
        if (i + 1 < plaintext.size() && prepared.back() == toupper(plaintext[i + 1])) {
            prepared += 'X'; // Insert 'X' between duplicate letters
        }
    }
    if (prepared.size() % 2 != 0) prepared += 'X'; // Add padding
    return prepared;
}

// Function to search for character position in key table
void findPosition(char keyTable[5][5], char ch, int &row, int &col) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (keyTable[i][j] == ch) {
                row = i;
                col = j;
                return;
            }
        }
    }
}

// Function to encrypt plaintext using the Playfair cipher
string encrypt(string plaintext, char keyTable[5][5]) {
    string ciphertext = "";
    for (size_t i = 0; i < plaintext.size(); i += 2) {
        char ch1 = plaintext[i];
        char ch2 = plaintext[i + 1];
        int row1, col1, row2, col2;
        findPosition(keyTable, ch1, row1, col1);
        findPosition(keyTable, ch2, row2, col2);

        if (row1 == row2) {
            // Same row: move right
            ciphertext += keyTable[row1][(col1 + 1) % 5];
            ciphertext += keyTable[row2][(col2 + 1) % 5];
        } else if (col1 == col2) {
            // Same column: move down
            ciphertext += keyTable[(row1 + 1) % 5][col1];
            ciphertext += keyTable[(row2 + 1) % 5][col2];
        } else {
            // Rectangle: swap columns
            ciphertext += keyTable[row1][col2];
            ciphertext += keyTable[row2][col1];
        }
    }
    return ciphertext;
}

int main() {
    string key, plaintext;

    cout << "Enter the keyword: ";
    getline(cin, key);
    cout << "Enter the plaintext: ";
    getline(cin, plaintext);

    char keyTable[5][5];
    generateKeyTable(key, keyTable);

    cout << "\nKey Table:\n";
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            cout << keyTable[i][j] << " ";
        }
        cout << endl;
    }

    string preparedText = preparePlaintext(plaintext);
    cout << "\nPrepared Plaintext: " << preparedText << endl;

    string ciphertext = encrypt(preparedText, keyTable);
    cout << "Ciphertext: " << ciphertext << endl;

    return 0;
}
