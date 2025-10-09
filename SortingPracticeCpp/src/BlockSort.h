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
 */

#ifndef BLOCKSORT_H_
#define BLOCKSORT_H_

#include <cstdlib>

//#include "BlockSortDataTypes.h"
#include "BlockSortIncludes.h"
#include "IntegerArithmetic.h"
#include "SortingUtilities.h"
#include "InsertionSort.h"
//#include "BlockSortDebugging.h"
//#include "BlockSortBlockDescriptors.h"
//#include "BlockSortBlockMoving.h"
//#include "BlockSortElementMoving.h"

#define DEBUG_VERBOSE_BLOCK_SORT

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
		index_t num_A_blocks = 0;

		for (index_t i = 0; i != num_blocks; i++) {
			if (block_descriptors[i].type != BlockType::A_BLOCK) {
				break;
			}
			num_A_blocks++;
		}

		//	Build a table of indices of the A_Blocks that
		//	are present in the array of block_descriptors
		index_t a_positions[num_A_blocks];
		for (index_t i = 0; i != num_A_blocks; i++) {
			a_positions[i] = i;
		}

		//	Create source & destination indices
		index_t table_i 	= 0;
		index_t b_source 	= num_A_blocks;
		index_t dst_block	= 0;
		index_t src_block	= 0;

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
					index_t array_size = block_descriptors[num_blocks-1].end_index+1;
					index_t mid = -1;
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
					index_t start = block_descriptors[0].start_index;
					index_t mid		= block_descriptors[dst_block].start_index;
					index_t end		= block_descriptors[dst_block].end_index;
//					metrics = mergeContiguousElementsByRotating(array, start, mid, end);
					metrics = insertionSortPartial(array, start, mid, end);
				}
			}

			for (index_t i = table_i; i < num_A_blocks; i++) {
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
				index_t array_size = block_descriptors[num_blocks-1].end_index+1;
				index_t mid = -1;
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
				for (index_t i = table_i; i < num_A_blocks; i++) {
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
				index_t start = block_descriptors[0].start_index;
				index_t mid		= block_descriptors[dst_block].start_index;
				index_t end		= block_descriptors[num_blocks-1].end_index;
				metrics = mergeContiguousElementsByRotating(array, start, mid, end);
//				metrics = insertionSortPartial(array, start, mid, end);
			}
		}
		return metrics;
	}

	/*
	 * 	ComparesAndMoves sortBlocks*Strategy*(array, size, block_descriptors, blocks);
	 *
	 * 	These functions sort the block order to ensure that the block order
	 * 		is ascending.  The underlying array by necessity is also rearranged
	 *
	 *	This assumes that an array of BlockDescriptors containing information
	 *	  about block types, begin & end indices, block keys (for sorting)
	 *	  are stored in 'block_descriptors'
	 *
	 *	The result of this function is that the blocks are arranged
	 *	  in ascending order to facilitate a block merge which will result
	 *	  in the array in order
	 *
	 *	Entry into this routine assumes that the values in the A_Blocks in the 'u' half of the array (left)
     *	  are in order and the values in the B_Blocks in the 'v' half of the array (right) are in order
     *	  although the total array may not be in order
     *
     *	  An A_Block's key value is the left most element and a B_Block's key value is the right most element
     *
     *	             0             1             2             3             4             5
     *	Name/Key: ALPHA='D'	    BRAVO='F'   CHARLIE='G'    DELTA='C'      ECHO='E'     FOXTROT='H'
     *	Tags:	  A_Block       A_Block		  A_Block		B_Block		  B_Block       B_Block
     *	Values:	{ B, C, D }   { F, G, H } : { G, H, I } : { A, B, C } : { C, D, E } : { E, F, H }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *		The span of unexamined A_Blocks is now [0:2] = [ALPHA:CHARLIE] and
     *		the span of unplaced   B_Blocks is now [3:5] = [DELTA:FOXTROT]
     *
     *
     *	Starting at the rightmost A_Block (CHARLIE in the above example),
     *	  use some strategy, (there is a separate function for each strategy) to search the B_Blocks
     *    until the first B_Block is found that is < the A_Block.  In the above example,
     *	  the result of the search returns ECHO='E' block is the largest of the B_Blocks
     *		that is smaller CHARLIE= 'G'.  All the B_Blocks upto, and including ECHO, will be rotated right
     *
     *	Note that A_Block BRAVO='F' is also greater than B_Block ECHO='E'.  Search the A_Blocks to the
     *	  left of CHARLIE (the right-most A_Block) for any other A_Blocks that should be rotated to the
     *	  right of ECHO.  In this case, the A_Blocks [BRAVO:CHARLIE] need to be rotated to a position
     *	  in the array of blocks to the left of ECHO.  Thus, the blocks from [BRAVO:ECHO] needs to
     *	  be rotated to the left by 2 block positions.  Note that ECHO's start_index is 12, and it
     *	  will move to blocks to the left, which will have echo starting at 6.
     *
     *	  The result of rotating only the BlockDesciptors is:
     *
     *	             0             1             2             3             4             5
     *	Name/Key: ALPHA='D'	    DELTA='C'     ECHO='E'      BRAVO='F'    CHARLIE='G'    FOXTROT='H'
     *	Tags:	  A_Block       B_Block		  B_Block		 A_Block	   A_Block       B_Block
     *	Values:	{ B, C, D }   { F, G, H } : { G, H, I } : { A, B, C } : { C, D, E } : { E, F, H }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *	Note that while the descriptors' keys are now in order, the underlying array values have not moved
     *	  The next step is to rotate the underlying array by the amount that ECHO's starting_index moved
     *	  which is 12-6 = 6 elements
     *
     *	             0             1             2             3             4             5
     *	Name/Key: ALPHA='D'	    DELTA='C'     ECHO='E'      BRAVO='F'    CHARLIE='G'    FOXTROT='H'
     *	Tags:	  A_Block       B_Block		  B_Block		 A_Block	   A_Block       B_Block
     *	Values:	{ B, C, D }   { A, B, C }   { C  D  E  }   { F, G, H }   { G, H, I }   { E, F, H }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *		The span of unexamined A_Blocks is now [0:0] = [ALPHA:ALPHA] and
     *		the span of unplaced   B_Blocks is now [1:2] = [DELTA:ECHO]
     *
     *	The algorithm will be repeated by looking at the rightmost A_Block, which is now [0] = ALPHA='D'
     *	  and finding the rightmost B_Block that is < A_Block which is [1] = DELTA='C'
     *
     *	             0             1             2             3             4             5
     *	Name/Key: DELTA='C'	    ALPHA='D'     ECHO='E'      BRAVO='F'    CHARLIE='G'    FOXTROT='H'
     *	Tags:	  B_Block       B_Block		  B_Block		 A_Block	   A_Block       B_Block
     *	Values:	{ A, B, C}    { B, C, D }   { C  D  E  }   { F, G, H }   { G, H, I }   { E, F, H }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *	The algorithm repeats until the A_Block index is -1, which in this example has occurred
	 */

	/*
	 * compares_and_swaps += sortBlocksBySwapping(array, block_descriptors, num_blocks);
	 *
	 *   Takes an array of blocks in order { A0, A1, ... Am, B0, B1 ... Bn }
	 * and places them in numerical order.
	 *
	 *   Note that the A blocks have to be in numerical order
	 *    and that the B blocks have to be in numerical order
	 *    and all of the blocks except for Bn have to be of the same size
	 *
	 *	Inputs:
	 *		array				- the array of elements
	 *		blocks_descriptors  - an array of BlockDescriptors
	 *		num_blocks			- number of elements in the array 'block_descriptor'
	 *
	 *	Outputs:
	 *		compares_and_moves	- the number of compares of block descriptor keys
	 *							  and the number of moves of array elements
	 *
	 *	Modifies:
	 *		the order of the elements in the 'array' may change
	 *		the order of the descriptors in 'block_descriptors' may change
	 *
	 *	The block types will not change.
	 *	The block descriptors' keys will not change even though the location
	 *	  of the key within the array will change if the block's position changes
	 */

	template <typename T>
	SortMetrics sortPointersToObjects(T**, index_t start, index_t mid, index_t end, index_t block_size) {

		SortMetrics metrics(0,0);

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

	template <typename T>
	SortMetrics sortPointerstoObjects(T **array, index_t size) {

		bool debug_verbose = false;
		bool print_error = true;
		std::stringstream msg;
		constexpr index_t minimum_block_size = 16;

		SortMetrics result(0,0);

		//	Do an insertion sort if the array size is not large enough
		//	  to justify the time spent managing the block overhead
		if (size < 2 * minimum_block_size) {
			result += InsertionSort::sortPointersToObjects(array, size);
			return result;
		}

		index_t start = 0;
		index_t mid = size/2;
		index_t end = size-1;
		index_t block_size = static_cast<index_t>(sqrt(size/2));
		if (block_size < minimum_block_size) {
			block_size = minimum_block_size;
		}
		std::unique_ptr<BlockDescriptor<T>[]> block_descriptors;

		// 	The first time, sort the elements within individual blocks
		int num_blocks = createBlockDescriptors_A0_Full(array, start, mid, end,
														block_size, block_descriptors);
		msg << "After creating block descriptors\n";
		msg	<< blockSortToString(array, size, mid,
								 block_descriptors, num_blocks)
		    << "\n\n";
		//	sort the elements within each block
		for (int i = 0; i != num_blocks; i++) {
			T** sub_array = &array[block_descriptors[i].start_index];
			index_t sub_array_size = block_descriptors[i].getWidth();
			result += InsertionSort::sortPointersToObjects(sub_array, sub_array_size);
		}

		msg << "after sorting each block's elements\n"
			<< blockSortToString(array, size, mid,
								 block_descriptors, num_blocks)
		    << "\n\n";

		//	the array has been re-ordered independently of the block descriptors
		//	assign the block's key value from the underlying array

		assignBlockKeys(array, block_descriptors, num_blocks);
		int first_b_block = 0;
		while (block_descriptors[first_b_block].type == BlockSort::BlockType::A_BLOCK) {
			first_b_block++;
		}
		result += sortBlocksByKey(array, size, block_descriptors, 0, first_b_block-1);
		result += sortBlocksByKey(array, size, block_descriptors, first_b_block, num_blocks-1);

		msg << "after ordering A_Blocks and ordering B_Blocks\n"
			<< blockSortToString(array, size, mid,
								 block_descriptors, num_blocks)
		    << "\n\n";

		constexpr bool merge_at_once = true;
		if (merge_at_once) {
			std::cout << " !!!! ERROR - BLOCK SORT NOT IMPLMENTED !!!! " << std::endl;
//			result += mergeAllBlocks(array, block_descriptors, num_blocks);
			goto BLOCK_SORT_RETURN_POINT;
		}

		while (num_blocks > 1) {
			msg << "before merging " << std::setw(4) << num_blocks << " block descriptors\n"
				<< blockSortToString(array, size, mid, block_descriptors, num_blocks)
				<< "\n";

			int left_block = 0;
			int right_block = 1;
			while (right_block < num_blocks) {
				index_t left_start 	= block_descriptors[left_block].start_index;
				index_t left_end	= block_descriptors[left_block].end_index;
				index_t right_start	= block_descriptors[right_block].start_index;
				index_t right_end	= block_descriptors[right_block].end_index;
				std::cout << " !!!! ERROR - BLOCK SORT NOT IMPLEMENTED !!!! " << std::endl;
				goto BLOCK_SORT_RETURN_POINT;

//				result += mergeTwoBlocksByTable(array, left_start, left_end,
//													   right_start, right_end);
				for (int i = left_start; i < right_end-1; i++) {
					if (*array[i] > *array[i+1]) {
						msg << std::setw(2) << num_blocks
						  << " block [" << left_block << "]"
						  << " spans [" << left_start
						  << ":" << left_end
						  << "]"
						  << " merged with"
						  << " block ["
						  << right_block << "] "
						  << " spans [" << right_start
						  << ":" << right_end
						  << "]"
						  << std::endl;
						msg << "ERROR: [" << i << "] = "
						  << *array[i] << " vs [" << i+1 << "] = "
						  << *array[i+1] << std::endl;
						for (int i = left_start; i < right_end; i++) {
							msg << std::setw(VALUE_WIDTH) << i << " ";
						}
						msg << std::endl;
						for (int i = left_start; i < right_end; i++) {
							msg << std::setw(VALUE_WIDTH) << *array[i] << " ";
						}
						msg << std::endl << std::endl;
						//	force the routine to print the error message
						if (print_error) {
							debug_verbose = true;
						}
						goto BLOCK_SORT_RETURN_POINT;
					}
				}
				left_block  = right_block+1;
				right_block = left_block+1;
			}
			//	if there was an odd number of blocks then left_block will be num_blocks-1
			//	  combined two previous blocks n-3:n-2 with the n-1 block
			if (left_block < num_blocks) {
				index_t left_start 	= block_descriptors[num_blocks-3].start_index;
				index_t right_start	= block_descriptors[num_blocks-1].start_index;
				index_t right_end	= block_descriptors[num_blocks-1].end_index;
				result += mergeContiguousElementsByRotating(array, left_start,
														  right_start, right_end);
			}

			msg << "after merging  " << std::setw(4) << num_blocks << " block descriptors\n"
				<< blockSortToString(array, size, mid, block_descriptors, num_blocks)
				<< "\n\n";

			//	each pair of blocks now forms an in-order span
			//	update the block descriptors to span all the in-order elements
			std::cout << " !!!! ERROR - BLOCK SORT NOT IMPLEMENTED !!!! " << std::endl;
			break;
//			num_blocks = mergeAdajacentPairsOfDescriptors(array,
//														  block_descriptors,
//														  num_blocks);
		}

BLOCK_SORT_RETURN_POINT:
		if (debug_verbose) {
			std::cout << msg.str();
		}
		return result;
	}
}

#endif /* BLOCKSORT_H_ */
