#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Fungsi untuk mencari invers modulo n
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // Invers modulo tidak ditemukan
}

// Fungsi untuk mengenkripsi teks
string encrypt(string plaintext, vector<vector<int>> key) {
    string ciphertext = "";
    int mod = 26;

    for (int i = 0; i < plaintext.length(); i += 2) {
        int block[2] = {plaintext[i] - 'A', plaintext[i + 1] - 'A'};
        int result[2] = {0, 0};

        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                result[j] += key[j][k] * block[k];
            }
            result[j] %= mod;
        }

        ciphertext += (char)(result[0] + 'A');
        ciphertext += (char)(result[1] + 'A');
    }

    return ciphertext;
}

// Fungsi untuk mendekripsi teks
string decrypt(string ciphertext, vector<vector<int>> key) {
    string plaintext = "";
    int mod = 26;

    int det = key[0][0] * key[1][1] - key[0][1] * key[1][0];
    det = (det % mod + mod) % mod; // Pastikan det >= 0

    int detInverse = modInverse(det, mod);
    if (detInverse == -1) {
        cerr << "Tidak dapat mendekripsi. Kunci tidak valid." << endl;
        return "";
    }

    int adj[2][2];
    adj[0][0] = key[1][1];
    adj[1][1] = key[0][0];
    adj[0][1] = -key[0][1];
    adj[1][0] = -key[1][0];

    for (int i = 0; i < ciphertext.length(); i += 2) {
        int block[2] = {ciphertext[i] - 'A', ciphertext[i + 1] - 'A'};
        int result[2] = {0, 0};

        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                result[j] += adj[j][k] * block[k];
            }
            result[j] = (result[j] * detInverse) % mod;
            result[j] = (result[j] % mod + mod) % mod;
        }

        plaintext += (char)(result[0] + 'A');
        plaintext += (char)(result[1] + 'A');
    }

    return plaintext;
}

// Fungsi untuk mencari kunci berdasarkan plain teks dan cipher teks
vector<vector<int>> findKey(string plaintext, string ciphertext) {
    vector<vector<int>> key(2, vector<int>(2));
    int mod = 26;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            key[i][j] = (ciphertext[i * 2 + j] - 'A' - plaintext[i * 2 + j] + 'A') % mod;
            key[i][j] = (key[i][j] % mod + mod) % mod; // Pastikan positif
        }
    }

    return key;
}

int main() {
    int choice;
    string plaintext, ciphertext;
    vector<vector<int>> key;

    do {
        cout << "Menu:\n";
        cout << "1. Enkripsi\n";
        cout << "2. Dekripsi\n";
        cout << "3. Mencari Kunci\n";
        cout << "0. Keluar\n";
        cout << "Pilihan Anda: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Masukkan teks yang akan dienkripsi (huruf kapital tanpa spasi, panjang harus kelipatan 2): ";
                cin >> plaintext;
                cout << "Masukkan kunci Hill Cipher 2x2 (4 angka): ";
                key.clear();
                for (int i = 0; i < 2; i++) {
                    vector<int> row(2);
                    for (int j = 0; j < 2; j++) {
                        cin >> row[j];
                    }
                    key.push_back(row);
                }
                ciphertext = encrypt(plaintext, key);
                cout << "Teks terenkripsi: " << ciphertext << endl;
                break;

            case 2:
                cout << "Masukkan teks yang akan didekripsi (huruf kapital tanpa spasi, panjang harus kelipatan 2): ";
                cin >> ciphertext;
                cout << "Masukkan kunci Hill Cipher 2x2 (4 angka): ";
                key.clear();
                for (int i = 0; i < 2; i++) {
                    vector<int> row(2);
                    for (int j = 0; j < 2; j++) {
                        cin >> row[j];
                    }
                    key.push_back(row);
                }
                plaintext = decrypt(ciphertext, key);
                cout << "Teks terdekripsi: " << plaintext << endl;
                break;

            case 3:
                cout << "Masukkan teks asli (plain teks, huruf kapital tanpa spasi, panjang harus kelipatan 2): ";
                cin >> plaintext;
                cout << "Masukkan teks terenkripsi (huruf kapital tanpa spasi, panjang harus kelipatan 2): ";
                cin >> ciphertext;
                key = findKey(plaintext, ciphertext);
                cout << "Kunci Hill Cipher yang sesuai (4 angka):\n";
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        cout << key[i][j] << ' ';
                    }
                    cout << endl;
                }
                break;

            case 0:
                break;

            default:
                cout << "Pilihan tidak valid. Silakan pilih lagi.\n";
        }
    } while (choice != 0);

    return 0;
}
