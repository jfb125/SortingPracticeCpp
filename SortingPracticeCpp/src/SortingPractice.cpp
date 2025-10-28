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
#include <string>

#include "SortTest.h"
#include "ResultOutput.h"
#include "TestFixtures.h"

#pragma push_macro("to_i")
#define to_i(e_num)	static_cast<int>(e_num)


/*	******************************************************************************	*/
/*	******************************************************************************	*/
/*									main()											*/
/*	*****************************************************************************	*/
/*	******************************************************************************	*/

//#include "BlockSort.h"

bool testBlockSort();

//	next_value has to be a standalone function to pass to generateReferenceVector

int main(int argc, char *argv[])
{
	std::cout << "Sorting Performance In C++" << " built on " __DATE__ << " at " __TIME__ << std::endl;

//	testBlockSort();
//	return EXIT_SUCCESS;
//	sortingDataTypesTest();
//	return EXIT_SUCCESS;

	//	Ensure that if the user selects 'All_PERMUTATIONS', they are
	//	asked to confirm a very large test run.
	constexpr FactorialType max_repetitions_without_confirmation= 100000;
	constexpr bool skip_confirm_permutation_size_confirmation 	= true;
	auto confirm_permutation_size =
		[&] (ArrayCompositions composition, array_size_t size) -> bool {
		if (skip_confirm_permutation_size_confirmation ||
			composition != ArrayCompositions::ALL_PERMUTATIONS) {
			return true;
		}
		FactorialType num_reps = SortingUtilities::factorial(size);
		if (num_reps > max_repetitions_without_confirmation) {
			std::cout << "With an array of size " << size
					  << " there will be " << num_reps
					  << " permutations."
					  << " Do you wish to proceed? (y/N)";
			char answer = 'n';
			std::cin >> answer;
			return answer == 'y' || answer == 'Y';
		} else {
			return true;
		}
	};

	using DataType = std::string;
	DataType first_value("AAAA");
	DataType last_value("ZZZZ");
	auto next_value = [] (DataType& current, DataType& first, DataType& last) {
		if (current == last) {
			current = first;
		} else {
			for (int i = current.size()-1; i >= 0; i--) {
				if (current[i] == 'Z') {
					current[i] = 'A';
				} else {
					current[i]++;
					break;
				}
			}
		}
	};

	array_size_t array_sizes[]	= { 64, 128, 256, 512 };
//	array_size_t array_sizes[] 	= { 8 };
	int num_array_sizes 	 	= sizeof(array_sizes) / sizeof(array_size_t);

	SortAlgorithms 	sort_algorithms[] = {
			SortAlgorithms::BUBBLE_SORT,
			SortAlgorithms::SELECTION_SORT,
			SortAlgorithms::INSERTION_SORT,
			SortAlgorithms::DUTCH_FLAG_SORT,
			SortAlgorithms::HEAP_SORT,
			SortAlgorithms::QUICK_SORT,
			SortAlgorithms::OPTIMIZED_QUICK_SORT,
			SortAlgorithms::MERGE_SORT,
			SortAlgorithms::INPLACE_MERGE,
//			SortAlgorithms::BLOCK_SORT,
	};
	int num_sort_algorithms = sizeof(sort_algorithms)/sizeof(SortAlgorithms);

	constexpr int num_discrete  = 3;	// number of distince values in FEW_DISCRETE
	constexpr int num_different = 2;	// number of non-normal values in FEW_DIFFERENT

#pragma push_macro("UNUSED_VAR")
#define UNUSED_VAR(expr) do { (void)(expr); }while(false)
	UNUSED_VAR(num_discrete);
	UNUSED_VAR(num_different);
#pragma pop_macro("UNUSED_VAR")

	//	BE VERY CAREFUL WHEN SELECTING 'ALL_PERMUTATIONS' - even
	//	an array size of 8 will generate 8! = 40320 repetitions
	ArrayComposition array_compositions[] = {
			{ArrayCompositions::ALL_DISCRETE},
//			{ArrayCompositions::ALL_SAME},
			{ArrayCompositions::FEW_DISTINCT, num_discrete, num_different},
			{ArrayCompositions::FEW_DIFFERENT, num_discrete, num_different},
//			{ArrayCompositions::ALL_PERMUTATIONS},
	};
	int num_compositions = sizeof(array_compositions)/sizeof(ArrayComposition);

	//	NOTE: InitialOrdering is ignored when ALL_PERMUTATIONS
	constexpr array_size_t num_elements_out_of_order = 3;
	InitialOrdering	initial_orderings[] = {
			{InitialOrderings::IN_RANDOM_ORDER, num_elements_out_of_order},
//			{InitialOrderings::IN_REVERSE_ORDER, num_elements_out_of_order},
//			{InitialOrderings::FEW_CHANGES, num_elements_out_of_order},
//			{InitialOrderings::NO_CHANGES, num_elements_out_of_order},
	};
	int num_initial_orderings = sizeof(initial_orderings)/sizeof(InitialOrdering);

	int num_results = num_array_sizes * num_sort_algorithms * num_compositions * num_initial_orderings;
	OneTestResult<SortingDataType<DataType>>* results[num_results];

	/*
	 * 	The ranodmizer is used to disorder the input data in sortTest
	 * 		Use the default value if a repeatable result is desired
	 * 		otherwise, use the seed based upon the system time
	 */
	uint64_t randomizer_seed = SIMPLE_RANDOMIZER_DEFAULT_SEED;
//	randomizer_seed = getChronoSeed();	// override default to get some variety
	SimpleRandomizer randomizer(randomizer_seed);

	int num_repetitions = 1000;

	std::cout 	<< "Algorithms: " << num_sort_algorithms
				<< " Compositions: " << num_compositions
				<< " Orderings: " << num_initial_orderings
				<< " of : " << num_array_sizes << " different sizes resulting in "
				<< num_sort_algorithms * num_compositions * num_initial_orderings * num_array_sizes
				<< " outcomes " << std::endl;

	int cnt = 0;
	// run the tests
	for (int algorithm_i = 0; algorithm_i != num_sort_algorithms; algorithm_i++) {
		for (int composition_i = 0; composition_i != num_compositions; composition_i++) {
			ArrayComposition composition = array_compositions[composition_i];
			for (int ordering_i = 0; ordering_i != num_initial_orderings; ordering_i++) {
				randomizer.seed(randomizer_seed);
				randomizer.restart();
				for (int size_i = 0; size_i < num_array_sizes; size_i++) {
					array_size_t array_size = array_sizes[size_i];
					if (!confirm_permutation_size(composition.composition, array_size)) {
						continue;
					}
					SortingDataType<DataType> test_values[array_size];
					SortingUtilities::generateReferenceTestVector<SortingDataType<DataType>, DataType>(
							test_values, array_size,
							composition,
							first_value, last_value,
							next_value);

					results[cnt] = testOneAlgorithm<SortingDataType<DataType>>(
							sort_algorithms[algorithm_i],
							composition,
							initial_orderings[ordering_i],
							randomizer,
							test_values,
							array_size,
							num_repetitions);

					if (!results[cnt]->m_failure_log->m_diagnostics.is_sorted) {
							std::cout << "Sort failed: ";
							terseDump(results[cnt], 1);
							std::cout << std::endl;
					}
					cnt++;
				}
			}
		}
	}
	ResultTableOrdering table_structure(
			ResultTableElements::ORDERING,
			ResultTableElements::COMPOSITION,
			ResultTableElements::ALGORITHM,
			ResultTableElements::COMPOSITION);

	printTestResults(results, cnt, table_structure);

	std::cout << "Completed Sorting Performance In C++" << std::endl;

	return EXIT_SUCCESS;
}

#pragma pop_macro("to_i")
