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

array_size_t blockSortModulo(array_size_t rotation_count, array_size_t span);

extern const uint16_t ccitt_hash[];

namespace SortingUtilities {

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*						functions that an algorithm will used			*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/


	/*	Returns the index of the first element that is greater than 'value'
	 * 	This is used to insert a value to the right of it's peers	*/
	template <typename T>
	array_size_t binarySearchLastElement(T** array,
										 array_size_t range_start,
										 array_size_t range_end,
										 T *value,
										 SortMetrics &metrics);

	/*	Returns the index of the first element that is equal to or greater than 'value'
	 * 	This is used to insert a value to the left of it's peers	*/
	template <typename T>
	array_size_t binarySearchFirstElement(T** array,
										  array_size_t range_start,
										  array_size_t range_end,
										  T *value,
										  SortMetrics & metrics);

	/*	rotates elements of an array [start:end] an amount, where negative
	 * values of 'amount' indicate to rotate the span to the left */
	template <typename T>
	SortMetrics rotateArrayElementsRight(T** array,
										 array_size_t start, array_size_t end,
										 array_size_t amount);

	/*	Some algorithms like QuickSort use this to ensure the array is not
	 *	 in reverse order, which can lead to n^2 performance */
	template <typename T>
	SortMetrics randomizeArray(T** array, array_size_t size);

	template <typename T>
	int swap(T**array, array_size_t i, array_size_t j);


	/*	******************************************************************	*/
	/*							Debugging functions							*/
	/*	******************************************************************	*/

	template <typename T>
	uint16_t crc16_ccitt(T ** buffer, size_t size);

	template <typename T>
	bool isSorted(T **array, array_size_t size);

	template <typename T>
	bool isSorted(T **array, array_size_t size, SortMetrics &metrics);

	template <typename T>
	bool isSorted(T **array, array_size_t size,
				  array_size_t &lower_index_unordered, array_size_t &upper_index_unordered);

	template <typename T>
	bool isSorted(T **array, array_size_t size, SortMetrics &metrics,
				  array_size_t &lower_index_unordered, array_size_t &upper_index_unordered);

	template <typename T>
	void printArray(T**array, array_size_t start, array_size_t end, std::string header);

	template <typename T>
	void printArrayAndPivot(T**array, array_size_t start, array_size_t end, array_size_t pivot, std::string header);



	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*								implementation								*/
	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*
	 * binaryFirst(array, first, last, value);
	 *
	 * This function returns the index of the first element that is
	 *  greater than or equal to value. If the passed value is greater than
	 *	all of the elements on the span, an index (range_end+1) is returned.
	 *
	 *	  0  1  2  3  4  5
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &-1) returns 0
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &0) returns 0
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &1) returns 2
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &2) returns 4
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &3) returns 6
	 */

	template <typename T>
	array_size_t binarySearchFirstElement(T** array,
										  array_size_t range_start,
										  array_size_t range_end,
										  T *value,
										  SortMetrics &metrics) {

		array_size_t start = range_start;
		array_size_t end	 	= range_end;
		array_size_t mid		= start + (end-start)/2;

		while (start != end) {
			//	determine the midpoint in an even size span
			//	or the index on the left of mid in an odd size span
			mid = start + (end-start)/2;
			metrics.compares++;
			if (*array[mid] < *value) {
				//	if the array value at [mid] is < value
				//	  look to the right for a [] >= value
				start = mid+1;
			} else {
				//	else the look to the right for a value < passed
				end = mid;
			}
		}
		if (start == range_end) {
			metrics.compares++;
			if (*array[start] < *value) {
				start++;
			}
		}
		return start;
	}


	/*
	 * binaryLast(array, first, last, value);
	 *
	 * This function returns the index of first element that is greater than value
	 *	If the passed value is equal to or greater than
	 *	all of the elements on the span, an index (range_end+1) is returned.
	 *
	 *	  0  1  2  3  4  5
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &-1) returns 0
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &0) returns 2
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &1) returns 4
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &2) returns 6
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &3) returns 6
	 */

	template <typename T>
	array_size_t binarySearchLastElement(T** array,
										 array_size_t range_start,
										 array_size_t range_end,
										 T *value,
										 SortMetrics &metrics) {

		array_size_t start = range_start;
		array_size_t end	 	= range_end;
		array_size_t mid		= start + (end-start)/2;

		while (start != end) {
			//	determine the midpoint in an even size span
			//	or the index on the left of mid in an odd size span
			mid = start + (end-start)/2;
			metrics.compares++;
			if (*array[mid] <= *value) {
				//	if the array value at [mid] is < value
				//	  look to the right for a [] >= value
				start = mid+1;
			} else {
				//	else the look to the right for a value < passed
				end = mid;
			}
		}
		if (start == range_end) {
			metrics.compares++;
			if (*array[start] <= *value) {
				start++;
			}
		}
		return start;
	}


	/*
	 * 	ComparesAndSwaps rotateArray(array, amount, start, end);
	 *
	 * 	rotate the contents of the array by 'amount' positions
	 * 	  where the array is defined as array[start]:array[end] (inclusive)
	 *
	 * 	algorithm:
	 * 		consider amount to rotate = 3
	 * 		array									{ A, B, C, D, E, F, G }
	 * 		reverse the array						{ G, F, E, D, C, B, A }
	 * 		reverse the sub array [start:amount-1]  { E, F, G  D, C, B, A }
	 * 		reverse the sub array [amount:end]		{ E, F, G, A, B, C, D }
	 *
	 * 	note that amount can be negative or greater than the span
	 * 		negative amounts are converted into the equivalent positive amount
	 * 		amounts greater than the span are modulo-division to be within the span
	 */

	template <typename T>
	SortMetrics rotateArrayElementsRight(T** array,
											  array_size_t start, array_size_t end,
											  array_size_t amount) {
		SortMetrics result(0,0);

		if (end <= start)
			return result;

		array_size_t span = end - start + 1;
		if (span == 0)
			return result;

		//	converts amounts that are not in [0,span) to in range
		amount = blockSortModulo(amount, span);

		if (amount == 0)
			return result;

		//	reverse the entire array
		T* temp;
		for (array_size_t i = start, j = end; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result.assignments += 3;
		}

		//	reverse the left hand side
		for (array_size_t i = start, j = start+amount-1; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result.assignments += 3;
		}

		//	reverse the right hand side
		for (array_size_t i = start + amount, j = end; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result.assignments += 3;
		}
		return result;
	}


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

	template <typename T>
	uint16_t SortingUtilities::crc16_ccitt(T ** buffer, size_t size)
	{
	    uint16_t crc = 0;
	    for (size_t i = 0; i != size; i++) {
	    	uint16_t _data = static_cast<uint16_t>(*buffer[i]);
	    	crc = (crc << 8) ^ ccitt_hash[((crc >> 8) ^ _data) & 0x00FF];
	    }
	    return crc;
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
	bool isSorted(T **array, array_size_t size, SortMetrics &metrics) {
		for (array_size_t i = size-1; i > 0 ; --i) {
			metrics.compares++;
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
	bool isSorted(T **array, array_size_t size, SortMetrics &metrics,
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
	SortMetrics selectAndPositionPivot(T** array, array_size_t start, array_size_t end) {

		constexpr bool _verbose = false;
		SortMetrics returned_value(0,0);
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
		returned_value.compares++;
		if (*array[low] < *array[high])
		{
			// start is smaller than end
			returned_value.compares++;
			if (*array[low] < *array[mid]) {
				// start is smaller than end and smaller than or equal to mid
				// start is the smallest, evaluate mid vs end
				returned_value.compares++;
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
			returned_value.compares++;
			// end is smaller than or equal to start
			if (*array[high] < *array[mid]) {
				// end is smaller than or equal to start and smaller than or equal to mid
				// end is the smallest, evaluate start vs mid
				returned_value.compares++;
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
		returned_value.assignments +=
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
	void printElements(std::string trailer, T** array, array_size_t size, int value_width, int element_width) {
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
	SortMetrics randomizeArray(T** array, array_size_t size) {

		SortMetrics ret_val(0, 0);

		if (size <= 1)
			return ret_val;

		uint64_t randomizer_seed = getChronoSeed();
		randomizer_seed = SIMPLE_RANDOMIZER_DEFAULT_SEED;
		SimpleRandomizer randomizer(randomizer_seed);

		array_size_t r;
		array_size_t end = size-1;

		for (array_size_t i = 0; i != end; i++) {
			r = randomizer.rand(i, size);
			ret_val.assignments +=
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
