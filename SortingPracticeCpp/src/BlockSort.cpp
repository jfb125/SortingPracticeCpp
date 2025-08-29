/*
 * 	BlockSortTest.cpp
 *
 *  Created on: Aug 14, 2025
 *      Author: joe
 */

#include <iostream>
#include <iomanip>
#include <string>
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
		return A_BLOCK_CHAR;
	case BlockType::B_BLOCK:
		return B_BLOCK_CHAR;
	case BlockType::UNSPECIFIED:
		return U_BLOCK_CHAR;
	default:
		return '?';
	}
}

std::ostream& operator<<(std::ostream& out, BlockType object) {
	out << to_char(object);
	return out;
}

/*
 * blockSortModulo(element_rotation_amount, span)
 *
 * 	this converts values of 'element_rotation_amount' that are either
 * 		negative of greater than span into an amount < span
 */

array_size_t blockSortModulo(array_size_t rotation_count, array_size_t span) {

	while (rotation_count < 0) {
		rotation_count += span;
	}
	rotation_count %= span;
	return rotation_count;
}

