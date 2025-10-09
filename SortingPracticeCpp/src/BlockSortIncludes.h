/*
 * BlockSortIncludes.h
 *
 *  Created on: Oct 7, 2025
 *      Author: joe
 */

#ifndef BLOCKSORTINCLUDES_H_
#define BLOCKSORTINCLUDES_H_

#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <cstdlib>

#include "BlockSortDataTypes.h"
#include "BlockSortBlockDescriptors.h"
#include "BlockSortElementMoving.h"
#include "BlockSortBlockMoving.h"
#include "BlockSortBlockSortAndMerge.h"
#include "BlockSortDebugging.h"

/*
 *	  When merging blocks, it is possible that the ascending values
 *	in each individual block will be interleaved:	A: { 0, 2, 5 }, B { 1, 3, 4 }
 *	It is necessary to identify where elements in the B Block need to go in A Block.
 *
 *	  Once the location of the B element(s) in A have been found, a rotate is used
 *	to move the B elements left-wards
 *
 *	Binary search uses a binary search to find the location of each B element in A
 *	Right to Left starts with the largest value in B and moves leftward
 *	Hybrid uses a binary search for the first location, the moves right to left
 *	  for each successive element in B
 */
constexpr bool MERGE_BLOCKS_WITH_BINARY_SEARCH 	= false;
constexpr bool MERGE_BLOCKS_WITH_RIGHT_TO_LEFT	= true;
constexpr bool MERGE_BLOCKS_WITH_HYBRID			= false;

/*
 * 	  The above discussion of ordering elements within pairs of blocks also applies
 * 	to the method of ordering blocks in the array.
 */
constexpr bool SORT_BLOCKS_WITH_BINARY_SEARCH	= false;
constexpr bool SORT_BLOCKS_WITH_RIGHT_TO_LEFT	= true;
constexpr bool SORT_BLOCKS_WITH_HYBRID			= false;

#endif /* BLOCKSORTINCLUDES_H_ */
