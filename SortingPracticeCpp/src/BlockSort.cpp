/*
 * BlockSortTest.cpp
 *
 *  Created on: Aug 14, 2025
 *      Author: joe
 */

#include <iostream>
#include <iomanip>
#include "BlockSort.h"

using namespace BlockSort;

#define TEST_BLOCK_SORT_FLOOR_LOG_2
#define TEST_BLOCK_SORT_ROTATE_ARRAY

#define runTest(func, name_str) do {\
	if (!func()) {\
		std::cout << "ERROR: " << name_str << " returns FALSE" << std::endl;\
	} else {\
		std::cout << "PASS:  " << name_str << " returns TRUE" << std::endl;\
	}\
} while(false)


/*	**********************************************	*/
/*	**********************************************	*/
/*					forward declarations			*/
/*	**********************************************	*/
/*	**********************************************	*/

bool testFloorLog2();
bool testRotateArray();


/*	**********************************************	*/
/*	**********************************************	*/
/*					testBlockSort()					*/
/*	**********************************************	*/
/*	**********************************************	*/

void testBlockSort() {
	std::cout << "testBlockSort()" << std::endl;

#ifdef TEST_BLOCK_SORT_FLOOR_LOG_2
	runTest(testFloorLog2, "function floorLog2()");
#endif

#ifdef	TEST_BLOCK_SORT_ROTATE_ARRY
	runTest(testRotateArray, "function rotateArray()");
	testRotateArray();
#endif

	constexpr index_t array_size = 8;
	char *test_array[array_size] = {
			new char('a'), new char('b'), new char('c'), new char('d'),
			new char('e'), new char('f'), new char('g'), new char('h') };

	ComparesAndMoves result;
	for (index_t sub_array_size = array_size; sub_array_size >= array_size-1; sub_array_size-- ) {
		for (index_t i = -sub_array_size; i <= 2*sub_array_size; i++) {
			char *dut[sub_array_size];
			for (index_t j = 0; j != sub_array_size; j++) {
				dut[j] = test_array[j];
			}
			result = rotateArray<char>(dut, i, 0, sub_array_size-1);
			std::cout << "rotate(" << std::setw(3) << i << ") ";
			for (int j = 0; j != sub_array_size; j++) {
				std::cout << *test_array[j];
			}
			std::cout << " yields ";
			for (int j = 0; j != sub_array_size; j++) {
				std::cout << *dut[j];
			}
			std::cout << " took " << std::setw(3) << result._moves << " moves";
			std::cout << std::endl;
		}
	}
}


/*	**********************************************	*/
/*	**********************************************	*/
/*					individual tests				*/
/*	**********************************************	*/
/*	**********************************************	*/

bool testFloorLog2() {

	for (index_t i = 1; i < (1<<30); i <<= 1) {
		std::cout << "floor(log2(" << i-1 << ")] = " << floorLog2(i-1) << std::endl;
		std::cout << "floor(log2(" << i << ")] = " << floorLog2(i) << std::endl;
	}
	return true;
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
				goto TEST_ROTATE_ARRAY_RETURN_POINT;
				break;
			}
		}
	}

TEST_ROTATE_ARRAY_RETURN_POINT:
	return test_passed;
}

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
	num |= (num>>1);
	num |= (num>>2);	// produces a nibble
	num |= (num>>4);	// produces a byte
	num |= (num>>8);	// produces 2 bytes
	num |= (num>>16);	// produces 4 bytes
//		num |= (num>>32);	// produces 8 bytes
	return num - (num>>1);
}

