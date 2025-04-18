#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

std::vector<bool> sieve(int max_value) {

    std::vector<bool> is_prime(max_value + 1, true);
    is_prime[0] = false;
    is_prime[1] = false;
    for (int i = 2; i <= max_value; ++i) {
        is_prime[i] = true;
        if(is_prime[i]) {
            for (int j = i * i; j <= max_value; j += i)
                is_prime[j] = false;
        }
    }
    return is_prime;
}

auto main() -> int {
    std::vector<int> A{2, 3, 9, 2, 5, 1, 3, 7, 10};
    std::vector<int> B{2, 1, 3, 4, 3, 10, 6, 6, 1, 7, 10, 10, 10};
    std::vector<int> C{};

    std::unordered_map<int,int> array;
    for (auto i: B)
        array[i]++;

    int max_frequency{};
    for (const auto& [key,value] : array)
        max_frequency = std::max(max_frequency, value);

    auto is_prime = sieve(max_frequency);

    for (auto i: A)
    {
        if (!is_prime[array[i]])
            C.push_back(i);
    }
    for (auto i: C)
    {
        std::cout << i << " ";
    }
    std::cout<< std::endl;


    return 0;
}