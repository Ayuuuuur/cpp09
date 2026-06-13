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
		Vect  = obj.Vect;
		Deq   = obj.Deq;
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
		Vect.push_back(std::make_pair(nums[i], nums[i + 1]));
		Deq.push_back(std::make_pair(nums[i], nums[i + 1]));
		i += 2;
	}

	if (nums.size() % 2 != 0)
		straggler = static_cast<int>(nums.back());
}

template <typename SortedCon, typename Cont>
void PmergeMe::mergeInsertSort(SortedCon &Sorted, Cont Str, double &time) 
{
	SortedCon pend;
	clock_t start = clock();

	for (size_t i = 0; i < Str.size(); i++)
	{
		if (Str[i].first > Str[i].second)
			std::swap(Str[i].first, Str[i].second);
	}

	for (size_t i = 0; i < Str.size(); i++)
	{
		pend.push_back(Str[i].first);
		Sorted.push_back(Str[i].second);
	}

	std::sort(Sorted.begin(), Sorted.end());
	for (size_t i = 0; i < pend.size(); i++)
		Sorted.insert(std::lower_bound(Sorted.begin(), Sorted.end(), pend[i]), pend[i]);
	if (straggler != -1)
		Sorted.insert(std::lower_bound(Sorted.begin(), Sorted.end(), straggler), straggler);
	time = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
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

void PmergeMe::run() 
{
	std::vector<unsigned int> sortedVec;
	std::deque<unsigned int>  sortedDq;

	// mergeInsertSort(sortedVec, Vect, vecTime);
	mergeInsertSort(sortedDq,  Deq,  dqTime);

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