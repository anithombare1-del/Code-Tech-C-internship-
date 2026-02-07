#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void writeFile(const string &filename) {
    ofstream fout(filename); // overwrite mode
    if (!fout) {
        cout << "Error opening file for writing!\n";
        return;
    }

    cout << "Enter text to write into file (end with a single line 'END'):\n";
    string line;
    cin.ignore(); // clear buffer
    while (true) {
        getline(cin, line);
        if (line == "END") break;
        fout << line << endl;
    }

    fout.close();
    cout << "Data written successfully.\n";
}

void appendFile(const string &filename) {
    ofstream fout(filename, ios::app); // append mode
    if (!fout) {
        cout << "Error opening file for appending!\n";
        return;
    }

    cout << "Enter text to append into file (end with a single line 'END'):\n";
    string line;
    cin.ignore();
    while (true) {
        getline(cin, line);
        if (line == "END") break;
        fout << line << endl;
    }

    fout.close();
    cout << "Data appended successfully.\n";
}

void readFile(const string &filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "Error opening file for reading! File may not exist.\n";
        return;
    }

    cout << "\n--- File Content ---\n";
    string line;
    while (getline(fin, line)) {
        cout << line << endl;
    }
    cout << "--------------------\n";

    fin.close();
}

int main() {
    string filename;
    int choice;

    cout << "Enter file name (e.g., data.txt): ";
    cin >> filename;

    do {
        cout << "\n===== FILE MANAGEMENT TOOL =====\n";
        cout << "1. Write to file (overwrite)\n";
        cout << "2. Append to file\n";
        cout << "3. Read file\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                writeFile(filename);
                break;
            case 2:
                appendFile(filename);
                break;
            case 3:
                readFile(filename);
                break;
            case 4:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
