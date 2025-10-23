#ifndef SORTINGUTITLITES_H
#define SORTINGUTITLITES_H 1

#include <iostream>
#include <iomanip>
#include "SortingDataTypes.h"
#include "IntegerArithmetic.h"
#include "SimpleRandomizer.h"
#include "OStreamState.h"
#include "SortingDebugOutput.h"

#pragma push_macro("_verbose")
//#undef _verbose
//#define _verbose true

array_size_t calcRotationWithModulo(array_size_t rotation_count, array_size_t span);

namespace SortingUtilities {

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							Function Declarations						*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	/*	Debugging	*/

	template <typename T>
	bool isSorted(T *array, array_size_t size);

	template <typename T>
	bool isSorted(T *array, array_size_t size, SortMetrics &metrics);

	template <typename T>
	bool isSorted(T *array, array_size_t size,
				  array_size_t &lower_index_unordered, array_size_t &upper_index_unordered);

	template <typename T>
	bool isSorted(T *array, array_size_t size, SortMetrics &metrics,
				  array_size_t &lower_index_unordered, array_size_t &upper_index_unordered);

	template <typename T>
	SortMetrics swap(T*array, array_size_t i, array_size_t j);

	/*	******************************************************************	*/
	/*					Functions used by some algorithms					*/
	/*	******************************************************************	*/

	/*	Returns the index of the first element that is greater than 'value'
	 * 	This is used to insert a value to the right of it's peers	*/
	template <typename T>
	array_size_t binarySearchLastElement(T* array,
										 array_size_t range_start,
										 array_size_t range_end,
										 T& value,
										 SortMetrics &metrics);

	/*	Returns the index of the first element that is equal to or greater than 'value'
	 * 	This is used to insert a value to the left of it's peers	*/
	template <typename T>
	array_size_t binarySearchFirstElement(T* array,
										  array_size_t range_start,
										  array_size_t range_end,
										  T& value,
										  SortMetrics & metrics);

	/*	rotates elements of an array [start:end] an amount, where negative
	 * values of 'amount' indicate to rotate the span to the left */
	template <typename T>
	SortMetrics rotateArrayElementsRight(T* array,
										 array_size_t start, array_size_t end,
										 array_size_t amount);

	/*	Some algorithms like QuickSort use this to ensure the array is not
	 *	 in reverse order, which can lead to n^2 performance */
	template <typename T>
	SortMetrics randomizeArray(T* array, array_size_t size);

	/*	This is used in all of the QuickSort based algorithms	*/
	template <typename T>
	SortMetrics selectAndPositionPivot( T* array,
										array_size_t start,
										array_size_t end);

	template <typename T>
	SortMetrics swap(T*array, array_size_t i, array_size_t j);


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
										  SortMetrics &metrics) {

		array_size_t start 	= range_start;
		array_size_t end	= range_end;
		array_size_t mid	= start + (end-start)/2;

		while (start != end) {
			//	determine the midpoint in an even size span
			//	or the index on the left of mid in an odd size span
			mid = start + (end-start)/2;
			metrics.compares++;
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
			metrics.compares++;
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
	array_size_t binarySearchLastElement(T* array,
										 array_size_t range_start,
										 array_size_t range_end,
										 T& value,
										 SortMetrics &metrics) {

		array_size_t start 	= range_start;
		array_size_t end	= range_end;
		array_size_t mid	= start + (end-start)/2;

		while (start != end) {
			//	determine the midpoint in an even size span
			//	or the index on the left of mid in an odd size span
			mid = start + (end-start)/2;
			metrics.compares++;
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
			metrics.compares++;
			if (array[start] <= value) {
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
	SortMetrics rotateArrayElementsRight(T* array,
										 array_size_t start, array_size_t end,
										 array_size_t amount) {
		SortMetrics result(0,0);

		if (end <= start)
			return result;

		array_size_t span = end - start + 1;
		if (span == 0)
			return result;

		//	converts amounts that are not in [0,span) to in range
		amount = calcRotationWithModulo(amount, span);

		if (amount == 0)
			return result;

		//	reverse the entire array
		for (array_size_t i = start, j = end; i < j; i++, j--) {
			result += SortingUtilities::swap(array, i, j);
		}

		//	reverse the left hand side
		for (array_size_t i = start, j = start+amount-1; i < j; i++, j--) {
			result += SortingUtilities::swap(array, i, j);
		}

		//	reverse the right hand side
		for (array_size_t i = start + amount, j = end; i < j; i++, j--) {
			result += SortingUtilities::swap(array, i, j);
		}
		return result;
	}

	template <typename T>
	SortMetrics randomizeArray(T* array, array_size_t size) {

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
			ret_val += SortingUtilities::swap(array, i, r);
		}

		return ret_val;
	}

	template <typename T>
	SortMetrics selectAndPositionPivot(T* array, array_size_t start, array_size_t end) {

		constexpr bool debug_verbose = false;
		SortMetrics returned_value(0,0);
		// examine elements 1/4 of the way, 1/2 the way, and 3/4 of the way
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

		if (debug_verbose) {
		std::cout << std::endl;
		std::cout << "selecting pivot based upon considering "
				  << low << ", " << mid << ", " << high << std::endl;
			SortingUtilities::printArrayAndPivot(array, start, end, 0, "prior to selecting and positioning pivot: ");
		}
		returned_value.compares++;
		if (array[low] < array[high])
		{
			// start is smaller than end
			returned_value.compares++;
			if (array[low] < array[mid]) {
				// start is smaller than end and smaller than or equal to mid
				// start is the smallest, evaluate mid vs end
				returned_value.compares++;
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
			returned_value.compares++;
			// end is smaller than or equal to start
			if (array[high] < array[mid]) {
				// end is smaller than or equal to start and smaller than or equal to mid
				// end is the smallest, evaluate start vs mid
				returned_value.compares++;
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
		returned_value += SortingUtilities::swap(array, start, pivot);

		if (debug_verbose) {
			std::cout << "array is now " << std::endl;
			printArrayAndPivot(array, start, end, start, "");
			std::cout << std::endl;
		}
		return returned_value;
	}

	template <typename T>
	SortMetrics swap(T* array, array_size_t i, array_size_t j) {
		T temp	 = array[i];
		array[i] = array[j];
		array[j] = temp;
		SortMetrics result(0,0);
		result.assignments = num_assignments_per_swap;
		return result;
	}


}	// namespace SortingUtilities

#pragma pop_macro("_verbose")

#endif	/*	header guard	*/
