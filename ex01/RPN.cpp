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
            int n2 = stck.top();
            stck.pop();
            int n1 = stck.top();
            stck.pop();
            if (word == "+")
                stck.push(n1 + n2);
            if (word == "-")
                stck.push(n1 - n2);
            if (word == "/")
            {
                if (n2 == 0)
                    throw std::exception();
                stck.push(n1 / n2);
            }
            if (word == "*")
                stck.push(n1 * n2);
        }
        else
        {
            if (word.size() != 1 || !std::isdigit(word[0]))
                throw std::exception();
            stck.push(word[0] - '0');
        }
    }
}
