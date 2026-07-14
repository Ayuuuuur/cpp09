#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <utility>
#include <ctime>
#include <climits>
#include <algorithm>

class PmergeMe 
{
	private:
		std::vector<unsigned int> input;
		std::vector<unsigned int> Vect;
		std::deque<unsigned int> Deq;
		double vecTime;
		double dqTime;

	public:
		PmergeMe();
		PmergeMe(const PmergeMe &src);
		PmergeMe &operator=(const PmergeMe &rhs);
		~PmergeMe();

		void AddNumbers(std::vector<unsigned int> nums);
		void run();

		void Print(const std::vector<unsigned int> &seq);

		void SortVector(std::vector<unsigned int> &sorted);
		void SortDeque(std::deque<unsigned int> &sorted);
};

void validateInput(std::string token);

#endif