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

#include "SortingPracticeDataTypes.h"
#include "SortingUtilities.h"

namespace MergeSort {

	template <typename T>
	ComparesAndMoves sortPointersToObjectsBottomUp(T**array, array_size_t size) {

		ComparesAndMoves ret_val(0,0);


		if (size <= 1) {
			return ret_val;
		}

		if (SortingUtilities::isSorted(array, size, ret_val)) {
			return ret_val;
		}

		T**aux = new T*[size];
		// this will be swapped before first use
		T**src_array = aux;
		T**dst_array = array;

		array_size_t dst;
		array_size_t left;
		array_size_t left_stop;
		array_size_t right;
		array_size_t right_stop;
//		std::cout << "Array size = " << size << std::endl;
		// for each successive size of sub-array
		for (array_size_t half = 1; half < size; half *= 2) {

			T** tmp = src_array;
			src_array = dst_array;
			dst_array = tmp;

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
				dst = start;
				left = start;
				right = left_stop;

//				std::cout << "(" << start << "," << left_stop << "," << right_stop << ") ";
				while (dst != right_stop) {
					// if the right source has been exhausted, copy left
					if (right == right_stop) {
						while (left < left_stop) {
							ret_val._moves++;
							dst_array[dst++] = src_array[left++];
						}
						break;
					}
					// if the left source has been exhausted, copy right
					if (left == left_stop) {
						while (right < right_stop) {
							ret_val._moves++;
							dst_array[dst++] = src_array[right++];
						}
						break;
					}
					// compare values on left & right and move the lesser value
					ret_val._compares++;
					if (*src_array[left] <= *src_array[right]) {
						ret_val._moves++;
						dst_array[dst++] = src_array[left++];
					} else {
						ret_val._moves++;
						dst_array[dst++] = src_array[right++];
					}
				}
			}
//			std::cout << std::endl;
		}

//		std::cout << "array: " << array << " aux: " << aux << " | dst: " << dst_array << ", src " << src_array << std::endl;
		// if the dst of the final merge was into aux,
		//	copy aux back into array
		if (dst_array == aux) {
//			std::cout << "... copying aux into array" << std::endl;
			for (array_size_t i = 0; i != size; i++) {
				array[i] = aux[i];
			}
		}

		delete[] aux;
		return ret_val;
	}

	template <typename T>
	ComparesAndMoves sortPointersToObjects(T** array, array_size_t size) {

		return sortPointersToObjectsBottomUp(array, size);
	}
};

#endif /* MERGESORT_H_ */
