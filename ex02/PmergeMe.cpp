#include "RPN.hpp"
#include "RPN.hpp"

void splitInput(std::string input, std::stack<int>& stck)
{
    std::stringstream ss(input);

    std::string word;
    while(getline(ss, word, ' '))
    {
        if (word == "")
            continue;
        if (word == "+" || word == "-" || word == "/" || word == "*")
        {
            if (stck.size() < 2)
                throw std::exception();
            int v2 = stck.top();
            stck.pop();
            int v1 = stck.top();
            stck.pop();
            if (word == "+")
                stck.push(v1 + v2);
            if (word == "-")
                stck.push(v1 - v2);
            if (word == "/")
            {
                if (v2 == 0)
                    throw std::exception();
                stck.push(v1 / v2);
            }
            if (word == "*")
                stck.push(v1 * v2);
        }
        else
        {
            if (isdigit((int)word[0]))
                stck.push(word[0] - '0');
            else
                throw std::exception();
        }
    }
}
