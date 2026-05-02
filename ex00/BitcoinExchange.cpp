#include "BitcoinExchange.hpp"

void parseLine(std::string line,double &value,std::string &date)
{
    size_t pos = line.find(",");
    if(pos == std::string::npos)
        throw std::runtime_error("Error: Invalide line");
    date = line.substr(0, pos);
    std::string valueStr = line.substr(pos + 1);
    value = std::strtod(valueStr.c_str(), NULL);
}

void getData(std::map<std::string,double> &map)
{
    std::string date;
    std::string line;
    double value;

    std::ifstream file("data.csv");
    if(!file.is_open())
        throw OpenError();
    std::getline(file, line); 
    while(getline(file,line))
    {
        parseLine(line,value,date);
        map[date] = value;
    }
}
void parseDate(std::string &date)
{
    // if(date.find("Error"))
    //     throw std::runtime_error(date);
    if (date.size() != 10)
        throw std::runtime_error("Error: Invalide date");
    if (date[4] != '-' || date[7] != '-')
        throw std::runtime_error("Error: Invalide date");
    for(size_t i = 0; i < date.length();i++)
    {
        if(i == 4 || i == 7)
            continue;
        if(!std::isdigit(date[i]))
            throw std::runtime_error("Error: Invalide date");
    }
    int year = std::atoi(date.substr(0,4).c_str());
    int month = std::atoi(date.substr(5,2).c_str());
    int day = std::atoi(date.substr(8,2).c_str());

    if(year < 2009)
        throw std::runtime_error("Error: invalide year");
    if(month > 12 || month < 1)
        throw std::runtime_error("Error: invalide month");
    if(day > 31 || day < 1)
        throw std::runtime_error("Error: invalide day");

}

void parseValue(double &value)
{
    if(value < 0)
        throw std::runtime_error("Error: not a positive number.");
    if(value > 1000)
        throw std::runtime_error("Error:  too large a number.");
}
void parseInput(std::string line,double &value,std::string &date)
{
        size_t pos = line.find(" | ");
        if(pos == std::string::npos)
        {
            date = line;
            value = (double)NULL;
            return;
        }
        date = line.substr(0, pos);
        std::string valueStr = line.substr(pos + 3);
        value = std::strtod(valueStr.c_str(), NULL);
    
}

void getInput(std::multimap<std::string,double> &input,char *av)
{
    std::string date;
    std::string line;
    double value;

    std::ifstream file(av);
    if(!file.is_open())
        throw OpenError();
    std::getline(file, line); 
    while(getline(file,line))
    {
        parseInput(line,value,date);
        input.insert(std::make_pair(date, value));   
    }
}


void processResults(std::map<std::string,double> &db,std::multimap<std::string,double> &input)
{
    std::map<std::string,double>::iterator it = input.begin();
    while (it != input.end())
    {
        std::string date = it->first;
        double value = it->second;
        std::cout << date << " => " <<value << std::endl;
        try
        {
            parseDate(date);
            parseValue(value);         
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            it++;
            continue;
        }
        std::map<std::string,double>::iterator found = db.lower_bound(date);
        if (found == db.begin() && found->first != date)
        {
            it++;
            continue;
        }
        if (found->first != date)
            --found;
        it++;
        std::cout << date << " => " << value << " = " << value * found->second << std::endl;
    }
}
