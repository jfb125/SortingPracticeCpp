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

/*	******************************************************	*/
/*	******************************************************	*/
/*					Algorthmic Functions					*/
/*	******************************************************	*/
/*	******************************************************	*/

/*	******************************************************	*/
/*	******************************************************	*/
/*					Formatted Output of enums				*/
/*	******************************************************	*/
/*	******************************************************	*/

namespace std
{
	char to_char(BlockSort::BlockType type) {
		switch(type) {
		case BlockSort::BlockType::A_BLOCK:
			return A_BLOCK_CHAR;
		case BlockSort::BlockType::B_BLOCK:
			return B_BLOCK_CHAR;
		case BlockSort::BlockType::UNSPECIFIED:
			return U_BLOCK_CHAR;
		default:
			return '?';
		}
	}

	std::ostream& operator<<(std::ostream& out, BlockSort::BlockType object) {
		out << to_char(object);
		return out;
	}
}


namespace std
{
	string to_string(BlockSort::BlockOrganizations organization) {
		switch(organization) {
		case BlockSort::BlockOrganizations::FULL_A0_BLOCK:
			return string(BLOCK_ORGANIZATION_FULL_A0_BLOCK_STRING);
		case BlockSort::BlockOrganizations::SYMMETRIC:
			return string(BLOCK_ORGANIZATION_SYMMETRIC_STRING);
		default:
			return string(BLOCK_ORGANIZATION_UNKNOWN_STRING);
		}
	};

	ostream& operator<<(ostream& out, const BlockSort::BlockOrganizations organization) {
		out << to_string(organization);
		return out;
	}
}


namespace std
{
	string to_string(BlockSort::BlockSortingStrategy strategy) {
		switch(strategy) {
		case BlockSort::BlockSortingStrategy::BINARY:
			return string(SORTING_STRATEGY_BINARY_STRING);
		case BlockSort::BlockSortingStrategy::HYBRID:
			return string(SORTING_STRATEGY_HYBRID_STRING);
		case BlockSort::BlockSortingStrategy::RIGHT_TO_LEFT:
			return string(SORTING_STRATEGY_RIGHT_TO_LEFT_STRING);
		case BlockSort::BlockSortingStrategy::TABLE:
			return string(SORTING_STRATEGY_TABLE_STRING);
		default:
			return string("UNRECOGNZED_SORTING_STRATEGY");
		}
	}

	std::ostream& operator<<(std::ostream& out, BlockSort::BlockSortingStrategy strategy) {
		out << to_string(strategy);
		return out;
	}
}

