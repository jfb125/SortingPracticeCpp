/*
 * blockSortUnused.h
 *
 *  Created on: Sep 22, 2025
 *      Author: Joe Baker
 *
 *    Most of these functions were developed for an implementation of block sort
 *	that relies on creating the blocks symmetrically around mid = array_size/2.
 *	I eventually reached the conclusion that the algorithm as described by that
 *	author was basically an in-place merge that required either an auxiliary array
 *	or required a separate table to keep track of where elements were swapped out
 *	of the destination block during an in place merge. (See the function in this
 *	file called 'mergeTwoBlocksByTable()'.  'mergeTwoBlocksByTable()' had a time
 *	complexity as measured by number of moves of n^1.5, which was scarcely better
 *	than an insertion sort and was NOT the promised nlog(n) for block sort.
 *
 *
 *
 *
 */

#ifndef BLOCKSORTUNUSED_H_
#define BLOCKSORTUNUSED_H_

#include "BlockSortDataTypes.h"

namespace BlockSort {

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*						forward declarations							*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	/*	Creates an array of block descriptors of types A & B {[A0][A1]..[Am][B0]..[Bn]}
	 * 		where [A0] and B[n] may not be full blocks. m <= n, n is at most m+1 */

//	template <typename T>
//	int createBlockDescriptorsSymmetrically(T** array,
//											index_t start, index_t mid, index_t end,
//		    								int block_size,
//											std::unique_ptr<BlockDescriptor<T>[]> &descriptors);
	#undef ENABLE_CREATE_BLOCK_DESCIRPTORS_SYMMETRICALLY

	/*	Combines all pairs of adjacent descriptors in the array into single descriptor.
	 *		0   1   2   3   4   5   6   7  becomes	0   1   2   3   4   5   6   7
	 *	   'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h'         'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h'
	 *	   |A0||A1        ||A2        ||A3|	       |A0           ||A1            | 	*/

//	template <typename T>
//	int mergeAdajacentPairsOfDescriptors(T** array,
//										 std::unique_ptr<BlockDescriptor<T>[]> &descriptors,
//										 int num_descriptors);
	#undef ENABLE_MERGE_ADJACENT_PAIRS_OF_DESCRIPTORS

	/*    In-pace merge.  Blocks do not have to be contiguous.  Uses a table of indices
	 * 	to keep track of where an array element has been moved during merging */

//	template <typename T>
//	ComparesAndMoves mergeTwoBlocksByTable(T** array,
//										   index_t left_start, index_t left_end,
//										   index_t right_start, index_t right_end);
	#undef ENABLE_MERGE_TWO_BLOCKS_BY_TABLE

	/*	  Uses a binary search of the A_Block into the remaining B_Blocks
	 * 	to identify which B_Blocks come before the A_Block.  Faster on the
	 * 	first block, but noticeably slower on successive blocks */
//	template <typename T>
//	ComparesAndMoves sortBlocksBinarySearch(T **array, index_t size,
//			std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_tags);
	#undef ENABLE_SORT_BLOCKS_BINARY_SEARCH

	/*	  Uses a binary search to find the location on the first A_Block
	 *  but then uses RightToLeft to find successive A_Block locations */
//	template <typename T>
//	ComparesAndMoves sortBlocksHybrid(T **array, index_t size,
//			std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_blocks);
	#undef ENABLE_SORT_BLOCKS_HYBRID

	/*	  Sorts the Blocks by starting at the left-most A_Block and the
	 * 	right-most B_Blocks. The initial search is longer than a binary search,
	 * 	but successive blocks search must quicker than restarting a binary search */
//	template <typename T>
//	ComparesAndMoves sortBlocksRightToLeft(T **array, index_t size,
//			std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_blocks);
	#undef ENABLE_SORT_BLOCKS_RIGHT_TO_LEFT

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*						function definitions							*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	/*
	 * 	createBlockDescriptorsSymmetrically(array, start, mid, end, block_size, descriptor_dst&);
	 *
	 *	Inputs: array		- pointer to an array of pointers to type T
	 *			start		- index of first element in the span
	 *			mid			- the expected location of the left most element  block B[0]
	 *			end			- index of last element in the span
	 *			block_size	- size of each block
	 *			descriptors	- pointer to the destination of the array of descriptors
	 *
	 *	Output:	returns number of descriptors created
	 *
	 * 	parses an array from [start:end] into blocks of span <= block_size
	 * 	 	to create a list of blocks of elements which is stored in descriptor_dst.
	 * 	 	The underlying array is not modified. The BlockTags contain
	 * 	 	information about the array, but they are not part of the array
	 *
	 *	'A_Block' is defined as a block that has a key value of the first element
	 *	'B_Block' is defined as a block that has a key value of the last element
	 *
	 * 	the blocks are generated in such a way to ensure that
	 * 		[mid-1] is the end_index of the last A-type block and
	 * 		[mid] is the start_index of the first B-type block
	 *
	 * 	consider the following array with start = 0, mid = 4, end = 8, block_size = 3
	 * 		             0  1  2  3  4  5  6  7  8
	 * 		           { a, c, e, g, i, b, d, f, h }
	 * 	tags types are  |A||A     ||       B||   B|
	 *  tag indices are [0] [1:3]       [4:6] [7:8]
	 *  tag keys are    'a' 'c'           'd'   'h'
	 *
	 * 	returns the count of tags which is 4
	 */

	//	  Sorting the blocks by swapping blocks only works if all of the blocks
	//	(except the last B_Block) are of the same size.  This subroutine has the
	//	potential to create a partial A_Block. That functionality was developed,
	//	and should continue to be regression tested, but it should not be used
	//	in the production code
#ifdef ENABLE_CREATE_BLOCK_DESCIRPTORS_SYMMETRICALLY
	template <typename T>
	int  createBlockDescriptorsSymmetrically(
			T** array,
			index_t start, index_t mid, index_t end,
			int block_size,
			std::unique_ptr<BlockDescriptor<T>[]> &blocks) {
#else
	template <typename T>
	int  regressionTestOnly_CreateBlockDescriptorsSymmetrically(
			T** array,
			index_t start, index_t mid, index_t end,
			int block_size,
			std::unique_ptr<BlockDescriptor<T>[]> &blocks) {
#endif

		index_t lower_span = mid-start;
		index_t upper_span = end-mid + 1;

		// calculate the total number of blocks
		int num_blocks = lower_span / block_size + upper_span / block_size;
		// if there is a partial first block
		//	  it is necessary to calculate the first_block_size in order to
		//	  know where .end_index is in the first block
		int first_block_size = lower_span % block_size;
		if (first_block_size != 0)
			num_blocks++;
		//	if there is a partial last block
		//	  it is not necessary to calculate the block_size of the last block
		//	  the algorithm will make the .end_index of the last block = 'end'
		if (upper_span % block_size)
			num_blocks++;

		// create the block storage
		blocks = std::unique_ptr<BlockDescriptor<T>[]>(new BlockDescriptor<T>[num_blocks]);

		if (num_blocks == 0) {
			return num_blocks;
		}

		//	assign values to the blocks
		int block_number = 0;
		index_t start_of_block = start;

		//	do the A_Blocks first
		//	if the first A_Block is a partial, it's .end_index != (.start_index+block_size-1)
		if (first_block_size) {
			blocks[block_number].type 			= BlockType::A_BLOCK;
			blocks[block_number].start_index	= start;
			blocks[block_number].end_index		= first_block_size-1;
			blocks[block_number].key			= array[start_of_block];
			start_of_block += first_block_size;
			block_number = 1;
		}

		//	the full A_Blocks where .end_index = (.start_index+block_size-1)
		while (start_of_block < mid) {
			blocks[block_number].type 			= BlockType::A_BLOCK;
			blocks[block_number].start_index	= start_of_block;
			blocks[block_number].end_index		= start_of_block+block_size-1;
			blocks[block_number].key			= array[start_of_block];
			start_of_block += block_size;
			block_number++;
		}

		//	the full B_Blocks where .end_index = (.start_index+block_size-1)
		while (block_number < num_blocks-1) {
			blocks[block_number].type 		= BlockType::B_BLOCK;
			blocks[block_number].start_index= start_of_block;
			blocks[block_number].end_index	= start_of_block+block_size-1;
			blocks[block_number].key		= array[start_of_block + block_size-1];
			start_of_block += block_size;
			block_number++;
		}

		//	the final B_Block may be a partial block, but regardless of its size,
		//		.end_index = 'end'
		blocks[block_number].type 			= BlockType::B_BLOCK;
		blocks[block_number].start_index	= start_of_block;
		blocks[block_number].end_index		= end;
		blocks[block_number].key			= array[end];

		return num_blocks;
	}


	/*
	 * 	num_descriptors mergeAdajacentPairsOfDescriptors(descriptors, num_descriptors)
	 *
	 *	  This goes through the array an combines adjacent descriptors into
	 *	a single descriptor.  This operation is necessary when doing a bottom-up
	 *  merge of blocks within the array.
	 *		0   1   2   3   4   5   6   7  becomes	0   1   2   3   4   5   6   7
	 *	   'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h'         'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h'
	 *	   |A0||A1        ||A2        ||A3|	       |A0           ||A1            |
	 *
	 * 	In an array of BlockDescriptors [], merges every pair of descriptors [n:n+1]
	 */

#ifdef ENABLE_MERGE_ADJACENT_PAIRS_OF_DESCRIPTORS
	template <typename T>
	int mergeAdajacentPairsOfDescriptors( T** array,
								std::unique_ptr<BlockDescriptor<T>[]> &descriptors,
								int num_descriptors) {
#else
	template <typename T>
	int regressionTestOnly_mergeAdajacentPairsOfDescriptors(
								T** array,
								std::unique_ptr<BlockDescriptor<T>[]> &descriptors,
								int num_descriptors) {
#endif
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


	/*
	 * 	mergeBocksByTable(array, left_start, left_end, right_start, right_end)
	 *
	 * 	This function merges the values the two blocks [l_start:l_end] with [r_start:r_end]
	 * 	It is assumed that the values within each block are in ascending order
	 *
	 * 	Note that this algorithm does not require the blocks to be contiguous,
	 * 	 nor does it require the size of the left block to be <= size of the right block
	 *
	 * 	The algorithm creates a table of where each element of the left block is after
	 * 	  each swap. This table of array indices allows the algorithm to only require
	 * 	  one temporary location for an array element during the swap.  However, it
	 * 	  requires the space necessary for a table of pointers that numbers the
	 * 	  same as the number of array elements in the left block.
	 *
	 *	In the following discussion
	 *		'ls' is left_start		'le' is left end
	 *		'rs' is right start		're' is right end
	 *		'-' indicates an index that is no longer part of the algorithm
	 *
	 * 	Consider the blocks "BDF" and "ACEG
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			   table 't'
	 * 	ls      le      rs      re     ||  dst  [ 0   1   2   ]     t_ptr  l_ptr    r_ptr
	 * 	B   D   F  ...  A   C   E   G  ||  m    [ m+0 m+1 m+2 ]     0      [0]=m+0  n+0
	 *
	 *  [t[0]] = 'B'  > [r_ptr] = 'A' .... xchg(dst, r_ptr); t[0]=r_ptr; r_ptr++;
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			   table 't'
	 * 	ls      le      rs      re     ||  dst  [ 0   1   2   ]     t_ptr  l_ptr    r_ptr0
	 * 	A   D   F  ...  B   C   E   G  ||  m+1  [ n+0 m+1 m+2 ]     0      [0]=n+0  n+1
	 *
	 *  [t[0]] = 'B' <= [r_ptr] = 'C' .... xchg(dst, l_ptr); t[1]=l_ptr; t_ptr++;
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			   table 't'
	 * 	ls      le      rs      re     ||  dst  [ 0   1   2   ]    t_ptr  l_ptr    r_ptr0
	 * 	A   B   F  ...  D   C   E   G  ||  m+2  [ -   n+0 m+2 ]    1      [1]=n+0  n+1
	 *
	 *  [t[1]] = 'D' >  [r_ptr] = 'C' .... xchg(dst, r_ptr); t[2]=r_ptr; r_ptr++;
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			   table 't'
	 * 	ls      le      rs      re     ||  dst  [ 0   1   2   ]     t_ptr  l_ptr    r_ptr0
	 * 	A   B   C  ...  D   F   E   G  ||  n+0  [ -   n+0 n+1 ]     1      [1]=n+0  n+2
	 *
	 * 	[t[1]] = 'D' <= [r_rptr] = 'E' ... xchng(dst, l_ptr); t[1]=l_ptr; t_ptr++
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			   table 't'
	 * 	ls      le      rs      re     ||  dst  [ 0   1   2   ]     t_ptr  l_ptr    r_ptr0
	 * 	A   B   C  ...  D   F   E   G  ||  n+1  [ -   -   n+1 ]     2      [2]=n+1  n+2
	 *
	 * 	[t[2]] = 'F' <= [r_rptr] = 'E' ... xchng(dst, r_ptr); t[2]=r_ptr; r_ptr++
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			   table 't'
	 * 	ls      le      rs      re     ||  dst  [ 0   1   2   ]     t_ptr  l_ptr    r_ptr0
	 * 	A   B   C  ...  D   E   F   G  ||  n+2  [ -   -   -   ]     3      [2]=n+0  n+3
	 *
	 * 	[t[2]] = 'F' <= [r_rptr] = 'G' ... xchng(dst, l_ptr); t[2]=r_ptr; t_ptr++
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			   table 't'
	 * 	ls      le      rs      re     ||  dst  [ 0   1   2   ]     t_ptr  l_ptr    r_ptr0
	 * 	A   B   C  ...  D   E   F   G  ||  n+3  [ -   -   -   ]     3      [2]=n+0  n+3
	 *
	 * 	t_ptr == sizeof(table) = 3, so all left_block elements are in their correct
	 * 		location.  That means the remaining right_block elements "G" is to the
	 * 		right of the last left_block element, so the array is sorted
	 *
	 *	Keep in mind that the positions of the left block, and the positions of the
	 *		right block are themselves destinations for the ordered, merged elements.
	 *		The fact that many moved elements out of the left block will be moved to
	 *		the right block, and will probably be moved multiple times, means that as
	 *		the merge operation begins storing ordered elements into the right block,
	 *		it will	probably be swapping out elements that were already themselves
	 *		swapped	out of the left block previously.  This leads to even further
	 *		disordering	of the elements towards the end of the merge.  Because these
	 * 		elements in the right block are not in order, swapping within the right block
	 * 		will be necessary.  Restoring the order of the left block elements to their
	 * 		correct order within the area formerly occupied by the right block was
	 * 		determined empirically to require a number of swaps similar to an insertion sort.
	 *		Not the efficiency requires of nlog2(n)
	 *
	 * 	Consider the blocks "EFG" and "ABCD"
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3   dst      table            t_ptr  l_ptr    r_ptr
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
	 * 	t_ptr= has exceeded limit of sizeof(table)=2 and all right blocks are in place, done
	 *
	 * 									Algorithm:
	 * 	nextDestination(dst)
	 *		if (dst == left_end)	dst = r_start
	 *		else					dst++
	 *
	 * 	dst = left_start
	 * 	t_ptr = 0
	 * 	r_ptr = right_start
	 * 	while (dst <= right_end)
	 * 		l_ptr = table[t_ptr]
	 * 		if (array[r_ptr] < array[l_ptr]
	 * 			xchg(dst, r_ptr)
	 * 			table[t_ptr] = r_ptr
	 * 			nextDestination(dst)
	 * 			if (r_ptr > right_end)
	 * 				// if all of the right elements have been placed
	 * 				//  restore order of remaining blocks, which are all left elements,
	 * 				//	which are now on the right
	 * 				while (t_ptr < size(table) {
	 * 					xchg(dst,l_ptr)
	 * 					find 'dst' in table at index 'x'
	 *					table[x] = dst
	 *					nextDestiniation(dst)
	 * 				break
	 * 		else
	 * 		    xchg[dst, l_ptr)
	 * 		    find 'dst' in table at index 'y'
	 * 		    table[y] = dst
	 * 		    t_ptr++
	 * 		    // if all of the left elements have been placed
	 * 		    //	the remaining elements to the right (if any) are right elements
	 * 		    if (t_ptr == sizeof(table)
	 * 		    	break
	 * 		    nextDestination(dst)
	 *
	 *
	 */
#ifdef ENABLE_MERGE_TWO_BLOCKS_BY_TABLE
	template <typename T>
	ComparesAndMoves mergeTwoBlocksByTable(T ** array,
										index_t left_start, index_t left_end,
										index_t right_start, index_t right_end) {
#else
	template <typename T>
	ComparesAndMoves regressionTestOnly_mergeTwoBlocksByTable(T ** array,
										index_t left_start, index_t left_end,
										index_t right_start, index_t right_end) {
#endif
		ComparesAndMoves result(0,0);

		bool debug_verbose = false;
		std::stringstream message;
	//	std::cout << std::endl;

		index_t left_span 	= left_end - left_start + 1;
		index_t right_span = right_end - right_start + 1;

		auto nextDestination = [left_end, right_start] (index_t &_dst) {
			if (_dst == left_end)	_dst = right_start;
			else					_dst++;
		};
	#if 0
		if (right_span < left_span) {
			// TODO - throw an error
			message << "ERROR " << __FUNCTION__ << "() has A_Block on right side: may cause instability, exiting\n";
			std::cout << message.str();
			return result;
		}
	#endif
		if (left_span == 0 || right_span == 0) {
			return result;
		}

		//
		index_t right_ptr	= right_start;
		index_t dst_ptr    	= left_start;

		index_t left_indices_table_size = left_span;
		index_t left_indices_table[left_indices_table_size];
		for (index_t i = 0, src = left_start; i < left_indices_table_size; ) {
			left_indices_table[i++] = src++;
		}
		index_t table_ptr = 0;

		auto debug_string = [&](index_t left_source) -> std::string {
			std::stringstream result;
			for (index_t i = left_start; i <= right_end; ) {
				result << std::setw(3) << *array[i] << " ";
				i++;
				if (i-1 == left_end) {
					i = right_start;
				}
			}
			result << " " << std::setw(3) << table_ptr
				   << " into table [";
			for (int i = 0; i != left_indices_table_size; i++) {
				if (i < table_ptr) {
					result << " - ";
				} else {
					result << std::setw(3) << left_indices_table[i];
				}
			}
			result 	<< "] dst " << std::setw(2) << dst_ptr
					<< " ls " << std::setw(2) << left_source
					<< " rt " << std::setw(2) << right_ptr;
			return result.str();
		};


		/*	******************************************************	*/
		/*					the algorithm code						*/
		/*	******************************************************	*/

		while (dst_ptr <= right_end)
		{
			// point to the current location of the next value from the
			//	left block, which may have been swapped out of its place
			//	in the left block if its initial location is now occupied
			//	by a value that has been stored / merged
			index_t left_source = left_indices_table[table_ptr];
			if (debug_verbose) {
				message << debug_string(left_source);
			}
			result._compares++;
			if (*array[left_source] <= *array[right_ptr]) {
				// the value to go in the destination isn't already in place
				if (dst_ptr != left_source) {
					result._moves += 3;
					T* tmp = array[dst_ptr];
					array[dst_ptr] = array[left_source];
					array[left_source] = tmp;

					//	if the value that was swapped out of [dst] was a member of the
					//	  indirection (a left block element), update its new position
					//	  in the indirections[]
					//	  (note that the last A value, [indirection_stop-1], does not have
					//	   an indirection[(indirection_i-1)+1] member after it)
					for (int i = table_ptr; i < left_indices_table_size; i++) {
						if (left_indices_table[i] == dst_ptr) {
							left_indices_table[i] = left_source;
							break;
						}
					}
				}
				if (debug_verbose) {
					message << " ----      left      ---- "
							<< debug_string(left_source) << std::endl;
				}
				//	if we have moved / merged all of the A_Block values, done
				if (++table_ptr == left_indices_table_size)	break;

				//	if all of the locations in the left block have
				//	  any merged value, left or right, stored in them,
				//	  move 'dst' to the right block
				nextDestination(dst_ptr);
			}
			else
			{
				// value from the right block is < value from the left block
				result._moves += 3;
				T* tmp = array[dst_ptr];
				array[dst_ptr] = array[right_ptr];
				array[right_ptr] = tmp;

				//	if the value that was exchanged with [dst] was an unmerged A_Block value,
				//	update the new location of the value in the indirection table
				for (int i = table_ptr; i < left_indices_table_size; i++) {
					if (left_indices_table[i] == dst_ptr) {
						left_indices_table[i] = right_ptr;
						break;
					}
				}
				if (debug_verbose) {
					message << " ----      right     ---- "
							<< debug_string(left_source) << std::endl;
				}
				//	if all the positions in the left block have been used,
				//	  move 'dst' to the start of the right block
				nextDestination(dst_ptr);
				right_ptr++;

				//	if all of the right_block values have been placed,
				//	  we need to arrange all the remaining a_block values
				//	  into / at the end of the array (they may be out of sequence)
				if (right_ptr > right_end)
				{
					while (table_ptr < left_indices_table_size)  {
						left_source = left_indices_table[table_ptr];
						if (left_source != dst_ptr) {
							result._moves += 3;
							T* tmp = array[dst_ptr];
							array[dst_ptr] = array[left_source];
							array[left_source] = tmp;
							//	check to see if we moved an unmerged A_Block (left)
							//	  value to a different location in the array
							for (int i = table_ptr; i < left_indices_table_size; i++) {
								if (left_indices_table[i] == dst_ptr) {
									left_indices_table[i] = left_source;
								}
							}
						}
						//	if all the positions in the left block have been used,
						//	  move 'dst' to the start of the right block
						nextDestination(dst_ptr);
						table_ptr++;
					}
					break;	// the loop that restored the a_block's values (left) is done
				}
			}
		}

		if (debug_verbose) {
			std::cout << message.str();
		}
		return result;
	}
	/*
	 *  BinarySearch - this version searches the B_Blocks for the largest value smaller than A_Block
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


#ifdef ENABLE_SORT_BLOCK_BINARY_SEARCH
	template <typename T>
	ComparesAndMoves sortBlocksBinarySearch(T **array, index_t size,
											std::unique_ptr<BlockDescriptor<T>[]> &blocks,
											int num_tags) {
#else
	template <typename T>
	ComparesAndMoves regressionTestOnly_sortBlocksBinarySearch(T **array, index_t size,
											std::unique_ptr<BlockDescriptor<T>[]> &blocks,
											int num_tags) {
#endif

		constexpr bool debug_verbose = true;

		#pragma push_macro("_debug")
		#define _debug(_dbg_msg_) do {\
			if (debug_verbose) {\
				std::cout << _dbg_msg_ << std::endl;\
			}\
		} while(false)

		_debug("sortBlocksBinarySearch()");

		ComparesAndMoves result(0,0);

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
			result._compares++;
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
	 *  Hybrid - this version initially searches for the first B_Block smaller than the A_Block
	 *  	using a Binary Search, but then uses a Right To Left strategy for successive blocks
	 *  	b/c the likelihood is that the next B_Block to be found < the next A_Block is
	 *  	probably close to where the previous B_Block was.
	 */
#ifdef ENABLE_SORT_BLOCKS_HYBRID
	template <typename T>
	ComparesAndMoves sortBlocksHybrid(T **array, index_t size,
			std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_blocks)
	{
#else
	template <typename T>
	ComparesAndMoves regressionTestOnly_sortBlocksHybrid(T **array, index_t size,
			std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_blocks)
	{
#endif
		constexpr bool debug_verbose = false;

		#pragma push_macro("_debug")
		#define _debug(_dbg_msg_) do {\
			if (debug_verbose) {\
				std::cout << _dbg_msg_ << std::endl;\
			}\
		} while(false)

		_debug("sortBlocksHybridSearch()");

		ComparesAndMoves result(0,0);

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
			result._compares++;
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

			if (!use_binary_search) {
				//	Find first B_Block that is less than this A_Block
				//	There may not be a B_Block that is greater than this A_Block
				while (b_block_index > a_block_index) {
					result._compares++;
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
				//	0	 1	  2	   3	4	 5	  6    7
				//			  ai             bi
				//	A=4, A=6, A=7, B=5, B=5, B=6, B=7, B=8

				//	If all B_Blocks are greater than this A_Block, the blocks are in order
				//	0	 1	  2	   3	4	 5	  6    7
				//			  ai
				//	A=4, A=6, A=7, B=8, B=9, B=10,B=11,B=12
				if (binary_search_result > num_blocks-1) {
					break;
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
				result._compares++;
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
			}
		}


		if (debug_verbose) {
			index_t array_size = blocks[num_blocks-1].end_index+1;
			index_t v = blocks[debug_middle_index].start_index;
			std::cout << "AFTER: \n"
					  << blockSortToString(array, array_size, v, blocks, num_blocks)
					  << std::endl;
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
	 * RightToLeft - this version searches the B_Blocks by starting at the last B_Block on the right
	 * 				 and moving through the B_Blocks until either
	 * 				 	the B_Block pointer == A_Block pointer which indicates that there are
	 * 				 		no B_Blocks that are less than the largest remaining A_Block
	 * 				 or the A_Block pointer == -1, which indicates there are no A_Blocks
	 * 				 		that are larger than the smallest B_Block
	 *
	 * 	Once the first B_Block that needs to be rotated left is found, successive searches for
	 * 		B_Blocks to rotate will probably be efficient as long as the next B_Block to be found
	 * 		is closeby to the left of the B_Block that was just rotated
	 *
	 * 	Finding the first B_Block to rotate can be expensive since the algorithm starts from the
	 * 		last B_Block on the right and moves one-at-a-time to the left
	 */

#ifdef ENABLE_SORT_BLOCKS_RIGHT_TO_LEFT
	template <typename T>
	ComparesAndMoves sortBlocksRightToLeft(T **array, index_t size,
										   std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_blocks) {
#else
	template <typename T>
	ComparesAndMoves regressionTestOnly_sortBlocksRightToLeft(T **array, index_t size,
										   std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_blocks) {
#endif
		constexpr bool debug_verbose = false;

		#pragma push_macro("_debug")
		#define _debug(_dbg_msg_) do {\
			if (debug_verbose) {\
				std::cout << _dbg_msg_ << std::endl;\
			}\
		} while(false)

		_debug("sortBlocksRightToLeft()");

		ComparesAndMoves result(0,0);

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
			result._compares++;
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
				result._compares++;
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
				result._compares++;
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
}	// namespace BlockSort;

#endif /* BLOCKSORTUNUSED_H_ */
