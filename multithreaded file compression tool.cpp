#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <string>
#include <mutex>

using namespace std;

// Simple RLE compression for a string chunk
string compressChunk(const string &data) {
    string result;
    int n = data.size();

    for (int i = 0; i < n; i++) {
        int count = 1;
        while (i + 1 < n && data[i] == data[i + 1]) {
            count++;
            i++;
        }
        result += data[i];
        result += to_string(count);
    }
    return result;
}

// Decompress RLE data
string decompressData(const string &data) {
    string result;
    for (size_t i = 0; i < data.size(); i++) {
        char ch = data[i];
        string num = "";
        i++;
        while (i < data.size() && isdigit(data[i])) {
            num += data[i];
            i++;
        }
        i--; // step back
        int count = stoi(num);
        result.append(count, ch);
    }
    return result;
}

int main() {
    string inputFile, outputFile;
    int choice;

    cout << "1. Compress File\n2. Decompress File\nEnter choice: ";
    cin >> choice;

    cout << "Enter input file name: ";
    cin >> inputFile;
    cout << "Enter output file name: ";
    cin >> outputFile;

    // Read entire file
    ifstream fin(inputFile, ios::binary);
    if (!fin) {
        cout << "Error opening input file!\n";
        return 1;
    }

    string data((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    fin.close();

    if (choice == 1) {
        // Compression
        int numThreads = 4; // you can change this
        int len = data.size();
        int chunkSize = len / numThreads;

        vector<string> results(numThreads);
        vector<thread> threads;

        for (int i = 0; i < numThreads; i++) {
            int start = i * chunkSize;
            int end = (i == numThreads - 1) ? len : start + chunkSize;

            threads.push_back(thread([&, i, start, end]() {
                string chunk = data.substr(start, end - start);
                results[i] = compressChunk(chunk);
            }));
        }

        for (auto &t : threads) {
            t.join();
        }

        ofstream fout(outputFile, ios::binary);
        for (const auto &s : results) {
            fout << s;
        }
        fout.close();

        cout << "File compressed successfully using multithreading!\n";
    }
    else if (choice == 2) {
        // Decompression (single thread for simplicity)
        string decompressed = decompressData(data);

        ofstream fout(outputFile, ios::binary);
        fout << decompressed;
        fout.close();

        cout << "File decompressed successfully!\n";
    }
    else {
        cout << "Invalid choice!\n";
    }

    return 0;
}
