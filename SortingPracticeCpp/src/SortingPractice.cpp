//============================================================================
// Name        : SortingPracticeCpp.cpp
// Author      : Joe Baker
// Version     :
// Copyright   : Student Project
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include <memory>

#include "SortTest.h"
#include "ResultOutput.h"

array_size_t multiply_by_2(array_size_t current_size) {
	return current_size * 2;
}

array_size_t multiply_by_4(array_size_t current_size) {
	return current_size * 4;
}

array_size_t next_increment(array_size_t current_size) {
	return current_size+1;
}

array_size_t multiply_by_10(array_size_t current_size) {
	return current_size * 10;
}

#pragma push_macro("to_i")
#define to_i(e_num)	static_cast<int>(e_num)


/*	******************************************************************************	*/
/*	******************************************************************************	*/
/*									main()											*/
/*	*****************************************************************************	*/
/*	******************************************************************************	*/
#include "StudentDataGenerator.h"
#include "BlockSort.h"

bool testBlockSort();

int main(int argc, char *argv[])
{
	std::cout << "Sorting Performance In C++" << " built on " __DATE__ << " at " __TIME__ << std::endl;

//	testBlockSort();
//	return EXIT_SUCCESS;

	int num_repetitions = 1;

	constexpr array_size_t min_array_size =   32;
	constexpr array_size_t max_array_size =  128;
	//	comment out all but the one used in this test
//	array_size_t (*next_size)(array_size_t current) = next_increment;
	array_size_t (*next_size)(array_size_t current) = multiply_by_2;
//	array_size_t (*next_size)(array_size_t current) = multiply_by_4;
//	array_size_t (*next_size)(array_size_t current) = multiply_by_10;
	int num_array_sizes = getNumSizes(min_array_size, max_array_size, next_size);

	SortAlgorithms 	sort_algorithms[] = {
//			SortAlgorithms::BUBBLE_SORT,
//			SortAlgorithms::SELECTION_SORT,
//			SortAlgorithms::INSERTION_SORT,
			SortAlgorithms::MERGE_SORT,
//			SortAlgorithms::HEAP_SORT,
//			SortAlgorithms::QUICK_SORT,
//			SortAlgorithms::OPTIMIZED_QUICK_SORT,
//			SortAlgorithms::DUTCH_FLAG_SORT,
			SortAlgorithms::INPLACE_MERGE,
			SortAlgorithms::BLOCK_SORT,
	};
	int num_sort_algorithms = sizeof(sort_algorithms)/sizeof(SortAlgorithms);

	constexpr int num_discrete  = 3;	//
	constexpr int num_different = 2;	// number of non-normal values in FEW_DIFFERENT
	ArrayComposition array_compositions[] = {
			{ArrayCompositions::ALL_DISCRETE, num_discrete, num_different},
//			{ArrayCompositions::ALL_SAME, num_discrete, num_different},
//			{ArrayCompositions::FEW_DISCRETE, num_discrete, num_different},
//			{ArrayCompositions::FEW_DIFFERENT, num_discrete, num_different}
	};
	int num_compositions = sizeof(array_compositions)/sizeof(ArrayComposition);

	constexpr array_size_t num_elements_out_of_order = 3;
	InitialOrdering	initial_orderings[] = {
			{InitialOrderings::IN_RANDOM_ORDER, num_elements_out_of_order},
//			{InitialOrderings::IN_REVERSE_ORDER, num_elements_out_of_order},
//			{InitialOrderings::FEW_CHANGES, num_elements_out_of_order},
//			{InitialOrderings::NO_CHANGES, num_elements_out_of_order},
	};
	int num_initial_orderings = sizeof(initial_orderings)/sizeof(InitialOrdering);

	int num_results = num_array_sizes * num_sort_algorithms * num_compositions * num_initial_orderings;
	OneTestResult* results[num_results];

	/*
	 * 	The ranodmizer is used to disorder the input data in sortTest
	 * 		Use the default value if a repeatable result is desired
	 * 		otherwise, use the seed based upon the system time
	 */
	uint64_t randomizer_seed = SIMPLE_RANDOMIZER_DEFAULT_SEED;
//	randomizer_seed = getChronoSeed();	// override default to get some variety
	SimpleRandomizer randomizer(randomizer_seed);

	std::cout 	<< "Algorithms: " << num_sort_algorithms
				<< " Compositions: " << num_compositions
				<< " Orderings: " << num_initial_orderings
				<< " of : " << num_array_sizes << " different sizes resulting in "
				<< num_sort_algorithms * num_compositions * num_initial_orderings * num_array_sizes
				<< " outcomes over " << num_repetitions << " repetitions" << std::endl;

	int cnt = 0;
	// run the tests
	for (int a = 0; a != num_sort_algorithms; a++) {
		for (int c = 0; c != num_compositions; c++) {
			for (int o = 0; o != num_initial_orderings; o++) {
				randomizer.seed(randomizer_seed);
				randomizer.restart();
				for (array_size_t size = min_array_size;
                              size <= max_array_size;
                              size = next_size(size)) {
                results[cnt] = testOneAlgorithm(sort_algorithms[a],
                                                array_compositions[c],
                                                initial_orderings[o],
                                                randomizer, size, num_repetitions);
                if (!results[cnt]->_failure_log->_diagnostics._is_sorted) {
						std::cout << "Sort failed: ";
						terseDump(&results[cnt], 1);
						std::cout << std::endl;
					}
					cnt++;
				}
			}
		}
	}

	printTestResults(results, cnt);

	std::cout << "Completed Sorting Performance In C++" << std::endl;

	return EXIT_SUCCESS;
}

#pragma pop_macro("to_i")
