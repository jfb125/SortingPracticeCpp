/*
 * BlockSort.h
 *
 *  Created on: Aug 11, 2025
 *      Author: Joe Baker
 */

/*	TODO - List
 * Change 'ComparesAndMoves' to 'ComparesAndAssignments'
 * Change every variable of tylpe 'ComparesAndMoves' to 'metrics'
 * Change every function that returns 'metrics' to access them by
 * 	a pointer passed the function parameters
 * Set the default value of the p_metrics to nullptr
 *
 * TODO - List
 * Include the version of insertionSort that uses a pointer to the leftmost
 * 	element that is known to be sorted in this namespace.
 * Use the local version of insertionSortFromN to do the initial insertion sort
 * In all comments showing Blocks under/above the array, make sure that the
 * 	boundaries of the blocks are denoted with '|', not '[]'
 *
 * 	TODO - Merging
 *
 *	Block Organization:
 *
 * 	The organizing for block structures, either symmetric or full_A0, has been
 * 	tested empirically to show a performance penalty of 5% in the sorting itself.
 *
 * 	Requiring the 'mid' point to be an integer multiple of the block size (full_A0)
 * 	complicates the divide & conquer by power of two algorithm.  The effort to
 * 	align the bottom up x2 algorithm with the full_A0 sorting of the blocks is
 * 	will not be supported for the time being.
 *
 *	Block Sorting:
 *
 *	Sorting of the blocks has been tested using BINARY_SEARCH, ROTATION, HYBRID
 *	and TABLE. TABLE performs twice as fast as the other algorithms and will thus
 *	be used from this point forward.
 *
 *	Merging Pairs of Blocks:
 *
 *	Merging pairs of blocks that are of equal size has been tested and found
 *	that using a TABLE is more than twice as fast as a method that uses
 *	binary searches to identify spans within the B_Block that can be rotated
 *	into place between to A_Block elements. The O(n) for the table method
 *	is n, whereas the O(n) for the binary search/rotate which is n^2.
 *	In the canonical implementation TABLE is used for the merge, where the
 *	table is implemented as	monotonically ascending sub-array of distinct values.
 *
 *	Merging all of the Blocks:
 *
 *	Merging all of the pairs of the blocks does not seem to be performing as
 *	expected.  This may be due to the fact that in the actual algorithm, the
 *	B_Block that must be merged is usually quite smaller than the 'Mega-Block'
 *	of previously merged values to its left.  Thus, the TABLE that is created
 *	of the Mega-block is always the largest table and thus the most moves.
 *	The next step will be to try to 	optimize TABLE merging to always build
 *	the TABLE based upon which block is	the smallest to see if this affects
 *	the number of moves necessary.
 */

#ifndef BLOCKSORT_H_
#define BLOCKSORT_H_

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

//#include "BlockSortDataTypes.h"
#include "BlockSortIncludes.h"
#include "IntegerArithmetic.h"
#include "SortingUtilities.h"
#include "SortingDebugOutput.h"
#include "InsertionSort.h"
//#include "BlockSortDebugging.h"
//#include "BlockSortBlockDescriptors.h"
//#include "BlockSortBlockMoving.h"
//#include "BlockSortElementMoving.h"

#pragma push_macro("_dbg_ln")
#pragma push_macro("_dbg")
#define _dbg_ln(msg)\
	do {\
		if (debug_verbose) std::cout << msg << std::endl;\
	} while(false)
#define _dbg(msg)\
	do {\
		if (debug_verbose) std::cout << msg;\
	} while (false)

namespace BlockSort {

	/*	**************************************************************************	*/
	/*	**************************************************************************	*/
	/*							algorithm function declarations						*/
	/*	**************************************************************************	*/
	/*	**************************************************************************	*/

	/*	  Performs a block sort by creating the elements from [start:mid-1]
	 * as the A_Blocks, and the elements form [mid:end] as the B_Blocks.
	 * The blocks are then sorted.  The blocks are then merged
	 */
	template <typename T>
	SortMetrics sort(T* array, array_size_t start, array_size_t mid, array_size_t end)
	{
		array_size_t u_size		= mid-start;
		array_size_t block_size = static_cast<array_size_t>(std::sqrt(u_size));

		Descriptors<T> descriptors;
		int num_desc;

		//
		//	How to organize the blocks
		//
		constexpr BlockSort::BlockOrganizations block_organization =
					BlockSort::BlockOrganizations::SYMMETRIC;

		int (*createDescriptors)(T* array,
								 array_size_t st, array_size_t mid, array_size_t end,
								 array_size_t block_size, Descriptors<T>&);

		switch (block_organization) {
			case BlockSort::BlockOrganizations::FULL_A0_BLOCK:
				createDescriptors = createBlockDescriptors_A0_Full;
				break;
			case BlockSort::BlockOrganizations::SYMMETRIC:
				createDescriptors = createBlockDescriptorsSymmetrically;
				break;
			}

		//
		//	How to sort the blocks
		//

		BlockSort::BlockSortingStrategy block_sorting_strategy =
				BlockSort::BlockSortingStrategy::TABLE;
		void (*sortBlocks)(T* array, Descriptors<T> &desc, int num, SortMetrics*);

		switch(block_sorting_strategy) {
		case BlockSort::BlockSortingStrategy::BINARY:
			sortBlocks = sortBlocksBinarySearch;
			break;
		case BlockSort::BlockSortingStrategy::HYBRID:
			sortBlocks = sortBlocksHybrid;
			break;
		case BlockSort::BlockSortingStrategy::RIGHT_TO_LEFT:
			sortBlocks = sortBlocksRightToLeft;
			break;
		default:
		case BlockSort::BlockSortingStrategy::TABLE:
			sortBlocks = sortBlocksByTable;
			break;
		}

		SortMetrics metrics(0,0);

		u_size		= mid-start;
		block_size 	= static_cast<array_size_t>(std::sqrt(u_size));

		//	creating block descriptors in either way, symmetric around mid
		//	  or having all of the fractional block in the last of b,
		//	  tested to have similar performance(?)
		num_desc = createDescriptors(array, start, mid, end, block_size, descriptors);
		sortBlocks(array, descriptors, num_desc, &metrics);
		mergeAllBlocksLeftToRight(array, descriptors, num_desc, &metrics);
//		metrics += mergeAllBlocksRightToLeft(array, descriptors, num_desc);
		return metrics;
	}

	/*	**************************************************	*/
	/*	**************************************************	*/
	/*						the sort						*/
	/*	**************************************************	*/
	/*	**************************************************	*/

	/*	**************************	*/
	/*		the sorting function	*/
	/*	**************************	*/

	constexpr array_size_t initial_block_size = 16;

	template <typename T>
	SortMetrics sort(T *array, array_size_t size) {

		constexpr bool debug_verbose = false;
		SortMetrics metrics(0,0);

		if (size < 2*initial_block_size) {
			metrics = InsertionSort::sort(array, size);
			return metrics;
		}

		//	Initially sort the elements within each block using an insertion sort
		for (array_size_t block_start = 0;
						  block_start < size;
						  block_start += initial_block_size)
		{
			array_size_t sub_array_size = initial_block_size;
			if (block_start + initial_block_size > size) {
				sub_array_size = size - block_start;
			}
			metrics += InsertionSort::sort(&array[block_start], sub_array_size);
		}
		_dbg_ln("  Made it through sorting initial blocks");

		array_size_t block_size = initial_block_size;
		array_size_t block_1_start = 0;

		while (block_size < size) {
			block_1_start = 0;
			array_size_t block_1_end	= block_1_start + block_size-1;
			array_size_t block_2_start 	= block_1_end + 1;
			array_size_t block_2_end	= block_2_start + block_size - 1;
			//	it is possible that block size is greater than half the array
			if (block_2_end > size-1)
				block_2_end = size-1;
			while (block_2_start < size) {
				metrics += sort(array, block_1_start, block_2_start, block_2_end);
				block_1_start = block_2_end+1;
				block_1_end	  = block_1_start + block_size - 1;
				//	if block 1 extends to or past the end of the array
				//	then there is not a block 2 and no need to merge
				if (block_1_end >= size-1)
					break;
				block_2_start = block_1_end + 1;
				block_2_end	  = block_2_start + block_size - 1;
				if (block_2_end > size-1)
					block_2_end = size-1;
			}
			_dbg_ln("  Made it through merging blocks of size " << block_size);
			block_size *= 2;
		}
		_dbg_ln("BlockSort() is returning");
		return metrics;
	}
}

#pragma pop_macro("_dbg")
#pragma pop_macro("_dbg_ln")

#endif /* BLOCKSORT_H_ */
