#include "BitcoinExchange.hpp"

/* ================================= Utils ================================= */

/*
** isValidNumber — returns true if str is a valid non-negative decimal number.
** Accepts an optional leading '+', digits, and at most one '.'.
** Rejects empty string, leading '.', or any non-digit non-dot character.
*/
static bool isValidNumber(const std::string &str)
{
    if (str.empty())
        return false;

    std::string::const_iterator it = str.begin();

    if (*it == '+')
        ++it;

    // Leading dot is not a valid number ("." or ".5" alone is ambiguous)
    if (it == str.end() || *it == '.')
        return false;

    bool hasDot   = false;
    bool hasDigit = false;

    for (; it != str.end(); ++it)
    {
        if (std::isdigit(*it))
            hasDigit = true;
        else if (*it == '.' && !hasDot)
            hasDot = true;
        else
            return false;
    }

    return hasDigit;
}

/*
** isLeapYear — returns true for Gregorian leap years.
** Rule: divisible by 4, EXCEPT century years must be divisible by 400.
*/
static bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/*
** daysInMonth — returns the number of days in a given month/year.
*/
static int daysInMonth(int year, int month)
{
    if (month == 2)
        return isLeapYear(year) ? 29 : 28;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    return 31;
}

/* ========================== Line Validation ============================== */

/*
** checkLine — validates one line from the input file.
** Expected format: "YYYY-MM-DD | <positive float <= 1000>"
** Throws the appropriate exception on any violation.
*/
static void checkLine(const std::string &line)
{
    // Minimum length: 10 (date) + 3 ( | ) + 1 (digit) = 14
    if (line.size() < 14)
        throw BadInput();

    // ── Date part ────────────────────────────────────────────────────────
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

    // ── Separator ─────────────────────────────────────────────────────────
    if (line.substr(10, 3) != " | ")
        throw BadInput();

    // ── Value part ────────────────────────────────────────────────────────
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

/* ====================== Database & Lookup ================================ */

/*
** fillDataBase — reads data.csv and populates baseData.
** Keys are date strings "YYYY-MM-DD"; std::map sorts them lexicographically,
** which is identical to chronological order for ISO-8601 dates.
*/
void fillDataBase(std::map<std::string, double> &baseData)
{
    std::ifstream f("data.csv");
    if (!f.is_open())
        throw ErrorOpeningFile();

    std::string line;
    std::getline(f, line); // skip header

    while (std::getline(f, line))
    {
        if (line.empty())
            continue;

        const std::string date  = line.substr(0, 10);
        const std::string value = line.size() > 11 ? line.substr(11) : "";

        if (value.empty())
            continue;

        baseData[date] = std::atof(value.c_str());
    }

    if (baseData.empty())
        throw std::runtime_error("Error: data.csv contains no valid entries.");
}

/*
** findRate — given a date string, finds the exchange rate using the
** closest earlier-or-equal date in baseData.
**
** std::map with string keys works perfectly here because ISO-8601 dates
** ("YYYY-MM-DD") sort lexicographically == chronologically.
** lower_bound(key) returns the first entry >= key.
** If it doesn't point exactly at key, we step back one to get the
** closest earlier date.
*/
static double findRate(const std::map<std::string, double> &baseData,const std::string &date)
{
    std::map<std::string, double>::const_iterator it = baseData.lower_bound(date);

    // Exact match
    if (it != baseData.end() && it->first == date)
        return it->second;

    // No earlier date exists in the DB
    if (it == baseData.begin())
        throw std::runtime_error("date out of range => " + date);

    // Step back to the closest lower date
    --it;
    return it->second;
}

/* ========================= Input File Processing ========================= */

/*
** parseInputFile — reads the user-supplied file line by line,
** validates each entry, looks up the rate, and prints the result.
** Per-line errors are caught and printed without stopping execution.
*/
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

            std::cout << date << " => " << quantity
                      << " = " << rate * quantity << "\n";
        }
        catch (const BadInput &e)
        {
            // BadInput appends the raw line so the user sees what was wrong
            std::cout << "Error: " << e.what() << line << "\n";
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}