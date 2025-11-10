/*
 * SelectionSort.h
 *
 *  Created on: Jun 27, 2025
 *      Author: joe
 */

#ifndef SELECTIONSORT_H_
#define SELECTIONSORT_H_

#include "SortingDataTypes.h"
#include "SortingUtilities.h"


namespace SelectionSort {

	/*	**************************************************************	*/
	/*							function declaration					*/
	/*	**************************************************************	*/

	// Declare the function with default value for SortMetrics pointer
	template <typename T>
	void sort(T *array, array_size_t array_size, SortMetrics *metrics = nullptr);


	/*	**************************************************************	*/
	/*							function definition						*/
	/*	**************************************************************	*/

	template <typename T>
	void sort(T *array, array_size_t array_size, SortMetrics *metrics) {

		if (!SortingUtilities::isSorted(array, array_size, metrics))
		{
			array_size_t index_of_smallest_value;

			for (array_size_t first_unsorted_element = 0;
							  first_unsorted_element < array_size-1;
							  first_unsorted_element++)
			{
				index_of_smallest_value = first_unsorted_element;

				for (array_size_t searching_index = first_unsorted_element+1;
								  searching_index < array_size;
								  searching_index++) {
					if (metrics) metrics->compares++;
					if (array[searching_index] < array[index_of_smallest_value]) {
						index_of_smallest_value = searching_index;
					}
				}

				if (index_of_smallest_value != first_unsorted_element) {
					SortingUtilities::swap(array,
										   index_of_smallest_value,
										   first_unsorted_element,
										   metrics);
				}
			}
		}
		return;
	}
}

#endif /* SELECTIONSORT_H_ */
