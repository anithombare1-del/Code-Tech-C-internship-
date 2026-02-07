#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

class Parser {
    string s;
    int pos;

public:
    Parser(const string& str) : s(str), pos(0) {}

    double parse() {
        double result = expression();
        skipSpaces();
        if (pos < s.length()) {
            throw runtime_error("Unexpected character at position " + to_string(pos));
        }
        return result;
    }

private:
    // expression = term { (+|-) term }
    double expression() {
        double result = term();
        while (true) {
            skipSpaces();
            if (pos < s.length() && s[pos] == '+') {
                pos++;
                result += term();
            } else if (pos < s.length() && s[pos] == '-') {
                pos++;
                result -= term();
            } else {
                break;
            }
        }
        return result;
    }

    // term = factor { (*|/) factor }
    double term() {
        double result = factor();
        while (true) {
            skipSpaces();
            if (pos < s.length() && s[pos] == '*') {
                pos++;
                result *= factor();
            } else if (pos < s.length() && s[pos] == '/') {
                pos++;
                double d = factor();
                if (d == 0) throw runtime_error("Division by zero");
                result /= d;
            } else {
                break;
            }
        }
        return result;
    }

    // factor = number | '(' expression ')'
    double factor() {
        skipSpaces();

        if (pos < s.length() && s[pos] == '(') {
            pos++; // skip '('
            double result = expression();
            skipSpaces();
            if (pos >= s.length() || s[pos] != ')') {
                throw runtime_error("Missing ')'");
            }
            pos++; // skip ')'
            return result;
        }

        return number();
    }

    // Parse number
    double number() {
        skipSpaces();
        int start = pos;
        while (pos < s.length() && (isdigit(s[pos]) || s[pos] == '.')) {
            pos++;
        }
        if (start == pos) {
            throw runtime_error("Expected number at position " + to_string(pos));
        }
        return stod(s.substr(start, pos - start));
    }

    void skipSpaces() {
        while (pos < s.length() && isspace(s[pos])) pos++;
    }
};

int main() {
    cout << "=== Simple Compiler: Expression Evaluator ===\n";
    cout << "Supports: +  -  *  /  ( )\n";
    cout << "Enter expression: ";

    string input;
    getline(cin, input);

    try {
        Parser parser(input);
        double result = parser.parse();
        cout << "Result = " << result << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
