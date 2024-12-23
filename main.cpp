#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <vector>
#include <openssl/sha.h>

using namespace std;

class Block {
public:
    int index;
    string previousHash;
    string timestamp;
    string data;
    string hash;

    Block(int idx, string prevHash, string ts, string d)
        : index(idx), previousHash(prevHash), timestamp(ts), data(d) {
        hash = calculateHash();
    }

    string calculateHash() {
        string input = to_string(index) + previousHash + timestamp + data;
        return sha256(input);
    }

    static string sha256(const string str) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.c_str(), str.size());
        SHA256_Final(hash, &sha256);
        string output;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            char buf[3];
            sprintf(buf, "%02x", hash[i]);
            output += buf;
        }
        return output;
    }

    void saveToFile(ofstream &file) {
        file << "Block " << index << ":\n";
        file << "  Previous Hash: " << previousHash << "\n";
        file << "  Timestamp: " << timestamp;
        file << "  Data: " << data << "\n";
        file << "  Hash: " << hash << "\n\n";
    }
};

string getCurrentTimestamp() {
    time_t now = time(0);
    return ctime(&now);
}

int main() {
    vector<Block> blockchain;
    ofstream file("blocks.txt", ios::trunc);
    Block genesisBlock(0, "0", getCurrentTimestamp(), "Genesis Block");
    blockchain.push_back(genesisBlock);
    genesisBlock.saveToFile(file);

    for (int i = 1; i <= 4; i++) {
        Block newBlock(i, blockchain.back().hash, getCurrentTimestamp(), "Block " + to_string(i) + " Data");
        blockchain.push_back(newBlock);
        newBlock.saveToFile(file);
    }

    file.close();
    return 0;
}
