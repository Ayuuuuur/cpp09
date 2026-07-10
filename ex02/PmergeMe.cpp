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
		odd     = obj.odd;
		input   = obj.input;
		Vect    = obj.Vect;
		Deq     = obj.Deq;
		vecTime = obj.vecTime;
		dqTime  = obj.dqTime;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::AddNumbers(std::vector<unsigned int> nums)
{
	input = nums;
	for(size_t i = 0 ; i < nums.size();i++)
	{
		Vect.push_back(nums[i]);
		Deq.push_back(nums[i]);
	}
}

//Jacobsthal numbers [0, 1, 1, 3, 5, 11, 21, 43...]
static std::vector<size_t> jacobsthal(size_t n)
{
	std::vector<size_t> jac;
	jac.push_back(0);
	jac.push_back(1);
	size_t i = 2;
	while (jac.back() < n)
	{
		jac.push_back(jac[i - 1] + 2 * jac[i - 2]);
		i++;
	}
	return jac;
}

static void mergeInsertSort(std::vector<unsigned int> &arr)
{
	if (arr.size() <= 1)
		return;

	std::vector<std::pair<unsigned int, unsigned int> > pairs;
	size_t i = 0;
	unsigned int leftover = 0;
	bool hasLeftover = false;

	// Sort pair [small, big]
	for (; i + 1 < arr.size(); i += 2)
	{
		unsigned int a = arr[i];
		unsigned int b = arr[i + 1];
		if (a < b)
			pairs.push_back(std::make_pair(a, b));
		else
			pairs.push_back(std::make_pair(b, a));
	}
	if (i < arr.size())
	{
		leftover = arr[i];
		hasLeftover = true;
	}

	// sort the "big" elements
	std::vector<unsigned int> bigs;
	for (size_t k = 0; k < pairs.size(); k++)
		bigs.push_back(pairs[k].second);
	
	//recursion
	mergeInsertSort(bigs);

	std::vector<unsigned int> pend(bigs.size());
	for (size_t k = 0; k < bigs.size(); k++)
	{
		for (size_t j = 0; j < pairs.size(); j++)
		{
			if (pairs[j].second == bigs[k])
			{
				pend[k] = pairs[j].first;
				pairs.erase(pairs.begin() + j); //delete the first
				break;
			}
		}
	}

	std::vector<unsigned int> main = bigs;
	
	if (!pend.empty())
		main.insert(main.begin(), pend[0]);

	//Jacobsthal numbers [0, 1, 1, 3, 5, 11, 21, 43...]
	std::vector<size_t> jac = jacobsthal(pend.size());
	std::vector<bool> inserted(pend.size(), false);
	if (!pend.empty())
		inserted[0] = true;
	//!inserted pend
	size_t Last = 0;
	for (size_t t = 1; t < jac.size(); t++)
	{
		size_t i = jac[t];
		if (i >= pend.size())
			i = pend.size() - 1;

		for (size_t k = i; k > Last; k--)
		{
			if (k < pend.size() && !inserted[k])
			{
				std::vector<unsigned int>::iterator bound = std::lower_bound(main.begin(), main.end(), pend[k]);
				main.insert(bound, pend[k]);
				inserted[k] = true;
			}
		}
		Last = i;
		if (i == pend.size() - 1)
			break;
	}

	for (size_t k = 0; k < pend.size(); k++)
	{
		if (!inserted[k])
		{
			std::vector<unsigned int>::iterator bound = std::lower_bound(main.begin(), main.end(), pend[k]);
			main.insert(bound, pend[k]);
		}
	}
	if (hasLeftover)
	{
		std::vector<unsigned int>::iterator bound = std::lower_bound(main.begin(), main.end(), leftover);
		main.insert(bound, leftover);
	}
	arr = main;
}

template <typename SortedCon, typename Cont>
void PmergeMe::SortAlgo(SortedCon &main, Cont arr, double &time)
{
	clock_t start = clock();
	std::vector<unsigned int> pend(arr.begin(), arr.end());

	mergeInsertSort(pend);

	for (size_t i = 0; i < pend.size(); i++)
		main.push_back(pend[i]);
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
	if (str.empty() || str.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("Error");
}