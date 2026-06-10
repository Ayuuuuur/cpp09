#include "PmergeMe.hpp"

int main(int argc, char **argv) 
{
	if(argc < 2)
	{
		std::cerr << "Error: no arguments provided" << std::endl;
		return 1;
	}

	try 
	{
		std::vector<unsigned int> numbers;
		for(int j = 1; j < argc; j++) 
		{
			validateInput(argv[j]);
			long val = std::atol(argv[j]);
			if(val > INT_MAX)
				throw std::runtime_error("Error: value out of range");
			numbers.push_back(static_cast<unsigned int>(val));
		}

		bool alreadySorted = true;
        for(size_t i = 1; i < numbers.size(); i++) 
		{
            if(numbers[i] < numbers[i - 1]) 
			{
                alreadySorted = false;
                break;
            }
        }
        if(alreadySorted)
            throw std::runtime_error("Error: sequence is already sorted");

		PmergeMe sorter;
		sorter.loadNumbers(numbers);
		sorter.run();
	}
	catch(const std::exception &e) 
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}