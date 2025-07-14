/*
 * MergeSort.h
 *
 *  Created on: Jun 28, 2025
 *      Author: joe
 */
#if 0
#ifndef MERGESORT_H_
#define MERGESORT_H_

#include "SortingPracticeDataTypes.h"
#include <iostream>
#include <iomanip>

template <typename T>

class MergeSort final {
private:

	ComparesAndMoves sortObjects(T* array, array_size_t start, array_size_t end) {

		ComparesAndMoves retval(0,0);

		return retval;
	}

	ComparesAndMoves sortPointersToObjects(T**array, T**aux, array_size_t start, array_size_t end) {

		ComparesAndMoves ret_val(0,0);
		ComparesAndMoves partition_result(0,0);


		if (start >= end) {
			return ret_val;
		}

		array_size_t span = end-start+1;	// determines size of temp array
		array_size_t mid = (span / 2);	// determines starting place of right side
		array_size_t start_of_left_partition = start;
		array_size_t end_of_left_partition = start + mid-1;
		array_size_t start_of_right_partition = start + mid;
		array_size_t end_of_right_partition = end;

		// partition this span of the array and mergeSort each partition
		ret_val += sortPointersToObjects(array, aux, start_of_left_partition, end_of_left_partition);
		ret_val += sortPointersToObjects(array, aux, start_of_right_partition, end_of_right_partition);

		//	each of the two partitions has all of their elements in order
		//	it is now time to merge the partitions onto this span
		array_size_t dst = 0;
		array_size_t src_left  = start_of_left_partition;
		array_size_t src_right = start_of_right_partition;

		while (dst < span) {
			// if the left side has been exhausted, copy the right side
			if (src_left > end_of_left_partition) {
				while (src_right <= end_of_right_partition) {
					ret_val._moves++;
					aux[dst++] = array[src_right++];
				}
			} else
			// if the right side has been exhausted, copy the left side
			if (src_right > end_of_right_partition) {
				while (src_left <= end_of_left_partition) {
					ret_val._moves++;
					aux[dst++] = array[src_left++];
				}
			} else  {
				// is  array[src_left] <= array[src_right] ? )
				//  i.e. = is array[src_right] < array[src_left]
				ret_val._compares++;
				ret_val._moves++;
				if (*array[src_right] < *array[src_left]) {
					aux[dst++] = array[src_right++];
				} else {
					aux[dst++] = array[src_left++];
				}
			}
		}

		array_size_t copy_src = 0;
		array_size_t copy_dst = start;

		while (copy_dst <= end) {
			ret_val._moves++;
			array[copy_dst++] = aux[copy_src++];
		}

		ret_val._moves += span;
		return ret_val;
	}

public:
	MergeSort() {}
	~MergeSort() {}

	ComparesAndMoves sortObjects(T* array, array_size_t size) {

		return sortObjects(array, 0, size-1);
	}

	ComparesAndMoves sortPointersToObjects(T** array, array_size_t size) {

		T** aux = new T*[size];
		return sortPointersToObjects(array, aux, 0, size-1);
		delete[] aux;
	}
};

#endif /* MERGESORT_H_ */
#endif
