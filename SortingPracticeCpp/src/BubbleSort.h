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

	/*	**********************************************************************	*/
	/*								function declaration						*/
	/*	**********************************************************************	*/

	//	Declare the function with a default value for the SortMetrics pointer
	template <typename T>
	void sort(T* array, array_size_t size, SortMetrics *metrics = nullptr);


	/*	**********************************************************************	*/
	/*								function definition							*/
	/*	**********************************************************************	*/

	template <typename T>
	void sort(T* array, array_size_t size, SortMetrics *metrics) {

		for (array_size_t lowest_unsorted = 0; lowest_unsorted != size-1; lowest_unsorted++) {
			bool was_swap = false;
			for (array_size_t i = size-1; i > lowest_unsorted; i--)
			{
				// if the element before you is larger than you,
				//   swap it - bubble your element up one position
				if (metrics) metrics->compares++;
				if (array[i-1] > array[i]) {
					SortingUtilities::swap(array, i-1, i, metrics);
					was_swap = true;
				}
			}
			if (!was_swap) {
				break;
			}
		}
	}
}

#endif /* BUBBLESORT_H_ */
