/*
 * BlockOperations.cpp
 *
 *  Created on: Oct 13, 2025
 *      Author: joe
 */

#include "BlockOperations.h"
namespace std
{
	string to_string(BlockOperations::MergeStrategy strategy) {
		switch (strategy) {
		case BlockOperations::MergeStrategy::AUXILLIARY:
			return string(MERGE_STRATEGY_AUXILLIARY_STRING);
		case BlockOperations::MergeStrategy::BINARY:
			return string(MERGE_STRATEGY_BINARY_STRING);
		case BlockOperations::MergeStrategy::HYBRID:
			return string(MERGE_STRATEGY_HYBRID_STRING);
		case BlockOperations::MergeStrategy::INSERTION:
			return string(MERGE_STRATEGY_INSERTION_STRING);
		case BlockOperations::MergeStrategy::RGT_TO_LFT:
			return string(MERGE_STRATEGY_ROTATE_STRING);
		case BlockOperations::MergeStrategy::TABLE:
			return string(MERGE_STRATEGY_TABLE_STRING);
		default:
			return string(MERGE_STRATEGY_UNKNOWN_STRING);
		};
	}

	std::ostream& operator<<(std::ostream& out, const BlockOperations::MergeStrategy strategy) {
		out << std::to_string(strategy);
		return out;
	}
}





