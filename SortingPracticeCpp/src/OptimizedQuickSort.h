/*
 * QuickInsertionHybridSort.h
 *
 *  Created on: Jul 3, 2025
 *      Author: joe
 */

#ifndef OPTIMIZEDQUICKSORT_H_
#define OPTIMIZEDQUICKSORT_H_

#include <iostream>
#include <iomanip>

#include "SortingPracticeDataTypes.h"
#include "SimpleRandomizer.h"
#include "SortingUtilities.h"
#include "InsertionSort.h"

#pragma push_macro("MAX_SIZE_TO_CUTOFF_TO_INSERTION_SORT")
#define MAX_SIZE_TO_CUTOFF_TO_INSERTION_SORT 11

namespace OptimizedQuickSort {

	/*	requires that the caller ensure that end > start	*/
	template <typename T>
	SortMetrics partitionArray(T** array, array_size_t start, array_size_t end) {

		SortMetrics ret_val(0,0);

		if (start == end) {
			return ret_val;
		}

		array_size_t span = end-start+1;

		// an array with only two elements can be sorted simply
		if (span == 2) {
			ret_val.compares++;
			if (*array[start] > *array[end]) {
				ret_val += SortingUtilities::swap(array, start, end);
			}
			return ret_val;
		}

		// if array is small enough try insertion sort
		if (span <= MAX_SIZE_TO_CUTOFF_TO_INSERTION_SORT) {
			ret_val += InsertionSort::sortPointersToObjects(&array[start], span);
			return ret_val;
		}

		// move the median of three values into the position slot
		ret_val += SortingUtilities::selectAndPositionPivot(array, start, end);

		array_size_t pivot = start;
		array_size_t upper = end;
		array_size_t lower = start+1;

		while (1) {
			ret_val.compares++;
			while (*array[upper] > *array[pivot]) {
				upper--;
				ret_val.compares++;
			}
			while (lower < upper) {
				ret_val.compares++;
				if (*array[lower] > *array[pivot])
					break;
				lower++;
			}
			if (lower >= upper) {
				// upper is at the right-most element that
				//	is less than or equal to the pivot
				ret_val += SortingUtilities::swap(array, pivot, upper);
				break;
			}
			ret_val += SortingUtilities::swap(array, lower, upper);
		}

		if (upper != start) {
			ret_val += partitionArray(array, start, upper-1);
		}
		if (upper != end) {
			ret_val += partitionArray(array, upper+1, end);
		}
		return ret_val;
	}

	template <typename T>
	SortMetrics sortPointerstoObjects(T** array, array_size_t size) {

		SortMetrics retval(0,0);

		if (size <= 1)
			return retval;

		if (!SortingUtilities::isSorted(array, size, retval)) {
			// randomizeArray() requires that size > 1
			//		retval += randomizeArray(array, size);
			retval += partitionArray(array, 0, size-1);
		}

		return retval;
	}
}

#pragma pop_macro("MAX_SIZE_TO_CUTOFF_TO_INSERTION_SORT")

#endif /* OPTIMIZEDQUICKSORT_H_ */
