#include "RPN.hpp"


int main(int ac, char **av)
{
    std::stack<int> stck;
    if (ac != 2)
        return (std::cerr << "Error\n",1);
    try
    {
        splitInput(av[1], stck);
    }
    catch(std::exception& e)
    {
        std::cerr << "Error\n";
        return 1;
    }
    if (stck.size() == 1)
        std::cout << stck.top() << "\n";
    else
    {
        std::cerr << "Error\n";
        return 1;
    }
    return 0;
}
