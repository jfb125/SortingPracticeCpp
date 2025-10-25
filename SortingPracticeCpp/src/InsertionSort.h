/*
 * InsertionSort.h
 *
 *  Created on: Jun 27, 2025
 *      Author: joe
 */

#ifndef INSERTIONSORT_H_
#define INSERTIONSORT_H_

#include <iostream>

#include "SortingDataTypes.h"
#include "SortingUtilities.h"

namespace InsertionSort {

	template <typename T>
	SortMetrics sort(T *array, array_size_t size) {

		SortMetrics ret_val(0,0);

		for (array_size_t i = 1;
						  i != size;
						  i ++)
		{
			// if the element to the left of element_in_question is equal to
			//	of smaller,	then element_in_question is in the correct place
			ret_val.compares++;
			if (array[i-1] <= array[i])
				continue;

			// make a copy of the element_in_question, which will be stored
			//  in the correct location once the correct location is found
			ret_val.assignments++;
			T current_value = array[i];

			// move the larger element to the right
			ret_val.assignments++;
			array[i] = array[i-1];
			array_size_t prev_i = i-1;

			// continue looking at elements to the left until either
			//  a value is found that is less or equal to current_value
			//  or the start of the array has been found
			while(prev_i != 0) {
				// if the element to the left of current
				//  is greater than current_value it needs to move to the right
				ret_val.compares++;
				if (array[prev_i-1] > current_value) {
					ret_val.assignments++;
					array[prev_i] = array[prev_i-1];
					prev_i--;
				} else {
					break;
				}
			}
			array[prev_i] = current_value;
			ret_val.assignments++;
		}
		return ret_val;
	}
}

#endif /* INSERTIONSORT_H_ */
