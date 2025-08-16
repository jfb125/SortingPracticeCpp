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

/*	**********************************************	*/
/*	**********************************************	*/
/*		forward declaration of test functions		*/
/*	**********************************************	*/
/*	**********************************************	*/

//#define TEST_BLOCK_SORT_FLOOR_LOG_2
//#define TEST_BLOCK_SORT_ROTATE_ARRAY
#define	TEST_BLOCK_SORT_SORT

bool testFloorLog2();
bool testRotateArray();
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
		//	from -2*sub_array_size (left) to +2*sub_array_size (right)
		for (index_t i = -sub_array_size; i <= 2*sub_array_size; i++) {
			// 	create an array to rotate
			char *rotated_array[sub_array_size];
			for (index_t j = 0; j != sub_array_size; j++) {
				rotated_array[j] = test_array[j];
			}
			//	rotate the array
			rotate_result = rotateArray<char>(rotated_array, i, 0, sub_array_size-1);
			// determine if array is reversed
			bool array_is_reversed = true;
			for (int i = 0, j = sub_array_size-1; i < sub_array_size; i++, j--) {
				if (test_array[i] != rotated_array[j]) {
					array_is_reversed = false;
					break;
				}
			}
			std::cout << (array_is_reversed ? "PASSED " : "FAILED ");
			//	print out before / after
			std::cout << "rotate(" << std::setw(3) << i << ") ";
			for (int j = 0; j != sub_array_size; j++) {
				std::cout << *test_array[j];
			}
			std::cout << " yields ";
			for (int j = 0; j != sub_array_size; j++) {
				std::cout << *rotated_array[j];
			}
			std::cout << " took " << std::setw(3) << rotate_result._moves << " moves";
			std::cout << std::endl;
			if (!array_is_reversed) {
				test_passed = false;
				goto TEST_ROTATE_ARRAY_RETURN_LABEL;
				break;
			}
		}
	}

TEST_ROTATE_ARRAY_RETURN_LABEL:
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

	printArrayIndices(array_size, value_width, element_width);
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
