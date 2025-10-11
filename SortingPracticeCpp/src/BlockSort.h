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

#define DEBUG_VERBOSE_BLOCK_SORT
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

	template <typename T>
	SortMetrics sortAndMergeBlocks(T** array, Descriptors<T>&, int num_blocks);

	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*							function definitions							*/
	/*	**********************************************************************	*/
	/*	**********************************************************************	*/

	/*
	 * 	  This sorts the blocks, and merges each block (if necessary) with the
	 * 	preceding block (if necessary).  The only time that a block has to be
	 * 	merged with the portion of the final array to the block's left, is
	 * 	when a B_Block follows an A_Block, i.e.
	 *
	 *   A_Block =	[ C  D  E  F]  followed by 	B_Block = [ A  B  C  G ]
	 *
	 *    See the discussion of sortInsertionFromN() documentation for a
	 *  more detailed discussion.
	 *
	 *  Usage:
	 *
	 *  callers_metrics = sortAndMergeBlocks(array, descriptors, num_blocks)
	 *
	 *  where:
	 *  	array is an array of pointers to elements
	 *  	descriptors is an array block_descriptors
	 *  	num_blocks is the number of descriptors in the array of block descriptors
	 *
	 *  A descriptor must contain
	 *  	the start index of the block,
	 *  	the end index of the block,
	 *  	the key of the block, which is an array element, and thus a pointer
	 *  	the block type, either A_Block or B_Block
	 */

	template <typename T>
	SortMetrics sortAndMergeBlocks(
				T** array,
				Descriptors<T> &block_descriptors,
				int num_blocks)
	{
		constexpr bool debug_verbose = false;
		SortMetrics metrics(0,0);

		//	if all the block_descriptors are A_Blocks, they do not need to be sorted
		if (block_descriptors[num_blocks-1].type == BlockType::A_BLOCK) {
			return metrics;
		}

		//	if all the block_descriptors are B_Blocks, they do not need to be sorted
		if (block_descriptors[0].type == BlockType::B_BLOCK) {
			return metrics;
		}
		//	Count the number of A Blocks
		array_size_t num_A_blocks = 0;

		for (array_size_t i = 0; i != num_blocks; i++) {
			if (block_descriptors[i].type != BlockType::A_BLOCK) {
				break;
			}
			num_A_blocks++;
		}

		//	Build a table of indices of the A_Blocks that
		//	are present in the array of block_descriptors
		array_size_t a_positions[num_A_blocks];
		for (array_size_t i = 0; i != num_A_blocks; i++) {
			a_positions[i] = i;
		}

		//	Create source & destination indices
		array_size_t table_i 	= 0;
		array_size_t b_source 	= num_A_blocks;
		array_size_t dst_block	= 0;
		array_size_t src_block	= 0;
//		array_size_t highest_b_position = block_descriptors[num_blocks-1].end_index;

		while (table_i != num_A_blocks && b_source != num_blocks) {

			bool debug_chose_a = true;

			//	determine the smaller block, with deference given to the
			//	  block on the left (A_Block) if the two blocks have equal
			//	  keys in order to preserve stability
			metrics.compares++;
			if (*block_descriptors[a_positions[table_i]].key <= *block_descriptors[b_source].key) {
				src_block = a_positions[table_i];
				table_i++;
			} else {
				debug_chose_a = false;
				src_block = b_source;
				b_source++;
			}

			//	if the block that goes here is already in-place, move on
			if (src_block == dst_block) {
				dst_block++;
				if (debug_verbose) {
					std::cout << "dst = " << dst_block-1
							  << " src = " << src_block
							  << (debug_chose_a ? " chose A" : " chose B")
							  << " b_source was " << (debug_chose_a ? b_source : b_source-1)
							  << " table_i was " << (debug_chose_a ? table_i-1 : table_i)
							  << " table = [";
					for (int i = 0; i != num_A_blocks; i++) {
						std::cout << a_positions[i] << " ";
					}
					std::string selection_text;
					if (debug_chose_a)
						selection_text += "chose A   :";
					else
						selection_text += "chose B   :";
					std::cout << "]" << std::endl;
					array_size_t array_size = block_descriptors[num_blocks-1].end_index+1;
					array_size_t mid = -1;
					std::cout << blockSortToString(array, array_size, mid,
												   block_descriptors, num_blocks,
												   selection_text.c_str(), 3, 4)
							  << std::endl << std::endl;
				}
				continue;
			}

			metrics += swapBlocks(array, block_descriptors, dst_block, src_block);

			//	  If the block got placed to the right of another block,
			//	it may be necessary to merge the block leftwards
			//	This is only necessary when a B_Block is placed to the
			//	left of an A_Block.
			//	{ A[m], A[m+1] }	are in order b/c A block sequence was in order
			//	{ B[n], B[n+1] }	are in order b/c B block sequence was in order
			//	{ B[n], A[m]   }	is in order b/c *A.start_index > *B.end_index
			//	{ A[m], B[n]   }	may not be in order
			//	Consider the A_Block 	{ 1, 3, 5, 6 } vs. B_Block { 2, 4, 7, 8 }
			//	They form the sequence 	{ 1, 3, 5, 6, 2, 4, 7, 8 }
			//	  which is not in final ordering.  The values of the B_Block must
			//	  be moved to their correct place in the combined sequence.
			if (dst_block > 0) {
				if (block_descriptors[dst_block].type == BlockType::B_BLOCK) {
					array_size_t start 	= block_descriptors[0].start_index;
					array_size_t mid	= block_descriptors[dst_block].start_index;
					array_size_t end	= block_descriptors[dst_block].end_index;
//					metrics = mergeContiguousElementsByRotating(array, start, mid, end);
					insertionSortPartial(array, start, mid, end, metrics);
				}
			}

			for (array_size_t i = table_i; i < num_A_blocks; i++) {
				//	If there was an A_Block in the table that was at
				//	  position 'dst_block', it has been displaced to 'src_block'.
				//    Update that A_Block's entry in the table
				if (a_positions[i] == dst_block) {
					a_positions[i] = src_block;
					break;
				}
			}
			dst_block++;
			if (debug_verbose) {
				std::cout << "dst = " << dst_block-1
						  << " src = " << src_block
							  << (debug_chose_a ? " chose A" : " chose B")
							  << " b_source was " << (debug_chose_a ? b_source : b_source)
							  << " table_i was " << (debug_chose_a ? table_i-1 : table_i)
						  << " table = [";
				for (int i = 0; i != num_A_blocks; i++) {
					std::cout << a_positions[i] << " ";
				}
				std::string selection_text;
				if (debug_chose_a)
					selection_text += "chose A   :";
				else
					selection_text += "chose B   :";
				std::cout << "]" << std::endl;
				array_size_t array_size = block_descriptors[num_blocks-1].end_index+1;
				array_size_t mid = -1;
				std::cout << blockSortToString(array, array_size, mid,
											   block_descriptors, num_blocks,
											   selection_text.c_str(), 3, 4)
						  << std::endl << std::endl;
			}
		}

		//	Either the A_Blocks or all of the B_Blocks has been placed (or both)
		//	  into their final position.  If the loop terminated because all
		//	  of the B_Blocks were placed, there may be some remaining displaced
		//	  A_Blocks that need to be restored to their original ordering

		while (table_i != num_A_blocks) {
			src_block = a_positions[table_i];
			table_i++;
			if (dst_block != src_block) {
				metrics += swapBlocks(array, block_descriptors, dst_block, src_block);
				//	The A_Block that was at 'dst' has now been swapped to 'src'
				//  Update that block's entry in the table
				for (array_size_t i = table_i; i < num_A_blocks; i++) {
					if (a_positions[i] == dst_block) {
						a_positions[i] = src_block;
						break;
					}
				}
			}
			dst_block++;
		}

		//	It is possible that the final A_Block was placed to the left
		//	  of remaining B_Block(s). In that case, the B_Block's will need
		//	  to be merged into the A_Block to the left.  Consider:
		//		Am-1:Am:Bn-2:Bn-1	[L M O P][Q X Y Z]:[A B C R][S T U V]
		//	In this case, all of the B blocks remaining need to be merged
		if (dst_block > 0 && dst_block != num_blocks) {
			if (block_descriptors[dst_block-1].type == BlockType::A_BLOCK &&
				block_descriptors[dst_block].type == BlockType::B_BLOCK) {
				if (debug_verbose) {
					std::cout << "IT IS NECESSARY TO MERGE BLOCK " << dst_block
							  << " TO THE RIGHT " << std::endl;
				}
				array_size_t start = block_descriptors[0].start_index;
				array_size_t mid		= block_descriptors[dst_block].start_index;
				array_size_t end		= block_descriptors[num_blocks-1].end_index;
				metrics =
					SortingUtilities::mergeTwoAdjacentBlocksByRotation(
											array, start, mid-1, mid, end);
//				metrics = insertionSortPartial(array, start, mid, end);
			}
		}
		return metrics;
	}


	/*	  Performs a block sort by creating the elements from [start:mid-1]
	 * as the A_Blocks, and the elements form [mid:end] as the B_Blocks.
	 */
	template <typename T>
	SortMetrics sort(T**array, 	array_size_t start,
								array_size_t mid,
								array_size_t end)
	{
		SortMetrics metrics(0,0);

		array_size_t u_size	= mid-start;
		array_size_t block_size = static_cast<array_size_t>(std::sqrt(u_size));
		Descriptors<T> descriptors;
		int num_desc =
				createBlockDescriptorsSymmetrically(array, start, mid, end,
													block_size, descriptors);
		metrics += sortBlocksByTable(array, descriptors, num_desc);
		metrics += mergeAllBlocksLeftToRight(array, descriptors, num_desc);
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
	SortMetrics sortPointerstoObjects(T **array, array_size_t size) {

		constexpr bool debug_verbose = false;
		SortMetrics metrics(0,0);

		if (size < 2*initial_block_size) {
			metrics = InsertionSort::sortPointersToObjects(array, size);
			return metrics;
		}
#if 0
		SortMetrics (*mergeInPlace)(T**array,
									array_size_t block_1_start,
									array_size_t block_1_end,
									array_size_t block_2_start,
									array_size_t block_2_end) =
//			SortingUtilities::mergeTwoAdjacentBlocksByRotation;
			SortingUtilities::mergeTwoBlocksElementsByTable;
#endif
		//	Initially sort the elements within each block using an insertion sort
		for (array_size_t block_start = 0;
						  block_start < size;
						  block_start += initial_block_size)
		{
			array_size_t sub_array_size = initial_block_size;
			if (block_start + initial_block_size > size) {
				sub_array_size = size - block_start;
			}
			metrics += InsertionSort::sortPointersToObjects(&array[block_start],
												 	 	 	 sub_array_size);
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
//				metrics += mergeInPlace(array,
//							 	 	 	block_1_start, block_1_end,
//										block_2_start, block_2_end);
				metrics += sort(array, block_1_start, block_2_start, block_2_end);
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
