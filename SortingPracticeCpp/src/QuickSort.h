/*
 * QuickSort.h
 *
 *  Created on: Jul 3, 2025
 *      Author: joe
 */

#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include <iostream>
#include <iomanip>

#include "SortingDataTypes.h"
#include "SortingUtilities.h"
#include "SimpleRandomizer.h"

#define VERBOSE_QUICK_SORT_MESSAGES false

namespace QuickSort {

	/*	requires that the caller ensure that end >= start	*/
	template <typename T>
	SortMetrics partitionArray(T* array, array_size_t start, array_size_t end) {

		SortMetrics ret_val(0,0);

		if (end == start) {
			return ret_val;
		}

		array_size_t span = end - start + 1;

		// an array with only two elements can be sorted simply
		if (span == 2) {
			ret_val.compares++;
			if (array[start] > array[end]) {
				ret_val += SortingUtilities::swap(array, start, end);
			}
			return ret_val;
		}

		array_size_t pivot = start;
		array_size_t upper = end;
		array_size_t lower = start+1;

		// from the right, find an array value that is <= pivot
		// from the left,  find an array value that is > pivot
		// exchange the two values
		while (1) {
			ret_val.compares++;
			// find an array value that is <= pivot
			while (array[upper] > array[pivot]) {
				upper--;
				ret_val.compares++;
			}
			// find an array value that is > pivot
			//   or stop when lower == upper
			//	 which means no value < pivot was found
			while (lower < upper) {
				ret_val.compares++;
				if (array[lower] > array[pivot])
					break;
				lower++;
			}
			//	if lower crossed upper, the partition is
			//	 complete, so swap pivot with upper and exit
			if (lower >= upper) {
				ret_val += SortingUtilities::swap(array, pivot, upper);
				break;
			}
			// [upper] <= pivot
			// [lower] >  pivot
			//   and lower < upper
			//   so exchange them
			ret_val += SortingUtilities::swap(array, lower, upper);
			// at this point,
			//	[upper] >  [pivot]
			//	[lower] <= [pivot]
			//	moving upper & lower is safe
			lower++;
			upper--;
		}

		// don't partition an array of only 1 element at 'start'
		if (upper != start) {
			ret_val += partitionArray(array, start, upper-1);
		}
		// don't partition an array of only 1 element at 'end'
		if (upper != end) {
			ret_val += partitionArray(array, upper+1, end);
		}
		return ret_val;
	}

	template <typename T>
	SortMetrics sort(T* array, array_size_t size) {

		SortMetrics retval(0,0);

		if (size <= 1)
			return retval;

		if (!SortingUtilities::isSorted(array, size, retval)) {
			retval += SortingUtilities::randomizeArray(array, size);
			retval += partitionArray(array, 0, size-1);
		}
		return retval;
	}
}

#endif /* QUICKSORT_H_ */
