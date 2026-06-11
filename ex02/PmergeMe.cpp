#include "PmergeMe.hpp"

PmergeMe::PmergeMe() : straggler(-1), vecTime(0), dqTime(0) {}

PmergeMe::PmergeMe(const PmergeMe &obj) 
{
	*this = obj;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &obj) 
{
	if (this != &obj) 
	{
		straggler = obj.straggler;
		input     = obj.input;
		vecPairs  = obj.vecPairs;
		dqPairs   = obj.dqPairs;
		vecTime   = obj.vecTime;
		dqTime    = obj.dqTime;
	}
	return *this;
}


PmergeMe::~PmergeMe() {}

void PmergeMe::loadNumbers(std::vector<unsigned int> nums)
{
	size_t i = 0;
	input = nums;
	straggler = -1;

	while (i + 1 < nums.size())
	{
		vecPairs.push_back(std::make_pair(nums[i], nums[i + 1]));
		dqPairs.push_back(std::make_pair(nums[i], nums[i + 1]));
		i += 2;
	}

	if (nums.size() % 2 != 0)
		straggler = static_cast<int>(nums.back());
}

template <typename Container, typename PairContainer>
void PmergeMe::mergeInsertSort(Container &main, PairContainer pairs, double &elapsed) 
{
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
	if (straggler != -1)
		main.insert(std::lower_bound(main.begin(), main.end(), straggler), straggler);

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

	mergeInsertSort(sortedVec, vecPairs, vecTime);
	mergeInsertSort(sortedDq,  dqPairs,  dqTime);

	std::cout << std::fixed;
	std::cout << "Before: "; printSeq(input);     std::cout << "\n";
	std::cout << "After:  "; printSeq(sortedVec);  std::cout << "\n";

	std::cout << "Time to process a range of " << sortedVec.size()
	          << " elements with std::vector : " << vecTime << " s\n";
	std::cout << "Time to process a range of " << sortedDq.size()
	          << " elements with std::deque  : " << dqTime  << " s\n";
}

void validateInput(std::string str) 
{
	if(str.empty() || str.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("Error");
}