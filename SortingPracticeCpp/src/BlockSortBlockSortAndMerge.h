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

	/*	Starting with the rightmost B_Block, merge blocks to the left	*/
	template <typename T>
	SortMetrics mergeAllBlocksRightToLeft(T** array,
											   Descriptors<T> &block_descriptors,
											   int num_blocks);

	/*	Merge blocks using binary searches to identify spans to rotate
	 * Although not strictly necessary, 'block_1_end' is passed to be consistent
	 * with the other block merging functions	*/
	template <typename T>
	SortMetrics mergeTwoAdjacentBlocksByRotation(T** array,
													  index_t block_1_start,
													  index_t block_1_end,
													  index_t block_2_start,
													  index_t block2_end);

	/*	Merge a pair of blocks by using a table to keep track of where displaced
	 * elements in block_1 are moved to in block_2 during the merge. */
	template <typename T>
	SortMetrics mergeTwoBlocksByTable(T ** array,
									  index_t block_1_start, index_t block_1_end,
									  index_t block_2_start, index_t block_2_end);

	/*	sort an array of [start:end] by key, ignoring block type	*/
	template <typename T>
	SortMetrics sortBlocksByKey(T** array, index_t size, Descriptors<T>&,
												int start, int end);

	/*	sort an array of [A_0..A_m:B_0..B_n] using a binary search to find the
	 * 	location where each of the blocks should go 							*/
	template <typename T>
	SortMetrics sortBlocksBinarySearch(T **array, index_t size,
											Descriptors<T> &blocks, int num_tags);

	/*	sort an array of [A_0..A_m:B_0..B_n] using a table to keep track of where
	 * 	the displaced A_Blocks go in the upper portion (B_Block) of the array	*/
	template <typename T>
	SortMetrics sortBlocksByTable(		T** array,
									   	  	Descriptors<T> &descriptors, int num_blocks);

	/*	sort an array of [A_0..A_m:B_0..B_n] using a binary search to find the
	 * 	location where each of the blocks should go 							*/
	template <typename T>
	SortMetrics sortBlocksHybrid(		T **array, index_t size,
											Descriptors<T> &blocks, int num_blocks);

	/*	sort an array of blocks [A_0..A_m:B_0..Bn] starting at right A_m & B_n	*/
	template <typename T>
	SortMetrics sortBlocksRightToLeft(	T **array, index_t size,
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

	/*	Merge blocks using binary searches to identify spans to rotate	*/
	template <typename T>
	SortMetrics mergeTwoAdjacentBlocksByRotation(T** array,
													  index_t block_1_start,
													  index_t block_1_end,
													  index_t block_2_start,
													  index_t block2_end);


	/*
	 * 	mergeBlocksByTable(	array,
	 * 						block_1_start, block_1_end,
	 * 						blocK_2_start, block_2_end)
	 *
	 * 	This function merges the array elements from [b1_start:b1_end] with
	 * 	  the array elements from [b2_start:b2_end] by using a table to keep
	 * 	  track of where the elements from [b1_start:b1_end] get moved as the
	 * 	  merge proceeds.
	 *
	 * 	It is assumed that the values within each block are in ascending order.
	 *
	 * 	Note that this algorithm does not require the blocks to be contiguous, nor
	 * 	 does it require the size of block_1 to be less than the size of block_2
	 *
	 * 	The algorithm creates a table of where each element from block_1
	 * 	  moves to if it is swapped out of its position in the array.
	 * 	  This table of array indices allows the algorithm to only require
	 * 	  one temporary location for an array element during the swap.  However, it
	 * 	  requires the space necessary for table of indices that initially has
	 * 	  a quantity of elements equal to the size of the block_1
	 *
	 *	Note that updating the table of block_1 indices takes an element-wise search
	 *	  through the table to determine which where a displaced block_1 element is
	 *	  stored in the table.  This is denoted in the following example by t[x] = t[t_ptr]
	 *	  where the block_1 element at 'dst' was stored in table at table index 'x'.
	 *	  Note in the example that block_1 elements often get displaced multiple times.
	 *	  The element in the table at table index [x] which was a 'dst' is now at:
	 *	  	b2_ptr if the source of the swap was an element from block_2,
	 *	  		which do not get displaced out of their original location during merging
	 *	  	t[tpr] if the source of the swap was an element originally in block_1,
	 *	  		which do often get displaced out of their original location during merging
	 *
	 *	In the following discussion
	 *		'b1s' is block_1_start		'b1e' is block_1_end
	 *		'b2s' is blocK_2_start		'b2e' is block_2_end
	 *		b1_ptr is the array index of the location of the next element from block_1
	 *		b2_ptr is the array index of the location of the next element from block_2
	 *		t_ptr  is the index into the table of where block_1 elements have been displaced
	 *		'-' indicates an index that is no longer part of the algorithm
	 *
	 * 	Consider the blocks "BDF" and "ACEG
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			    table 't'
	 * 	b1s     b1e     b2s         b2e  ||  dst  [ 0   1   2   ]    t_ptr  t[t_ptr]  b2_ptr
	 * [B   D   F  ]   [A   C   E   G  ] ||  m    [ m+0 m+1 m+2 ]    0      m+0       n+0
	 *
	 *  [t[t_ptr=0]=m+0] = 'B'  > [b2_ptr] = 'A' .... xchg(dst, b2_ptr);    t[0]=b2_ptr; b2_ptr++;
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			    table 't'
	 * 	b1s     b1e     b2s         b2e  ||  dst  [ 0   1   2   ]    t_ptr  t[t_ptr]  b2_ptr
	 * [A   D   F  ]   [B   C   E   G  ] ||  m+1  [ n+0 m+1 m+2 ]    0      n+0       n+1
	 *
	 *  [t[t_ptr=0]=n+0] = 'B' <= [b2_ptr] = 'C' .... xchg(dst, t[t_ptr]);  t[x=1]=t[t_ptrr]; t_ptr++;
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			    table 't'
	 * 	b1s     b1e     b2s         b2e  ||  dst  [ 0   1   2   ]    t_ptr  t[t_ptr]  b2_ptr
	 * [A   B   F  ]   [D   C   E   G  ] ||  m+2  [ -   n+0 m+2 ]    1      n+0       n+1
	 *
	 *  [t[t_ptr=1]=n+0] = 'D' >  [b2_ptr] = 'C' .... xchg(dst, b2_ptr);    t[x=2]=b2_ptr; b2_ptr++;
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			    table 't'
	 * 	b1s     b1e     b2s         b2e  ||  dst  [ 0   1   2   ]    t_ptr  t[t_ptr]  b2_ptr
	 * [A   B   C  ]   [D   F   E   G  ] ||  n+0  [ -   n+0 n+1 ]    1      n+0       n+2
	 *
	 * 	[t[tpr=1]=n+0] = 'D' <= [r_rptr] = 'E' ... dst == t[t_ptr=1], no swap occurred, t_ptr++
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			    table 't'
	 * 	b1s     b1e     b2s         b2e  ||  dst  [ 0   1   2   ]    t_ptr  t[t_ptr]  b2_ptr
	 * [A   B   C  ]   [D   F   E   G  ] ||  n+1  [ -   -   n+1 ]    2      n+1       n+2
	 *
	 * 	[t[t_ptr=2]=n+1] = 'F' <= [r_rptr] = 'E' ... xchng(dst, b2_ptr);    t[x=2]=b2_ptr; b2_ptr++
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			   table 't'
	 * 	b1s     b1e     b2s         b2e  ||  dst  [ 0   1   2   ]    t_ptr  t[t_ptr]  b2_ptr
	 * [A   B   C  ]   [D   E   F   G  ] ||  n+2  [ -   -   -   ]    3       -        n+3
	 *
	 * 	t_ptr has been exhausted because t_ptr = sizeof(table)),
	 * 		therefore all block_1 elements are in their correct location and all unexamined
	 * 		elements are block_2 elements which are in the originial, and correct, locations
	 *
	 *	The displacement of the block_1 elements, often multiple times, causes this algorithm
	 *	  to use more swaps than a merge sort that uses an auxilliary array, which has a
	 *	  guaranteed min & max number of swaps == nlog(n).  It was empirically observed
	 *	  that this algorithm is on the order of swap complexity of insertion sort.
	 *
	 * 	As an example of the number of displacements of block_1 elements that can occur,
	 * 	  consider the blocks "EFG" and "ABCD"
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3   dst      table            t_ptr  t[t_ptr] b2_ptr
	 * 	E   F   G  ...  A   B   C   D  ||  m+0  [ m+0 m+1 m+2 ]     0      [0]=m+0  n+0
	 * 	A   F   G  ...  E   B   C   D  ||  m+1  [ n+0 m+1 m+2 ]     0      [0]=n+0  n+1
	 * 	A   B   C  ...  E   F   G   D  ||  m+2  [ n+0 n+1 m+2 ]     0      [0]=n+0  n+2
	 * 	A   B   C  ...  D   F   G   E  ||  n+0  [ n+3 n+1 n+2 ]     0      [0]=n+0  n+3
	 * 	A   B   C  ...  D   F   G   E  ||  n+1  [ n+3 n+1 n+2 ]     0      [0]=n+3  n+4
	 *
	 * 	All of the right block values "A:D" have been placed b/c n+4 > right_end = n+3,
	 * 		but the left block values are no longer in sequence
	 *
	 * 	A   B   C  ...  D   F   G   E  ||  n+1  [ n+3 n+1 n+2 ]     0      [0]=n+3  -
	 * 	A   B   C  ...  D   E   G   F  ||  n+2  [ -   n+3 n+2 ]     1      [1]=n+3  -
	 * 	A   B   C  ...  D   E   F   G  ||  n+3  [ -   -   n+3 ]     2      [1]=n+3  -
	 * 	A   B   C  ...  D   E   F   G  ||  n+4  [ -   -   -   ]     3      -        -
	 *
	 * 	t_ptr= has exceeded sizeof(table)=2 and all right blocks are in place, done
	 *
	 * 									Algorithm:
	 * 	nextDestination(dst)
	 *		if (dst == block_1_end)	dst = block_2_start
	 *		else					dst++
	 *
	 * 	dst = block_1_start
	 * 	t_ptr = 0
	 * 	b2_ptr = blocK_2_start
	 * 	while (dst <= right_end) {
	 * 		b1_ptr = table[t_ptr]
	 *
	 * 		if (array[b1_ptr] <= array[b2_ptr] {
	 * 			// the element to be merged came from block_1
	 * 			if (dst != b1_ptr) {
	 * 			 	swap(dst, b1_ptr)
	 * 			 	//	the element block was from block_1
	 * 			 	for (x = t_ptr+1; x != sizeof(table), x++) {
	 * 			 		if(table[x] == dst) {
	 * 			 			table[x] = table[t_ptr]
	 * 			 			break
	 * 			 		}
	 * 			 	}
	 * 			t_ptr++
	 * 			if (t_ptr == sizeof(table))
	 * 				break;
	 * 		} else {
	 * 			// the element came from block_2
	 * 			if (dst != b2_ptr) {
	 * 				swap(dst, b2_ptr)
	 * 				//	block_2 elements always overwrite block_1 elements
	 * 				//	search table to determine if the element
	 * 				//	  displaced element was from block_1
	 * 				for (x = t_ptr; x != sizeof(table), x++) {
	 * 					if(table[x] == dst)
	 * 						table[x] = b2_ptr
	 * 				}
	 * 			}
	 * 			b2_ptr++
	 *		}
	 * 		next_destination(dst)
	 */

	template <typename T>
	SortMetrics mergeTwoBlocksByTable(T ** array,
											index_t block_1_start, index_t block_1_end,
											index_t block_2_start, index_t block_2_end) {

		/*	**************************************************************	*/
		/*							debug									*/
		/*	**************************************************************	*/

		constexpr bool debug_verbose = false;
		std::stringstream message;

		/*	**************************************************************	*/
		/*							lambdas									*/
		/*	**************************************************************	*/

		auto next_destination = [=] (index_t _dest) -> index_t {
			if (_dest != block_1_end)
				return _dest + 1;
			else
				return block_2_start;
		};

		//	If an element swapped into 'dst' was in the displacement table,
		//	update the table's entry for the element to now be located at 'src'
		auto update_locations_table = [] (index_t *table, index_t start, index_t end,
										  index_t dst, index_t src) {
			for (index_t i = start; i <= end; i++) {
				//	the element in the table which was
				//	previously at 'dst', it is now at 'src'
				if (table[i] == dst) {
					table[i] = src;
					break;
				}
			}
		};

		/*	**************************************************************	*/
		/*							algorithm								*/
		/*	**************************************************************	*/

		SortMetrics result(0,0);

		index_t block_1_span = block_1_end - block_1_start + 1;
		index_t block_2_span = block_2_end - block_2_start + 1;

		if (block_1_span == 0 || block_2_span == 0) {
			return result;
		}

		index_t block_1_locations_table_size = block_1_span;
		index_t block_1_locations_table[block_1_locations_table_size];

		for (index_t i = 0, src = block_1_start; i < block_1_locations_table_size; ) {
			block_1_locations_table[i++] = src++;
		}

		index_t block_1_locations_table_index = 0;
		index_t block_2_index				  = block_2_start;
		index_t destination_index  			  = block_1_start;

		auto debug_string = [&]() -> std::string {
			std::stringstream result;
			for (index_t i = block_1_start; i <= block_2_end; ) {
				result << std::setw(3) << *array[i] << " ";
				i++;
				if (i-1 == block_1_end) {
					i = block_2_start;
				}
			}
			result << " " << std::setw(3) << block_1_locations_table_index
				   << " using table [";
			for (int i = 0; i != block_1_locations_table_size; i++) {
				if (i < block_1_locations_table_index) {
					result << " - ";
				} else {
					result << std::setw(3) << block_1_locations_table[i];
				}
			}
			result 	<< "] dst " << std::setw(2) << destination_index
					<< " t_ptr " << std::setw(2) << block_1_locations_table_index
					<< " b2_ptr " << std::setw(2) << block_2_index;
			return result.str();
		};


		/*	******************************************************	*/
		/*					the algorithm code						*/
		/*	******************************************************	*/

		while (destination_index <= block_2_end)
		{
			// Point to the current location of the next block_1 element
			//	which may not be stored in its original position b/c
			//	the block_1 element may have been displaced in a previous
			//	pass through this loop.
			index_t block_1_index = block_1_locations_table[block_1_locations_table_index];

			result.compares++;
			if (*array[block_1_index] <= *array[block_2_index]) {
				// the value from the left block goes into destination
				if (destination_index != block_1_index) {
					T* tmp 					 = array[destination_index];
					array[destination_index] = array[block_1_index];
					array[block_1_index] 	 = tmp;
					result.assignments += 3;

					// Update the table location of the entry that was just displaced,
					//	which will be somewhere in the table after the current entry
					update_locations_table(block_1_locations_table,
								   	   	   block_1_locations_table_index+1, block_1_locations_table_size-1,
										   destination_index, block_1_index);
				}

				if (debug_verbose) message   << "left:  " << debug_string() << std::endl;

				destination_index = next_destination(destination_index);

				//	if we have moved / merged all of the block_1 elements, we are done
				if (++block_1_locations_table_index == block_1_locations_table_size) {
					if (debug_verbose) message << "Terminated due to table_index == table_size " << std::endl;
					break;
				}
			}
			else
			{
				// value from the right block is < value from the left block
				T* tmp 					 = array[destination_index];
				array[destination_index] = array[block_2_index];
				array[block_2_index] 	 = tmp;
				result.assignments += 3;

				//	Update the table entry of the location of the element
				//	that was just displaced, which may be in any position in the table
				update_locations_table(block_1_locations_table,
								   	   block_1_locations_table_index, block_1_locations_table_size-1,
									   destination_index, block_2_index);

				if (debug_verbose) message << "right: " << debug_string() << std::endl;

				destination_index = next_destination(destination_index);

				//	if all the elements from block_2 are in place, break loop
				if (++block_2_index > block_2_end) {
					if (debug_verbose) message << "Terminated due to block_2_index > block_2_end" << std::endl;
					break;
				}
			}
		}

		//	If the while loop terminated because all block_2 elements are now in place,
		//	  it is possible that there are displaced block_1 elements that are not
		//	  in order.  Reorder any remaining block_1 values that have been displaced
		while(destination_index <= block_2_end &&
			  block_1_locations_table_index < block_1_locations_table_size)
		{
			if (debug_verbose) message << "flush: " << debug_string() << std::endl;

			index_t block_1_index	= block_1_locations_table[block_1_locations_table_index];
			T* temp 				= array[destination_index];
			array[destination_index]= array[block_1_index];
			array[block_1_index]	= temp;
			result.assignments += 3;
			//	update the table's contents from AFTER the element that was just stored
			update_locations_table(block_1_locations_table,
								   block_1_locations_table_index+1, block_1_locations_table_size-1,
								   destination_index, block_1_index);
			block_1_locations_table_index++;
			destination_index = next_destination(destination_index);
		}

		if (debug_verbose)	std::cout << message.str() << std::endl;
		return result;
	}


	//	starting at the right-most block, merge the previous (left) block into
	//	  the elements to the right
	template <typename T>
	SortMetrics mergeAllBlocksRightToLeft(T** array,
									std::unique_ptr<BlockDescriptor<T>[]> &block_descriptors,
									int num_blocks) {

		SortMetrics result;
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
	SortMetrics sortBlocksBinarySearch(T **array, index_t size,
											std::unique_ptr<BlockDescriptor<T>[]> &blocks,
											int num_tags) {

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
		if (blocks[num_tags-1].type == BlockType::A_BLOCK) {
			_debug("All blocks are A Blocks");
			return result;
		}

		// 	if the array consists of all B_Blocks, we are done
		if (blocks[0].type == BlockType::B_BLOCK) {
			 _debug("All blocks are B Blocks");
			return result;
		}

		bool tagsAreSorted = true;
		for (int i = 1; i != num_tags; i++) {
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
		for (int i = 0; i != num_tags; i++) {
			if (blocks[i].type == BlockType::A_BLOCK)
				num_a_blocks++;
			else
				break;
		}

		index_t a_block_index  = num_a_blocks-1;
		index_t b_block_start  = num_a_blocks;
		index_t b_block_end	= num_tags-1;

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
			index_t lm_b;
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
			index_t lm_a;
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
				index_t array_size = blocks[num_tags-1].end_index+1;
				index_t v = blocks[num_a_blocks].start_index;
				std::cout << "BEFORE: "
						  << " lm_b = " << std::setw(2) << lm_b
						  << " lm_a = " << std::setw(2) << a_block_index
						  << " rotate left by " << std::setw(2) << rotate_left_count
						  << "\n"
						  << blockSortToString(array, array_size, v, blocks, num_tags)
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
				index_t array_size = blocks[num_tags-1].end_index+1;
				index_t v = blocks[num_a_blocks].start_index;
				std::cout << "AFTER: \n"
						  << blockSortToString(array, array_size, v, blocks, num_tags)
						  << std::endl;
			}
		}

		if (debug_verbose) {
			index_t array_size = blocks[num_tags-1].end_index+1;
			index_t v = blocks[num_a_blocks].start_index;
			std::cout << "EXITING: \n"
					  << blockSortToString(array, array_size, v, blocks, num_tags)
					  << std::endl;
			if (areBlocksSorted(blocks, num_tags)) {
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
	SortMetrics sortBlocksByKey(T** array, index_t size,
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
		index_t num_A_blocks = 0;

		for (index_t i = 0; i != num_blocks; i++) {
			if (descriptors[i].type != BlockType::A_BLOCK) {
				break;
			}
			num_A_blocks++;
		}

		//	Build a table of A_Block indices
		index_t a_positions[num_A_blocks];
		for (index_t i = 0; i != num_A_blocks; i++) {
			a_positions[i] = i;
		}

		//	Create source & destination indices

		index_t table_i 	= 0;
		index_t b_source 	= num_A_blocks;
		index_t dst			= 0;
		index_t src			= 0;

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

			for (index_t i = table_i; i < num_A_blocks; i++) {
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
				for (index_t i = table_i; i < num_A_blocks; i++) {
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
		SortMetrics sortBlocksHybrid(T **array, index_t size,
				std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_blocks)
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

			index_t a_block_index = 0;
			index_t b_block_index = 0;

			//	find the first B_Block
			while (b_block_index < num_blocks && blocks[b_block_index].type == BlockType::A_BLOCK) {
				b_block_index++;
			}
			a_block_index = b_block_index-1;

			index_t debug_middle_index = b_block_index;

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
					index_t binary_search_result;
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
						std::cout << "After binary search "
								  << " a_i = " << a_block_index
								  << " b_i = " << b_block_index
								  << "\n"
								  << blockSortToString(array, size, -1,
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
					index_t array_size = blocks[num_blocks-1].end_index+1;
					index_t v = blocks[debug_middle_index].start_index;
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
					index_t array_size = blocks[num_blocks-1].end_index+1;
					index_t v = blocks[debug_middle_index].start_index;
					std::cout << "AFTER: \n"
							  << blockSortToString(array, array_size, v, blocks, num_blocks)
							  << std::endl;
					std::cout << "LOOP COMPLETE" << std::endl << std::endl;
				}
			}

			if (debug_verbose) {
				index_t array_size = blocks[num_blocks-1].end_index+1;
				index_t v = blocks[debug_middle_index].start_index;
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
	SortMetrics sortBlocksRightToLeft(T **array, index_t size,
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
		index_t a_block_index = num_a_blocks-1;
		index_t b_block_index = num_blocks-1;
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
				index_t array_size = blocks[num_blocks-1].end_index+1;
				index_t v = blocks[num_a_blocks].start_index;
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
				index_t array_size = blocks[num_blocks-1].end_index+1;
				index_t v = blocks[num_a_blocks].start_index;
				std::cout << "AFTER: \n"
						  << blockSortToString(array, array_size, v, blocks, num_blocks)
						  << std::endl;
			}
		}

		if (debug_verbose) {
			index_t array_size = blocks[num_blocks-1].end_index+1;
			index_t v = blocks[num_a_blocks].start_index;
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
