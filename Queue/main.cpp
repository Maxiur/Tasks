#include <iostream>

struct BiList {
    int value;
    BiList* next;
    BiList* prev;

    explicit BiList(int val=0, BiList* nxt=nullptr, BiList* prev=nullptr) : value(val), next(nxt), prev(prev) {}
};

class QueueLinked {
private:
    BiList* head = new BiList(0);
    BiList* tail = new BiList(0, nullptr, head);

public:
    QueueLinked() {
        head->next = tail;
    }

    ~QueueLinked() {
        while (!isEmpty()) {
            this->pop();
        }
        delete head;
        delete tail;
    }


    void push(int val) {
        BiList* node = new BiList(val, head->next, head);
        head->next = node;
        node->next->prev = node;
    }

    int pop() {
        if (isEmpty()) return -1;

        BiList* toDelete = tail->prev;
        int val = toDelete->value;
        tail->prev = toDelete->prev;
        tail->prev->next = tail;

        delete toDelete;

        return val;
    }

    int getFront() {
        if (!isEmpty())
            return tail->prev->value;

        return -1;
    }

    bool isEmpty() {
        return head->next == tail;
    }
};

class QueueArray {
private:
    int* arr;
    int capacity;
    int front;
    int rear;
    int count;
public:
    explicit QueueArray(int cap = 10) : capacity(cap), front(0), rear(0), count(0) {
        arr = new int[capacity];
    }

    ~QueueArray() {
        delete[] arr;
    }

    void push(int val) {
        if (isFull()) return;

        arr[rear] = val;
        rear = (rear + 1) % capacity;
        count++;
    }

    int pop() {
        if (isEmpty()) return -1;

        int val = arr[front];
        front = (front + 1) % capacity;
        count--;

        return val;
    }

    int getFront() {
        if (!isEmpty()) return arr[front];
        return -1;
    }

    const bool isEmpty() const {
        return count == 0;
    }

    const bool isFull() {
        return count == capacity;
    }
};

int main() {
    QueueLinked queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);

    for (int i = 0; i < 3; i++) {
        std::cout << queue.pop() << std::endl;
    }

    QueueArray qa(3);
    qa.push(10);
    qa.push(20);
    qa.push(30);
    std::cout << qa.pop() << std::endl;   // 10
    std::cout << qa.pop() << std::endl;   // 20
    qa.push(40);
    std::cout << qa.getFront() << std::endl;  // 30

}
