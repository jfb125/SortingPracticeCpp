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

#pragma push_macro("_verbose")
#undef _verbose
#define _verbose false
#pragma push_macro("MAX_SIZE_TO_CUTOFF_TO_INSERTION_SORT")
#undef MAX_SIZE_TO_CUTOFF_TO_INSERTION_SORT
#define MAX_SIZE_TO_CUTOFF_TO_INSERTION_SORT 0
#define SAFETY_COUNTER_VALUE 1000

namespace DutchFlagSort {

	static array_size_t global_start;
	static array_size_t global_end;

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


	template <typename T>
	SortMetrics threeWayPartition(T* array, array_size_t start, array_size_t end) {

		SortMetrics result(0,0);

		array_size_t size = end-start+1;

		// don't partition arrays that have 1 or fewer elements
		if (size <= 1)
			return result;


		if (size == 2) {
			if (_verbose)	std::cout << "cutoff partition size 2" << std::endl;
			result.compares++;
			if (array[start] > array[end]) {
				SortingUtilities::swap(array, start, end, &result);
			}
			return result;
		}

		if (size < MAX_SIZE_TO_CUTOFF_TO_INSERTION_SORT) {
			if (_verbose)	std::cout << "cutoff to insertion sort" << std::endl;
			return InsertionSort::sort(&array[start], size);
		}

		SortingUtilities::selectAndPositionPivot(array, start, end, &result);

		// left most copy of pivot
		array_size_t lo = start;
		// element to the right of the right-most copy of the pivot
		array_size_t i = lo + 1;
		// index that will move leftward to seek next value <= pivot when i is stopped
		array_size_t hi = end;

		if(_verbose) {
			printThreeWayPartitionLine(array, start, end, lo, i, hi);
			std::cout << " before partitioning" << std::endl;
		}

		while (i <= hi)
		{
			if(_verbose) {
				printThreeWayPartitionLine(array, start, end, lo, i, hi);
				std::cout << " at top of while loop " << std::endl;
			}
			if(_verbose) {
				static int safety_counter = SAFETY_COUNTER_VALUE;
				if (!safety_counter--) {
					std::cout << " safetey counter " << SAFETY_COUNTER_VALUE << " hit 0" << std::endl;
					while(1) {}
				}
			}

			// is [i] < [lo] which contains the pivot?
			//   if so, exchange [i] & [lo] so that the
			//      the smaller goes to lo & the pivot value goes to i
			//   then advance i and advance lo
			result.compares++;
			if (array[i] < array[lo]) {
				SortingUtilities::swap(array, i, lo, &result);
				lo++;
				i++;
				continue;
			}

			// if [i] == [lo] which contains the pivot
			//   then [i] is in the right place, therefore move i along
			result.compares++;
			if (array[i] == array[lo]) {
				i++;
				continue;
			}

			if(_verbose) {
				printThreeWayPartitionLine(array, start, end, lo, i, hi);
				std::cout << " seeking [hi] <= [pivot]" << std::endl;
			}
			// [i] > pivot which is stored at [lo]
			// find right-most element <= pivot at [lo]
			//   which may involve hi moving down past 8
			result.compares++;
			while (array[hi] > array[lo]) {
				if (--hi == i)
					break;
				result.compares++;
				if(_verbose) {
					printThreeWayPartitionLine(array, start, end, lo, i, hi);
					std::cout << " seeking [hi] <= [pivot]" << std::endl;
				}
			}
			// if hi has not reached i
			if (!(hi <= i)) {
				SortingUtilities::swap(array, i, hi, &result);
			}
			if(_verbose) {
						printThreeWayPartitionLine(array, start, end, lo, i, hi);
						std::cout << " exchanged " << std::endl;
					}
			hi--;
		}

		if(_verbose) {
			printThreeWayPartitionLine(array, start, end, lo, i, hi);
			std::cout << " completed partitioning " << std::endl;
		}

		// at this point, hi < i to terminate the while loop
		result += threeWayPartition(array, start, lo-1);
		result += threeWayPartition(array, i, end);

		if(_verbose) {
			printThreeWayPartitionLine(array, start, end, lo, i, hi);
			std::cout << " after combining sub-partitions " << result << std::endl;
		}
		return result;
	}

	template <typename T>
	SortMetrics sort(T* array, array_size_t size) {

		global_start = 0;
		global_end = size-1;
		SortMetrics result(0,0);

		if (size <= 1)
			return result;

		if (SortingUtilities::isSorted(array, size, &result)) {
			return result;
		}

		result += threeWayPartition(array, 0, size-1);

		return result;
	}
}

#pragma pop_macro("_isVerbose")
#pragma pop_macro("MAX_SIZE_TO_CUTOFF_TO_INSERTION_SORT")

#endif /* DUTCHFLAGSORT_H_ */
