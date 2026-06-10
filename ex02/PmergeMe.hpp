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

class PmergeMe {
	private:
		std::vector<unsigned int> _input;
		std::vector<std::pair<unsigned int, unsigned int> > _vecPairs;
		std::deque<std::pair<unsigned int, unsigned int> > _dqPairs;
		int _straggler;
		double _vecTime;
		double _dqTime;

	public:
		PmergeMe();
		PmergeMe(const PmergeMe &src);
		PmergeMe &operator=(const PmergeMe &rhs);
		~PmergeMe();

		void loadNumbers(std::vector<unsigned int> nums);
		void run();

		template <typename Container>
		void printSeq(Container seq);

		template <typename Container, typename PairContainer>
		void mergeInsertSort(Container &main, PairContainer pairs, double &elapsed);
};

void validateInput(std::string token);

#endif