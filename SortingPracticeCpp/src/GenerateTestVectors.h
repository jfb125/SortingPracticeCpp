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
#include <limits>

#include "SortingDataTypes.h"
#include "ArrayComposition.h"
#include "nChoosek.h"

/*	Generates all permutations of a vector of values	*/
//	TODO - m_reload count is just the position,
//	TODO - make a copy of the initial values
//		   compare each new state to the initial to detect 'm_done'
//	       This will be more reliable in situations with repeated values
template <typename T>
class PermutationGenerator {
private:
	using StateDigit = int;
	bool 		m_done;				// set when the last value is returned,
									// stays set until user calls init()
	int	 		m_width;			// size of the vector of values
	T*	 		m_values;			// the vector of values
	StateDigit	*m_reload_counts;	// values of StateDigits that cause a carry
	StateDigit	*m_rotate_counters;	// the state as a sequence of StateDigits
	bool 		m_initialized;		// all pointers are valid

	void clear() {
		m_done 				= false;
		m_width 			= 0;
		m_values			= nullptr;
		m_reload_counts 	= nullptr;
		m_rotate_counters 	= nullptr;
		m_initialized		= false;
	}

	void erase_and_clear() {
		if (m_values) 			delete[] m_values;
		if (m_reload_counts) 	delete[] m_reload_counts;
		if (m_rotate_counters)	delete[] m_rotate_counters;
		clear();
	}

	void reset_state() {
		if (m_initialized) {
			m_done = false;
			for (int i = 0; i < m_width; i++) {
				m_reload_counts[i]		= (m_width-1)-i;
				m_rotate_counters[i]	= (m_width-1)-i;
			}
		}
	}

	void translate_state(T*dst) {
		for (int i = 0; i != m_width; i++) {
			dst[i] = m_values[i];
		}
	}

	//	0 1 2 3
	//	A B C D  rotate_left_b_1(values, 1, 3) -> A C D B
	void rotate_left_by_1(T* values, int left_boundary, int right_boundary) {
		if (left_boundary > right_boundary) {
			int tmp 		= left_boundary;
			left_boundary 	= right_boundary;
			right_boundary	= tmp;
		}
		T leftmost_value = values[left_boundary];
		for (int i = left_boundary; i < right_boundary ; i++) {
			values[i] = values[i+1];
		}
		values[right_boundary] = leftmost_value;
	}

	//	generating vectors is done by rotating digits left
	//					rotate_counters	 { A, B, C, D }
	//	{ A, B, C, D }	3 2 1 x	 becomes { A, B, D, C }  3 2 0 x DONE
	//	{ A, B, D, C }	3 2 0 x  becomes { A, B, C, D }  3 2 1 x BORROW
	//	{ A, B, C, D }  3 2 1 x  becomes { A, C, D, B }  3 1 1 x BORROW DONE
	//	{ A, C, D, B }  3 1 1 x  becomes { A, C, B, D }  3 1 0 x DONE
	//	{ A, C, D, B }  3 1 0 x  becomes { A, C, D, B }  3 1 1 x BORROW
	//	{ A, C, D, B }  3 1 1 x  becomes { A, D, B, C }  3 0 1 x BORROW DONE
	//  { A, D, B, C }  3 0 1 x  becomes { A, D, C, B }  3 0 0 x DONE
	//	{ A, D, C, B }  3 0 0 x  becomes { A, B, C, D }  3 2 1 x BORROW
	//	{ A, B, C, D }  3 2 1 x  becomes { D, A, B, C }  2 2 1 x BORROW DONE
	//	Rule: always rotate values 1 to the right
	//		  if you are at zero, reload, and move position to the left
	//		  keep doing this until a position is reached where count != 0
	//		  or all positions are examined
	//
	//	position = 1;
	//	do {
	//		rotate right [position:0] by 1
	//		if (rotate_counters[position] != 0
	//	  		rotate_counters[position]--;
	//	  		break;
	//		else
	//	  		rotate_counters[position] = reload_counts[position]
	//	  		position++;
	//	} while (position < m_width)
	//	if (position == m_width)
	//		done = true;_
	bool advance_state() {
		// 	start one digit to the left of the least significant digit,
		//	where the lsdigit is the end of the array, m_width-1
		int right_boundary 	= m_width-1;
		int left_boundary	= right_boundary-1;
		do {
			rotate_left_by_1(m_values, left_boundary, right_boundary);
			if (m_rotate_counters[left_boundary] != 0) {
				--m_rotate_counters[left_boundary];
				break;
			} else {
				m_rotate_counters[left_boundary] = m_reload_counts[left_boundary];
				left_boundary--;
			}
		} while (left_boundary >= 0);
		if (left_boundary < 0) {
			m_done = true;
		}
		return m_done;
	}

public:

	PermutationGenerator() {
		clear();
	}

	PermutationGenerator(T* values, int num_values) {
		clear();
		if (num_values && values) {
			m_width	 = num_values;
			m_values = new T[m_width];
			for (int i = 0; i != m_width; i++) {
				m_values[i] = values[i];
			}
			m_reload_counts 	= new StateDigit[m_width];
			m_rotate_counters 	= new StateDigit[m_width];
			m_initialized 		= true;
			reset_state();
		}
	}

	PermutationGenerator(const PermutationGenerator & other) {
		clear();
		if (other.m_initialized) {
			m_initialized 		= true;
			m_done				= other.m_done;
			m_width				= other.m_width;
			//	deep copy other's values
			m_values			= new T[m_width];
			for (int i = 0; i != m_width; i++) {
				m_values[i] = other.m_values[i];
			}
			//	deep copy other's state variables
			m_reload_counts		= new StateDigit[m_width];
			m_rotate_counters	= new StateDigit[m_width];
			for (int i = 0; i != m_width; i++) {
				m_reload_counts[i]	= other.m_reload_counts[i];
				m_rotate_counters[i]= other.m_rotate_counters[i];
			}
		}
	}

	PermutationGenerator& operator=(const PermutationGenerator& other) {
		if (this != &other) {
			erase_and_clear();
			if (other.m_initialized) {
				m_initialized			= true;
				m_done				= other.m_done;
				m_width				= other.m_width;
				//	deep copy other's values
				m_values			= new T[m_width];
				for (int i = 0; i != m_width; i++) {
					m_values[i] = other.m_values[i];
				}
				//	deep copy other's state
				m_reload_counts		= new StateDigit[m_width];
				m_rotate_counters	= new StateDigit[m_width];
				for (int i = 0; i != m_width; i++) {
					m_reload_counts[i]	= other.m_reload_counts[i];
					m_rotate_counters[i]= other.m_rotate_counters[i];
				}
			}
		}
		return *this;
	}

	PermutationGenerator(const PermutationGenerator&& other) noexcept {
		clear();
		if (other.m_initialized) {
			m_initialized			= true;
			m_done					= other.m_done;
			m_width					= other.m_width;
			m_values				= other.m_values;
			m_reload_counts			= other.m_reload_counts;
			m_rotate_counters		= other.m_rotate_counters;
			other.m_values			= nullptr;
			other.m_reload_counts	= nullptr;
			other.m_rotate_counters	= nullptr;
		}
	}

	PermutationGenerator& operator=(const PermutationGenerator&& other) noexcept {
		if (this != &other) {
			erase_and_clear();
			if (other.m_initialized) {
				m_initialized 			= true;
				m_done					= other.m_done;
				m_width					= other.m_width;
				m_values				= other.m_values;
				other.m_values			= nullptr;
				m_reload_counts			= other.m_reload_counts;
				other.m_reload_counts	= nullptr;
				m_rotate_counters		= other.m_rotate_counters;
				other.m_rotate_counters	= nullptr;
			}
		}
		return *this;
	}

	~PermutationGenerator() {
		erase_and_clear();
	}

	bool is_initialized(void) const {	return m_initialized;	}
	bool is_done(void) const 		{	return m_done;	}

	long long num_vectors(void) const {
		long long result 	= 1;
		long long was		= result;
		for (int i = m_width; i > 1; i--) {
			result *= i;
			// if an overflow occurred, return max
			if (result < was) {
				return std::numeric_limits<long long>::max();
			}
			was = result;
		}
		return result;
	}

	void reset() {
		if (m_initialized) {
			reset_state();
		}
	}

	//	returns the state of m_done **AFTER** state variables are advanced
	bool next(T*dst) {
		translate_state(dst);
		if (m_width > 1) {
			return advance_state();
		} else {
			m_done = true;
		}
		return m_done;
	}
};

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

	template <typename WRAPPER, typename DATA_TYPE>
	array_size_t generateReferenceTestVector(WRAPPER *dst, array_size_t size,
											 ArrayComposition& composition,
											 DATA_TYPE &first_value,
											 DATA_TYPE &last_value,
											 void (*next_value)( DATA_TYPE& crnt,
													 	 	 	 DATA_TYPE& frst,
																 DATA_TYPE& last))
	{
		DATA_TYPE value = first_value;
		switch (composition.composition) {
		case ArrayCompositions::ALL_DISCRETE:
		case ArrayCompositions::ALL_PERMUTATIONS:
			{
				for (array_size_t i = 0; i != size; i++) {
					dst[i] = value;
					next_value(value, first_value, last_value);
				}
			}
			break;
		case ArrayCompositions::FEW_DISTINCT:
			{
				array_size_t counts_per_value =
						size / composition.num_distinct_values;
				if (size % composition.num_distinct_values)
					counts_per_value++;
				for (array_size_t i = 0, value_counter = counts_per_value;
								  i < size; i++) {
					dst[i] = value;
					if (--value_counter == 0) {
						value_counter = counts_per_value;
						next_value(value, first_value, last_value);
					}
				}
			}
			break;
		case ArrayCompositions::FEW_DIFFERENT:
			{
				array_size_t same_counter = size - composition.num_different;
				same_counter--;	// because the counter is NOT a predecrement

				for (array_size_t i = 0; i != size; i++) {
					dst[i] = value;
					if (same_counter) {
						same_counter--;
					} else {
						next_value(value, first_value, last_value);
					}
				}
			}
			break;
		case ArrayCompositions::ALL_SAME:
			for (array_size_t i = 0; i != size; i++) {
				dst[i] = first_value;
			}
			break;
		case ArrayCompositions::INVALID:
		default:
			break;
		}
		return size;
	}

}	// namespace GenerateTestVectors



#endif /* GENERATETESTVECTORS_H_ */
