#include "PmergeMe.hpp"
#include <iomanip>

int main(int ac, char **av)
{
    if (ac < 2)
        return (std::cerr << "Error\n", 1);

    std::vector<int> parsed;

    for (int i = 1; i < ac; i++)
    {
        std::string s(av[i]);
        for (size_t j = 0; j < s.size(); j++)
            if (!std::isdigit(s[j]))
                return (std::cerr << "Error\n", 1);
        int n = std::atoi(s.c_str());
        if (n < 0)
            return (std::cerr << "Error\n", 1);
        parsed.push_back(n);
    }

    std::cout << "Before: ";
    for (size_t i = 0; i < parsed.size(); i++)
        std::cout << parsed[i] << " ";
    std::cout << "\n";

    std::vector<int> vec;
    std::deque<int>  deq;

    clock_t s1 = clock();
    vec.assign(parsed.begin(), parsed.end());
    mergeInsertVector(vec);
    clock_t e1 = clock();

    clock_t s2 = clock();
    deq.assign(parsed.begin(), parsed.end());
    mergeInsertDeque(deq);
    clock_t e2 = clock();

    std::cout << "After: ";
    for (size_t i = 0; i < vec.size(); i++)
        std::cout << vec[i] << " ";
    std::cout << "\n";

    double timeVec = (double)(e1 - s1) / CLOCKS_PER_SEC * 1000000;
    double timeDeq = (double)(e2 - s2) / CLOCKS_PER_SEC * 1000000;

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << vec.size()
              << " elements with std::[vector] : " << timeVec << " us\n";
    std::cout << "Time to process a range of " << deq.size()
              << " elements with std::[deque]  : " << timeDeq << " us\n";
    return 0;
}