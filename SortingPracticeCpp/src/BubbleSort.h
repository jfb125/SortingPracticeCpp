/*
 * BubbleSort.h
 *
 *  Created on: Jun 18, 2025
 *      Author: joe
 */

#ifndef BUBBLESORT_H_
#define BUBBLESORT_H_

#include "SortingDataTypes.h"
#include "SortingUtilities.h"


namespace BubbleSort {

	template <typename T>
	SortMetrics sort(T* array, array_size_t size) {

		SortMetrics result(0,0);

		for (array_size_t lowest_unsorted = 0; lowest_unsorted != size-1; lowest_unsorted++) {
			bool was_swap = false;
			for (array_size_t i = size-1; i > lowest_unsorted; i--)
			{
				// if the element before you is larger than you,
				//   swap it - bubble your element up one position
				result.compares++;
				if (array[i-1] > array[i]) {
					SortingUtilities::swap(array, i-1, i, &result);
					was_swap = true;
				}
			}
			if (!was_swap) {
				break;
			}
		}
		return result;
	}
}

#endif /* BUBBLESORT_H_ */
