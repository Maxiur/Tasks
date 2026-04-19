#include <iostream>
#include <vector>
#include <chrono>
#include <random>

template <typename T>
class PriorityQueueBinary {
private:
    std::vector<T> heap;

    void heapify(int i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int n = heap.size();

        // Szukamy najmniejszego wśród rodzica i dwójki dzieci
        if (left < n && heap[left] < heap[smallest]) {
            smallest = left;
        }

        if (right < n && heap[right] < heap[smallest]) {
            smallest = right;
        }

        // Jeśli rodzic nie jest najmniejszy, robimy roszadę
        if (smallest != i) {
            std::swap(heap[i], heap[smallest]);
            heapify(smallest);
        }
    }

    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (heap[parent] > heap[i]) {
                std::swap(heap[i], heap[parent]);
                i = parent;
            }
            else
                break;
        }
    }

public:
    PriorityQueueBinary() = default;

    void insert(T val) {
        heap.push_back(val);
        siftUp(heap.size() - 1);
    }

    T popMin() {
        if (heap.empty()) throw std::runtime_error("Priority Queue is empty");


        T minVal = heap[0];

        // Wrzucamy z ostatniego miejsca
        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            heapify(0);
        }

        return minVal;
    }

};

void benchmark() {
    std::cout << "Rozmiar(N),Czas_popMin_Kopiec(ns)\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 1000000);

    for (int n = 10000; n <= 100000; n += 10000) {
        PriorityQueueBinary<int> pq;

        for (int i = 0; i < n; ++i) {
            pq.insert(dist(gen));
        }

        auto start = std::chrono::high_resolution_clock::now();
        pq.popMin();
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        std::cout << n << "," << duration << "\n";
    }
}

int main() {
    PriorityQueueBinary<int> pq;

    std::cout << "--- Szybki tescik logiki ---\n";
    PriorityQueueBinary<int> testPq;
    testPq.insert(42);
    testPq.insert(10);
    testPq.insert(100);
    testPq.insert(5);

    std::cout << "Wyciagamy (ma byc 5): " << testPq.popMin() << "\n";
    std::cout << "Kolejny (ma byc 10): " << testPq.popMin() << "\n";
    std::cout << "Kolejny (ma byc 42): " << testPq.popMin() << "\n\n";

    std::cout << "--- benchmarki ---\n";
    benchmark();

    return 0;
}