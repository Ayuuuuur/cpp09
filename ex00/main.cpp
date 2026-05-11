#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cout << "Error: could not open file.\n";
        return 1;
    }

    try
    {
        std::map<std::string, double> baseData;
        fillDataBase(baseData);
        parseInputFile(baseData, av[1]);
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << "\n";
        return 1;
    }

    return 0;
}