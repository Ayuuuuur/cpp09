#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cstdlib>
#include <cctype>
#include <exception>
#include <algorithm>

class ErrorOpeningFile : public std::exception 
{
    public:
        const char *what() const throw() 
        {
            return "Error: could not open file."; 
        }
};

class BadInput : public std::exception 
{
    public:
        const char *what() const throw() 
        {
            return "bad input => "; 
        }
};

class NegativeValue : public std::exception 
{
    public:
        const char *what() const throw() 
        { 
            return "not a positive number."; 
        }
};

class LargeNumber : public std::exception 
{
    public:
        const char *what() const throw()
        {
            return "too large a number.";
        }
};

void    fillDataBase(std::map<std::string, double> &baseData);
void    parseInputFile(const std::map<std::string, double> &baseData,const std::string &inputFile);

#endif