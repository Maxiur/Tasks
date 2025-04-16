#include <iostream>
#include <vector>
#include <unordered_map>

bool isPrime(int n)
{
    if (n < 2) return false;
    int k=2;
    while (n%k != 0 && k*k<=n)
        k++;
    if (k*k > n)
        return true;
    return false;
}

auto main() -> int {
    std::vector<int> A{2, 3, 9, 2, 5, 1, 3, 7, 10};
    std::vector<int> B{2, 1, 3, 4, 3, 10, 6, 6, 1, 7, 10, 10, 10};
    std::vector<int> C{};

    std::unordered_map<int,int> array;
    for (auto i: B)
        array[i]++;

    for (auto i: A)
    {
        if (!isPrime(array[i]))
            C.push_back(i);
    }
    for (auto i: C)
    {
        std::cout << i << " ";
    }
    std::cout<< std::endl;


    return 0;
}