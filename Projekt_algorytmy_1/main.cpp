#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>

using ll = long long;
using namespace std;

class ONPConverter {
private:
    // Priorytety operatorów
    int getPriority(char op) {
        switch (op) {
            case '~': return 4; // Unarny minus
            case '^': return 3;
            case '*':
            case '/': return 2;
            case '+':
            case '-': return 1;
            default: return 0;
        }
    }

    // Łączność operatorów (prawostronna dla potęgowania i unarnego minusa)
    bool isRightAssociative(char op) {
        return op == '^' || op == '~';
    }

    bool isNumber(const string& token) {
        if (token.empty()) return false;
        // Obsługa liczb ujemnych, jeśli wystąpiłyby bezpośrednio w RPN
        size_t start = (token[0] == '-' && token.size() > 1) ? 1 : 0;
        return isdigit(token[start]);
    }

    bool isOperator(char c) {
        return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '~');
    }

    // Szybkie potęgowanie dla liczb całkowitych
    ll intPow(ll base, ll exp) {
        if (exp < 0) return 0;
        if (exp == 0) return 1;
        ll result = 1;
        while (exp > 0) {
            if (exp & 1) result *= base;
            base *= base;
            exp >>= 1;
        }
        return result;
    }

public:
    // Konwersja Infix -> RPN (Stacja Rozrządowa Shunting-Yard)
    string toRPN(const string& expr) {
        stack<char> operatorStack;
        stringstream output;
        stringstream ss(expr);
        string token, prevToken = "";

        while (ss >> token) {
            if (isNumber(token)) {
                output << token << " ";
            }
            else if (token == "(") {
                operatorStack.push('(');
            }
            else if (token == ")") {
                while (!operatorStack.empty() && operatorStack.top() != '(') {
                    output << operatorStack.top() << " ";
                    operatorStack.pop();
                }
                if (!operatorStack.empty()) operatorStack.pop(); // Usuń '('
            }
            else {
                char op = token[0];
                // Zamiana binarnego '-' na unarny '~' jeśli występuje na początku lub po innym operatorze
                if (op == '-' && (prevToken.empty() || prevToken == "(" ||
                    (prevToken.length() == 1 && isOperator(prevToken[0])))) {
                    op = '~';
                }

                while (!operatorStack.empty() && operatorStack.top() != '(') {
                    int topPrio = getPriority(operatorStack.top());
                    int currPrio = getPriority(op);

                    if (topPrio > currPrio || (topPrio == currPrio && !isRightAssociative(op))) {
                        output << operatorStack.top() << " ";
                        operatorStack.pop();
                    } else break;
                }
                operatorStack.push(op);
            }
            prevToken = token;
        }

        while (!operatorStack.empty()) {
            output << operatorStack.top() << " ";
            operatorStack.pop();
        }

        string result = output.str();
        if (!result.empty()) result.pop_back(); // Usuwanie ostatniej spacji
        return result;
    }

    // Ewaluacja wyrażenia RPN
    ll evaluateRPN(const string& rpn) {
        stack<ll> st;
        stringstream ss(rpn);
        string token;

        while (ss >> token) {
            if (isNumber(token)) {
                st.push(stoll(token));
            }
            else if (token == "~") {
                ll a = st.top(); st.pop();
                st.push(-a);
            }
            else {
                ll b = st.top(); st.pop();
                ll a = st.top(); st.pop();

                switch (token[0]) {
                    case '+': st.push(a + b); break;
                    case '-': st.push(a - b); break;
                    case '*': st.push(a * b); break;
                    case '/': st.push(a / b); break;
                    case '^': st.push(intPow(a, b)); break;
                }
            }
        }
        return st.top();
    }
};

int main() {
    ONPConverter converter;

    // Test cases for converting INFIX to RPN
    vector<pair<string, string>> testCasesInfix = {
        {"2 * ( 3 + 3 )", "Infix: 2 * (3 + 3)"},
        {"10 + 20 * 3", "Infix: 10 + 20 * 3"},
        {"( 5 + 3 ) * 2", "Infix: (5 + 3) * 2"},
        {"2 ^ 3 ^ 2", "Infix: 2 ^ 3 ^ 2 (right-associative)"},
        {"100 / 5 - 10", "Infix: 100 / 5 - 10"},
        {"( 10 + 5 ) * ( 3 - 1 )", "Infix: (10 + 5) * (3 - 1)"},
        {"- 5 + 3", "Infix: -5 + 3 (unary minus)"},
        {"3 * ( - 4 )", "Infix: 3 * (-4)"},
        {"- 10 * - 2", "Infix: -10 * -2"},
        {"5 - 3", "Infix: 5 - 3 (binary minus)"},
        {"- 5 - 3", "Infix: -5 - 3 (unary then binary)"},
        {"( - 5 + 3 ) * 2", "Infix: (-5 + 3) * 2"}
    };

    // Test cases for evaluating RPN
    vector<pair<string, string>> testCasesRPN = {
        {"2 3 3 + *", "RPN: 2 3 3 + *"},
        {"10 20 3 * +", "RPN: 10 20 3 * +"},
        {"5 3 + 2 *", "RPN: 5 3 + 2 *"},
        {"2 3 2 ^ ^", "RPN: 2 3 2 ^ ^"},
        {"100 5 / 10 -", "RPN: 100 5 / 10 -"},
        {"10 5 + 3 1 - *", "RPN: 10 5 + 3 1 - *"},
        {"-5 3 +", "RPN: -5 3 + (with negative number)"},
        {"-10 -2 *", "RPN: -10 -2 * (two negatives)"}
    };

    cout << "============== INFIX TO RPN CONVERSION ==============\n\n";

    for (const auto& test : testCasesInfix) {
        const string& expr = test.first;
        const string& desc = test.second;

        cout << desc << "\n";
        cout << "Input: " << expr << "\n";

        string rpn = converter.toRPN(expr);
        cout << "RPN: " << rpn << "\n";

        ll result = converter.evaluateRPN(rpn);
        cout << "Result: " << result << "\n";
        cout << "-----------------------------------\n\n";
    }

    cout << "\n============== RPN EVALUATION ============== \n\n";

    for (const auto& test : testCasesRPN) {
        const string& expr = test.first;
        const string& desc = test.second;

        cout << desc << "\n";
        cout << "Input: " << expr << "\n";

        ll result = converter.evaluateRPN(expr);
        cout << "Result: " << result << "\n";
        cout << "-----------------------------------\n\n";
    }

    return 0;
}
