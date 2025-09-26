#ifndef SORTINGUTITLITES_H
#define SORTINGUTITLITES_H 1

#include <iostream>
#include <iomanip>
#include "SortingPracticeDataTypes.h"
#include "IntegerArithmetic.h"
#include "SimpleRandomizer.h"
#include "OStreamState.h"

#pragma push_macro("_verbose")
//#undef _verbose
//#define _verbose true

constexpr int 	ELEMENT_WIDTH 		= 5;
constexpr int 	VALUE_WIDTH 		= 4;

std::string arrayIndicesToString(array_size_t size, array_size_t v,
								 int element_width = ELEMENT_WIDTH);
std::string arrayIndicesToString(std::string trailer,
								 array_size_t size, array_size_t v,
								 int element_width = ELEMENT_WIDTH);
std::string arrayIndicesToStringLine(array_size_t size,
								 int element_width = ELEMENT_WIDTH);
std::string arrayStartMiddleEndToString(array_size_t size,
										array_size_t start, array_size_t mid, array_size_t end,
										int element_width = ELEMENT_WIDTH);

namespace SortingUtilities {

	/*	**********************************************************************	*/
	/*								Forward declarations						*/
	/*	**********************************************************************	*/

	template <typename T>
	bool isSorted(T **array, array_size_t size);
	template <typename T>
	bool isSorted(T **array, array_size_t size, ComparesAndMoves &metrics);
	template <typename T>
	bool isSorted(T **array, array_size_t size,
				  array_size_t &lower_index_unordered, array_size_t &upper_index_unordered);
	template <typename T>
	bool isSorted(T **array, array_size_t size, ComparesAndMoves &metrics,
				  array_size_t &lower_index_unordered, array_size_t &upper_index_unordered);

	template <typename T>
	void printArray(T**array, array_size_t start, array_size_t end, std::string header);
	template <typename T>
	void printArrayAndPivot(T**array, array_size_t start, array_size_t end, array_size_t pivot, std::string header);



	template <typename T>
	ComparesAndMoves randomizeArray(T** array, array_size_t size);

	template <typename T>
	int swap(T**array, array_size_t i, array_size_t j);


	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*								implementation								*/
	/*	**********************************************************************	*/
	/*	**********************************************************************	*/

	template <typename T>
	std::string arrayElementsToString(T** array, array_size_t size,
									  int value_width = VALUE_WIDTH,
									  int element_width = ELEMENT_WIDTH) {
		OStreamState current_state;
		std::stringstream result;
		int spacer_width = element_width - value_width;
		for (int i = 0; i < size-1; i++) {
			result << std::right << std::setw(value_width) << *array[i];
			if (spacer_width) {
				result << std::setw(spacer_width) << ' ';
			}
		}
		result << std::right << std::setw(value_width) << *array[size-1];
		return result.str();
	}

	template <typename T>
	std::string arrayElementsToString(std::string trailer,
									  T** array, array_size_t size,
									  int value_width, int element_width) {
		OStreamState current_state;
		std::stringstream result;
		result << arrayElementsToString(array, size, value_width, element_width);
		result << trailer;
		return result.str();
	}

	// returns bool
	template <typename T>
	bool isSorted(T **array, array_size_t size) {
		for (array_size_t i = size-1; i > 0 ; --i) {
			if (*array[i] < *array[i-1]) {
				return false;
			}
		}
		return true;
	}

	// 	returns bool
	//	 updates the number of compares in 'metrics'
	template <typename T>
	bool isSorted(T **array, array_size_t size, ComparesAndMoves &metrics) {
		for (array_size_t i = size-1; i > 0 ; --i) {
			metrics._compares++;
			if (*array[i] < *array[i-1]) {
				return false;
			}
		}
		return true;
	}

	// 	returns bool
	//	stores location of the unordered elements in i, j
	//		always updates value of i & j
	template <typename T>
	bool isSorted(T **array, array_size_t size,
				  array_size_t &lower_index_unordered, array_size_t &upper_index_unordered) {
		lower_index_unordered = 0;
		upper_index_unordered = 0;
		for (array_size_t i = size-1; i > 0 ; --i) {
			if (*array[i] < *array[i-1]) {
				lower_index_unordered = i-1;
				upper_index_unordered = i;
				return false;
			}
		}
		return true;
	}

	// returns bool
	// stores number of compares in 'metrics'
	// stores location of mismatch in lower_index_unordered & upper_index_unordered
	// 	lower_index_unordered & upper_index_unordered are always updated
	template <typename T>
	bool isSorted(T **array, array_size_t size, ComparesAndMoves &metrics,
				  array_size_t &lower_index_unordered, array_size_t &upper_index_unordered) {
		lower_index_unordered = 0;
		upper_index_unordered = 0;
		for (array_size_t i = size-1; i > 0 ; --i) {
			if (*array[i] < *array[i-1]) {
				lower_index_unordered = i-1;
				upper_index_unordered = i;
				return false;
			}
		}
		return true;
	}

	template <typename T>
	ComparesAndMoves selectAndPositionPivot(T** array, array_size_t start, array_size_t end) {

		constexpr bool _verbose = false;
		ComparesAndMoves returned_value(0,0);
		array_size_t half = (end-start)/2;
		array_size_t mid = start + half;
		half /= 2;
		array_size_t low = start + half;
		array_size_t high = mid + half;
		array_size_t pivot;

		if (low < start) {
			std::cout << "Error: low  " << low << " is less than start " << start << std::endl;
		}
		if (high > end) {
			std::cout << "Error: high " << low << " is more than end   " << end << std::endl;
		}

		if (_verbose) {
		std::cout << std::endl;
		std::cout << "selecting pivot based upon considering "
				  << low << ", " << mid << ", " << high << std::endl;
			SortingUtilities::printArrayAndPivot(array, start, end, 0, "prior to selecting and positioning pivot: ");
		}
		returned_value._compares++;
		if (*array[low] < *array[high])
		{
			// start is smaller than end
			returned_value._compares++;
			if (*array[low] < *array[mid]) {
				// start is smaller than end and smaller than or equal to mid
				// start is the smallest, evaluate mid vs end
				returned_value._compares++;
				if (*array[mid] < *array[high]) {
					// mid is greater than or equal to start
					//	but less than end
					pivot = mid;	// { 1, 2, 3 }
				} else {
					// mid is greater than or equal to start
					//  and greater than or equal to end
					pivot = high;	// { 1, 3, 2 }
				}
			} else {
				// start is smaller than end
				//  but equal to or greater than mid
				pivot = low;	// { 2, 1, 3 }
			}
		} else {
			returned_value._compares++;
			// end is smaller than or equal to start
			if (*array[high] < *array[mid]) {
				// end is smaller than or equal to start and smaller than or equal to mid
				// end is the smallest, evaluate start vs mid
				returned_value._compares++;
				if (*array[low] < *array[mid]) {
					// smart is greater than or equal to end but smaller than mid
					pivot = low;	// { 2, 3, 1 }
				} else {
					// smart is greater than or equal to end and greater than or equal to mid
					pivot = mid;		// { 3, 2, 1 }
				}
			} else {
				// end is smaller than or equal to start but greater than mid
				pivot = high;			// { 3, 1, 2 }
			}
		}
		if (_verbose) {
			std::cout << "selected pivot as element " << pivot << std::endl;
			printArrayAndPivot(array, start, end, pivot, "");
		}
		returned_value._moves +=
			SortingUtilities::swap(array, start, pivot);

		if (_verbose) {
			std::cout << "array is now " << std::endl;
			printArrayAndPivot(array, start, end, start, "");
			std::cout << std::endl;
		}
//		static int stop_count = 5;
//		static int stop_counter = stop_count;
//		if (false && --stop_counter == 0) {
//			std::cout << "selectAndPositionPivot() stops after interation #" << stop_count << std::endl;
//			while(1){}
//		}
		return returned_value;
	}

	template <typename T>
	void printArray(T**array, array_size_t start, array_size_t end, std::string header) {
			std::cout << header << " [" << start << ":" << end << "]"
					  << std::endl;
			for (array_size_t i = start; i <= end; i++) {
				std::cout << std::setw(2) << std::right << " " << i << "  : "
						  << array[i]->last_name << std::endl;
			}
	}
#if 0
	template <typename T>
	void printElements(std::string trailer, T** array, index_t size, int value_width, int element_width) {
		OStreamState current_state;

		int spacer_width = element_width - value_width;
		for (int i = 0; i != size; i++) {
			if (spacer_width) {
				std::cout << std::setw(spacer_width) << ' ';
			std::cout << std::setw(value_width) << *array[i];
			}
		}
		std::cout << trailer;
	}
#endif
	template <typename T>
	void printArrayAndPivot(T**array, array_size_t start, array_size_t end, array_size_t pivot, std::string header) {
			std::cout << header <<  " [" << start << ":" << end << "]"
					  << std::endl;
			for (array_size_t i = start; i <= end; i++) {
				if (i == pivot) {
					std::cout << std::setw(2) << ">" << i << "< : ";
				} else {
					std::cout << std::setw(2) << " " << i << "  : ";
				}
				std::cout  << array[i]->last_name << std::endl;
			}
	}

	template <typename T>
	ComparesAndMoves randomizeArray(T** array, array_size_t size) {

		ComparesAndMoves ret_val(0, 0);

		if (size <= 1)
			return ret_val;

		uint64_t randomizer_seed = getChronoSeed();
		randomizer_seed = SIMPLE_RANDOMIZER_DEFAULT_SEED;
		SimpleRandomizer randomizer(randomizer_seed);

		array_size_t r;
		array_size_t end = size-1;

		for (array_size_t i = 0; i != end; i++) {
			r = randomizer.rand(i, size);
			ret_val._moves +=
				SortingUtilities::swap(array, i, r);
		}

		return ret_val;
	}

	// returns the number of moves
	template <typename T>
	int swap(T**array, array_size_t i, array_size_t j) {
		T* tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
		return 3;
	}

}	// namespace SortingUtilities

#pragma pop_macro("_verbose")

#endif	/*	header guard	*/
