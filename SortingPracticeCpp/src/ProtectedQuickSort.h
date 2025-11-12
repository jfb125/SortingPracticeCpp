/*
 * QuickInsertionHybridSort.h
 *
 *  Created on: Jul 3, 2025
 *      Author: Joe Baker
 */

#ifndef PROTECTEDQUICKSORT_H_
#define PROTECTEDQUICKSORT_H_

#include <iostream>
#include <iomanip>

#include "SortingDataTypes.h"
#include "SortingUtilities.h"
#include "SimpleRandomizer.h"
#include "InsertionSort.h"

//	There are two protections added to Quick Sort in this algorithm
//	1.	Pre-randomize the array.
//		This protects the algorithm	from arrays that are in reverse order.
//		The cost is	an increase of 43% in the number of assignments with
//		no appreciable change in the number of compares
//	2.	Select the mid value of three separated elements for the pivot
//		This saves comparisons and moves on arrays that are almost sorted
//		This reduces the number of comparisons slightly, but increases
//		the number of moves by 35%

namespace ProtectedQuickSort {

	// Below a certain size, do an insertion sort rather
	//	than all the overhead of partitioning

	constexpr array_size_t max_size_to_cutoff_to_insertion_sort = 0;
	/*	**********************************************************	*/
	/*						function declarations					*/
	/*	**********************************************************	*/

	// Declare the functions with default parameter for SortMetrics*
	template <typename T>
	void partitionArray(T* array, array_size_t start, array_size_t end,
					    SortMetrics *metrics = nullptr);

	template <typename T>
	void sort(T* array, array_size_t size,
			  SortMetrics *metrics = nullptr);


	/*	**********************************************************	*/
	/*						function definitions					*/
	/*	**********************************************************	*/

	template <typename T>
	void sort(T* array, array_size_t size, SortMetrics *metrics) {

		if (size <= 1)
			return;

		if (!SortingUtilities::isSorted(array, size, metrics)) {
//			SortingUtilities::randomizeArray(array, size, metrics);
			partitionArray(array, 0, size-1, metrics);
		}

		return;
	}


	/*
	 * 	Partition array recursively into [ <= pivot : > pivot ]
	 */

	template <typename T>
	void partitionArray(T* array, array_size_t start, array_size_t end,
						SortMetrics *metrics) {

		if (end <= start) {
			return;
		}

		array_size_t span = end-start+1;

		// an array with only two elements can be sorted simply
		if (span == 2) {
			if (metrics) metrics->compares++;
			if (array[start] > array[end]) {
				SortingUtilities::swap(array, start, end, metrics);
			}
			return;
		}

		// if array is small enough try insertion sort
		if (span <= max_size_to_cutoff_to_insertion_sort) {
			InsertionSort::sort(&array[start], span, metrics);
		}

		// move the median of three values into the position slot
		SortingUtilities::selectAndPositionPivot(array, start, end, metrics);

		array_size_t pivot = start;
		array_size_t upper = end;
		array_size_t lower = start+1;

		while (1) {
			if (metrics) metrics->compares++;
			while (array[upper] > array[pivot]) {
				upper--;
				if (metrics) metrics->compares++;
			}
			while (lower < upper) {
				if (metrics) metrics->compares++;
				if (array[lower] > array[pivot])
					break;
				lower++;
			}
			if (lower >= upper) {
				// upper is at the right-most element that
				//	is less than or equal to the pivot
				SortingUtilities::swap(array, pivot, upper, metrics);
				break;
			}
			SortingUtilities::swap(array, lower, upper, metrics);
			lower++;
			upper--;
		}

		if (upper != start) {
			partitionArray(array, start, upper-1, metrics);
		}
		if (upper != end) {
			partitionArray(array, upper+1, end, metrics);
		}
		return;
	}
}

#endif /* PROTECTEDQUICKSORT_H_ */
