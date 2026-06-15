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
		std::vector<std::pair<unsigned int, unsigned int> > Vect;
		std::deque<std::pair<unsigned int, unsigned int> > Deq;
		int odd;
		double vecTime;
		double dqTime;

	public:
		PmergeMe();
		PmergeMe(const PmergeMe &src);
		PmergeMe &operator=(const PmergeMe &rhs);
		~PmergeMe();

		void AddNumbers(std::vector<unsigned int> nums);
		void run();

		template <typename Container>
		void Print(Container seq);

		template <typename Container, typename PairContainer>
		void SortAlgo(Container &main, PairContainer pairs, double &elapsed);
};

void validateInput(std::string token);

#endif