/*
 * MergeSort.h
 *
 *  Created on: Jun 28, 2025
 *      Author: Joe Baker
 *
 *      This is a bottom up version of merge sort that uses a
 *        single auxiliary array rather than allocating an
 *        auxiliary array each time two halves are merged
 */

#ifndef MERGESORT_H_
#define MERGESORT_H_

#include <iostream>
#include <iomanip>

#include "SortingDataTypes.h"
#include "SortingUtilities.h"

namespace MergeSort {

	/*	**************************************************************	*/
	/*						function declarations						*/
	/*	**************************************************************	*/

	// Declare function with default value for SortMetrics pointer
	template <typename T>
	void sort(T* array, array_size_t size, SortMetrics *metrics = nullptr);

	template <typename T>
	void sortObjectsBottomUp(T* array, array_size_t size, SortMetrics *metrics=nullptr);


	/*	**************************************************************	*/
	/*						function definitions						*/
	/*	**************************************************************	*/

	/*
	 * 	Wrapper to match convention of other sort algorithms
	 */

	template <typename T>
	void sort(T* array, array_size_t size, SortMetrics *metrics) {

		return sortObjectsBottomUp(array, size, metrics);
	}

	/*
	 * Sort objects without recursion which avoids allocating memory
	 * every time the function is recursively called and the call overhead
	 */

	template <typename T>
	void sortObjectsBottomUp(T*array, array_size_t size, SortMetrics *metrics) {

		if (size <= 1) {
			return;
		}

		T aux[size];
		// this will be swapped before first use
		T* src_array = aux;
		T* dst_array = array;

		array_size_t dst;
		array_size_t left;
		array_size_t left_stop;
		array_size_t right;
		array_size_t right_stop;

		// for each successive size of sub-array
		for (array_size_t half = 1; half < size; half *= 2) {
			// ping-pong
			//	make the new dst_array the address of the previous src_array
			T* 		tmp = src_array;
			src_array 	= dst_array;
			dst_array 	= tmp;

			// work across the array in chunks that are of width 2*half
			for (array_size_t start = 0; start < size; start += 2*half) {
				// if there is more than half of a span, merge both
				left_stop 	= start+half;
				right_stop 	= start+half+half;
				// if there is a left half (or less)
				if (left_stop > size) {
					// disable the right half copy
					left_stop 	= size;
					right_stop 	= size;
				} else
				//	if there is not a full right half
				if (right_stop > size) {
					// terminate the right copy at end of array
					right_stop = size;
				}
				dst 	= start;
				left 	= start;
				right 	= left_stop;

				while (left != left_stop && right != right_stop) {
					// compare values on left & right and move the lesser value
					//	or give priority to the left value if they are equal
					//	which guarantees stability
					if (metrics) metrics->compares++;
					if (src_array[left] <= src_array[right]) {
						if (metrics) metrics->assignments++;
						dst_array[dst++] = src_array[left++];
					} else {
						if (metrics) metrics->assignments++;
						dst_array[dst++] = src_array[right++];
					}
				}
				// The above loop terminated because either
				// 	right == right_stop or left == left_stop, but not both.
				// 	Finish copying the source half that was not completed.
				while (left < left_stop) {
					if (metrics) metrics->assignments++;
					dst_array[dst++] = src_array[left++];
				}
				while (right < right_stop) {
					if (metrics) metrics->assignments++;
					dst_array[dst++] = src_array[right++];
				}
			}
		}

		//	The array is sorted.  If it is stored in the aux buffer,
		//	copied it over to the passed parameter
		if (dst_array == aux) {
			if (metrics) metrics->assignments += size;
			for (array_size_t i = 0; i != size; i++) {
				array[i] = aux[i];
			}
		}
		return;
	}
};

#endif /* MERGESORT_H_ */
