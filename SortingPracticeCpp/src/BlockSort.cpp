/*
 * BlockSortTest.cpp
 *
 *  Created on: Aug 14, 2025
 *      Author: joe
 */

#include <iostream>
#include <iomanip>
#include <numeric>
#include <cmath>

#include "BlockSort.h"

using namespace BlockSort;

/*	******************************************************	*/
/*	******************************************************	*/
/*							functions						*/
/*	******************************************************	*/
/*	******************************************************	*/

/*
 * index_size_t floorLog2(index_size_t num);
 *
 * 	returns the logarithm to the base two of
 * 	  the largest power of 2 that is <= to num
 *
 * 	  floorLog2(63) returns 32
 * 	  floorLog2(64) returns 64
 *
 *	algorithm:
 *		sign extend the msbit to the right
 *	      by ORing in a shifted version of num
 *	    mask off the lower bits by subtracting
 *	      a shifted>>1 (/2) version of the sign extended
 *
 *	  'b01_0000 | 'b00_1000	= 'b01_1000
 *	  'b01_1000 | 'b00_0110 = 'b01_1110
 *	  'b01_1110 | 'b00_0001 = 'b01_1111
 *	  'b01_1111 - 'b00_1111 = 'b01_0000
 *
 */

array_size_t floorLog2(array_size_t num) {
	if (num < 0) {
		// TODO - throw error
		return 0;
	}
	num |= (num>>1);
	num |= (num>>2);	// produces a nibble
	num |= (num>>4);	// produces a byte
	num |= (num>>8);	// produces 2 bytes
	num |= (num>>16);	// produces 4 bytes
//		num |= (num>>32);	// produces 8 bytes
	return num - (num>>1);
}

char to_char(BlockType type) {
	switch(type) {
	case BlockType::A_BLOCK:
		return 'A';
	case BlockType::B_BLOCK:
		return 'B';
	case BlockType::UNSPECIFIED:
	default:
		return '?';
	}
}

	//	handles negative amounts and amounts > span
amount_t calculateRotationAmount(amount_t amount, amount_t span)  {
	amount_t result = amount;
	while (result < 0)
		result += span;
	result %= span;
	return result;
}


/*	**********************************************************************	*/
/*	**********************************************************************	*/
/*								debugging resources							*/
/*	**********************************************************************	*/
/*	**********************************************************************	*/

void printArrayIndices(array_size_t size, int value_width, int element_width) {

	OStreamState io_state;	// the destructor will restore state
	std::cout.fill('.');
	std::cout << std::right;

	for (int i = 0; i != size; i++) {
		std::cout << std::setw(element_width) << i;
	}
}

void printArrayIndices(std::string trailer, array_size_t size, int value_width, int element_width) {
	printArrayIndices(size, value_width, element_width);
	std::cout << trailer;
}

void printLineArrayIndices(array_size_t size, int value_width, int element_width) {
	printArrayIndices(size, value_width, element_width);
	std::cout << std::endl;
}

/*	**********************************************	*/
/*	**********************************************	*/
/*		forward declaration of test functions		*/
/*	**********************************************	*/
/*	**********************************************	*/

//#define TEST_MODULO
//#define TEST_BLOCK_SORT_FLOOR_LOG_2
//#define TEST_BLOCK_SORT_ROTATE_ARRAY
#define TEST_BLOCK_MERGE
//#define	TEST_BLOCK_SORT_SORT

bool testFloorLog2();
bool testRotateArray();
bool testBlockMerge();
bool testBlockSortSort();

#define runTest(result, func, name_str) do {\
	if (!func()) {\
		std::cout << "ERROR: " << name_str << " returns FALSE" << std::endl;\
		result = false;\
	} else {\
		std::cout << "PASS:  " << name_str << " returns TRUE" << std::endl;\
	}\
} while(false)


/*	**********************************************	*/
/*	**********************************************	*/
/*					testBlockSort()					*/
/*	**********************************************	*/
/*	**********************************************	*/

bool testBlockSort() {
	bool passed = true;
	std::cout << "testBlockSort()" << std::endl;

#ifdef TEST_MODULO
	for (array_size_t span = 7; span <= 9; span++) {
		for (array_size_t i = -9; i <= 9; i++) {
			array_size_t expected = calculateRotationAmount(i, span);
			std::cout << std::setw(2) << i << " % " << span << " = " << expected <<  " | ";
		}
		std::cout << std::endl;;
	}
#endif

#ifdef TEST_BLOCK_SORT_FLOOR_LOG_2
	runTest(passed, testFloorLog2, "function floorLog2()");
	if (!passed)
		return passed;
#endif

#ifdef	TEST_BLOCK_SORT_ROTATE_ARRAY
	runTest(passed, testRotateArray, "function rotateArray()");
	if (!passed)
		return passed;
#endif

#ifdef TEST_BLOCK_MERGE
	runTest(passed, testBlockMerge, "function testBlockMerge()");
#endif

#ifdef	TEST_BLOCK_SORT_SORT
	runTest(passed, testBlockSortSort, "function testBlockSortSort()");
	if (!passed)
		return passed;
#endif
	return true;
}


/*	**********************************************	*/
/*	**********************************************	*/
/*					individual tests				*/
/*	**********************************************	*/
/*	**********************************************	*/

bool testFloorLog2() {

	//	a way of calculating that uses a loop,
	//		which is slow due to branching
	auto calc_expected = [] (index_t num) {
		if (num == 0)
			return 0;
		for (int i = 1; i <= 1<<30; i *=2) {
			if (i == num) {
				return i;
			}
			if (i > num) {
				return i>>1;
			}
		}
		return 1<<30;
	};

	constexpr int num_width = 10;
	bool passed = true;
	index_t value = floorLog2(0);
	index_t expected = calc_expected(0);
	std::cout << "floor(log2(" << std::setw(num_width) << 0 << ")) = "
			  << std::setw(num_width) << value;
	if (value != expected) {
		std::cout << " does not match expected " << std::setw(num_width) << expected << std::endl;
		passed = false;
		goto TEST_FLOOR_LOG_2_RETURN_LABEL;
	}
	std::cout << std::endl;

	for (index_t i = 2; i < (1<<30); i <<= 1) {
		// one less than a power of 2
		value = floorLog2(i-1);
		expected = calc_expected(i-1);
		std::cout << "floor(log2(" << std::setw(num_width) << i-1 << ")) = "
				  << std::setw(num_width) << value;
		if (value != expected) {
			std::cout << " does not match expected " << std::setw(num_width) << expected << std::endl;
			passed = false;
			goto TEST_FLOOR_LOG_2_RETURN_LABEL;
		}
		std::cout << std::endl;

		// a power of 2
		value = floorLog2(i);
		expected = calc_expected(i);
		std::cout << "floor(log2(" << std::setw(num_width) << i << ")) = "
				  << std::setw(num_width) << value;
		if (value != expected) {
			std::cout << " does not match expected " << std::setw(num_width) << expected << std::endl;
			passed = false;
			goto TEST_FLOOR_LOG_2_RETURN_LABEL;
		}
		std::cout << std::endl;
	}
TEST_FLOOR_LOG_2_RETURN_LABEL:
	return passed;
}

template <typename T>
void rotateArrayLongWay(T**array, index_t size, index_t count) {

	T* tmp;

	if (count < 0) {
		for (index_t i = count ; i != 0; i++) {
			tmp = array[0];
			for (index_t j = 0; j != size-1; j++) {
				array[j] = array[j+1];
			}
			array[size-1] = tmp;
		}
	} else {
		for (index_t i = 0; i != count; i++) {
			tmp = array[size-1];
			for (index_t j = size-1; j != 0; j--) {
				array[j] = array[j-1];
			}
			array[0] = tmp;
		}
	}
}

bool testRotateArray() {

	bool test_passed = true;
	ComparesAndMoves rotate_result;
	constexpr index_t array_size = 8;
	char *test_array[array_size] = {
			new char('a'), new char('b'), new char('c'), new char('d'),
			new char('e'), new char('f'), new char('g'), new char('h') };

	// test an array of even size and an array of odd size
	for (index_t sub_array_size = array_size; sub_array_size >= array_size-1; sub_array_size-- ) {
		// test different rotate amounts, left and right,
		//	from -2*sub_array_size - 1 (left) to +2*sub_array_size + 1 (right)
		for (index_t i = -sub_array_size - 1; i <= 2*sub_array_size + 1; i++) {
			// 	create an array to test rotation and an array of expecteds
			char *rotated_array[sub_array_size];
			char *expected_array[sub_array_size];
			for (index_t j = 0; j != sub_array_size; j++) {
				rotated_array[j] = test_array[j];
				expected_array[j]= test_array[j];
			}
			rotateArrayLongWay(expected_array, sub_array_size, i);
			rotate_result = rotateArray<char>(rotated_array, i, 0, sub_array_size-1);
			//	check results
			for (int i = 0, j = sub_array_size-1; i < sub_array_size; i++, j--) {
				if (rotated_array[j] != expected_array[j]) {
					test_passed = false;
					break;
				}
			}
			std::cout << (test_passed ? "PASSED " : "FAILED ");
			//	print out before / after
			std::cout << "rotate(" << std::setw(3) << i << ") \"";
			for (int j = 0; j != sub_array_size; j++) {
				std::cout << *test_array[j];
			}
			std::cout << "\" yields \"";
			for (int j = 0; j != sub_array_size; j++) {
				std::cout << *rotated_array[j];
			}
			std:: cout << "\" vs expected \"";
			for (int j = 0; j != sub_array_size; j++) {
				std::cout << *expected_array[j];
			}
			std::cout << "\" took:" << std::setw(3) << rotate_result._moves << " moves";
			std::cout << std::endl;

			if (!test_passed) {
				break;
			}
		}
		if (!test_passed) {
			break;
		}
	}

	return test_passed;
}


bool testBlockMerge() {
#ifdef BLOCK_MERGE_BY_ROTATE
	std::cout << "test blockMerge with the rotate algorithm" << std::endl;
#endif
	bool test_passed = true;
	SimpleRandomizer randomizer;

	void (*printArray)(char**, int) = [] (char **l_array, int l_size){
		std::cout << "\"";
		for (int i = 0; i != l_size; i++) {
			std::cout << *l_array[i];
		}
		std::cout << "\"";
	};

	ComparesAndMoves (*sortArray)(char**, int, int) = [] (char **l_array, int l_start, int l_end) {
		ComparesAndMoves result(0,0);
		for (int i = l_start+1; i <= l_end; i++) {
			for (int j = i; j != l_start; j--) {
				result._compares++;
				if (*l_array[j-1] > *l_array[j]) {
					char *tmp = l_array[j-1];
					l_array[j-1] = l_array[j];
					l_array[j] = tmp;
					result._moves += 3;
					result._compares++;
				} else {
					break;
				}
			}
		}
		return result;
	};

	int min_array_size = 2;
	int max_array_size = 128;

	for (int array_size = min_array_size; array_size <= max_array_size; array_size *= 2) {

		bool verbose = false;
		ComparesAndMoves total_result(0,0);
		int num_tests = 100;

		char *array[array_size];
		char *before[array_size];
		char *expected[array_size];

		for (int test_number = 0; test_number != num_tests; test_number++) {

			ComparesAndMoves result(0,0);
			//	create a linear array
			for (int i = 0; i != array_size; i++) {
				char value = 'a'+(i%26);
				array[i] = new char(value);
				expected[i] = new char(value);
			}

			sortArray(expected, 0, array_size-1);

			//	randomize the array
			for (int i = 0; i != array_size; i++) {
				int r = randomizer.rand(i, array_size);
				char *tmp = array[i];
				array[i] = array[r];
				array[r] = tmp;
			}

			for (int i = 0; i != array_size; i++) {
				before[i] = new char(*array[i]);
			}

			//	sort each subarray, u & v, using an insertion sort
			result += sortArray(array, 0, array_size / 2-1);
			result += sortArray(array, array_size / 2, array_size-1);

			if (verbose) {
				std::cout << "  before: ";
				printArray(before, array_size);
			}

#ifdef BLOCK_MERGE_BY_ROTATE
			result += blockMergeByRotate(array, 0, array_size/2, array_size-1);
#endif
			total_result += result;

			if (verbose) {
				std::cout << " after: ";
				printArray(array, array_size);
				std::cout << " used: " << result;
			}

			for (int i = 0; i != array_size; i++) {
				if (*array[i] != *expected[i]) {
					test_passed = false;
					std::cout << "array size " << array_size
							  << " test pass " << test_number
							  << " before ";
					printArray(before, array_size);
					std::cout << " expected ";
					printArray(expected, array_size);
					std::cout << " actual ";
					printArray(array, array_size);
					std::cout << std::endl;
					std::cout << " FAILED: [" << std::setw(3) << i << "]"
							  << " expected " << expected[i]
							  << " vs actual " << *array[i]
							  << std::endl;
					goto TEST_BLOCK_MERGE_RETURN_LABEL;
				}
			}
			if (verbose)
				std::cout << std::endl;
		}
		std::cout << "  Array size " << array_size << " nlog2(n)(array_size) = "
				  << array_size * std::log2(array_size)
				  << " averages "
				  << static_cast<double>(total_result._compares) / num_tests
				  << " compares and "
				  << static_cast<double>(total_result._moves) / num_tests
				  << " moves" << std::endl;
	}

TEST_BLOCK_MERGE_RETURN_LABEL:
	return test_passed;
}

bool testBlockSortSort() {
	bool passed = true;
	ComparesAndMoves sort_result;
	constexpr index_t array_size = 35;
	index_t v = 16 ; // /2
	int *test_array[array_size];

	for (int i = 0, val = 0; i < v; val += 2, i++) {
		test_array[i] = new int(val);
	}
	for (int i = v, val = 1; i < array_size; val += 2, i++)  {
		test_array[i] = new int(val);
	}

	int element_width = 4;
	int value_width = 3;

	printArrayIndices(std::string("\n"), array_size, value_width, element_width);
	std::cout << std::endl;
	BlockSort::printElements(std::string(" initially\n"), test_array, array_size, value_width, element_width);
	SortingUtilities::randomizeArray(test_array, array_size);
	BlockSort::printElements(std::string(" randomized\n"), test_array, array_size, value_width, element_width);
	BlockSort::sortPointersToObjects(test_array, array_size);
	BlockSort::printElements(std::string(" after sorting\n"), test_array, array_size, value_width, element_width);
	goto TEST_BLOCK_SORT_SORT_RETURN_LABEL;

TEST_BLOCK_SORT_SORT_RETURN_LABEL:
	return passed;
}
