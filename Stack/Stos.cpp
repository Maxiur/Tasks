#include <iostream>

class StackArray {
private:
    int* stack;
    int capacity;
    int top;

public:
    StackArray(int size) : capacity(size), top(-1) {stack = new int[capacity];
    }

    ~StackArray() {
        delete[] stack;
    }

    void push(int value) {
        if (isFull()) throw std::overflow_error("Stack is full");

        top++;
        stack[top] = value;
    }

    int pop() {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        top--;
        return stack[top + 1];
    }

    int peek() {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        return stack[top];
    }

    bool isEmpty() {
        return top == -1;
    }
    bool isFull() {
        return top == capacity - 1;
    }
};

struct Node {
    Node* next;
    int value;
    Node(int val, Node* nxt = nullptr) : value(val), next(nxt) {}
};

class StackLinked {
private:
    Node* top;

public:
    StackLinked() = default;
    ~StackLinked() {
        while (!isEmpty())
            pop();
    }

    void push(int value) {
        top = new Node(value, top);
    }

    int pop() {
        if (isEmpty()) throw std::underflow_error("Stack is empty");

        Node* oldTop = top;
        int data = oldTop->value;
        top = top->next;

        delete oldTop;

        return data;
    }

    int peek() {
        if (isEmpty()) throw std::underflow_error("Stack is empty");
        return top->value;
    }

    bool isEmpty() {
        return top == nullptr;
    }

};

class ONPCalculator {
private:
    StackLinked stack;

public:
    ONPCalculator() = default;
    ~ONPCalculator() = default;

    double calculate(const std::string& expression) {
        for (char c : expression) {
            if (c == ' ') continue;
            if (isdigit(c)) {
                stack.push(c - '0');
                continue;
            }
            double b = stack.pop();
            double a = stack.pop();
            switch (c) {
                case '+': stack.push(a + b); break;
                case '-': stack.push(a - b); break;
                case '*': stack.push(a * b); break;
                case '/':
                    if (b == 0.0) {
                        throw std::runtime_error("Division by zero");
                    }
                    else
                        stack.push(a / b); break;
            }
        }
        return stack.pop();
    }
};

int main() {
    ONPCalculator calc;

    std::cout << calc.calculate("23+") << " == 5\n";       // 2 + 3 = 5
    std::cout << calc.calculate("82/") << " == 4\n";       // 8 / 2 = 4
    std::cout << calc.calculate("92-") << " == 7\n";       // 9 - 2 = 7
    std::cout << calc.calculate("34*") << " == 12\n";      // 3 * 4 = 12
    std::cout << calc.calculate("56+3*") << " == 33\n";    // (5 + 6) * 3 = 33

    return 0;
}