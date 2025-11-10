/*
 * DutchFlagSort.h
 *
 *  Created on: Jul 6, 2025
 *      Author: joe
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
	// 	printed, not only the portion that was passed to the partitioning function
	static array_size_t global_start;
	static array_size_t global_end;

	template <typename T>
	void printArrayPivotPossibilities(T* array,
									  array_size_t start, array_size_t end,
									  array_size_t lo, array_size_t mid, array_size_t hi);
	template <typename T>
	void printThreeWayPartitionLine(  T*array,
									  array_size_t start, array_size_t end,
									  array_size_t lo, array_size_t i, array_size_t hi);


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

		//	These define the width of the whole array in printThreeWayPartitionLine()
		global_start= 0;
		global_end 	= size-1;

		if (size <= 1)
			return;

		if (SortingUtilities::isSorted(array, size, metrics)) {
			return;
		}

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
			if (debug_verbose)	std::cout << "cutoff partition size 2" << std::endl;
			if (metrics) metrics->compares++;
			if (array[start] > array[end]) {
				SortingUtilities::swap(array, start, end, metrics);
			}
			return;
		}

		if (size < max_size_to_cutoff_to_insertion_sort) {
			if (debug_verbose)	std::cout << "cutoff to insertion sort" << std::endl;
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

		if(debug_verbose) {
			printThreeWayPartitionLine(array, start, end, lo, i, hi);
			std::cout << " before partitioning" << std::endl;
		}

		while (i <= hi)
		{
			if(debug_verbose) {
				printThreeWayPartitionLine(array, start, end, lo, i, hi);
				std::cout << " at top of while loop " << std::endl;
			}
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
				continue;
			}

			// if [i] == [lo] which contains the pivot
			//   then [i] is in the right place, therefore move i along
			if (metrics) metrics->compares++;
			if (array[i] == array[lo]) {
				i++;
				continue;
			}

			if(debug_verbose) {
				printThreeWayPartitionLine(array, start, end, lo, i, hi);
				std::cout << " seeking [hi] <= [pivot]" << std::endl;
			}
			// [i] > pivot which is stored at [lo]
			// find right-most element <= pivot at [lo]
			//   which may involve hi moving down past 8
			if (metrics) metrics->compares++;
			while (array[hi] > array[lo]) {
				if (--hi == i)
					break;
				if (metrics) metrics->compares++;
				if(debug_verbose) {
					printThreeWayPartitionLine(array, start, end, lo, i, hi);
					std::cout << " seeking [hi] <= [pivot]" << std::endl;
				}
			}
			// if hi has not reached i
			if (!(hi <= i)) {
				SortingUtilities::swap(array, i, hi, metrics);
			}
			if(debug_verbose) {
				printThreeWayPartitionLine(array, start, end, lo, i, hi);
				std::cout << " exchanged " << std::endl;
			}
			hi--;
		}

		if(debug_verbose) {
			printThreeWayPartitionLine(array, start, end, lo, i, hi);
			std::cout << " completed partitioning " << std::endl;
		}

		// at this point, hi < i to terminate the while loop
		threeWayPartition(array, start, lo-1, metrics);
		threeWayPartition(array, i, end, metrics);

		if(debug_verbose) {
			printThreeWayPartitionLine(array, start, end, lo, i, hi);
			std::cout << " after combining sub-partitions " << *metrics << std::endl;
		}
		return;
	}


	/*	**********************************************************************	*/
	/*								debugging resources							*/
	/*	**********************************************************************	*/

	template <typename T>
	void printThreeWayPartitionLine(T*array, array_size_t start, array_size_t end, array_size_t lo, array_size_t i, array_size_t hi) {

		constexpr int lngth = 10;
		std::cout << lo << ", " << i << ", " << hi << " ";
		for (array_size_t q = global_start; q <= global_end; q++) {
			if (q < start or q > end) {
				std::cout 			<< "    " << std::setw(lngth) << std::right << " "	<< " ";
				continue;
			}
			if(q == lo) {
				std::cout 			<< " lo>" << std::setw(lngth) << " " /*array[q]*/ << "<";
			} else {
				if (q == i) {
					std::cout 		<< "  i>" << std::setw(lngth) << " " /*array[q]*/ << "<";
				} else {
					if (q == hi) {
						std::cout 	<< " hi>" << std::setw(lngth) << " " /*array[q]*/ << "<";
					} else {
						std::cout 	<< "    " << std::setw(lngth) << " " /*array[q]*/ << " ";
					}
				}
			}
		}
	}

	template <typename T>
	void printArrayPivotPossibilities(T*array, array_size_t start, array_size_t end, array_size_t lo, array_size_t mid, array_size_t hi) {

		int lngth = array[start]->last_name.size();
		std::cout << "          ";
		for (array_size_t q = global_start; q <= global_end; q++) {
			if (q < start or q > end) {
				std::cout << "  " << std::setw(lngth) << std::right << " " 	<< "   ";
				continue;
			}
			if (q == lo || q == mid || q == hi) {
				std::cout << " [" << std::setw(lngth) << array[q]->last_name << "]  ";
			} else {
				std::cout << "  " << std::setw(lngth) << array[q]->last_name << "   ";
			}
		}
	}
}

#pragma pop_macro("debug_verbose")

#endif /* DUTCHFLAGSORT_H_ */
