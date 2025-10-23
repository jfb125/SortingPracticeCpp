/*
 * MergeSort.h
 *
 *  Created on: Jun 28, 2025
 *      Author: joe
 *      This is a bottom up version of merge sort that uses a
 *        single auxilliary array
 */

#ifndef MERGESORT_H_
#define MERGESORT_H_

#include <iostream>
#include <iomanip>

#include "SortingDataTypes.h"
#include "SortingUtilities.h"

namespace MergeSort {

	template <typename T>
	SortMetrics sortObjectsBottomUp(T*array, array_size_t size) {

		SortMetrics ret_val(0,0);

		if (size <= 1) {
			return ret_val;
		}

		if (SortingUtilities::isSorted(array, size, ret_val)) {
			return ret_val;
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
//		std::cout << "Array size = " << size << std::endl;
		// for each successive size of sub-array
		for (array_size_t half = 1; half < size; half *= 2) {
			// ping-pong
			//	make the new dst_array the address of the previous src_array
			T* 		tmp = src_array;
			src_array 	= dst_array;
			dst_array 	= tmp;

//			std::cout << "  src: " << src_array << " dst: " << dst_array << std::endl;
			// work across the array in chunks that are of width 2*half
			for (array_size_t start = 0; start < size; start += 2*half) {
				// if there is more than half of a span, merge both
				left_stop = start+half;
				right_stop = start+half+half;
				// if there is a left half (or less)
				if (left_stop > size) {
					// disable the right half copy
					left_stop = size;
					right_stop = size;
				} else
				//	if there is not a full right half
				if (right_stop > size) {
					// terminate the right copy at end of array
					right_stop = size;
				}
				dst 	= start;
				left 	= start;
				right 	= left_stop;

//				std::cout << "(" << start << "," << left_stop << "," << right_stop << ") ";
//				while (dst != right_stop) {
				while (left != left_stop && right != right_stop) {
					// compare values on left & right and move the lesser value,
					//	given precedence to the left value if they are equal
					//	which guarantees stability
					ret_val.compares++;
					if (src_array[left] <= src_array[right]) {
						ret_val.assignments++;
						dst_array[dst++] = src_array[left++];
					} else {
						ret_val.assignments++;
						dst_array[dst++] = src_array[right++];
					}
				}
				// 	The above loop terminated because either
				// right == right_stop or left == left_stop, but not both.
				// Finish copying the source half that was not completed.
				while (left < left_stop) {
					ret_val.assignments++;
					dst_array[dst++] = src_array[left++];
				}
				while (right < right_stop) {
					ret_val.assignments++;
					dst_array[dst++] = src_array[right++];
				}
			}
		}

//		std::cout << "array: " << array << " aux: " << aux << " | dst: " << dst_array << ", src " << src_array << std::endl;
		// if the dst of the final merge was into aux,
		//	copy aux back into array
		if (dst_array == aux) {
//			std::cout << "... copying aux into array" << std::endl;
			ret_val.assignments += size;
			for (array_size_t i = 0; i != size; i++) {
				array[i] = aux[i];
			}
		}

		delete[] aux;
		return ret_val;
	}

	template <typename T>
	SortMetrics sort(T* array, array_size_t size) {

		return sortObjectsBottomUp(array, size);
	}
};

#endif /* MERGESORT_H_ */
