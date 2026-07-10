#include "BitcoinExchange.hpp"

static bool isValidNumber(const std::string& str)
{
    if (str.empty())
        return false;

    bool hasDot = false;
    bool hasDigit = false;
    size_t start = 0;

    if (str[0] == '+')
        start = 1;
    if (start >= str.size() || str[start] == '.')
        return false;
    for (size_t i = start; i < str.size(); i++)
    {
        if (std::isdigit(str[i]))
            hasDigit = true;
        else if (str[i] == '.' && !hasDot)
            hasDot = true;
        else
            return false;
    }
    return hasDigit;
}

void fillDataBase(std::map<std::string, double> &baseData)
{
    std::ifstream f("data.csv");
    if (!f.is_open())
        throw ErrorOpeningFile();
    std::string line;
    std::getline(f, line);

    while (std::getline(f, line))
    {
        if (line.empty())
            continue;
        const std::string date  = line.substr(0, 10);
        std::string value = "";
        if (line.size() > 11)
            value = line.substr(11);
        if (value.empty())
            continue;
        baseData[date] = std::atof(value.c_str());
    }
    if (baseData.empty())
        throw std::runtime_error("Error: data.csv contains no valid entries.");
}

static double findRate(const std::map<std::string, double> &baseData,const std::string &date)
{
    std::map<std::string, double>::const_iterator it = baseData.lower_bound(date);

    if (it != baseData.end() && it->first == date)
        return it->second;
    if (it == baseData.begin())
        throw std::runtime_error("date out of range => " + date);
    --it;
    return it->second;
}

static bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static int daysInMonth(int year, int month)
{
    if (month == 2)
        return isLeapYear(year) ? 29 : 28;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    return 31;
}

static void checkLine(const std::string &line)
{
    if (line.size() < 14)
        throw BadInput();

    const std::string sYear  = line.substr(0, 4);
    const std::string sMonth = line.substr(5, 2);
    const std::string sDay   = line.substr(8, 2);

    if (!isValidNumber(sYear) || !isValidNumber(sMonth) || !isValidNumber(sDay))
        throw BadInput();
    if (line[4] != '-' || line[7] != '-')
        throw BadInput();

    const int year  = std::atoi(sYear.c_str());
    const int month = std::atoi(sMonth.c_str());
    const int day   = std::atoi(sDay.c_str());

    if (year < 1)
        throw BadInput();
    if (month < 1 || month > 12)
        throw BadInput();
    if (day < 1 || day > daysInMonth(year, month))
        throw BadInput();
    if (line.substr(10, 3) != " | ")
        throw BadInput();

    const std::string value = line.substr(13);

    if (value.empty())
        throw BadInput();
    if (value[0] == '-')
        throw NegativeValue();
    if (!isValidNumber(value))
        throw BadInput();
    if (std::atof(value.c_str()) > 1000.0)
        throw LargeNumber();
}

void parseInputFile(const std::map<std::string, double> &baseData,const std::string &inputFile)
{
    std::ifstream f(inputFile.c_str());
    if (!f.is_open())
        throw ErrorOpeningFile();

    std::string line;
    std::getline(f, line);
    if (line != "date | value")
        throw std::runtime_error("Error: bad file header, expected \"date | value\".");

    while (std::getline(f, line))
    {
        try
        {
            checkLine(line);
            const std::string date     = line.substr(0, 10);
            const double      quantity = std::atof(line.substr(13).c_str());
            const double      rate     = findRate(baseData, date);

            std::cout << date << " => " << quantity << " = " << rate * quantity << "\n";
        }
        catch (const BadInput &e)
        {
            std::cout << "Error: " << e.what() << line << "\n";
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}