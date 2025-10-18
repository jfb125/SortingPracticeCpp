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

#include <iostream>


namespace SelectionSort {

	template <typename T>
	SortMetrics sortPointerstoObjects(T **array, array_size_t array_size) {

		SortMetrics result(0,0);

		if (!SortingUtilities::isSorted(array, array_size, result))
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
					result.compares++;
					if (*array[searching_index] < *array[index_of_smallest_value]) {
						index_of_smallest_value = searching_index;
					}
				}

				if (index_of_smallest_value != first_unsorted_element) {
					result +=
						SortingUtilities::swap(array,
											   index_of_smallest_value,
											   first_unsorted_element);
				}
			}
		}

		return result;
	}
}

#endif /* SELECTIONSORT_H_ */
