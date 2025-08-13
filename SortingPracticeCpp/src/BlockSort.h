/*
 * BlockSort.h
 *
 *  Created on: Aug 11, 2025
 *      Author: joe
 */
/*
 * BubbleSort.h
 *
 *  Created on: Jun 18, 2025
 *      Author: joe
 */

#ifndef BLOCKSORT_H_
#define BLOCKSORT_H_

#include "SortingPracticeDataTypes.h"
#include "SortingUtilities.h"

namespace BlockSort {
using index_t = array_size_t;
using amount_t	= array_size_t;

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

	template <typename T>
	index_t floorLog2(index_t num) {
		num |= (num>>1);
		num |= (num>>2);	// produces a nibble
		num |= (num>>4);	// produces a byte
		num |= (num>>8);	// produces 2 bytes
		num |= (num>>16);	// produces 4 bytes
//		num |= (num>>32);	// produces 8 bytes
		return num - (num>>1);
	}


	/*
	 * 	void rotateArray(array, amount, start, end);
	 *
	 * 	rotate the contents of the array by 'amount' positions
	 * 	  where the array is defined as array[start]:array[end] (inclusive)
	 *
	 * 	algorithm:
	 * 		reverse the array
	 * 		reverse the sub array [start:amount-1]
	 * 		reverse the sub array [amount:end]
	 *
	 * 	note that amount can be negative or greater than the span
	 * 		negative amounts are converted into the equivalent positive amount
	 * 		amounts greater than the span are % to be within the span
	 */

	template <typename T>
	ComparesAndMoves rotateArray(T** array, amount_t amount, index_t start, index_t end) {

		ComparesAndMoves result(0,0);

		if (end <= start)
			return result;

		index_t span = end - start + 1;

		amount %= span;
		if (amount < 0) {
			amount = span+amount;
		}

		if (amount == 0)
			return result;

		T* temp;
		for (index_t i = start, j = end; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result._moves += 3;
		}

		for (index_t i = start, j = start+amount-1; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result._moves += 3;
		}

		for (index_t i = start + amount, j = end; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result._moves += 3;
		}
		return result;
	}

	/*
	 * ComparesAndMoves blockMerge(uv_array, size, v);
	 *
	 *   This uses rotate operations to put the { u[], v[n] } array in order
	 *   The requirement is that the sub portions of uv_array, u[] and v[]
	 *   	are in ascending order.
	 *        e.g. if  u[] = { 2, 3, 3, 8 } and v[] = { 1, 3, 7, 7 }
	 *      then uv_array[] = { 2, 3, 3, 8, 1, 3, 7, 7 }
	 *
	 *   The position of the first element in v, in this case [4] = 1,
	 *   	must be provided by the caller
	 *
	 *   The algorithm looks for the first element in v that is > u
	 *      the first element in u, then rotates
	 * 		{ 2, 3, 3, 8, 1, 3, 7, 7 }	[u=0] is 2, [v=4] is 1, count = 0
	 * 		{ 2, 3, 3, 8, 1, 3, 7, 7 }  [u=0] is 2, [v=5] is 3, count = 1
	 * 		arrayRotate(uv_array, count = 1, start = u = 0, end = v-1 = 4
	 * 		{ 1, 2, 3, 3, 8, 3, 7, 7 }
	 * 			u = u + count = 1
	 * 			search u for the first element != '2' which is [u=2] is 3
	 * 			search v for the first element < [u=2] is 3, [v=6] is 7
	 * 	    arrayRotate(uv_array, count = 1, start = 2, end = 7-1 = 6)
	 * 	    { 1, 2, 3, 3, 3, 8, 7, 7 }
	 * 	    	search u for the first element != 3 which is [u=5] is 8
	 * 	    	search v for the first element > 8 v stops at size
	 * 	    arrayRotate(uv_array, count = 2, start = 5, end = size-1)
	 * 	    { 1, 2, 3, 3, 3, 7, 7, 8 }
	 */

	template <typename T>
	ComparesAndMoves blockMerge(T** uv_array, index_t size, index_t v) {

		ComparesAndMoves result(0,0);

		index_t u = 0;
		amount_t rotate_count;

		while (1) {
			rotate_count = 0;
			T* u_value = uv_array[u];
			// search for an element in v that is greater than array[u]
			while (v < size && *uv_array[v++] <= *u_value) {
				result._compares++;
				rotate_count++;
			}
			//	rotate u to the right in the array
			//	  past the last value in v that was equal to u_value
			result += rotateArray(uv_array, rotate_count, u, v+rotate_count-1);

			if (v == size)
				break;

			//	the u portion of the array has been rotated 'rotate_count'
			//	  to the right in the array. All values to the left of
			//	  u are < or = to the start of the u segment
			//	  find the next value to the right in u that is >
			//	  the start of u
			u += rotate_count;
			//	advance u until a value is found that is greater than
			//	  the current u_value
			while (u < size && *uv_array[u] <= *u_value) {
				result._compares++;
				u++;
			}
			//	if there were no values found > the current u value,
			//	  no more rotates need to be performed -> done
			if (u == size)
				break;
		}
		return result;
	}

	template <typename T>
	ComparesAndMoves sortPointersToObjects(T **array_of_pointers, array_size_t size) {
		return *(new ComparesAndMoves(0,0));
	}
	template <typename T>
	void testBlockSort() {
		std::cout << "testBlockSort()" << std::endl;

//		for (index_size_t i = 1; i < (1<<30); i <<= 1) {
//			std::cout << "floor(log2(" << i-1 << ")] = " << floorLog2<char>(i-1) << std::endl;
//			std::cout << "floor(log2(" << i << ")] = " << floorLog2<char>(i) << std::endl;
//		}

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
}

#endif /* BLOCKSORT_H_ */
