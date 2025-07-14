/*
 * InsertionSort.h
 *
 *  Created on: Jun 27, 2025
 *      Author: joe
 */

#ifndef INSERTIONSORT_H_
#define INSERTIONSORT_H_

#include <iostream>

#include "SortingPracticeDataTypes.h"
#include "SortingUtilities.h"

template <typename T>
class InsertionSort final {
public:
	InsertionSort() {}
	~InsertionSort() {}

	ComparesAndMoves sortPointersToObjects(T **array, array_size_t size) {

		ComparesAndMoves ret_val(0,0);

		for (array_size_t element_in_question = 1;
						  element_in_question != size;
						  element_in_question ++)
		{
			// if the element to the left is smaller,
			//	then this element is in the correct place
			ret_val._compares++;
			if (*array[element_in_question-1] <= *array[element_in_question])
				continue;

			// make a copy of this element, which will be stored
			//  in the correct location once the correct location is found
			//  this reduces the number of swaps that are required, thereby
			//	reducing the number of moves by a factor of three since
			//  each swap involves three move
			ret_val._moves++;
			T* tmp = array[element_in_question];

			// move the larger element to the right
			ret_val._moves++;
			array[element_in_question] = array[element_in_question-1];
			array_size_t look_to_the_left_i = element_in_question-1;

			// continue looking at elements to the left until either
			//  a value is found that is less or equal to temp
			//  or the start of the array has been found
			while(look_to_the_left_i != 0) {
				// if the element to the left of look_to_the_left_i
				//  is greater than tmp it needs to move to the right
				ret_val._compares++;
				if (*array[look_to_the_left_i-1] > *tmp) {
					ret_val._moves++;
					array[look_to_the_left_i] = array[look_to_the_left_i-1];
					look_to_the_left_i--;
				} else {
					break;
				}
			}
			array[look_to_the_left_i] = tmp;
			ret_val._moves++;
		}
		return ret_val;
	}
};

#endif /* INSERTIONSORT_H_ */
