#include "BitcoinExchange.hpp"

int main(int ac,char **av)
{
    try
    {
        std::map<std::string,double> map;
        std::map<std::string ,double> input;
        if(ac != 2)
        {
            std::cerr <<"Error: could not open file." << std::endl;
            return 1;
        }
        getData(map);
        getInput(input,av[1]);
        processResults(map,input);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 1;
}