#include "RPN.hpp"
#include "RPN.hpp"

void splitInput(std::string input, std::stack<int>& stck)
{
    std::stringstream str(input);
    std::string buffer;

    while(getline(str, buffer, ' '))
    {
        if (buffer == "")
            continue;
        if (buffer == "+" || buffer == "-" || buffer == "/" || buffer == "*")
        {
            if (stck.size() < 2)
                throw std::exception();
            int n2 = stck.top();
            stck.pop();
            int n1 = stck.top();
            stck.pop();
            if (buffer == "+")
                stck.push(n1 + n2);
            if (buffer == "-")
                stck.push(n1 - n2);
            if (buffer == "/")
            {
                if (n2 == 0)
                    throw std::exception();
                stck.push(n1 / n2);
            }
            if (buffer == "*")
                stck.push(n1 * n2);
        }
        else
        {
            if (buffer.size() != 1 || !std::isdigit(buffer[0]))
                throw std::exception();
            stck.push(buffer[0] - '0');
        }
    }
}
