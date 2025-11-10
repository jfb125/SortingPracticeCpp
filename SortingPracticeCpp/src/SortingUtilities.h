#ifndef SORTINGUTITLITES_H
#define SORTINGUTITLITES_H 1

#include <iostream>
#include <iomanip>
#include <limits>
#include "SortingDataTypes.h"
#include "GenerateTestVectors.h"
#include "IntegerArithmetic.h"
#include <limits>
#include "SimpleRandomizer.h"
#include "OStreamState.h"
#include "SortingDebugOutput.h"

#pragma push_macro("_verbose")
//#undef _verbose
//#define _verbose true
using factorial_t = long long;
const factorial_t sorting_utilities_max_factorial = std::numeric_limits<long long>::max();

array_size_t calcRotationWithModulo(array_size_t rotation_count, array_size_t span);

namespace SortingUtilities {

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							Function Declarations						*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	/*	Returns the index of the first element that is greater than 'value'
	 * 	This is used to insert a value to the right of it's peers	*/
	template <typename T>
	array_size_t binarySearchLastElement(T* array,
										 array_size_t range_start,
										 array_size_t range_end,
										 T& value,
										 SortMetrics *metrics = nullptr);

	/*	Returns the index of the first element that is equal to or greater than 'value'
	 * 	This is used to insert a value to the left of it's peers	*/
	template <typename T>
	array_size_t binarySearchFirstElement(T* array,
										  array_size_t range_start,
										  array_size_t range_end,
										  T& value,
										  SortMetrics *metrics = nullptr);

	// 'hide' this in the namespace to try to prevent collisions with
	//	possible functions with identical signatures in other namespaces
	factorial_t factorial(array_size_t);

	//	This can be used by a sorting algorithm to determine if a cutoff
	//	to insertion sort should occur.  Since it can be used as part of
	//  an algorithm, it can be passed 'metics'
	template <typename T>
	bool isSorted(T *array, array_size_t size, SortMetrics *metrics = nullptr);

	// returns bool
	// stores number of compares in 'metrics'
	// stores location of mismatch in lower_index_unordered & upper_index_unordered
	// 	lower_index_unordered & upper_index_unordered are always updated
	template <typename T>
	bool isSorted(T *array, array_size_t size,
				  array_size_t &lower_index_unordered,
				  array_size_t &upper_index_unordered,
				  SortTestMetrics *metrics = nullptr);

	/*	Some algorithms like QuickSort use this to ensure the array is not
	 *	 in reverse order, which might otherwise lead to n^2 performance */
	template <typename T>
	void randomizeArray(T* array, array_size_t size,
						SortMetrics *metrics = nullptr);

	/*	rotates elements of an array [start:end] an amount, where negative
	 * values of 'amount' indicate to rotate the span to the left.
	 * Blocks must be adjacent / contiguous	*/
	template <typename T>
	void rotateArrayElementsRight(T* array,
								  array_size_t block_1_start,
								  array_size_t block_2_end,
							 	  array_size_t amount,
								  SortMetrics *metrics = nullptr);

	/*	This is used in all of the QuickSort based algorithms	*/
	template <typename T>
	void selectAndPositionPivot(T* array,
								array_size_t start,
								array_size_t end,
								SortMetrics *metrics = nullptr);

	//	This improves understanding & readability in swap-based algorithms
	template <typename T>
	void swap(T*array, array_size_t i, array_size_t j,
			  SortMetrics *metrics = nullptr);


	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*							Function Definitions							*/
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
	array_size_t binarySearchFirstElement(T* array,
										  array_size_t range_start,
										  array_size_t range_end,
										  T& value,
										  SortMetrics *metrics) {

		array_size_t start 	= range_start;
		array_size_t end	= range_end;
		array_size_t mid	= start + (end-start)/2;

		while (start != end) {
			//	determine the midpoint in an even size span
			//	or the index on the left of mid in an odd size span
			mid = start + (end-start)/2;
			if (metrics)	metrics->compares++;
			if (array[mid] < value) {
				//	if the array value at [mid] is < value
				//	  look to the right for a [] >= value
				start = mid+1;
			} else {
				//	else the look to the right for a value < passed
				end = mid;
			}
		}
		if (start == range_end) {
			if (metrics)	metrics->compares++;
			if (array[start] < value) {
				start++;
			}
		}
		return start;
	}


	/*
	 * binaryLast(array, first, last, value);
	 *
	 * This function returns the index of first element that is greater than value
	 *	If the passed value is equal to or greater than all of the elements
	 *	on the span, an index (range_end+1) is returned.
	 *
	 *	  0  1  2  3  4  5
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &-1) returns 0
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &0) returns 2
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &1) returns 4
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &2) returns 6
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &3) returns 6
	 */

	template <typename T>
	array_size_t binarySearchLastElement(T* array,
										 array_size_t range_start,
										 array_size_t range_end,
										 T& value,
										 SortMetrics *metrics) {

		array_size_t start 	= range_start;
		array_size_t end	= range_end;
		array_size_t mid	= start + (end-start)/2;

		while (start != end) {
			//	determine the midpoint in an even size span
			//	or the index on the left of mid in an odd size span
			mid = start + (end-start)/2;
			if (metrics) metrics->compares++;
			if (array[mid] <= value) {
				//	if the array value at [mid] is < value
				//	  look to the right for a [] >= value
				start = mid+1;
			} else {
				//	else the look to the right for a value < passed
				end = mid;
			}
		}
		if (start == range_end) {
			if (metrics) metrics->compares++;
			if (array[start] <= value) {
				start++;
			}
		}
		return start;
	}


	/*
	 * 	bool isSorted(array, size, metrics);
	 *
	 *	This function is used by some sort algorithms to improve
	 *	the efficiency of the algorithm.  metrics indicates the time
	 *	complexity of this, which is added to the complexity of the sort
	 */

	template <typename T>
	bool isSorted(T *array, array_size_t size, SortMetrics *metrics)
	{
		for (array_size_t i = size-1; i > 0 ; --i) {
			if (metrics) metrics->compares++;
			if (array[i] < array[i-1]) {
				return false;
			}
		}
		return true;
	}


	/*
	 * bool isSorted(array, size, &err_i, &err_j, metrics)
	 *
	 * This updates references to indices which are the locations
	 * of two out-of-order elements.  Typically, the elements will
	 * be side by side, but the algorithm returns them separately
	 *
	 * If sorted, 		'true'  will be returns and &err_i =  &err_j = 0
	 * If not sorted,   'false' will be returned an &err_i != &err_j
	 */

	template <typename T>
	bool isSorted(T *array, array_size_t size,
				  array_size_t &lower_index_unordered,
				  array_size_t &upper_index_unordered,
				  SortTestMetrics *metrics) {
		lower_index_unordered = 0;
		upper_index_unordered = 0;
		for (array_size_t i = size-1; i > 0 ; --i) {
			if (metrics) metrics->compares++;
			if (array[i] < array[i-1]) {
				lower_index_unordered = i-1;
				upper_index_unordered = i;
				return false;
			}
		}
		return true;
	}


	/*
	 * void randomizeArray(array, size, metrics)
	 *
	 * Randomizes array.  Some sorts such as QuickSort randomize the
	 * array elements initially to avoid n^2 complexity when the elements
	 * are in reverse order.  Because this can be used as a part of a
	 * sort algorithm, metrics are tallied
	 */

	template <typename T>
	void randomizeArray(T* array, array_size_t size, SortMetrics *metrics) {

		if (size <= 1)
			return;

		uint64_t randomizer_seed = getChronoSeed();
		randomizer_seed = SIMPLE_RANDOMIZER_DEFAULT_SEED;
		SimpleRandomizer randomizer(randomizer_seed);

		array_size_t r;
		array_size_t end = size-1;

		for (array_size_t i = 0; i != end; i++) {
			r = randomizer.rand(i, size);
			SortingUtilities::swap(array, i, r, metrics);
		}
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
	 * 	note that amount can be negative (left) or greater than the span
	 * 		negative amounts are converted into the equivalent positive amount
	 * 		amounts greater than the span are modulo-division to be within the span
	 */

	template <typename T>
	void rotateArrayElementsRight(T* array,
								  array_size_t span_start,
								  array_size_t span_end,
								  array_size_t amount,
								  SortMetrics *metrics) {

		array_size_t span = span_end-span_start;

		//	qualify all of the block indices
		if (span <= 0)
			return;

		//	converts amounts that are not in [0,span) to in range
		amount = calcRotationWithModulo(amount, span);
		if (amount == 0)
			return;

		//	reverse the entire array
		for (array_size_t i = span_start, j = span_end; i < j; i++, j--) {
			SortingUtilities::swap(array, i, j, metrics);
		}

		//	reverse the left hand side
		for (array_size_t i = span_start, j = span_start+amount-1; i < j; i++, j--) {
			SortingUtilities::swap(array, i, j, metrics);
		}

		//	reverse the right hand side
		for (array_size_t i = span_start + amount, j = span_end; i < j; i++, j--) {
			SortingUtilities::swap(array, i, j, metrics);
		}
	}


	/*
	 * 	In the QuickSort family of algorithms, it was determined empirically
	 * 	that there is a significant reduction in the number of compares on arrays
	 * 	that are mostly	sorted three values are examined as potential pivots
	 * 	and the middle value is chosen as the pivot.  This increases the number
	 * 	of assignments because the middle value, once chosen, has to be swapped
	 * 	into the [start] position for partitioning
	 */

	template <typename T>
	void selectAndPositionPivot(T* array, array_size_t start, array_size_t end,
								SortMetrics *metrics) {

		constexpr bool debug_verbose = false;

		// examine elements 1/4 of the way, 1/2 the way, and 3/4 of the way
		array_size_t half 	= (end-start)/2;
		array_size_t mid 	= start + half;
		array_size_t quarter= half /= 2;
		array_size_t low 	= start + quarter;
		array_size_t high 	= mid + quarter;
		array_size_t pivot;

		if (low < start) {
			std::cout << "Error: low  " << low << " is less than start " << start << std::endl;
		}
		if (high > end) {
			std::cout << "Error: high " << low << " is more than end   " << end << std::endl;
		}

		if (debug_verbose) {
		std::cout << std::endl;
		std::cout << "selecting pivot based upon considering "
				  << low << ", " << mid << ", " << high << std::endl;
			SortingUtilities::printArrayAndPivot(array, start, end, 0, "prior to selecting and positioning pivot: ");
		}
		if (metrics) metrics->compares++;
		if (array[low] < array[high])
		{
			// start is smaller than end
			if (metrics) metrics->compares++;
			if (array[low] < array[mid]) {
				// start is smaller than end and smaller than or equal to mid
				// start is the smallest, evaluate mid vs end
				if (metrics) metrics->compares++;
				if (array[mid] < array[high]) {
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
			if (metrics)	metrics->compares++;
			// end is smaller than or equal to start
			if (array[high] < array[mid]) {
				// end is smaller than or equal to start and smaller than or equal to mid
				// end is the smallest, evaluate start vs mid
				if (metrics)	metrics->compares++;
				if (array[low] < array[mid]) {
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
		if (debug_verbose) {
			std::cout << "selected pivot as element " << pivot << std::endl;
			printArrayAndPivot(array, start, end, pivot, "");
		}

		if (pivot != start) {
			SortingUtilities::swap(array, start, pivot, metrics);
		}

		if (debug_verbose) {
			std::cout << "array is now " << std::endl;
			printArrayAndPivot(array, start, end, start, "");
			std::cout << std::endl;
		}
	}


	/*
	 * 	swap(array, i, j, metrics)
	 *
	 * 	This is provided to improve readability and understanding within
	 * 	source code for sort algorithms that swap elements
	 */

	template <typename T>
	void swap(T* array, array_size_t i, array_size_t j, SortMetrics *metrics) {
		T temp	 = array[i];
		array[i] = array[j];
		array[j] = temp;
		if (metrics) {
			metrics->assignments += num_assignments_per_swap;
		}
	}
}	// namespace SortingUtilities

#pragma pop_macro("_verbose")

#endif	/*	header guard	*/
