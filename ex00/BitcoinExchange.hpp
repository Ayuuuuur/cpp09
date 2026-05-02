#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP
#include <iostream>
#include <algorithm>
#include <map>
#include <fstream>
#include <utility>
#include <exception>
#include <string.h>
#include <string>
#include <ctype.h>
#include <sstream>

class OpenError : public std::exception
{
    public:
        const char* what() const throw()
        {
            return "Error : Problem in oppening file";
        }
};

void getData(std::map<std::string,double> &map);
void getInput(std::multimap<std::string,double> &input,char *av);
void processResults(std::map<std::string,double> &db,std::multimap<std::string,double> &input);

#endif