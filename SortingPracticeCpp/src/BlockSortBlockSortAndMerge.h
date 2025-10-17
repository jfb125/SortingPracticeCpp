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


	/*	Merges each pair of adjacent descriptors.  This maybe(?) necessary after a pass
	 * through the array that combines adjacent descriptors to avoid having to regenerate
	 * the descriptors each time the block size is doubled in a bottom up merge
	 */
	template <typename T>
	int mergeAdajacentPairsOfDescriptors(T** array,
										 Descriptors<T> &descriptors,
										 int num_descriptors);

	/*	Starting with the leftmost A_Block, merge blocks
	 * expects a set of descriptors that is in order by key	*/
	template <typename T>
	SortMetrics mergeAllBlocksLeftToRight(T** array,
										   Descriptors<T> &block_descriptors,
										   int num_blocks);

	/*	Starting with the rightmost B_Block, merge blocks to the left	*/
	template <typename T>
	SortMetrics mergeAllBlocksRightToLeft(T** array,
										  Descriptors<T> &block_descriptors,
										  int num_blocks);

	/*	Merge blocks using binary searches to identify spans to rotate
	 * Although not strictly necessary, 'block_1_end' is passed to be consistent
	 * with the other block merging functions	*/
//	template <typename T>
//	SortMetrics mergeTwoAdjacentBlocksByRotation(T** array,
//													  array_size_t block_1_start,
//													  array_size_t block_1_end,
//													  array_size_t block_2_start,
//													  array_size_t block2_end);
//
	/*	Merge a pair of blocks by using a table to keep track of where displaced
	 * elements in block_1 are moved to in block_2 during the merge. */
//	template <typename T>
//	SortMetrics mergeTwoBlocksByTable(T ** array,
//									  array_size_t block_1_start, array_size_t block_1_end,
//									  array_size_t block_2_start, array_size_t block_2_end);

	/*	sort an array of [start:end] by key, ignoring block type	*/
	template <typename T>
	SortMetrics sortBlocksByKey(T** array, array_size_t size, Descriptors<T>&,
												int start, int end);

	/*	sort an array of [A_0..A_m:B_0..B_n] using a binary search to find the
	 * 	location where each of the blocks should go 							*/
	template <typename T>
	SortMetrics sortBlocksBinarySearch( T **array,
										Descriptors<T> &descriptors, int num_blocks);

	/*	sort an array of [A_0..A_m:B_0..B_n] using a table to keep track of where
	 * 	the displaced A_Blocks go in the upper portion (B_Block) of the array	*/
	template <typename T>
	SortMetrics sortBlocksByTable(		T** array,
										Descriptors<T> &descriptors, int num_blocks);

	/*	sort an array of [A_0..A_m:B_0..B_n] using a binary search to find the
	 * 	location where each of the blocks should go 							*/
	template <typename T>
	SortMetrics sortBlocksHybrid(		T **array,
										Descriptors<T> &blocks, int num_blocks);

	/*	sort an array of [A_0..A_m:B_0..B_n] using an insertion sort			*/
	template <typename T>
	SortMetrics sortBlocksInsertioin(	T **array,
										Descriptors<T> &blocks, int num_blocks);

	/*	sort an array of blocks [A_0..A_m:B_0..Bn] starting at right A_m & B_n	*/
	template <typename T>
	SortMetrics sortBlocksRightToLeft(	T **array,
									   	Descriptors<T> &blocks, int num_blocks);


	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*							function definitions							*/
	/*	**********************************************************************	*/
	/*	**********************************************************************	*/

	/*
	 * 	num_descriptors mergeAdajacentPairsOfDescriptors(descriptors, num_descriptors)
	 *
	 *	  This goes through the array an combines adjacent descriptors into
	 *	a single descriptor.
	 *
	 *	  This operation is necessary when doing a bottom-up merge of blocks
	 *	within the array to avoid having to regenerate descriptors on each pass
	 *
	 *		0   1   2   3   4   5   6   7  becomes	0   1   2   3   4   5   6   7
	 *	   'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h'         'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h'
	 *	   |A0||A1        ||A2        ||A3|	       |A0           ||A1            |
	 *
	 * 	In an array of BlockDescriptors [], merges every pair of descriptors [n:n+1]
	 */

	template <typename T>
	int mergeAdajacentPairsOfDescriptors(T** array,
										Descriptors<T> &descriptors, int num_descriptors)
	{
		int result_count = 0;
		int dst = 0;
		int l_src = 0;
		int r_src = 1;

		//	the new descriptors will span the gap of two of the previous
		while (r_src < num_descriptors) {
			result_count++;
			descriptors[dst].start_index = descriptors[l_src].start_index;
			descriptors[dst].end_index	 = descriptors[r_src].end_index;
			dst++;
			l_src = r_src+1;
			r_src = l_src+1;
		}
		//	it is possible that there was an odd number of blocks
		if (l_src < num_descriptors) {
			result_count++;
			descriptors[dst++] = descriptors[l_src];
		}
		//	set the blocks in the left half to be type 'A_BLOCK'
		for (dst = 0; dst < result_count/2; dst++) {
			descriptors[dst].type = BlockType::A_BLOCK;
		}
		//	set the blocks in the left half to be type 'B_BLOCK'
		for (; dst < result_count; dst++) {
			descriptors[dst].type = BlockType::B_BLOCK;
		}
		//	assign key values to each block
		assignKeys(array, descriptors, result_count);
		return result_count;
	}

//	/*	Merge blocks using binary searches to identify spans to rotate	*/
//	template <typename T>
//	SortMetrics mergeTwoAdjacentBlocksByRotation(T** array,
//													  array_size_t block_1_start,
//													  array_size_t block_1_end,
//													  array_size_t block_2_start,
//													  array_size_t block2_end);
//

	/*	********************************************************************* */
	/*	SortMetrics mergeAllBlocksLeftToRight(array, descriptors, num_desc);  */
	/*	********************************************************************* */

	/*	 Moves through the blocks starting with block[0], building an ordered
	 * span in the left portion of the array.
	 *
	 * 	 It is important to remember when analyzing this algorithm that
	 * all of the elements in the A portion of the array are in order
	 * and all of the elements in the B portion of the array are in order.
	 * What remains to be done is to interleave segments of the B array
	 * into their correct place in the A array.
	 *
	 *   An A_Block that follows an A_Block will be in order
	 *   A  B_Block that follows a  B_BLock will be in order
	 *   An A_Block that follows a  B_Block will be in order
	 *     (Because B's key is the last element,
	 *      and A's key is the first element)
	 *   A  B_Block that follows an A_Block may need to be merged.
	 *
	 * Consider 	A_Block containing { A, B, G, H } 	whose key is 'A'
	 *followed by	B_Block containing { C, D, E, F }   whose key is 'F'
	 *
	 * While the two block's keys are in order { A, F }, their elements
	 * need to be interleaved.
	 *
	 *   After a B_Block is merged with the preceding A_Block(s), the portion
	 * of the up to where the last element of the B_Block ended up is in order
	 *
	 * Consider:
	 *	[A0        ][A1        ][        B0][        B1]	keys
	 * 	 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
	 *   A  B  H  I  J  K  L  M  C  D  E  F  G  N  O  P		A, J, F, P
	 *
	 *	The first swap will swap B0='F' with A1='G'
	 *	[A0        ][        B0][A1        ][        B1]
	 * 	 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
	 *   A  B  H  I  C  D  E  F  J  K  L  M  G  N  O  P		A, F, J, P
	 *
	 *	A call to "merge(array, [0:3], [4:7])" will produce
	 *	[A0  ][        B0][A0  ][A1       ][         B1]
	 * 	 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
	 *   A  B  C  D  E  F  H  I  J  K  L  M  G  N  O  P
	 *
	 *  Note that everything to the left of, including, the highest
	 *    element from B0, 'F' at position 5, is in order.
	 *    Further note that the 'G' in block B1 at position 12 will
	 *    need to be merged into the array at position 6.
	 *    Thus, 'last_ordered' is position 5
	 *
	 *	(In some author's discussion, the portion of the array up to
	 *	 position is called the 'Mega-block' or M_Block)
	 *
	 *  The next merge operation, which will combine Block B1
	 *  with the previous portion of the array, only needs to merge
	 *  	A[6:11] = { H .. M } with B[12:15] = { G, N, O, P }
	 *
	 *	Thus, it is important to keep track of where the highest element from
	 *	the most recent B_Block merge is now located.  The element after that
	 *	highest element is the left start of the next 'B after A merge'.
	 */

	template <typename T>
	SortMetrics mergeAllBlocksLeftToRight(T** array,
										   Descriptors<T> &block_descriptors,
										   int num_blocks) {

		BlockOperations::MergeStrategy merge_strategy =
				BlockOperations::MergeStrategy::TABLE;

		array_size_t (*mergeBlocks)(T** array,
									array_size_t block_1_start,
									array_size_t block_1_end,
									array_size_t block_2_start,
									array_size_t block_2_end,
									SortMetrics &metrics);


		switch(merge_strategy) {
		case BlockOperations::MergeStrategy::AUXILLIARY:
			mergeBlocks =
				BlockOperations::mergeTwoBlocksElementsUsingAuxiliaryBuffer;
			break;
		case BlockOperations::MergeStrategy::BINARY:
			mergeBlocks	=
				BlockOperations::mergeTwoAdjacentBlocksBy_Rotation_BinarySearch;
			break;
		case BlockOperations::MergeStrategy::HYBRID:
			mergeBlocks =
				BlockOperations::mergeTwoAdjacentBlocksBy_Rotation_Hybrid;
			break;
		case BlockOperations::MergeStrategy::INSERTION:
			mergeBlocks =
				BlockOperations::insertionSortPartial;
			break;
		case BlockOperations::MergeStrategy::RGT_TO_LFT:
			mergeBlocks =
				BlockOperations::mergeTwoAdjacentBlocksBy_Rotation_RightToLeft;
			break;
		case BlockOperations::MergeStrategy::TABLE:
		default:
			mergeBlocks =
				BlockOperations::mergeTwoBlocksElementsByTable;
			break;
		}


		enum class ParsingState {
			A_LOOKING_FOR_B,
			B_LOOKING_FOR_A
		};

		SortMetrics metrics(0,0);

		//	ordered_end is the position of the largest b_element in the
		//	portion of the array to the left of where the search for a B->A
		//	transition is in progress  a.k.a - the end of the Mega-block
		array_size_t ordered_end;
		array_size_t first_b_element;
		array_size_t previous_blocks_end;
		int			 block_i;
		ParsingState parsing_state;
		bool		 a_block_seen_previously;

		if (block_descriptors[0].type == BlockType::B_BLOCK) {
			a_block_seen_previously = false;
			first_b_element = block_descriptors[0].start_index;
			parsing_state = ParsingState::B_LOOKING_FOR_A;
		} else {
			a_block_seen_previously = true;
			parsing_state = ParsingState::A_LOOKING_FOR_B;
		}

		block_i 			= 1;
		ordered_end			= block_descriptors[0].start_index-1;
		previous_blocks_end = block_descriptors[0].end_index;

		while (block_i < num_blocks) {
			switch(parsing_state) {
			case ParsingState::A_LOOKING_FOR_B:
				if (block_descriptors[block_i].type == BlockType::B_BLOCK) {
					first_b_element = block_descriptors[block_i].start_index;
					parsing_state 	= ParsingState::B_LOOKING_FOR_A;
				}
				break;
			case ParsingState::B_LOOKING_FOR_A:
				if (block_descriptors[block_i].type == BlockType::A_BLOCK) {
					if (a_block_seen_previously) {
						array_size_t start 	= ordered_end + 1;
						array_size_t mid	= first_b_element;
						array_size_t end	= previous_blocks_end;
						ordered_end =
							mergeBlocks( array, start, mid-1, mid, end, metrics);
					}
					a_block_seen_previously = true;
					parsing_state 			= ParsingState::A_LOOKING_FOR_B;
				}
				break;
			}
			previous_blocks_end = block_descriptors[block_i].end_index;
			block_i++;
		}

		//	If the final block was a B_Block, ensure that it gets merged
		//	with any unordered portions of the array to its left
		if (block_descriptors[num_blocks-1].type == BlockType::B_BLOCK) {
			//	Although unlikely, it is possible that this function gets
			//	called with all of the block types being B_Blocks
			if (a_block_seen_previously) {
				array_size_t start	= ordered_end + 1;
				array_size_t mid	= first_b_element;
				array_size_t end	= block_descriptors[num_blocks-1].end_index;
				BlockOperations::mergeTwoBlocksElementsByTable(array,
																start, mid-1,
																mid, end,
																metrics);
			}
		}
		return metrics;
	}


	//	starting at the right-most block, merge the previous (left) block into
	//	  the elements to the right
	template <typename T>
	SortMetrics mergeAllBlocksRightToLeft(T** array,
									std::unique_ptr<BlockDescriptor<T>[]> &block_descriptors,
									int num_blocks) {

		SortMetrics metrics;
		array_size_t sorted_span_start 	= block_descriptors[num_blocks-1].start_index;
		array_size_t sorted_span_end		= block_descriptors[num_blocks-1].end_index;
		int left_block = num_blocks-2;

		while (left_block >= 0) {
			array_size_t left_start 	= block_descriptors[left_block].start_index;
			array_size_t left_end	= block_descriptors[left_block].end_index;
			BlockOperations::mergeTwoBlocksElementsByTable(array,
											left_start, left_end,
											sorted_span_start, sorted_span_end,
											metrics);
			//	all of the elements [left.start:size-1] are now in order
			sorted_span_start = left_start;
			//	move the block pointer to the next block to the left
			left_block--;
		}
		return metrics;
	}


	/*
	 * 	ComparesAndMoves sortBlocksBinarySearch(array, size, descriptors, num_desc);
	 *
	 *  This version searches the B_Blocks for the largest value smaller than A_Block
	 *  	on every pass by performing a binary search on the span of unexamined B_Blocks.
	 *
	 *	Finding the first B_Block that needs to be rotated left is log2(n) where n is the number
	 *		of B_Blocks left unexamined
	 *
	 *	Each successive search is worst case log2(m), where 'm' is at least 1 block less than
	 *		the previous 'n'. However, it is likely that the next B_Block that needs to be rotated left
	 *		is close to the B_Block that was just rotated, if the underlying array is well disordered
	 *		Thus, the log2() penalty is paid every pass through	the algorithm.
	 *
	 *	Testing indicates that restarting the binary search each time makes this algorithm
	 *		less efficient than starting at the rightmost B_Block and moving left one block at a time.
	 */


	template <typename T>
	SortMetrics sortBlocksBinarySearch(	T **array,
										Descriptors<T> &blocks,
										int num_blocks) {

		constexpr bool debug_verbose = false;

		#pragma push_macro("_debug")
		#define _debug(_dbg_msg_) do {\
			if (debug_verbose) {\
				std::cout << _dbg_msg_ << std::endl;\
			}\
		} while(false)

		_debug("sortBlocksBinarySearch()");

		SortMetrics result(0,0);

//		constexpr array_size_t binary_search_done = smaller_block_not_found;

		//	if the array consists of all A_Blocks, we are done
		if (blocks[num_blocks-1].type == BlockType::A_BLOCK) {
			_debug("All blocks are A Blocks");
			return result;
		}

		// 	if the array consists of all B_Blocks, we are done
		if (blocks[0].type == BlockType::B_BLOCK) {
			 _debug("All blocks are B Blocks");
			return result;
		}

		bool tagsAreSorted = true;
		for (int i = 1; i != num_blocks; i++) {
			result.compares++;
			if (blocks[i-1] > blocks[i]) {
				tagsAreSorted = false;
				break;
			}
		}

		if (tagsAreSorted) {
			_debug("...Tags are initially sorted\n");
			return result;
		} else {
			_debug("...Tags are not sorted");
		}

		// 	process every A_Block starting with the blocks A[i = 7] & B[j = 14]
		//	for this example, imagine that A[m] is > B[0], B[1] but <= B[2]
		//									0  1    6    7  8  9  10     14
		//	 								A0 A1 ...    Am B0 B1 B2 ....Bn
		// 		yielding 					A0 A1 . Am-1 B0 B1 Am B2 ... Bn
		//	 	A[m] ... B[n] are now in order and i = 7 while j = 9
		//		update i = 6 (Am-1) and j = 8 (B1)
		//	continue until j == i or i < 0

		int num_a_blocks = 0;
		for (int i = 0; i != num_blocks; i++) {
			if (blocks[i].type == BlockType::A_BLOCK)
				num_a_blocks++;
			else
				break;
		}

		array_size_t a_block_index  = num_a_blocks-1;
		array_size_t b_block_start  = num_a_blocks;
		array_size_t b_block_end	= num_blocks-1;

		while (a_block_index >= 0) {

			//	Binary search for the smallest B_Block that is > this A_Block
			//	There may not be a B_Block that is less than this A_Block

			//	consider the following sequence of blocks, where the number
			//	  after the block type indicates the block's key
			//			0    1    2    3    4    5    6    7
			//					  ai   b_st                b_end
			//			A4,  A6,  A6,  B5,  B5,  B7,  B7,  B8
			//	will return 5 for the leftmost block (aka 'lm_b') because the block
			//	in position 5 has a key of '7', which is > block 2's key of '6'

			// 	The index of the leftmost b_block that is greater than
			// the block at 'a_block_index'
			array_size_t lm_b;
			result += binarySearchLastBlock(blocks, b_block_start, b_block_end,
											blocks[a_block_index].key,
											lm_b);


			//	If all B_Blocks are greater than this A_Block, the blocks are in order
			//	    ai, st == lmb = 3
			//	    2   3
			//	... A2, B3 ...
			if (lm_b == b_block_start) {
				break;
			}

			//			A4, A6, A6, B5, B5, B7, B7, B8
			//			0    1    2    3    4    5    6    7
			//					  ai   b_st      lm_b      b_end
			//			A4,  A6,  A6,  B5,  B5,  B7,  B7,  B8

			//	Determine how many of the A_Blocks to the left of the current
			//	  A_Blocks are also smaller than the leftmost b_block, 'lm'
			//    In this example, blocks 1 & 2 with keys '6' are greater than
			//	  block 4 with key '5'. leftmost block (aka 'lmb') will be 1

			//	The index of the leftmost a_block that is greater than
			// the rightmost b_block [lm_b-1] that is going to be rotated left
			array_size_t lm_a;
			result += binarySearchLastBlock(blocks, 0, a_block_index,
											 blocks[lm_b-1].key,
											 lm_a);

			//			A4, A6, A6, B5, B5, B7, B7, B8
			//			0    1    2    3    4    5    6    7
			//				 lm_a ai   b_st      lm_b      b_end
			//			A4,  A6,  A6,  B5,  B5,  B7,  B7,  B8

			//	rotate_left_count = 2 (rotate left 2 places)
			int rotate_left_count = (b_block_start - lm_a);

			if (debug_verbose) {
				array_size_t array_size = blocks[num_blocks-1].end_index+1;
				array_size_t v = blocks[num_a_blocks].start_index;
				std::cout << "BEFORE: "
						  << " lm_b = " << std::setw(2) << lm_b
						  << " lm_a = " << std::setw(2) << a_block_index
						  << " rotate left by " << std::setw(2) << rotate_left_count
						  << "\n"
						  << blockSortToString(array, array_size, v, blocks, num_blocks)
						  << std::endl;
			}
			result += rotateBlocksRight(array, blocks,
										lm_a, lm_b-1,
										-rotate_left_count);

			//			A4, A6, A6, B5, B5, B7, B7, B8
			//			0    1    2    3    4    5    6    7
			//				 lm_a ai   b_st      lm_b      b_end
			//			A4,  B5   B5   A6,  A6,  B7,  B7,  B8
			//	rotate_left_count = 2

			//	The right most b_block that has been moved is the block
			//	  that was to the left of 'lm_b'
			b_block_end = lm_b-1 - rotate_left_count;
			//	The left most b_block is now in the position lm_a
			b_block_start = lm_a;
			//	The evaluation of a_blocks should start with the A_Block to
			//	the left of the left-most b_block
			a_block_index = b_block_start-1;

			//			A4, A6, A6, B5, B5, B7, B7, B8
			//			0    1    2    3    4    5    6    7
			//			ai	 b_st b_end
			//			A4,  B5   B5   A6,  A6,  B7,  B7,  B8

			if (debug_verbose) {
				array_size_t array_size = blocks[num_blocks-1].end_index+1;
				array_size_t v = blocks[num_a_blocks].start_index;
				std::cout << "AFTER: \n"
						  << blockSortToString(array, array_size, v, blocks, num_blocks)
						  << std::endl;
			}
		}

		if (debug_verbose) {
			array_size_t array_size = blocks[num_blocks-1].end_index+1;
			array_size_t v = blocks[num_a_blocks].start_index;
			std::cout << "EXITING: \n"
					  << blockSortToString(array, array_size, v, blocks, num_blocks)
					  << std::endl;
			if (areBlocksSorted(blocks, num_blocks)) {
				_debug("Tags are sorted");
			} else {
				_debug("Tags are NOT sorted");
			}
		}

		_debug("sortBlocksBinarySearch() took " << result << "\n");
		return result;
		#pragma pop_macro("_debug")
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
	SortMetrics sortBlocksByKey(T** array, array_size_t size,
									 Descriptors<T> &blocks,
									 int start, int end)
	{
		SortMetrics metrics;

		// insertion sort the A_Blocks
		for (int i = start+1; i <= end; i++) {
			for (int j = i; j > 0; j--) {
				metrics.compares++;
				if (*blocks[j].key >= *blocks[j-1].key) {
					break;
				}
				metrics += rotateBlocksRight(array, blocks, j-1, j, 1);
			}
		}
		return metrics;
	}


	/*
	 * 	ComparesAndMoves sortBlocksByTable(array, block_descriptors, num_blocks)
	 *
	 *	This only works on an array where all of the blocks are the same size
	 *	except for (possibly) the final (rightmost) block.
	 *
	 * 	  Sorts the array using the block descriptors.  The descriptors themselves
	 * 	are sorted using an in-place MergeSort that has a table to keep track of
	 * 	where elements that are swapped out of place are moved to.
	 *
	 * 	  At any given time, the table contains a pointer / index to the location
	 * 	of the next element to be merged from the A_Blocks.  The table's index,
	 * 	'i' is used.
	 *
	 * 	  If an A_Block is moved to make was for a dst block, the A_Block's
	 * 	entry in the table must be updated, which is not O(n)
	 *
	 *	indices 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
	 *  values  D   E   F   G   H   I   S   T   X   A   B   C   M   N   R   P
	 * 	blocks  |A0        ||A1        ||A2        ||      B0||        B1||B2|
	 *
	 *	Summarizing Block:Key pairs, i is the index into the A positions table
	 *	       Block Descriptors            Table      a =
	 *    0    1    2    3    4    5      A0 A1 A2  i  [i]  b   dst   AKey BKey
	 *	  A0:D A1:G A2:S B0:C B1:R B2:P   0  1  2   0   0   3    0    :D > :A
	 *	  B0:A A1:6 A2:S A0:D B1:R B2:P   3  1  2   0   3   4    1    :D <=:O
	 *	  B0:A AO:D A2:S A1:G B1:R B2:P   x  3  2   1   3   4    2    :G <=:O
	 *	  B0:A AO:D A1:G A2:S B1:R B2:P   x  x  3   2   3   4    3    :S > :O
	 *	  B0:A AO:D A1:G B1:R A2:S B2:P   x  x  4   2   4   5    4    :S <=:P
	 *	  B0:A AO:D A1:G B1:R B2:P A2:S   x  x  x   3   x   5    5    complete
	 */

	template <typename T>
	SortMetrics sortBlocksByTable(T** array,
								  Descriptors<T> &descriptors,
								  int num_blocks) {

		SortMetrics result(0,0);

		//	if all the descriptors are A_Blocks, they do not need to be sorted
		if (descriptors[num_blocks-1].type == BlockType::A_BLOCK) {
			return result;
		}

		//	if all the descriptors are B_Blocks, they do not need to be sorted
		if (descriptors[0].type == BlockType::B_BLOCK) {
			return result;
		}
		//	Count the number of A Blocks
		array_size_t num_A_blocks = 0;

		for (array_size_t i = 0; i != num_blocks; i++) {
			if (descriptors[i].type != BlockType::A_BLOCK) {
				break;
			}
			num_A_blocks++;
		}

		//	Build a table of A_Block indices
		array_size_t a_positions[num_A_blocks];
		for (array_size_t i = 0; i != num_A_blocks; i++) {
			a_positions[i] = i;
		}

		//	Create source & destination indices

		array_size_t table_i 	= 0;
		array_size_t b_source 	= num_A_blocks;
		array_size_t dst			= 0;
		array_size_t src			= 0;

		while (table_i != num_A_blocks && b_source != num_blocks) {

			result.compares++;
			//	determine the smaller block, with deference given to the
			//	  block on the left (A_Block) if the two blocks have equal
			//	  keys in order to preserve stability
			if (*descriptors[a_positions[table_i]].key <= *descriptors[b_source].key) {
				src = a_positions[table_i];
				table_i++;
			} else {
				src = b_source;
				b_source++;
			}

			//	if the block that goes here is already in-place, move on
			if (src == dst) {
				dst++;
				continue;
			}

			result += swapBlocks(array, descriptors, dst, src);

			for (array_size_t i = table_i; i < num_A_blocks; i++) {
				//	If there was an A_Block in the table that was at
				//	  position 'dst', it has been swapped to 'src'.
				//    If that block at dst was in the table, it is now at src
				if (a_positions[i] == dst) {
					a_positions[i] = src;
					break;
				}
			}
			dst++;
		}

		//	  If there are any remaining A_Blocks,
		//	move them within the array back to in order
		while (table_i != num_A_blocks) {
			src = a_positions[table_i];
			table_i++;
			if (dst != src) {
				result += swapBlocks(array, descriptors, dst, src);
				//	The A_Block that was at 'dst' has now been swapped to 'src'
				//  Update that block's entry in the table
				for (array_size_t i = table_i; i < num_A_blocks; i++) {
					if (a_positions[i] == dst) {
						a_positions[i] = src;
						break;
					}
				}
			}
			dst++;
		}

		return result;
	}

		/*
		 *  Hybrid - this version initially searches for the first B_Block smaller
		 * 	  than the largest (rightmost) A_Block	using a Binary Search,
		 * 	  but then uses a Right To Left strategy for successive blocks
		 *    b/c the likelihood is that the next B_Block to the left to be
		 *    that is less than the next A_Block is probably close to where
		 *    the previous B_Block was.
		 */
		template <typename T>
		SortMetrics sortBlocksHybrid(T **array,
									 Descriptors<T> &blocks, int num_blocks)
		{
			constexpr bool debug_verbose = false;

			#pragma push_macro("_debug")
			#define _debug(_dbg_msg_) do {\
				if (debug_verbose) {\
					std::cout << _dbg_msg_ << std::endl;\
				}\
			} while(false)

			_debug("sortBlocksHybridSearch()");

			SortMetrics result(0,0);

			//	if the array consists of all A_Blocks, we are done
			if (blocks[num_blocks-1].type == BlockType::A_BLOCK) {
				_debug("All blocks are A Blocks");
				return result;
			}

			// 	if the array consists of all B_Blocks, we are done
			if (blocks[0].type == BlockType::B_BLOCK) {
				 _debug("All blocks are B Blocks");
				return result;
			}

			bool blocks_are_sorted = true;
			for (int i = 1; i != num_blocks; i++) {
				result.compares++;
				if (blocks[i-1] > blocks[i]) {
					blocks_are_sorted = false;
					break;
				}
			}

			if (blocks_are_sorted) {
				_debug("...Blocks are initially sorted\n");
				return result;
			} else {
				_debug("...Blocks are not sorted");
			}

			array_size_t a_block_index = 0;
			array_size_t b_block_index = 0;

			//	find the first B_Block
			while (b_block_index < num_blocks && blocks[b_block_index].type == BlockType::A_BLOCK) {
				b_block_index++;
			}
			a_block_index = b_block_index-1;

			array_size_t debug_middle_index = b_block_index;

			bool use_binary_search = true;

			while (a_block_index >= 0) {

				if (debug_verbose) {
					std::cout << "BEGINNING LOOP TO USE "
							<< (use_binary_search ? "BINARY_SEARCH\n" : "RIGHT_TO_LEFT\n");
				}
				if (!use_binary_search) {
					//	Find the rightmost B_Block that is less than this A_Block
					while (b_block_index > a_block_index) {
						result.compares++;
						if (*blocks[b_block_index].key < *blocks[a_block_index].key) {
							break;
						}
						b_block_index--;
					}
					//	if no B_Blocks were found that are less than this A_Block
					//	the tags are in order
					if (a_block_index == b_block_index) {
						break;
					}
				} else {
					use_binary_search = false;
					//	Binary search for the largest B_Block that is less than this A_Block
					//	There may not be a B_Block that is less than this A_Block

					//	0	 1	  2	   3	4	 5	  6    7
					//			  ai   bi                  num-1
					//	A=4, A=6, A=7, B=5, B=5, B=6, B=7, B=8
					//	b_block_index is passed by value as the start of the B_Block span
					//	b_block_index is updated by reference as the result of the binary search
					array_size_t binary_search_result;
					result += binarySearchLastBlock(blocks, b_block_index, num_blocks-1,
													blocks[a_block_index].key, binary_search_result);

					//	Descriptor[binary_search_result] is the left most block
					//	that is greater than descriptor[a_index], which is '6'
					//	0	 1	  2	   3	4	 5	  6    7
					//			  ai                  \/
					//	A=4, A=6, A=7, B=5, B=5, B=6, B=7, B=8

					//	There may be no B_Block that is greater than this A_Block
					//	0	 1	  2	   3	4	 5	  6    7
					//			  ai
					//	A=4, A=6, A=9, B=1, B=2, B=3, B=6, B=8
					//	In this case, restart the sort in the Right_To_Left mode
					if (binary_search_result > (num_blocks-1)) {
						b_block_index = num_blocks-1;
						continue;
					}

					//	  Update b_block_index to point to the largest B_Block
					//	that is < a_source
					b_block_index = binary_search_result-1;
					if (debug_verbose) {
						array_size_t debug_array_size =
							blocks[num_blocks-1].end_index -
								blocks[0].start_index + 1;

						std::cout << "After binary search "
								  << " a_i = " << a_block_index
								  << " b_i = " << b_block_index
								  << "\n"
								  << blockSortToString(array, debug_array_size, -1,
													   blocks, num_blocks)
								  << std::endl << std::endl;
					}
				}

				// there will be at least one A_Block that is rotated left from
				//	the left side over to the right-hand side of the B_Block < [a_block_index]
				int tag_rotate_count = -1;

				//	continue looking through the A_Blocks to the left for
				//	  A_Blocks that are less than [b_block_index] which
				//	  can also be included in the rotate count
				a_block_index--;

				while (a_block_index >= 0) {
					result.compares++;
					if (*blocks[a_block_index].key <= *blocks[b_block_index].key)
						break;
					a_block_index--;
					tag_rotate_count--;
				}

				if (debug_verbose) {
					array_size_t array_size = blocks[num_blocks-1].end_index+1;
					array_size_t v = blocks[debug_middle_index].start_index;
					std::cout << "BEFORE: "
							  << " b_index (end) = " << std::setw(2) << b_block_index
							  << " a_index+1 (start) = " << std::setw(2) << a_block_index+1
							  << " rotation count = " << std::setw(2) << tag_rotate_count
							  << "\n"
							  << blockSortToString(array, array_size, v, blocks, num_blocks)
							  << std::endl;
				}
				// At this point 'a_block_index' points to the largest A_Block
				//	that is <= [b_block_index],  but [a_block_index+1] is greater
				//	than [b_block_index] b/c the previous while loop exited
				//	because a [b_block_index] that was smaller was found, and
				//	the comparison b_block_index == a_block_index return false
				result += rotateBlocksRight(array, blocks,
											a_block_index+1, b_block_index,
											tag_rotate_count);
				// the B_Block has been shifted left by 'tag_rotate_count'
				b_block_index += tag_rotate_count;
				// a_block_index points to the block to the left of the span
				//	that was rotated, and the span that was rotated is now in order

				if (debug_verbose) {
					array_size_t array_size = blocks[num_blocks-1].end_index+1;
					array_size_t v = blocks[debug_middle_index].start_index;
					std::cout << "AFTER: \n"
							  << blockSortToString(array, array_size, v, blocks, num_blocks)
							  << std::endl;
					std::cout << "LOOP COMPLETE" << std::endl << std::endl;
				}
			}

			if (debug_verbose) {
				array_size_t array_size = blocks[num_blocks-1].end_index+1;
				array_size_t v = blocks[debug_middle_index].start_index;
				std::cout << "EXITING: \n"
						  << blockSortToString(array, array_size, v, blocks, num_blocks)
						  << std::endl;
				if (areBlocksSorted(blocks, num_blocks)) {
					_debug("Tags are sorted");
				} else {
					_debug("Tags are NOT sorted");
				}
			}

			_debug("sortBlocksBinarySearch() took " << result << "\n");
			return result;
			#pragma pop_macro("_debug")

			return result;
		}


	/*	sort an array of [A_0..A_m:B_0..B_n] using an insertion sort			*/
	template <typename T>
	SortMetrics sortBlocksInsertion(T **array, Descriptors<T> &blocks, int num_blocks) {
		SortMetrics metrics;
		return metrics;
	}

	/*
	 * ComparesAndMoves	sortBlocksRightToLeft(array, size, descriptors, num_descriptors);
	 *
	 * RightToLeft - this version searches the B_Blocks by starting at the last B_Block on the right
	 * 				 and moving through the B_Blocks until either
	 * 				 	the B_Block pointer == A_Block pointer which indicates that there are
	 * 				 		no B_Blocks that are less than the largest remaining A_Block
	 * 				 or the A_Block pointer == -1, which indicates there are no A_Blocks
	 * 				 		that are larger than the smallest B_Block
	 *
	 * 	Once the first B_Block that needs to be rotated left is found, successive searches for
	 * 		B_Blocks to rotate will probably be efficient as long as the next B_Block to be found
	 * 		is close by to the left of the B_Block that was just rotated
	 *
	 * 	Finding the first B_Block to rotate can be expensive since the algorithm starts from the
	 * 		last B_Block on the right and moves one-at-a-time to the left
	 */

	template <typename T>
	SortMetrics sortBlocksRightToLeft(T **array,
									  Descriptors<T> &blocks, int num_blocks) {
		constexpr bool debug_verbose = false;

		#pragma push_macro("_debug")
		#define _debug(_dbg_msg_) do {\
			if (debug_verbose) {\
				std::cout << _dbg_msg_ << std::endl;\
			}\
		} while(false)

		_debug("sortBlocksRightToLeft()");

		SortMetrics result(0,0);

		//	if the array consists of all A_Blocks, we are done
		if (blocks[num_blocks-1].type == BlockType::A_BLOCK) {
			_debug("All blocks are A Blocks");
			return result;
		}

		// 	if the array consists of all B_Blocks, we are done
		if (blocks[0].type == BlockType::B_BLOCK) {
			 _debug("All blocks are B Blocks");
			return result;
		}

		int num_a_blocks = 0;
		for (int i = 0; i != num_blocks; i++) {
			if (blocks[i].type == BlockType::A_BLOCK)
				num_a_blocks++;
			else
				break;
		}

		bool tagsAreSorted = true;
		for (int i = 1; i != num_blocks; i++) {
			result.compares++;
			if (blocks[i-1] > blocks[i]) {
				tagsAreSorted = false;
				break;
			}
		}

		if (tagsAreSorted) {
			_debug("...Blocks are initially sorted\n");
			return result;
		} else {
			_debug("...Blocks are not sorted");
		}

		// 	process every A_Block starting with the blocks A[i = 7] & B[j = 14]
		//	for this example, imagine that A[m] is > B[0], B[1] but <= B[2]
		//									0  1    6    7  8  9  10     14
		//	 								A0 A1 ...    Am B0 B1 B2 ....Bn
		// 		yielding 					A0 A1 . Am-1 B0 B1 Am B2 ... Bn
		//	 	A[m] ... B[n] are now in order and i = 7 while j = 9
		//		update i = 6 (Am-1) and j = 8 (B1)
		//	continue until j == i or i < 0
		array_size_t a_block_index = num_a_blocks-1;
		array_size_t b_block_index = num_blocks-1;
		while (a_block_index >= 0) {

			//	Find first B_Block that is less than this A_Block
			//	There may not be a B_Block that is greater than this A_Block
			while (b_block_index > a_block_index) {
				result.compares++;
				if (*blocks[b_block_index].key < *blocks[a_block_index].key) {
					break;
				}
				b_block_index--;
			}

			//	if no B_Blocks were found that are less than this A_Block
			//	the tags are in order
			if (a_block_index == b_block_index) {
				break;
			}


			// there will be at least one A_Block that is rotated left
			//	 to the right-hand side of the B_Block < [a_block_index]
			int tag_rotate_count = -1;

			//	continue looking through the A_Blocks to the left for
			//	  A_Blocks that are less than [b_block_index] which
			//	  can also be included in the rotate count
			a_block_index--;

			//	con
			while (a_block_index >= 0) {
				result.compares++;
				if (*blocks[a_block_index].key <= *blocks[b_block_index].key)
					break;
				a_block_index--;
				tag_rotate_count--;
			}

			if (debug_verbose) {
				array_size_t array_size = blocks[num_blocks-1].end_index+1;
				array_size_t v = blocks[num_a_blocks].start_index;
				std::cout << "BEFORE: "
						  << " b_index (end) = " << std::setw(2) << b_block_index
						  << " a_index+1 (start) = " << std::setw(2) << a_block_index+1
						  << " rotation count = " << std::setw(2) << tag_rotate_count
						  << "\n"
						  << blockSortToString(array, array_size, v, blocks, num_blocks)
						  << std::endl;
			}
			// At this point 'a_block_index' points to the largest A_Block
			//	that is <= [b_block_index],  but [a_block_index+1] is greater
			//	than [b_block_index] b/c the previous while loop exited
			//	because a [b_block_index] that was smaller was found, and
			//	the comparison b_block_index == a_block_index return false
			result += rotateBlocksRight(array, blocks,
										a_block_index+1, b_block_index,
										tag_rotate_count);
			// the B_Block has been shifted left by 'tag_rotate_count'
			b_block_index += tag_rotate_count;
			// a_block_index points to the block to the left of the span
			//	that was rotated, and the span that was rotated is now in order

			if (debug_verbose) {
				array_size_t array_size = blocks[num_blocks-1].end_index+1;
				array_size_t v = blocks[num_a_blocks].start_index;
				std::cout << "AFTER: \n"
						  << blockSortToString(array, array_size, v, blocks, num_blocks)
						  << std::endl;
			}
		}

		if (debug_verbose) {
			array_size_t array_size = blocks[num_blocks-1].end_index+1;
			array_size_t v = blocks[num_a_blocks].start_index;
			std::cout << "EXITING: \n"
					  << blockSortToString(array, array_size, v, blocks, num_blocks)
					  << std::endl;
			if (areBlocksSorted(blocks, num_blocks)) {
				_debug("Tags are sorted");
			} else {
				_debug("Tags are NOT sorted");
			}
		}

		_debug("sortBlocksRightToLeft() took " << result << "\n");
		return result;
		#pragma pop_macro("_debug")
	}

}	// namespace BlockSort{}

#endif /* BLOCKSORTBLOCKSORTANDMERGE_H_ */
