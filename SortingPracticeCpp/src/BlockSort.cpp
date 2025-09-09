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
 * 	converts negative rotation counts (rotate left) into
 * 		an equivalent positive (rotate right)
 * 	ensures that the rotate count is [0:span) to avoid
 * 		excessive rotations
 */

index_t blockSortModulo(index_t rotation_count, index_t span) {
	while (rotation_count < 0) {
		rotation_count += span;
	}
	rotation_count %= span;
	return rotation_count;
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
 */

index_t floorLog2(index_t num) {
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


/*	**********************************************************************	*/
/*	**********************************************************************	*/
/*								debugging resources							*/
/*	**********************************************************************	*/
/*	**********************************************************************	*/

template <typename T>
bool areTagsSorted(std::unique_ptr<BlockDescriptor<T>[]> &tags, int num_tags) {

	for (int i = 0; i != num_tags-1; i++) {
		if (tags[i+1] < tags[i])
			return false;
	}
	return true;
}

std::string arrayIndicesToString(array_size_t size, array_size_t v, int element_width) {

	OStreamState io_state;	// the destructor will restore state
	std::stringstream result;
	if (size != 0) {
		for (int i = 0; i < size-1; i++) {
			if (i != v) {
				result << std::right << std::setw(element_width-1) << i;
				result << ' ';
			} else {
				result << std::setw(element_width) << "\\V/";
			}
		}
		result << std::right << std::setw(element_width-1) << size-1;
	}
	return result.str();
}

std::string arrayIndicesToString(std::string trailer, array_size_t size, array_size_t v, int element_width) {
	OStreamState ostream_state;
	std::stringstream result;
	result << arrayIndicesToString(size, v, element_width);
	result << trailer;
	return result.str();
}

std::string arrayIndicesToString(array_size_t size, int value_width, int element_width) {

	std::stringstream result;

	if (size != 0) {
		constexpr const char separator = ' ';
		OStreamState io_state;	// the destructor will restore state
		std::cout.fill(separator);
		std::cout << std::right;

		for (int i = 0; i < size-1; i++) {
			result << std::setw(element_width-1) << i;
			result << ' ';
		}
		result << std::setw(element_width-1) << size-1;
	}
	return result.str();
}

std::string printArrayIndices(std::string trailer, array_size_t size, int value_width, int element_width) {
	std::stringstream result;
	result << arrayIndicesToString(size, value_width, element_width);
	result << trailer;
	return result.str();
}

std::string printLineArrayIndices(array_size_t size, int value_width, int element_width) {
	std::stringstream result;
	result << arrayIndicesToString(size, value_width, element_width);
	result << std::endl;
	return result.str();
}

std::string printArrayStartMiddleEnd(array_size_t size, array_size_t start, array_size_t mid, array_size_t end, int element_width) {
	OStreamState state;	// destructor will restore ostream state
	std::stringstream result;

	for (int i = 0; i < start ; i++) {
		result << std::setw(element_width) << ' ';
	}
	result << std::setw(element_width-1) << 's' << ' ';
	for (int i = start+1; i < mid; i++) {
		result << std::setw(element_width) << ' ';
	}
	result << std::setw(element_width-1) << 'm' << ' ';
	for (int i = mid+1; i < end;  i++) {
		result << std::setw(element_width) << ' ';
	}
	result << std::setw(element_width-1) << 'e' << ' ';
	return result.str();
}

std::string printLineArrayStartMiddleEnd(array_size_t size, array_size_t start, array_size_t mid, array_size_t end, int element_width) {
	std::stringstream result;
	result << printArrayStartMiddleEnd(size, start, mid, end, element_width);
	result << std::endl;
	return result.str();
}

