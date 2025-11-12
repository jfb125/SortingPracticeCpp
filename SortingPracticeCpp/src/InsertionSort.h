/*
 * InsertionSort.h
 *
 *  Created on: Jun 27, 2025
 *      Author: Joe Baker
 */

#ifndef INSERTIONSORT_H_
#define INSERTIONSORT_H_

#include <iostream>

#include "SortingDataTypes.h"
#include "SortingUtilities.h"

namespace InsertionSort {

	/*	**************************************************************	*/
	/*						function declarations						*/
	/*	**************************************************************	*/

	// Declare the function with default value for SortMetrics pointer
	template <typename T>
	void sort(T *array, array_size_t size, SortMetrics *metrics = nullptr);


	/*	**************************************************************	*/
	/*						function definition							*/
	/*	**************************************************************	*/

	template <typename T>
	void sort(T *array, array_size_t size, SortMetrics *metrics) {

		for (array_size_t i = 1; i != size; i++)
		{
			// if the element to the left of 'i' is equal to
			//	or of smaller than element[i], then 'i' is in
			//	the correct place
			if (metrics) metrics->compares++;
			if (array[i-1] <= array[i])
				continue;

			// make a copy of the [i] which will be stored in the
			//	correct location once an element <= to [i] is found
			if (metrics) metrics->assignments++;
			T current_value = array[i];

			// move the larger element to the right
			if (metrics) metrics->assignments++;
			array[i] = array[i-1];
			array_size_t prev_i = i-1;

			// continue looking at elements to the left until either
			//  a value is found that is less or equal to current_value
			//  or the start of the array has been found
			while(prev_i != 0) {
				// if the element to the left of current
				//  is greater than current_value it needs to move to the right
				if (metrics) metrics->compares++;
				if (array[prev_i-1] > current_value) {
					if (metrics) metrics->assignments++;
					array[prev_i] = array[prev_i-1];
					prev_i--;
				} else {
					break;
				}
			}
			if (metrics) metrics->assignments++;
			array[prev_i] = current_value;
		}
	}
}

#endif /* INSERTIONSORT_H_ */
