/*
 * BlockSort.h
 *
 *  Created on: Aug 11, 2025
 *      Author: Joe Baker
 */

/*
 * TODO - List
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

	template <typename T>
	void assignBlockMergeFunction(
			BlockOperations::MergeFunction& function,
			BlockOperations::MergeStrategy strategy);

	template <typename T>
	void assignCreateDescriptorsFunction(
			BlockSort::CreateDescriptorsFunction<T> &function,
			BlockSort::BlockOrganizations organization);

	template <typename T>
	void assignSortBlocksFunction(
			BlockSort::SortBlocksFunction<T>& function,
			BlockSort::BlockSortingStrategy strategy);

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
	void sort(	T* array,
				array_size_t start, array_size_t mid, array_size_t end,
				SortMetrics *metrics = nullptr)
	{
		BlockSort::CreateDescriptorsFunction createDescriptors;
		assignCreateDescriptorsFunction<T>(
				createDescriptors,
				BlockSort::BlockOrganizations::SYMMETRIC);

		BlockSort::SortBlocksFunction sortBlocks;
		assignSortBlocksFunction<T>(
				sortBlocks,
				BlockSort::BlockSortingStrategy::TABLE);

		BlockOperations::MergeFunction 	mergeBlocks;
		assignBlockMergeFunction<T>(
				mergeBlocks,
				BlockOperations::MergeStrategy::TABLE);

		//	The size of the blocks is defined as the sqrt of the size of
		//	the lower half of the array, which is always <= the size of
		//	the upper half
		array_size_t u_size		= mid-start;
		array_size_t block_size = static_cast<array_size_t>(std::sqrt(u_size));

		Descriptors<T> descriptors;
		int num_desc;

		num_desc = createDescriptors(array, start,
									 mid, end, block_size,
									 descriptors,
									 metrics);
		sortBlocks(array, descriptors, num_desc, metrics);
		mergeBlocks(array, descriptors, num_desc, metrics);
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
	void sort(T *array, array_size_t size, SortMetrics *metrics) {

		constexpr bool debug_verbose = false;

		if (size < 2*initial_block_size) {
			return InsertionSort::sort(array, size, metrics);
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
			InsertionSort::sort(&array[block_start], sub_array_size, metrics);
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
				sort(array, block_1_start, block_2_start, block_2_end, metrics);
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
		return;
	}	// function srot

	/*	******************************************************************	*/
	/*					Functions to make main function more concise		*/
	/*	******************************************************************	*/

	template <typename T>
	void assignCreateDescriptorsFunction(
			BlockSort::CreateDescriptorsFunction<T> &function,
			BlockSort::BlockOrganizations organization)
	{
		switch (organization) {
			case BlockSort::BlockOrganizations::FULL_A0_BLOCK:
				function = createBlockDescriptors_A0_Full<T>;
				break;
			case BlockSort::BlockOrganizations::SYMMETRIC:
				function = createBlockDescriptorsSymmetrically<T>;
				break;
			}
	}

	template <typename T>
	void assignSortBlocksFunction(
			BlockSort::SortBlocksFunction<T>& function,
			BlockSort::BlockSortingStrategy strategy)
	{
		switch(strategy) {
		case BlockSort::BlockSortingStrategy::BINARY:
			function = sortBlocksBinarySearch<T>;
			break;
		case BlockSort::BlockSortingStrategy::HYBRID:
			function = sortBlocksHybrid<T>;
			break;
		case BlockSort::BlockSortingStrategy::RIGHT_TO_LEFT:
			function = sortBlocksRightToLeft<T>;
			break;
		default:
		case BlockSort::BlockSortingStrategy::TABLE:
			function = sortBlocksByTable<T>;
			break;
		}
	}

	template <typename T>
	void assignBlockMergeFunction(
				BlockOperations::MergeFunction& function,
				BlockOperations::MergeStrategy strategy)
	{
		switch(strategy) {
		case BlockOperations::MergeStrategy::AUXILLIARY:
			function =
				BlockOperations::mergeTwoBlocksElementsUsingAuxiliaryBuffer<T>;
			break;
		case BlockOperations::MergeStrategy::BINARY:
			function =
				BlockOperations::mergeTwoAdjacentBlocksBy_Rotation_BinarySearch<T>;
			break;
		case BlockOperations::MergeStrategy::HYBRID:
			function =
				BlockOperations::mergeTwoAdjacentBlocksBy_Rotation_Hybrid<T>;
			break;
		case BlockOperations::MergeStrategy::INSERTION:
			function =
				BlockOperations::insertionSortPartial<T>;
			break;
		case BlockOperations::MergeStrategy::RGT_TO_LFT:
			function =
				BlockOperations::mergeTwoAdjacentBlocksBy_Rotation_RightToLeft<T>;
			break;
		case BlockOperations::MergeStrategy::TABLE:
		default:
			function =
				BlockOperations::mergeTwoBlocksElementsByTable<T>;
			break;
		}
	}

}	// namespace BlockSort

#pragma pop_macro("_dbg")
#pragma pop_macro("_dbg_ln")

#endif /* BLOCKSORT_H_ */
