/*
 * InPlaceMerge.cpp
 *
 *  Created on: Oct 9, 2025
 *      Author: joe
 */

#include <iostream>
#include <iomanip>
#include <string>

#include "SortingPracticeDataTypes.h"
#include "SortingUtilities.h"
#include "InsertionSort.h"

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

		BlockSort::MergeStrategy merge_strategy =
					BlockSort::MergeStrategy::TABLE;

		SortMetrics (*mergeBlocks)(T** array,
								   array_size_t block_1_start,
								   array_size_t block_1_end,
								   array_size_t block_2_start,
								   array_size_t block_2_end);

		switch(merge_strategy) {
		case BlockSort::MergeStrategy::ROTATE:
			mergeBlocks = SortingUtilities::mergeTwoAdjacentBlocksByRotation;
			break;
		case BlockSort::MergeStrategy::TABLE:
		default:
			mergeBlocks = SortingUtilities::mergeTwoBlocksElementsByTable;
			break;
		}

		if (debug_verbose) std::cout << "InPlaceMerge using " << merge_strategy << std::endl;

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

		if (debug_verbose) std::cout << "  Made it through sorting initial blocks\n";

		//	continuously merge pairs of adjoining blocks of ever larger sizes
		while (block_size < size) {
			array_size_t block_1_start = 0;
			array_size_t block_1_end	= block_1_start + block_size-1;
			array_size_t block_2_start 	= block_1_end + 1;
			array_size_t block_2_end	 = block_2_start + block_size - 1;
			//	it is possible that block size is greater than half the array
			if (block_2_end > size-1)
				block_2_end = size-1;
			while (block_2_start < size) {
				metrics += mergeBlocks(	array,
							 	 	 	block_1_start, block_1_end,
										block_2_start, block_2_end);
				block_1_start = block_2_end+1;
				block_1_end	  = block_1_start + block_size - 1;
				//	if block 1 extends to or past the end of the array
				//	then there is not a block_2 and no need to merge
				if (block_1_end >= size-1)
					break;
				block_2_start = block_1_end + 1;
				block_2_end	  = block_2_start + block_size - 1;
				if (block_2_end > size-1)
					block_2_end = size-1;
			}
			if (debug_verbose) {
				std::cout << "  Made it through merging blocks of size " << block_size << std::endl;
			}
			block_size *= 2;
		}
		if (debug_verbose) {
			std::cout << __FUNCTION__ << " is returning\n";
		}
		return metrics;
	}
}
