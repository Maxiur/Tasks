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
    // Returns operator priority
    int getPriority(char op) {
        switch (op) {
            case '^': return 3;
            case '*':
            case '/': return 2;
            case '+':
            case '-': return 1;
            default: return 0;
        }
    }

    // Check if operator is right-associative
    bool isRightAssociative(char op) {
        return op == '^';
    }

    // Check if string is a number (including negative numbers like -5)
    bool isNumber(const string& token) {
        if (token.empty()) return false;

        size_t start = 0;
        if (token[0] == '-') {
            if (token.size() == 1) return false;
            start = 1;
        }

        for (size_t i = start; i < token.size(); ++i) {
            if (!isdigit(token[i])) return false;
        }
        return true;
    }

    // Check if token is an operator
    bool isOperator(const string& token) {
        return token.length() == 1 &&
               (token[0] == '+' || token[0] == '-' || token[0] == '*' ||
                token[0] == '/' || token[0] == '^');
    }

    // Integer power function
    ll intPow(ll base, ll exp) {
        if (exp < 0) return 0;
        ll result = 1;
        while (exp > 0) {
            if (exp & 1) result *= base;
            base *= base;
            exp >>= 1;
        }
        return result;
    }

    // Tokenize expression and handle negative numbers
    vector<string> tokenize(const string& expr) {
        vector<string> tokens;
        stringstream ss(expr);
        string token;
        string prevToken = "";

        while (ss >> token) {
            // Check if '-' should be treated as part of a negative number
            if (token == "-") {
                // It's unary minus if:
                // 1. It's the first token
                // 2. Previous token was an operator
                // 3. Previous token was '('
                if (prevToken.empty() ||
                    isOperator(prevToken) || prevToken == "(") {

                    // Read next token (the number)
                    string nextToken;
                    if (ss >> nextToken) {
                        // Combine '-' with the number
                        tokens.push_back("-" + nextToken);
                        prevToken = "-" + nextToken;
                        continue;
                    }
                }
            }

            tokens.push_back(token);
            prevToken = token;
        }

        return tokens;
    }

public:
    // Convert infix to RPN (Reverse Polish Notation) using Shunting Yard algorithm
    string toRPN(const string& expr) {
        stack<char> operatorStack;
        stringstream output;

        vector<string> tokens = tokenize(expr);

        for (const string& token : tokens) {
            // If it's a number (including negative), add to output
            if (isNumber(token)) {
                output << token << " ";
            }
            // If it's an operator
            else if (isOperator(token)) {
                char op = token[0];

                // Pop operators with higher or equal priority (considering associativity)
                while (!operatorStack.empty() && operatorStack.top() != '(' &&
                       (getPriority(operatorStack.top()) > getPriority(op) ||
                        (getPriority(operatorStack.top()) == getPriority(op) &&
                         !isRightAssociative(op)))) {
                    output << operatorStack.top() << " ";
                    operatorStack.pop();
                }

                operatorStack.push(op);
            }
            // Left parenthesis
            else if (token == "(") {
                operatorStack.push('(');
            }
            // Right parenthesis
            else if (token == ")") {
                while (!operatorStack.empty() && operatorStack.top() != '(') {
                    output << operatorStack.top() << " ";
                    operatorStack.pop();
                }
                if (!operatorStack.empty()) {
                    operatorStack.pop();  // remove '('
                }
            }
        }

        // Pop remaining operators
        while (!operatorStack.empty()) {
            output << operatorStack.top() << " ";
            operatorStack.pop();
        }

        string result = output.str();
        if (!result.empty() && result.back() == ' ') {
            result.pop_back();
        }
        return result;
    }

    // Evaluate RPN expression
    ll evaluateRPN(const string& rpn) {
        stack<ll> st;
        stringstream ss(rpn);
        string token;

        while (ss >> token) {
            if (isNumber(token)) {
                st.push(stoll(token));
            }
            else if (isOperator(token)) {
                // Binary operator - need 2 operands
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
