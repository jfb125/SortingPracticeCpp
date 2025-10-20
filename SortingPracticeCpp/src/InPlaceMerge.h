/*
 * InPlaceMerge.cpp
 *
 *  Created on: Oct 9, 2025
 *      Author: joe
 */

#ifndef INPLACEMERGE_H
#define INPLACEMERGE_H
#include <iostream>
#include <iomanip>
#include <string>

#include "BlockOperations.h"
#include "SortingDataTypes.h"
#include "SortingUtilities.h"
#include "InsertionSort.h"

#pragma push_macro("_dbg_ln")
#pragma push_macro("_dbg")
#define	_dbg_ln(msg)	do { \
						if (debug_verbose)\
							std::cout << msg << std::endl;\
					}while(false)
#define _dbg(msg)	do {\
						if (debug_verbose)\
							std::cout << msg;\
					} while(false)

namespace InPlaceMerge {

	/*
	 * 	Address the array as being made up of blocks, and merges adjacent blocks.
	 *
	 * 	Double the block size each time so what was previously two separate blocks
	 * 	  is now treated as a single block.  Note that the size of the last block
	 * 	  is possibly not an power of 2
	 *
	 * 	  	First pass	[0:15][16:31][32:47][48:63][64:79][80:95][96:102][103:104]
	 * 	  	Second pass	[    0:31   ][    32:63   ][   64:95    ][    96:104     ]
	 * 	  	Third pass	[          0:63           ][            64:104           ]
	 *
	 *	If the array size is less than 32, use an insertion sort and return
	 *
	 *	The mergeInPlace is done using different algorithms
	 */

	constexpr array_size_t initial_block_size = 16;


	template <typename T>
	SortMetrics sortPointerstoObjects(T **array, array_size_t size) {

		bool debug_verbose = false;

		SortMetrics metrics(0,0);

		BlockOperations::MergeStrategy merge_strategy =
					BlockOperations::MergeStrategy::TABLE;
		_dbg_ln("InPlaceMerge using " << merge_strategy);

		BlockOperations::MergeFunction<T> mergeBlocks;

		switch(merge_strategy) {
		case BlockOperations::MergeStrategy::AUXILLIARY:
			mergeBlocks = BlockOperations::mergeTwoBlocksElementsUsingAuxiliaryBuffer;
			break;
		case BlockOperations::MergeStrategy::INSERTION:
			mergeBlocks = BlockOperations::insertionSortPartial;
			break;
		case BlockOperations::MergeStrategy::BINARY:
			mergeBlocks = BlockOperations::mergeTwoAdjacentBlocksBy_Rotation_BinarySearch;
			break;
		case BlockOperations::MergeStrategy::HYBRID:
			mergeBlocks = BlockOperations::mergeTwoAdjacentBlocksBy_Rotation_Hybrid;
			break;
		case BlockOperations::MergeStrategy::RGT_TO_LFT:
			mergeBlocks = BlockOperations::mergeTwoAdjacentBlocksBy_Rotation_RightToLeft;
			break;
		case BlockOperations::MergeStrategy::TABLE:
		default:
			mergeBlocks = BlockOperations::mergeTwoBlocksElementsByTable;
			break;
		}

		//	Small arrays can just be InsertionSorted and done
		if (size < 2*initial_block_size) {
			metrics = InsertionSort::sortPointersToObjects(array, size);
			return metrics;
		}

		array_size_t block_size = initial_block_size;
		array_size_t block_start = 0;

		//	Ensure that each block's elements are sorted
		while (block_start < size) {
			array_size_t num_elements = block_size;
			if (block_start + num_elements > size)
				num_elements = size - block_start;
			metrics +=
				InsertionSort::sortPointersToObjects(&array[block_start],
													 num_elements);
			block_start += block_size;
		}

		_dbg_ln("  Made it through sorting initial blocks");

		//	continuously merge pairs of adjoining blocks of ever larger sizes
		while (block_size < size)
		{
			//	assign the block boundaries to the first pair of blocks
			array_size_t block_1_start	= 0;
			array_size_t block_1_end	= block_1_start + block_size-1;
			array_size_t block_2_start 	= block_1_end + 1;
			array_size_t block_2_end	= block_2_start + block_size - 1;
			//	It is possible that block size is greater than half the array
			//	  when size is not an integer multiple of a power of 2
			if (block_2_end > size-1) 	  block_2_end = size-1;

			//	move through each pair of blocks left to right merging them
			while (block_2_start < size) {
				mergeBlocks(array, 	block_1_start, block_1_end,
									block_2_start, block_2_end,
									metrics);
				//	move the indices to the next pair of blocks
				block_1_start = block_2_end+1;
				block_1_end	  = block_1_start + block_size - 1;
				//	if block 1 extends to or past the end of the array
				//	then there is not a block_2 and no need to merge
				if (block_1_end >= size-1)
					break;
				block_2_start = block_1_end + 1;
				block_2_end	  = block_2_start + block_size - 1;
				//	if bloc_2 is not full size, adjust end to the array's end
				if (block_2_end > size-1)
					block_2_end = size-1;
			}
			_dbg_ln("  Made it through merging blocks of size "<<block_size);
			block_size *= 2;
		}
		_dbg_ln("InPlaceMerge sort is returning");
		return metrics;
	}
}

#pragma pop_macro("_dbg")
#pragma pop_macro("_dbg_ln")
#endif
