/*
 * BlockSortDataTypes.h
 *
 *  Created on: Sep 22, 2025
 *      Author: joe
 */

#ifndef BLOCKSORTDATATYPES_H
#define BLOCKSORTDATATYPES_H

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <memory>

#include "SortingPracticeDataTypes.h"

/*	MACROS FOR ENABLING / DISABLING ALGORITHMS	*/
#undef ENABLE_CREATE_BLOCK_DESCIRPTORS_SYMMETRICALLY

using index_t = array_size_t;

namespace BlockSort {
	template <typename T>
	class BlockDescriptor;

	template <typename T>
	using Descriptors = std::unique_ptr<BlockDescriptor<T>[]>;
}


	/*	**********************************************************	*/
	/*							BlockType							*/
	/*	**********************************************************	*/

	namespace BlockSort {
	enum class BlockType {
		A_BLOCK,
		B_BLOCK,
		UNSPECIFIED
	};
	}
	//	I do this to force the compiler to error if the type
	//	is instantiated without the name space
	namespace std{
	enum class BlockType {
		A_BLOCK,
		B_BLOCK,
		UNSPECIFIED
	};
	}
	#define A_BLOCK_CHAR 'A'
	#define B_BLOCK_CHAR 'B'
	#define U_BLOCK_CHAR 'U'

	namespace std {
		char to_char(BlockSort::BlockType type);
		ostream& operator<<(ostream& out, BlockSort::BlockType object);
	}

	/*	**********************************************************	*/
	/*							BlockOrganizations					*/
	/*	**********************************************************	*/

	namespace BlockSort {
	enum class BlockOrganizations {
		FULL_A0_BLOCK,
		SYMMETRIC
	};
	}
	//	I do this to force the compiler to error if the type
	//	is instantiated without the name space
	namespace std {
	enum class BlockOrganizations {
		FULL_A0_BLOCK,
		SYMMETRIC
	};
	}

	#define	BLOCK_ORGANIZATION_FULL_A0_BLOCK_STRING	"FULL_A0_BLOCK"
	#define BLOCK_ORGANIZATION_SYMMETRIC_STRING		"SYMMETRIC"
	#define	BLOCK_ORGANIZATION_UNKNOWN_STRING		"UNKNOWN"
	#define	BLOCK_ORGANIZATION_MAX_STRING_LENGTH	13

	namespace std {
		string to_string(BlockSort::BlockOrganizations organization);
		ostream& operator<<(ostream& out, const BlockSort::BlockOrganizations organization);
	}

	/*	**********************************************************	*/
	/*							MergeStrategy						*/
	/*	**********************************************************	*/

	namespace BlockSort {
	enum class MergeStrategy {
		 TABLE, AUXILLIARY, ROTATE
	};
	}

	#define MERGE_STRATEGY_TABLE_STRING			"TABLE"
	#define MERGE_STRATEGY_AUXILLIARY_STRING	"AUXILLIARY"
	#define MERGE_STRATEGY_ROTATE_STRING		"ROTATE"
	#define MERGE_STRATEGY_UNKNOWN_STRING		"UNKNOWN"
	#define MERGE_STRATEGY_MAX_STRING_LENGTH	10

	namespace std{
		string to_string(BlockSort::MergeStrategy strategy);
		ostream& operator<<(ostream& out, const BlockSort::MergeStrategy strategy);
	}

	namespace std {
	enum class MergeStrategy {
		TABLE, AUXILLIARY, ROTATE
	};
	}

	/*	**********************************************************	*/
	/*						SortingStrategy							*/
	/*	**********************************************************	*/

	namespace BlockSort {
	enum class SortingStrategy {
		BINARY,
		HYBRID,
		RIGHT_TO_LEFT,
		TABLE
	};
	}
	//	I do this to force the compiler to error if the type
	//	is instantiated without the name space
	namespace std {
	enum class SortingStrategy {
		BINARY,
		HYBRID,
		RIGHT_TO_LEFT,
		TABLE
	};
	}

	#define SORTING_STRATEGY_BINARY_STRING			"BINARY"
	#define SORTING_STRATEGY_HYBRID_STRING			"HYBRID"
	#define SORTING_STRATEGY_RIGHT_TO_LEFT_STRING	"RIGHT_TO_LEFT"
	#define SORTING_STRATEGY_TABLE_STRING			"TABLE"
	#define SORTING_STRATEGY_MAX_STRING_LENGTH		13

	namespace std {
		string to_string(BlockSort::SortingStrategy strategy);
		ostream& operator<<(ostream& out, const BlockSort::SortingStrategy organization);
	}

namespace BlockSort {
	template <typename T>
	class BlockDescriptor;
}

#endif
