#include "PmergeMe.hpp"

PmergeMe::PmergeMe() : odd(-1), vecTime(0), dqTime(0) {}

PmergeMe::PmergeMe(const PmergeMe &obj) 
{
	*this = obj;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &obj) 
{
	if (this != &obj) 
	{
		odd = obj.odd;
		input     = obj.input;
		Vect  = obj.Vect;
		Deq   = obj.Deq;
		vecTime   = obj.vecTime;
		dqTime    = obj.dqTime;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::AddNumbers(std::vector<unsigned int> nums)
{
	size_t i = 0;
	input = nums;
	odd = -1;

	while (i + 1 < nums.size())
	{
		Vect.push_back(std::make_pair(nums[i], nums[i + 1]));
		Deq.push_back(std::make_pair(nums[i], nums[i + 1]));
		i += 2;
	}
	if (nums.size() % 2 != 0)
		odd = static_cast<int>(nums.back());
}

template <typename SortedCon, typename Cont>
void PmergeMe::SortAlgo(SortedCon &main, Cont arr, double &time) 
{
	SortedCon pend;
	clock_t start = clock();

	for (size_t i = 0; i < arr.size(); i++)
	{
		if (arr[i].first > arr[i].second)
			std::swap(arr[i].first, arr[i].second);
	}
	for (size_t i = 0; i < arr.size(); i++)
	{
		pend.push_back(arr[i].first);
		main.push_back(arr[i].second);
	}
	std::sort(main.begin(), main.end());
	for (size_t i = 0; i < pend.size(); i++)
		main.insert(std::lower_bound(main.begin(), main.end(), pend[i]), pend[i]);
	if (odd != -1)
		main.insert(std::lower_bound(main.begin(), main.end(), odd), odd);
	time = static_cast<double>(clock() - start) / CLOCKS_PER_SEC * 1000000;
}

template <typename Container>
void PmergeMe::Print(Container seq)
{
	typename Container::iterator it = seq.begin();
	while (it != seq.end())
	{
		std::cout << *it;
		if (++it != seq.end())
			std::cout << " ";
	}
}

void PmergeMe::run() 
{
	std::vector<unsigned int> sortedVec;
	std::deque<unsigned int>  sortedDq;

	SortAlgo(sortedVec, Vect, vecTime);
	SortAlgo(sortedDq,  Deq,  dqTime);

	std::cout << std::fixed;
	std::cout << "Before: "; Print(input);     std::cout << "\n";
	std::cout << "After:  "; Print(sortedVec);  std::cout << "\n";

	std::cout << "Time to process a range of " << sortedVec.size() << " elements with std::vector : " << vecTime << " us\n";
	std::cout << "Time to process a range of " << sortedDq.size()<< " elements with std::deque  : " << dqTime  << " us\n";
}

void validateInput(std::string str) 
{
	if(str.empty() || str.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("Error");
}