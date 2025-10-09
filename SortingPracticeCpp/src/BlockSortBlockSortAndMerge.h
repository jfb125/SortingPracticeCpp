/*
 * BlockSortBlockSortAndMerge.h
 *
 *  Created on: Oct 6, 2025
 *      Author: joe
 */

#ifndef BLOCKSORTBLOCKSORTANDMERGE_H_
#define BLOCKSORTBLOCKSORTANDMERGE_H_

#include "BlockSortIncludes.h"

namespace BlockSort {

	//	starting at the right-most block, merge the previous (left) block into
	//	  the elements to the right
	template <typename T>
	ComparesAndMoves mergeAllBlocksRightToLeft(T** array,
									std::unique_ptr<BlockDescriptor<T>[]> &block_descriptors,
									int num_blocks);

	/*	sort an array of blocks by key, ignoring block type	*/
	template <typename T>
	ComparesAndMoves sortBlocksByKey(T** array, index_t size, Descriptors<T>&,
												int start, int end);


	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*							function definitions							*/
	/*	**********************************************************************	*/
	/*	**********************************************************************	*/

	//	starting at the right-most block, merge the previous (left) block into
	//	  the elements to the right
	template <typename T>
	ComparesAndMoves mergeAllBlocksRightToLeft(T** array,
									std::unique_ptr<BlockDescriptor<T>[]> &block_descriptors,
									int num_blocks) {

		ComparesAndMoves result;
		index_t sorted_span_start 	= block_descriptors[num_blocks-1].start_index;
		index_t sorted_span_end		= block_descriptors[num_blocks-1].end_index;
		int left_block = num_blocks-2;

		while (left_block >= 0) {
			index_t left_start 	= block_descriptors[left_block].start_index;
			index_t left_end	= block_descriptors[left_block].end_index;
			result += mergeTwoBlocksByTable(array,
											left_start, left_end,
											sorted_span_start, sorted_span_end);
			//	all of the elements [left.start:size-1] are now in order
			sorted_span_start = left_start;
			//	move the block pointer to the next block to the left
			left_block--;
		}
		return result;
	}


	/*
	 * 	ComparesAndSwaps sortBlocksByKey(array, blocks, start, end);
	 *
	 * 	  Sorts an array of blocks, including the underlying array,
	 * 	by key only.  Does not consider block type
	 *
	 */

	// TODO - implement an insertion sort where you are not exchanging each
	//			pair of blocks at every compare, but instead moving blocks
	//			right-ward until the location of the block in question has
	//			been found, then write the block in question to the location
	//		[0] [1] [2] [3] [4]	  i  tmp  j [j-1]
	//		 a   c   d   b   e    3  'b'  3   d
	//		 a   c   d   d   e	  3	 'b'  2   c
	//	     a   c   c   d   e    3  'b'  1   a
	//	     a   b   c   d   e    3

	template <typename T>
	ComparesAndMoves sortBlocksByKey(T** array, index_t size,
									 Descriptors<T> &blocks,
									 int start, int end)
	{
		ComparesAndMoves metrics;

		// insertion sort the A_Blocks
		for (int i = start+1; i <= end; i++) {
			for (int j = i; j > 0; j--) {
				metrics._compares++;
				if (*blocks[j].key >= *blocks[j-1].key) {
					break;
				}
				metrics += rotateBlocksRight(array, blocks, j-1, j, 1);
			}
		}
		return metrics;
	}

}	// namespace BlockSort{}

#endif /* BLOCKSORTBLOCKSORTANDMERGE_H_ */
