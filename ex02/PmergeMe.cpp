#include "PmergeMe.hpp"

PmergeMe::PmergeMe() : _straggler(-1), _vecTime(0), _dqTime(0) {}

PmergeMe::PmergeMe(const PmergeMe &src) {
	*this = src;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &rhs) {
	if (this != &rhs) {
		_straggler = rhs._straggler;
		_input     = rhs._input;
		_vecPairs  = rhs._vecPairs;
		_dqPairs   = rhs._dqPairs;
		_vecTime   = rhs._vecTime;
		_dqTime    = rhs._dqTime;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::loadNumbers(std::vector<unsigned int> nums) {
	_input = nums;
	_straggler = -1;

	size_t idx = 0;
	while (idx + 1 < nums.size()) {
		_vecPairs.push_back(std::make_pair(nums[idx], nums[idx + 1]));
		_dqPairs.push_back(std::make_pair(nums[idx], nums[idx + 1]));
		idx += 2;
	}

	if (nums.size() % 2 != 0)
		_straggler = static_cast<int>(nums.back());
}

template <typename Container, typename PairContainer>
void PmergeMe::mergeInsertSort(Container &main, PairContainer pairs, double &elapsed) {
	Container pend;
	clock_t start = clock();

	// Step 1: sort each pair so smaller is first
	for (size_t k = 0; k < pairs.size(); k++) {
		if (pairs[k].first > pairs[k].second)
			std::swap(pairs[k].first, pairs[k].second);
	}

	// Step 2: build pend (smaller elements) and main (larger elements)
	for (size_t k = 0; k < pairs.size(); k++)
		pend.push_back(pairs[k].first);
	for (size_t k = 0; k < pairs.size(); k++)
		main.push_back(pairs[k].second);

	// Step 3: sort the main chain
	std::sort(main.begin(), main.end());

	// Step 4: binary-insert pend elements into main
	for (size_t k = 0; k < pend.size(); k++)
		main.insert(std::lower_bound(main.begin(), main.end(), pend[k]), pend[k]);

	// Step 5: insert straggler if present
	if (_straggler != -1)
		main.insert(std::lower_bound(main.begin(), main.end(), _straggler), _straggler);

	elapsed = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
}

template <typename Container>
void PmergeMe::printSeq(Container seq) {
	typename Container::iterator it = seq.begin();
	while (it != seq.end()) {
		std::cout << *it;
		if (++it != seq.end())
			std::cout << " ";
	}
}

void PmergeMe::run() {
	std::vector<unsigned int> sortedVec;
	std::deque<unsigned int>  sortedDq;

	mergeInsertSort(sortedVec, _vecPairs, _vecTime);
	mergeInsertSort(sortedDq,  _dqPairs,  _dqTime);

	std::cout << std::fixed;
	std::cout << "Before: "; printSeq(_input);     std::cout << "\n";
	std::cout << "After:  "; printSeq(sortedVec);  std::cout << "\n";

	std::cout << "Time to process a range of " << sortedVec.size()
	          << " elements with std::vector : " << _vecTime << " s\n";
	std::cout << "Time to process a range of " << sortedDq.size()
	          << " elements with std::deque  : " << _dqTime  << " s\n";
}

void validateInput(std::string str) 
{
	if(str.empty() || str.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("Error");
}