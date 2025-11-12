/*
 * DutchFlagSort.h
 *
 *  Created on: Jul 6, 2025
 *      Author: Joe Baker
 */

#ifndef DUTCHFLAGSORT_H_
#define DUTCHFLAGSORT_H_

#include <iostream>
#include <iomanip>

#include "SortingDataTypes.h"
#include "SortingUtilities.h"
#include "InsertionSort.h"

#pragma push_macro("debug_verbose")
#define debug_verbose false


namespace DutchFlagSort {

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							Debugging Resources							*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	constexpr int max_size_to_cutoff_to_insertion_sort = 0;

	//	During debugging, there were times when the logic in the partitioning
	//	loop would not terminate.  This is a maximum number of times through
	//	partitioning loop before an error is declared
	constexpr int safety_counter_value 	= 1000;
	constexpr bool use_safety_counter	= false;

	// 	To print the debugging messages, the whole array needs to be
	// 	printed, not just the portion that was passed to the partitioning function
	static array_size_t global_start;
	static array_size_t global_end;

	template <typename T>
	std::string threeWayPartitionToString(T*array,
									  	  array_size_t start, array_size_t end,
										  array_size_t lo, array_size_t i,
										  array_size_t hi);

	#define dbg_msg(msg) do {\
		if (debug_verbose) {\
			std::cout << threeWayPartitionToString(array,start,end,lo,i,hi)\
				  	  << msg << std::endl;\
		}\
	} while (false)


	/*	**********************************************************************	*/
	/*							function declarations							*/
	/*	**********************************************************************	*/

	//	Declare the functions with a default value for the SortMetrics pointer
	template <typename T>
	void sort(T* array, array_size_t size, SortMetrics *metrics = nullptr);

	template <typename T>
	void threeWayPartition(T* array, array_size_t start, array_size_t end,
						   SortMetrics *metrics = nullptr);


	/*	**********************************************************************	*/
	/*							function definitions							*/
	/*	**********************************************************************	*/

	/*
	 * top level function that has same signature as other sort algorithms
	 */

	template <typename T>
	void sort(T* array, array_size_t size, SortMetrics *metrics) {

		//	These define the width of the whole array for threeWayPartitionToString()
		global_start= 0;
		global_end 	= size-1;

		if (size <= 1)
			return;

		threeWayPartition(array, 0, size-1, metrics);
		return;
	}


	/*
	 * Three way partitioning function called recursively
	 */

	template <typename T>
	void threeWayPartition(T* array, array_size_t start, array_size_t end,
						   SortMetrics *metrics) {

		array_size_t size = end-start+1;

		// don't partition arrays that have 1 or fewer elements
		if (size <= 1)
			return;


		if (size == 2) {
			if (metrics) metrics->compares++;
			if (array[start] > array[end]) {
				SortingUtilities::swap(array, start, end, metrics);
			}
			return;
		}

		if (size < max_size_to_cutoff_to_insertion_sort) {
			InsertionSort::sort(&array[start], size, metrics);
			return;
		}

		SortingUtilities::selectAndPositionPivot(array, start, end, metrics);

		// left most copy of pivot
		array_size_t lo	= start;
		// element to the right of the right-most copy of the pivot
		array_size_t i 	= lo + 1;
		// index that will move leftward to seek next value <= pivot when i is stopped
		array_size_t hi = end;

		//	If pivot == 3, an intermediate state of array may be:
		//	{ 0  1  3  3  6  4  5 }
		//	        lo    i     hi

		dbg_msg(" before partitioning ");

		while (i <= hi)
		{
			dbg_msg(" at top of while loop ");

			if(debug_verbose) {
				static int safety_counter = safety_counter_value;
				if (use_safety_counter && !safety_counter--) {
					std::cout << " safety counter " << safety_counter_value << " hit 0" << std::endl;
					while(1) {}
				}
			}

			// is [i] < [lo] which contains the pivot?
			//   if so, exchange [i] & [lo] so that the
			//      the smaller goes to lo & the pivot value goes to i
			//   then advance i and advance lo
			if (metrics) metrics->compares++;
			if (array[i] < array[lo]) {
				SortingUtilities::swap(array, i, lo, metrics);
				lo++;
				i++;
				//	go to the top of the loop to evaluate i <= hi
				continue;
			}

			// if [i] == [lo] which contains the pivot
			//   then [i] is in the right place, therefore move i along
			if (metrics) metrics->compares++;
			if (array[i] == array[lo]) {
				i++;
				//	go to the top of the loop to evaluate i <= hi
				continue;
			}

			dbg_msg(" seeking [hi] <= [pivot]");
			// [i] > pivot which is stored at [lo]
			// find right-most element <= pivot
			// --hi may reach i if no elements are <= pivot
			if (metrics) metrics->compares++;
			while (array[hi] > array[lo]) {
				//	hi is being moved, so each time it moves it has
				//	to be compared to i to see if we are done
				if (--hi == i)
					//	all the elements to the right of i,  which is itself
					//	greater than the pivot, are greater than i
					//	then the partitioning is done
					break;
				dbg_msg(" seeking [hi] <= [pivot]");
				if (metrics) metrics->compares++;
			}
			// 	if the above loop did NOT terminate b/c hi == i,
			// 		then [hi] <= pivot.  Swap it with i, which is one
			//	element to the right of the pivot(s)
			//	The result of this swap will not result in the final
			//		position of the element at [hi].
			//	if [hi] == pivot, then i will get the pivot, which will
			//	  	require the next pass through the loop to increment i
			//	if [hi] <  pivot, then [i] will get the value which is < pivot
			//	  	which will require the next pass through the loop to
			//		swap [i] vs [lo]

			if (!(hi <= i)) {
				SortingUtilities::swap(array, i, hi, metrics);
			}
			dbg_msg(" exchanged [i] vs [hi] ");
			//	The element at [hi] has been moved from 'hi', although not to its
			//	final position. Decrement 'hi' to point to the previous element
			hi--;
		}

		dbg_msg(" completed partitioning ");

		// at this point, hi < i to terminate the while loop
		threeWayPartition(array, start, lo-1, metrics);
		threeWayPartition(array, i, end, metrics);

		dbg_msg(" after combining sub-partitions ");
		return;
	}


	/*	**********************************************************************	*/
	/*								debugging resources							*/
	/*	**********************************************************************	*/

	template <typename T>
	std::string threeWayPartitionToString(T*array,
										  array_size_t start, array_size_t end,
										  array_size_t lo, array_size_t i,
										  array_size_t hi) {

		constexpr int lngth = 8;
		std::stringstream result;
		result << std::setw(4) << lo << ", "
			   << std::setw(4) << i  << ", "
			   << std::setw(4) << hi << " ";
		for (array_size_t q = global_start; q <= global_end; q++) {
			//	elements in the array that are not part of this partitioning pass
			if (q < start or q > end) {
				result	<< "     " << std::setw(lngth+1) << std::right << " ";
				continue;
			}
			//	the element at the leftmost copy of the pivot
			if(q == lo) {
				result	<< "<lo> " << std::setw(lngth) << array[q] << " ";
			} else
			//	the element immediately after the rightmost copy of pivot
			if (q == i) {
				result	<< "< i> " << std::setw(lngth) << array[q] << " ";
			} else
			//	the rightmost element that is has not been placed
			if (q == hi) {
				result	<< "<hi> " << std::setw(lngth) << array[q] << " ";
			} else {
			//	an element in the sub-array that is not lo, i, nor hi
				result	<< "     " << std::setw(lngth) << array[q] << " ";
			}
		}
		return result.str();
	}
}

#pragma pop_macro("debug_verbose")

#endif /* DUTCHFLAGSORT_H_ */
