/*
 * QuickSort.h
 *
 *  Created on: Jul 3, 2025
 *      Author: Joe Baker
 */

#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include <iostream>
#include <iomanip>

#include "SortingDataTypes.h"
#include "SortingUtilities.h"
#include "SimpleRandomizer.h"

#define VERBOSE_QUICK_SORT_MESSAGES false

/*
 * 	This has no optimizations:
 * 		- the array is *NOT* checked to see if it is already in order
 * 		- the array is *NOT* pre-randomized
 * 		- the pivot is *ALWAYS* selected as the first element of the span
 */

namespace QuickSort {

	/*	**********************************************************	*/
	/*						function declarations					*/
	/*	**********************************************************	*/

	//	Declare the functions with default values for the SortMetrics pointer
	template <typename T>
	void partitionArray(T* array, array_size_t start, array_size_t end,
						SortMetrics *metrics = nullptr);

	template <typename T>
	void sort(T* array, array_size_t size, SortMetrics *metrics=nullptr);


	/*	**********************************************************	*/
	/*						function definitions					*/
	/*	**********************************************************	*/

	template <typename T>
	void sort(T* array, array_size_t size, SortMetrics *metrics) {

		if (size <= 1)
			return;

		partitionArray(array, 0, size-1, metrics);
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

		array_size_t span = end - start + 1;

		// an array with only two elements can be sorted simply
		if (span == 2) {
			if (metrics) metrics->compares++;
			if (array[start] > array[end]) {
				SortingUtilities::swap(array, start, end, metrics);
			}
			return;
		}

		array_size_t pivot = start;
		array_size_t upper = end;
		array_size_t lower = start+1;

		// from the right, find an array value that is <= pivot
		// from the left,  find an array value that is > pivot
		// exchange the two values
		while (1) {
			if (metrics) metrics->compares++;
			// find an array value that is <= pivot
			while (array[upper] > array[pivot]) {
				upper--;
				if (metrics) metrics->compares++;
			}
			// find an array value that is > pivot
			//   or stop when lower == upper
			//	 which means no value < pivot was found
			while (lower < upper) {
				if (metrics) metrics->compares++;
				if (array[lower] > array[pivot])
					break;
				lower++;
			}
			//	if lower crossed upper, the partition is
			//	 complete, so swap pivot with upper and exit
			if (lower >= upper) {
				SortingUtilities::swap(array, pivot, upper, metrics);
				break;
			}
			// [upper] <= pivot
			// [lower] >  pivot
			//   and lower < upper
			//   so exchange them
			SortingUtilities::swap(array, lower, upper, metrics);
			// at this point,
			//	[upper] >  [pivot]
			//	[lower] <= [pivot]
			//	moving upper & lower is safe
			lower++;
			upper--;
		}

		// don't partition an array of only 1 element at 'start'
		if (upper != start) {
			partitionArray(array, start, upper-1, metrics);
		}
		// don't partition an array of only 1 element at 'end'
		if (upper != end) {
			partitionArray(array, upper+1, end, metrics);
		}
		return;
	}
}

#endif /* QUICKSORT_H_ */
