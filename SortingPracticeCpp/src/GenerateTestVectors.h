/*
 * GenerateTestVectors.h
 *
 *  Created on: Oct 25, 2025
 *      Author: joe
 */

#ifndef GENERATETESTVECTORS_H_
#define GENERATETESTVECTORS_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "nChoosek.h"

namespace SortingUtilities{

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							function declarations						*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	/*	This generates all possible combinations of test vectors possible
	 * by choosing 'mid' elements from a collection 'test_value' of size
	 * 'test_vector_size'.  It is the caller's responsibility to ensure that
	 * num_test_vectors (size of T test_vectors[num_test_vecors][test_vector_sizse]
	 * is sufficient to hold all combinations.  Used to test block merging */
	template <typename T>
	bool generateAllCombinationsOfValues(T** test_vectors,
										 T   test_values[],
										 int num_test_vectors,
										 int test_vector_size,
										 int mid);

	/*	Generates al 'test_vector_size! compinations of test_values[] */
	template <typename T>
	bool generateAllPermutationsOfValues(T** test_vectors,
										 T   test_values[],
										 int num_test_vectors,
										 int test_vector_size);

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							function declarations						*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	/*
	 * 	 Generates a series of test vectors that are split from [0:mid-1] [mid:size-1]
	 * 	such that any given distinct combination of 'mid' values appears only once
	 * 	in the [0:mid-1].  This is useful for testing functions that sort each half
	 * 	of the test vector before operating on the array
	 * 		Consider the values { A, B, C, D, E, F } with mid = 3
	 * 		The test patterns	{ A, C, E, B, D, F } and { E, C, A, F, D, B }
	 * 		will be equivalent vectors after sorting [0:2] and [3:5]
	 * 		to { A, C, E, B, D, F }
	 */

	template <typename T>
	bool generateAllCombinationsOfValues(T** test_vectors,
										 T*  test_values,
										 int num_test_vectors,
										 int test_vector_size,
										 int mid) {

		constexpr bool debug_verbose = false;

		/*	******************************************	*/
		/*					lambdas						*/
		/*	******************************************	*/

		auto displayVector = [] (std::vector<T>vector) {
			std::cout << "{ ";
			for (size_t i = 0; i != vector.size(); i++) {
				std::cout << vector.at(i) << " ";
			}
			std::cout << "}";
		};

		auto displayArray = [] (T* array, int num_elements) {
			std::cout << "{ ";
			for (int i = 0; i != num_elements; i++) {
				std::cout << array[i] << " ";
			}
			std::cout << "}";
		};

		//	append any allowed_values that are not already in
		//	the left side of test_vector
		auto finishBuildingTestVector = [] (T* test_vector,
											int test_vector_size, int dst_i,
											std::vector<T> &values) {
			//	for all of the values in 'allowed'
			for (auto it : values) {
				int i = 0;
				bool found = false;
				//	see if the allowed value is already in test_vector
				for (; i != dst_i; i++) {
					if (test_vector[i] == it) {
						found = true;
						break;
					}
				}
				//	if the value is not in the test_vector, add it
				if (!found) {
					test_vector[dst_i] = it;
					dst_i++;
				}
				//	if we have completely filled the test vector, done
				if (dst_i == test_vector_size)
					break;
			}
		};

		/*	******************************************	*/
		/*			main code starts here				*/
		/*	******************************************	*/

		//	create a vector of test_values for generator
		std::vector<T> allowed_values;
		for (int i = 0; i != test_vector_size; i++) {
			allowed_values.emplace_back(test_values[i]);
		}

		//	Instantiate an instance of the class that will
		//	generate all unique combinations of the test_values
		nChoosek generator(test_vector_size, mid, allowed_values);

		//	create each test vector

		for (int vector_num = 0; vector_num != num_test_vectors; ++vector_num) {
			std::vector<T>combo;
			generator.next(combo);
			if (debug_verbose) {
				std::cout << std::setw(4) << vector_num << ": ";
				displayVector(combo);
				std::cout << " generates ";
			}
			//	copy elements for left side of test vector
			size_t elem_num = 0;
			for ( ; elem_num != combo.size(); ++elem_num) {
				test_vectors[vector_num][elem_num] = combo.at(elem_num);
			}
			//	Append values not already in the test vector to the test vector
			finishBuildingTestVector(test_vectors[vector_num],
									 test_vector_size,
									 elem_num,
									 allowed_values);
			if (debug_verbose) {
				displayArray(test_vectors[vector_num], test_vector_size);
				std::cout << std::endl;
			}
		}
		return true;
	}

	/*	Generates every possible sequence of test_values[].  It is the
	 *	callers responsibility to ensure that
	 *		test_vectors[num_test_vectors][test_vector_size] exists */
	template <typename T>
	bool generateAllPermutationsOfValues(T** test_vectors,
										 T	 test_values[],
										 int num_test_vectors,
										 int test_vector_size) {

		bool result = true;

		constexpr bool verbose_messages = false;

		auto testVectorToString = [] (T* array, int num) -> std::string {
			std::stringstream result;
			result << '"';
			for (int i = num-1; i >= 0; i--) {
				result << array[i];
			}
			result << '"';
			return result.str();
		};

		auto rotateValuesRight = [] (T *array, int start, int end, int amount)
		{
			auto xchng = [] (T *array, int i, int j) {
				T tmp = array[i]; 	array[i] = array[j]; 	array[j] = tmp;
			};

			for (int i = start, j = end; i < j; i++, j--)  				xchng(array, i, j);
			for (int i = start, j = start+amount-1; i < j; i++, j--)  	xchng(array, i, j);
			for (int i = start+amount, j = end; i < j; i++, j--) 		xchng(array, i, j);
		};

		auto validateTestVectors = [testVectorToString] (T** _vectors, int _num, int _size) -> bool {
			for (int i = 0; i < _num-1; i++) {
				for (int j = i+1; j < _num; j++) {
					bool different = false;
					for (int k = 0; k != _size; k++) {
						if (_vectors[i][k] != _vectors[j][k]) {
							different = true;
							break;
						}
					}
					if (!different) {
						std::cout << "ERROR: vector[" << i << "] "
								  << testVectorToString(_vectors[i], _size)
								  << " == vector[" << j << "] "
								  << testVectorToString(_vectors[j], _size)
								  << std::endl;
						return false;;
					}
				}
			}
			return true;
		};

		auto copyTestVector = [ ](T* dst, T* src, int size) {
			for (int i = 0; i != size; i++) {
				dst[i] = src[i];
			}
		};

		auto rotateCounterToString = [] (int *array, int size) -> std::string {
			std::stringstream result;
			result << "<";
			for (int i = size-1; i > 0; i--) {
				result << std::setw(2) << array[i] << " :";
			}
			result <<  std::setw(2) << array[0] << " >";
			return result.str();
		};

		/*	**********************************************	*/
		/*				code starts here					*/
		/*	**********************************************	*/

		//	consider a test space of 6 possible values for a digit
		//	there will be 6! = 720 vectors
		//	5 4 3 2 1 0  indices
		//	A B C D E F  digits
		//  7 6 5 4 3 2  counters

		int num_rotate_digits = test_vector_size-1;

		int rotate_counts[num_rotate_digits];
		int rotate_counters[num_rotate_digits];
		for (int i = 0; i < num_rotate_digits; i++) {
			rotate_counts[i] 	= i+1;
			rotate_counters[i] 	= i+1;
		}

		int test_vector_number 	= 0;
		int active_digit 		= 0;
		while(test_vector_number < num_test_vectors) {
			if (verbose_messages) {
				std::cout << "test_vector[" << std::setw(3) << test_vector_number << "] = "
						  << testVectorToString(test_values, test_vector_size)
						  << " " << rotateCounterToString(rotate_counters, num_rotate_digits) << std::endl;
			}
			copyTestVector(test_vectors[test_vector_number++], test_values, test_vector_size);
			while(active_digit < num_rotate_digits) {
				rotateValuesRight(test_values, 0, active_digit+1, 1);
				if (--rotate_counters[active_digit] < 0) {
					// if the current digit's count is 0, all permutations have been stored
					rotate_counters[active_digit] = rotate_counts[active_digit];
					active_digit++;
				} else {
					// the current digit's count is not 0, so we do not need to rotate the next count
					break;
				}
			}
			active_digit = 0;
		}
		if (verbose_messages) {
			std::cout << "Validating that " << num_test_vectors << " are unique\n";
		}
		if (!validateTestVectors(test_vectors, num_test_vectors, test_vector_size)) {
			result = false;
		}
		if (verbose_messages) {
			std::cout << __FUNCTION__ << " returns " << result << std::endl;
		}
		return result;
	}

}	// namespace GenerateTestVectors



#endif /* GENERATETESTVECTORS_H_ */
