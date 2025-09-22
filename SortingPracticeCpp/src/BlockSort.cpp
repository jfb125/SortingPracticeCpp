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
