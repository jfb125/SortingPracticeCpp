/*
 * BlockSort.h
 *
 *  Created on: Aug 11, 2025
 *      Author: Joe Baker
 */

/*	TODO - List
 * 	Create a sqrtMerge() that uses an auxiliary array the size of an A_Block
 * 		to hold the A_Block, then merges the A_Block & B_Block into the
 * 		place in the original array where the A_Block is.
 *  Aux                Aux                Aux            	Aux
 *  0 3 5 6            x 3 5 6            x x x 6           x x x x
 * 	A	    B          A       B          A       B         A       B
 * 	0 3 5 6 1 2 4 7    0 1 2 3 1 2 4 7    0 1 2 3 4 5 4 7   0 1 2 3 4 5 6 7
 */

#ifndef BLOCKSORT_H_
#define BLOCKSORT_H_

#include "BlockSortDataTypes.h"
#include "IntegerArithmetic.h"
#include "SortingUtilities.h"
#include "InsertionSort.h"

//#define DEBUG_VERBOSE_SORT_BLOCKS 1
//#define DEBUG_SUMMARY_SORT_BLOCKS
#define DEBUG_VERBOSE_BLOCK_SORT
	constexpr char 	TAG_BOUNDARY_CHAR 	= '|';
	constexpr char 	TAG_SPACE_CHAR 		= ' ';

index_t blockSortModulo(index_t rotation_count, index_t span);

/*	for the block swapping portions of the algorithm to work, all of the A_Blocks
 * 	have to be full sized blocks.  Any remaining elements have to be
 * 	in the last B_Block, which may not be a full B_Block.  This variable
 */
#pragma push_macro("CREATE_BLOCKS_SYMMETRICALLY")
#undef CREATE_BLOCKS_SYMMETRICALLY
//#define CREATE_BLOCKS_SYMMETRICALLY

namespace BlockSort {

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							debugging resources							*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	template <typename T>
	bool areBlocksSorted(std::unique_ptr<BlockDescriptor<T>[]> &tags, int num_tags);
	/*		outputing the contentes of the array		*/
	template <typename T>
	std::string arrayElementsToString(T** array, index_t size,
									  int value_width = VALUE_WIDTH,
									  int element_width = ELEMENT_WIDTH);
	template <typename T>
	std::string arrayElementsToString(std::string trailer, T** array, index_t size,
									  int value_width = VALUE_WIDTH,
									  int element_width = ELEMENT_WIDTH);
	/* outputting line of tags to go under line of array elements*/
	template <typename T>
	std::string blockDescriptorsToString(std::unique_ptr<BlockDescriptor<T>[]> &tags,
										 int num_tags,
										 int element_width = ELEMENT_WIDTH);
	/* outputting block type, start_index, end_index */
	template <typename T>
	std::string blockDescriptorsSummaryToString(std::unique_ptr<BlockDescriptor<T>[]> &descriptors,
												int num_tags);
	template <typename T>
	std::string blockSortToString(T** array, index_t size, index_t v,
							 	  std::unique_ptr<BlockDescriptor<T>[]> &tags,
								  int num_tags,
								  int value_width = VALUE_WIDTH,
								  int element_width = ELEMENT_WIDTH);

	/*	**************************************************************************	*/
	/*	**************************************************************************	*/
	/*							algorithm function declarations						*/
	/*	**************************************************************************	*/
	/*	**************************************************************************	*/

	/*	Assigns key values to the blocks if the underlying array has been reordered */
	template <typename T>
	ComparesAndMoves assignKeys(T**array, std::unique_ptr<BlockDescriptor<T>[]> &descriptors, int num_descriptors);

	/*	In an array of blockDescriptors, find the leftmost block that is > key */
	template <typename T>
	ComparesAndMoves binarySearchLastBlock(std::unique_ptr<BlockDescriptor<T>[]> &blocks,
											   index_t start, index_t end,
								   	   	   	   T* key, index_t &key_location);

	/*	Returns the index of the first element that is greater than 'value'
	 * 	This is used to insert a value to the right of it's peers	*/
	template <typename T>
	index_t binarySearchLastElement(T** array, index_t range_start, index_t range_end, T *value);

	/*	Returns the index of the first element that is equal to or greater than 'value'
	 * 	This is used to insert a value to the left of it's peers	*/
	template <typename T>
	index_t binarySearchFirstElement(T** array, index_t range_start, index_t range_end, T *value);

	/*	Creates an array of block descriptors of types A & B {A[0]A[1]..A[m]B[0]..B[n]}
	 * 	  where A[0] is a full block but B[n] if present will not be full. */
	template <typename T>
	int createBlockDescriptors_A0_Full( T** array, index_t start, index_t mid, index_t end,
			    						int block_size,
										std::unique_ptr<BlockDescriptor<T>[]> &descriptors);

	/*	Merges all blocks from left to right using an insertion sort.  This is expected
	 * to possibly be better than just an insertion sort b/c sorting the blocks previously
	 * has possibly placed the array in partial order. */
	template <typename T>
	ComparesAndMoves mergeAllBlocks(T** array, index_t size,
					std::unique_ptr<BlockDescriptor<T>[]> &block_descriptors, int num_blocks);
	/*
	 * 	Blocks must be contiguous
	 */
	template <typename T>
	ComparesAndMoves mergeContiguousBlocksByRotating(T** array, index_t start,
																index_t mid, index_t end);

	template <typename T>
	ComparesAndMoves rotateArrayElementsRight(T** array, index_t start, index_t end, index_t amount);
	template <typename T>

	ComparesAndMoves rotateBlocksRight(T** array, std::unique_ptr<BlockDescriptor<T>[]> &tags,
					index_t first_tag, index_t last_tag, int tag_rotate_count);

	/*	  Sorts all of the A_Blocks, and then sorts all of the B_Blocks, using
	 *	an insertion sort on each range.  This is done after creating the blocks
	 *	on an unordered array and after sorting the elements within the blocks	*/
	template <typename T>
	ComparesAndMoves sortEachBlockTypeSeparately(T** array, index_t size,
			std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_blocks);
	template <typename T>

	ComparesAndMoves swapBlocks(T** array,
								index_t block1_start, index_t block1_end,
								index_t block2_start, index_t block2_end);
	template<typename T>
	ComparesAndMoves swapBlockDescriptors(std::unique_ptr<BlockDescriptor<T[]>> &tags,
										  int i, int j);

	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*							function definitions							*/
	/*	**********************************************************************	*/
	/*	**********************************************************************	*/

	/*	Assigns each block's key value based on the values in 'array'.  This
	 *	  is necessary when the underlying array has been reordered after the
	 *	  block descriptors were created
	 */
	template <typename T>
	ComparesAndMoves assignKeys(T**array, std::unique_ptr<BlockDescriptor<T>[]> &descriptors, int num_descriptors) {

		for (int i = 0; i != num_descriptors; i++) {
			BlockDescriptor<T>*p = &descriptors[i];	// improves readability
			switch(p->type) {
			case BlockType::A_BLOCK:
				p->key = array[p->start_index];
				break;
			case BlockType::B_BLOCK:
				p->key = array[p->end_index];
				break;
			default:
				//	TODO - throw an exception
				p->key = nullptr;
			}
			p++;
		}

		return ComparesAndMoves(0,0);
	}


	/*
	 *	c_and_m binarySearchLastBlock(p_blocks, start, end, p_key, p_location);
	 *
	 *	  Perform a binary search for the index of the left-most block that is greater
	 *	than the passed 'key' on the interval [start:end].
	 *
	 *	If *p_key is <= than *p_blocks[start].key, 	'start' is assigned to p_location
	 *	If *p_key is >  than *p_blocks[end].key, 	'end+1' is assigned to p_location
	 *
	 *	The number of compares of p_key is returned for diagnostic purposes
	 */


	template <typename T>
	ComparesAndMoves binarySearchLastBlock(std::unique_ptr<BlockDescriptor<T>[]> &blocks,
											   index_t range_left, index_t range_right,
								   	   	   	   T* key, index_t &key_location)
	{
		//	debugging resources
		constexpr bool debug_verbose = false;
		OStreamState ostream_state;
		std::stringstream msg;

		// return number of compares & moves(=0) which is a
		//	figure of merit in evaluating the algorithm
		ComparesAndMoves result(0,0);

		index_t start = range_left;
		index_t end	  = range_right;
		while (start != end) {
			//	debug
			index_t mid = start + (end-start)/2;
			if (debug_verbose) {
				msg << "start = " << std::setw(2) << start
					<< " end = " << std::setw(2) << end
					<< " evaluating [" << std::setw(2) << mid
					<< "] = " << std::setw(2) << *blocks[mid].key
					<< (*blocks[mid].key > *key ? " is    " : " is NOT")
					<< " greater than " << *key;
			}

			result._compares++;
			if (*blocks[mid].key <= *key) {
				start = mid+1;
			} else {
				end = mid;
			}

			if (debug_verbose) {
				//	debug stuff
				msg << " next guess = " << std::setw(2) << mid << std::endl;
			}
		}

		//	  If the search ended on the right side of the array
		//	and the element at the right side IS NOT > key,
		//	return the element after the right side.
		//	  It is the caller's responsibility to ensure the range_right+1
		//	is within the bounds of the caller's array of blocks
		result._compares++;
		if (start == range_right && *blocks[start].key <= *key) {
			msg << "start = " << std::setw(2) << start
				<< " end = " << std::setw(2) << end
				<< " evaluating [" << std::setw(2) << start
				<< "] = " << std::setw(2) << *blocks[start].key
				<< (*blocks[start].key > *key ? " is    " : " is NOT")
				<< " greater than " << *key
				<< " therefore returning 'end+1'\n";
			start = start+1;
		} else {
			msg << "start = " << std::setw(2) << start
				<< " end = " << std::setw(2) << end
				<< " evaluating [" << std::setw(2) << start
				<< "] = " << std::setw(2) << *blocks[start].key
				<< (*blocks[start].key > *key ? " is    " : " is NOT")
				<< " greater than " << *key
				<< " therefore returning 'end'\n";
		}

		key_location = start;

		if (debug_verbose) {
			msg << " returning " << start << std::endl;
			std::cout << msg.str();
		}

		//	return diagnostices
		return result;
	}


	/*
	 * binaryFirst(array, first, last, value);
	 *
	 * This function returns the index of the first element that is
	 *  greater than or equal to value. If the passed value is greater than
	 *	all of the elements on the span, an index (range_end+1) is returned.
	 *
	 *	  0  1  2  3  4  5
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &-1) returns 0
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &0) returns 0
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &1) returns 2
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &2) returns 4
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &3) returns 6
	 */

	template <typename T>
	index_t binarySearchFirstElement(T** array, index_t range_start, index_t range_end, T *value) {

		index_t start = range_start;
		index_t end	 	= range_end;
		index_t mid		= start + (end-start)/2;

		while (start != end) {
			//	determine the midpoint in an even size span
			//	or the index on the left of mid in an odd size span
			mid = start + (end-start)/2;
			if (*array[mid] < *value) {
				//	if the array value at [mid] is < value
				//	  look to the right for a [] >= value
				start = mid+1;
			} else {
				//	else the look to the right for a value < passed
				end = mid;
			}
		}
		if (start == range_end && *array[start] < *value)
			start++;
		return start;
	}


	/*
	 * binaryLast(array, first, last, value);
	 *
	 * This function returns the index of first element that is greater than value
	 *	If the passed value is equal to or greater than
	 *	all of the elements on the span, an index (range_end+1) is returned.
	 *
	 *	  0  1  2  3  4  5
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &-1) returns 0
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &0) returns 2
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &1) returns 4
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &2) returns 6
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &3) returns 6
	 */

	template <typename T>
	index_t binarySearchLastElement(T** array, index_t range_start, index_t range_end, T *value) {

		index_t start = range_start;
		index_t end	 	= range_end;
		index_t mid		= start + (end-start)/2;

		while (start != end) {
			//	determine the midpoint in an even size span
			//	or the index on the left of mid in an odd size span
			mid = start + (end-start)/2;
			if (*array[mid] <= *value) {
				//	if the array value at [mid] is < value
				//	  look to the right for a [] >= value
				start = mid+1;
			} else {
				//	else the look to the right for a value < passed
				end = mid;
			}
		}
		if (start == range_end && *array[start] <= *value)
			start++;
		return start;
	}


	/*
	 * 	createBlockDescriptors_A0_Full(array, start, mid, end, block_size, descriptor_dst&);
	 *
	 *		<<< !!! 'mid' must be an integer multiple of 'block_size' from 'start' !!! >>>
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
	 *	  This creates an array of block descriptors such that block A[0] is guaranteed
	 *	to be a full 'block_size' wide.  If the size of the span, 'end' - 'start' +1
	 *	is not an integer multiple of 'block_size', the final will be < '_size'.
	 *
	 *	'A_Block' is defined as a block that has a key value of the first element
	 *	'B_Block' is defined as a block that has a key value of the last element
	 *
	 *	The array of descriptors is of the form {A[0],A[1]..A[m],B[0]..B[n]}
	 *
	 * 	  If in creating the A_Blocks there would not being an integer number of
	 * 	A_Blocks in the portion of the array from [start:mid-1], 0 is returned.
	 *	Thus, it is the caller's responsibility to ensure that
	 *		(mid-start) % block_size == 0
	 *
	 * 	  The underlying array is not modified. The BlockTags contain
	 * 	information about the array, but they are not part of the array
	 *
	 * 	The blocks are generated in such a way to ensure that
	 * 		[mid-1] is the end_index of the last A-type block and
	 * 		[mid] is the start_index of the first B-type block
	 *
	 * 	Consider the following array with start = 0, mid = 6, end = 9, block_size = 3
	 * 		             0  1  2  3  4  5  6  7  8  9
	 * 		           { a, c, e, g, i, k, b, d, f, h
	 * 	tags types are  |A      ||A      ||B      ||B|
	 *  tag indices are [0:2    ][3:5    ][    6:8][9]
	 *  tag keys are    'a'      'g'            'f''h'
	 *
	 * 	Returns the count of tags which is 4
	 */

	template <typename T>
	int createBlockDescriptors_A0_Full(
			T** array,
			index_t start, index_t mid, index_t end,
			int block_size,
			std::unique_ptr<BlockDescriptor<T>[]> &blocks) {

		index_t lower_span = mid-start;
		index_t upper_span = end-mid + 1;

		// calculate the total number of blocks
		int num_blocks = lower_span / block_size + upper_span / block_size;
		//	if there is a partial last B_Block
		if (upper_span % block_size)
			num_blocks++;

		// create the block storage
		blocks = std::unique_ptr<BlockDescriptor<T>[]>(new BlockDescriptor<T>[num_blocks]);

		// if there are no blocks, or if there is a partial block in the A_Blocks
		if ((num_blocks == 0) || (lower_span % block_size != 0)) {
			return 0;
		}

		//	assign values to the blocks
		int block_number = 0;
		index_t start_of_block = start;

		//	the full A_Blocks where .end_index = (.start_index+block_size-1)
		while (start_of_block < mid) {
			blocks[block_number].type 			= BlockType::A_BLOCK;
			blocks[block_number].start_index	= start_of_block;
			blocks[block_number].end_index		= start_of_block+block_size-1;
			blocks[block_number].key			= array[start_of_block];
			start_of_block += block_size;
			block_number++;
		}

		//	If there are any B_Blocks
		if (block_number != num_blocks) {
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
		}

		return num_blocks;
	}


	/*
	 * 	mergeTwoBlocksByAuxiliary(array, left_start, left_end, right_start, right_end)
	 *
	 * 	Merges the two blocks by copying the smaller of the two blocks into an
	 * 		auxiliary array
	 */
	template <typename T>
	ComparesAndMoves mergeTwoBlocksByAuxiliary( T ** array,
												index_t left_start, index_t left_end,
												index_t right_start, index_t right_end) {
		ComparesAndMoves result(0,0);
		return result;
	}


	/*
	 * 	TODO -  add optimization of looking for all values on the left (u[]) side
	 * 			of the array that are > the v[n]
	 * ComparesAndMoves mergeBlocksByRotating(arr, start, mid, end);
	 *
	 *   This uses rotate operations to put the { u[], v[] } array in order
	 *   The requirement is that the sub portions of uv_array, u[] and v[]
	 *   	are in ascending order.
	 *   		Consider an array where start = 5, mid = 9 & end is equal to 12
	 *   					 s           m        e
	 *   		             5  6  7  8  9 10 11 12
	 *      then array[] = { B, C, C, D, A, C, G, G }
	 *
	 *   The algorithm looks for all elements in [mid:end] that are less
	 *   	than the current element in u, then rotates all those elements
	 *   	in v that are less than u.  This is equivalent to finding the
	 *   	first element in v that is > u.
	 *      	start with u = start = 5 & v = mid = 9, count = 0
	 *
	 *      Search u for a value > v
	 *        5  6  7  8  9 10 11 12
	 * 		{ B, C, C, D, A, C, G, G }	arr[u=5].'B' > arr[v= 9].'A'       count = 0
	 * 		Search v for a value < u
	 * 		{ B, C, C, D, A, C, G, G }	arr[u=5].'B' <= arr[v= 9].'A' v++  count++;
	 * 		{ B, C, C, D, A, C, G, G }	arr[u=5].'B' >  arr[v=10].'C'      count = 1
	 * 		arrayRotate(array, count = 1, start = u = 5, end = v-1 = 10 -1 = 9)
	 * 		{ A, B, C, C, E, C, G, G }

	 * 		Search u for a value > v
	 * 		{ A, B, C, C, E, C, G, G }	arr[u=6].'B' <= arr[v=10].'C', u++ count = 0
	 * 		{ A, B, C, C, E, C, G, G }	arr[u=7].'C' <= arr[v=10].'C', u++ count = 0
	 * 		{ A, B, C, C, E, C, G, G }	arr[u=8].'C' <= arr[v=10].'C', u++ count = 0
	 * 		{ A, B, C, C, E, C, G, G }	arr[u=9].'E' <= arr[v=10].'C',     count = 0
	 * 		Search v for a  value < u
	 * 		{ A, B, C, C, E, C, G, G }	arr[u=9].'E' <= arr[v=10].'C', v++ count++
	 * 		{ A, B, C, C, E, C, G, G }	arr[u=9].'E' <= arr[v=11].'G'      count = 1
	 * 		arrayRotate(array, count = 1, start = u = 9, end = v-1 = 11 - 10)
	 * 		{ A, B, C, C, C, E, G, G }
	 *
	 * 		Search u for a value > v
	 * 		{ A, B, C, C, C, E, G, G }	arr[u=9].'E' <= arr[v=11].'G' u++  count = 0
	 * 		{ A, B, C, C, C, E, G, G }	arr[u=10].'E' <= arr[v=11].'G' u++  count = 0
	 * 		{ A, B, C, C, C, E, G, G }	arr[u=11].'E' <= arr[v=11].'G' u++  count = 0
	 * 		STOP u == v
	 */

	template <typename T>
	ComparesAndMoves mergeContiguousBlocksByRotating(T** array, index_t start, index_t mid, index_t end) {

		bool debug_verbosely = false;

		ComparesAndMoves result(0,0);

		index_t u = mid-1;
		index_t v = end;
		index_t rotate_count;

		while (u >= start) {
			//	find the first [v] that is less than [u]
			while (v > u) {
				result._compares++;
				if (*array[u] > *array[v])
					break;
				--v;
			}

			//	if v is pointing to u, then all elements in [v+1:end]
			//	  and all elements [start:u] are in order and [u] < [v+1]
			//	  therefore the array is in order
			if (u == v)
				break;

			//	u is pointing to a value that is > v
			//	it is possible that lesser values to the left of [u]
			//	  are also > v. Keep moving --u until a value is
			//	  found that is <= [v].  At that point, everything
			rotate_count = -1;
			u--;
			while (u >= start) {
				result._compares++;
				if (*array[u] <= *array[v]) {
					break;
				}
				--u;
				--rotate_count;
			}

			if (debug_verbosely) {
				std::cout << "BEFORE\n";
				std::cout << "start " << start << " end " << end
						  << " u+1 " << u+1 << " v " << v
						  << " rotating " << rotate_count
						  << std::endl;
				for (int i = start; i != end; i++) {
					std::cout << std::setw(3) << i << ' ';
				}
				std::cout << std::endl;
				std::cout << SortingUtilities::arrayElementsToString(&array[start], end-start+1, 3, 4)
						  << std::endl;
			}
			//	at this point `
			//	rotate u to the right in the array
			//	  past the last value in v that was equal to u_value
			result += rotateArrayElementsRight(array, u+1, v, rotate_count);

			if (debug_verbosely) {
				std::cout << "AFTER\n";
				std::cout << SortingUtilities::arrayElementsToString(&array[start], end-start+1, 3, 4)
						  << std::endl;
			}
			//  point to the element that is one past the element
			//    that u was pointing to which has now been rotated
			v = v + rotate_count;
		}

		if (debug_verbosely)
			std::cout << "Exiting mergeBlocksByRotation()\n\n";

		return result;
	}

	//	starting at the right-most block, merge the previous (left) block into
	//	  the elements to the right
	template <typename T>
	ComparesAndMoves mergeAllBlocks(T** array,
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
	 * 	ComparesAndSwaps rotateArray(array, amount, start, end);
	 *
	 * 	rotate the contents of the array by 'amount' positions
	 * 	  where the array is defined as array[start]:array[end] (inclusive)
	 *
	 * 	algorithm:
	 * 		consider amount to rotate = 3
	 * 		array									{ A, B, C, D, E, F, G }
	 * 		reverse the array						{ G, F, E, D, C, B, A }
	 * 		reverse the sub array [start:amount-1]  { E, F, G  D, C, B, A }
	 * 		reverse the sub array [amount:end]		{ E, F, G, A, B, C, D }
	 *
	 * 	note that amount can be negative or greater than the span
	 * 		negative amounts are converted into the equivalent positive amount
	 * 		amounts greater than the span are modulo-division to be within the span
	 */

	template <typename T>
	ComparesAndMoves rotateArrayElementsRight(T** array,
											  index_t start, index_t end,
											  index_t amount) {
		ComparesAndMoves result(0,0);

		if (end <= start)
			return result;

		index_t span = end - start + 1;
		if (span == 0)
			return result;

		//	converts amounts that are not in [0,span) to in range
		amount = blockSortModulo(amount, span);

		if (amount == 0)
			return result;

		//	reverse the entire array
		T* temp;
		for (index_t i = start, j = end; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result._moves += 3;
		}

		//	reverse the left hand side
		for (index_t i = start, j = start+amount-1; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result._moves += 3;
		}

		//	reverse the right hand side
		for (index_t i = start + amount, j = end; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result._moves += 3;
		}
		return result;
	}

	/*
	 * 	ComparesAndMoves rotateBlocksRight(array, tags, first, last, count);
	 *
	 * 	Takes an array of tags which contain the information about the array's blocks
	 * 		and rotates the blocks on [first::last] by tag_rotate_count
	 *		This requires rotating the underlying array to which the blocks refer
	 *
	 * 	Consider a tag array with the associated start:end values
	 * 		   0	  1		 2        3        4		5
	 * 		a[0:2] a[3:6] a[7:10] b[11:14] b[15:18] b[19:20]
	 *      key='d'key='e'key='f' key='b'  key='c'  key='g'
	 *
	 * 	rotateTags(array, tags, 0, 4, 2) results in
	 *
	 * 		   0	  1		  2        3        4		5
	 * 		b[0:3] b[4:7]  a[8:10] a[11:14] a[15:18] b[19:20]
	 * 		k='b'  key='c' key='d' key='e'  key='f'  key='g'
	 *
	 *	Note that blocks are not necessarily all of the same size
	 *
	 *		rotate the underlying array by
	 *			tags[first].start - tags[first+tag_rotate_count].start
	 *		over the span
	 *			[tags[first].start, tags[last].end]
	 *
	 *		rotate the tags [first:last] by tag_rotate_count
	 *
	 *		update the .tart & .end indices of the tags on [first:last]
	 */

	template <typename T>
	ComparesAndMoves rotateBlocksRight(T** array,
									   std::unique_ptr<BlockDescriptor<T>[]> &tags,
							           index_t first, index_t last,
									   int tag_rotate_count)
	{
		/*	**************************************	*/
		/*				Debugging					*/
		/*	**************************************	*/

		constexpr bool debug_verbose = false;

		#pragma push_macro("_debug")
		#define _debug(_dbg_msg_)\
		do {\
			if (debug_verbose) {\
				std::cout << _dbg_msg_ << std::endl;\
			}\
		} while(false)\

		auto tagIndicesToString = [&](int line_number) ->std::string {
			std::stringstream tag_indices_string;
			tag_indices_string << line_number << ": ";
			for (int i = first; i <= last; i++) {
				tag_indices_string << tags[i].spanString();
			}
			return tag_indices_string.str();
		};

		auto tagsToString = [&]() -> std::string {
			std::stringstream tags_string;
			for (int i =  first; i <= last; i++) {
				tags_string << "|" << i << ": " << tags[i].to_string() << "|";
			}
			return tags_string.str();
		};

		auto rotateParametersToString = [&](index_t array_rotation) -> std::string {
			std::stringstream sstring;
			sstring << "tag rotation: " << tag_rotate_count
					<< " element rotation " << array_rotation << std::endl;
			sstring << tagIndicesToString(0) << std::endl;
			return sstring.str();
		};

		/*	**************************************	*/
		/*				rotate lambdas				*/
		/*	**************************************	*/

		auto swapTags = [&tags] (int i, int j) -> ComparesAndMoves {
			BlockSort::BlockDescriptor<T> tmp = tags[i];
			tags[i] = tags[j];
			tags[j] = tmp;
			return ComparesAndMoves(0,3);
		};

		// does not update the tag indices
		auto rotateTagPersonalities = [&]() -> ComparesAndMoves {
			ComparesAndMoves compares_and_moves(0,0);
			_debug(tagsToString());

			// swap entire array
			for (int i = first, j = last; i < j; ++i, --j) {
				compares_and_moves += swapTags(i,j);
			}
			_debug(tagsToString());

			// swap (restore) left side
			for (int i = first, j = first + tag_rotate_count-1; i < j; ++i, --j) {
				compares_and_moves += swapTags(i,j);
			}
			_debug(tagsToString());

			// swap (restore) right side
			for (int i = first+tag_rotate_count, j = last; i < j; ++i, --j) {
				compares_and_moves += swapTags(i,j);
			}
			_debug(tagsToString());

			return compares_and_moves;
		};

		/*	**************************************	*/
		/*				function logic				*/
		/*	**************************************	*/

		ComparesAndMoves compares_and_moves(0,0);

		//	determine the start & end tag
		if (last == first) {
			return compares_and_moves;
		}

		//	if they are in the opposite order, correct them
		if (last < first) {
			index_t tmp = first;
			first = last;
			last = tmp;
		}

		index_t tag_span = last - first + 1;
		tag_rotate_count = blockSortModulo(tag_rotate_count, tag_span);
		if (tag_rotate_count == 0) {
			return compares_and_moves;
		}

		// make a record of the span of the array covered by the tags
		//	 rotating the array needs these two indices
		//	 updating the indices of the rotated tags needs to know the start_index
		index_t first_start_index = tags[first].start_index;
		index_t last_end_index	= tags[last].end_index;
		//	calculate how far the array elements on the right of the array span
		//	  will be rotated off of the end of the span back to the start left
		int array_rotate_count = 0;
		for (int count = tag_rotate_count, tag_i = last; count > 0; --count, --tag_i) {
			array_rotate_count += tags[tag_i].getWidth();
		}

		/*	rotate the underlying array	*/

		compares_and_moves +=
			rotateArrayElementsRight(array, first_start_index, last_end_index, array_rotate_count);

		/*	rotate the tags	*/

		compares_and_moves += rotateTagPersonalities();

		/*	assign indices	*/

		_debug(rotateParametersToString(array_rotate_count));

		//	The tag.type & tag.key are in the correct place in the tag array
		//	The indices still point the the original place in the underlying array
		//	Although each tag's indices are incorrect, the span between the
		//	  incorrect indices has not changed.  Use this to assign new indices

		index_t _start_index = first_start_index;
		for (int tag_i = first; tag_i <= last; tag_i++) {
			index_t _end_index = _start_index + tags[tag_i].getWidth() - 1;
			tags[tag_i].start_index = _start_index;
			tags[tag_i].end_index   = _end_index;
			_start_index 		= _end_index + 1;
			_debug(tagIndicesToString(tag_i));
		}
		_debug(tagsToString());

		return compares_and_moves;

		#pragma pop_macro("_debug")
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
	ComparesAndMoves sortBlocksBySwapping(T** array,
										  std::unique_ptr<BlockDescriptor<T>[]> &blocks,
										  int num_blocks) {
		ComparesAndMoves result(0,0);
		return result;
	}

	/*
	 * 	ComparesAndSwaps sortBlockTypes(array, size, blocks, num_blocks)
	 *
	 * 	Sorts the A_Blocks amongst themselves and the B_Blocks amongst themselves.
	 *
	 * 	Immediately after the first set of blocks are created on the unordered array
	 * 	  the elements within the blocks are not sorted, although their keys have
	 * 	  been assigned.  Then, the elements within each block are sorted.
	 * 	  Changing the position of the array elements within the blocks makes it
	 * 	  likely that the element in the first position of an A_Block, its key
	 * 	  of the element in the last position of a B_Block, its key, has change
	 *
	 * 	The assignment of which blocks are A_Blocks vs B_Blocks is solely based
	 * 	  upon dividing the array into (roughly) halves. All blocks to the left
	 * 	  of the mid point are assigned A_Blocks, and all to the right of the
	 * 	  midpoint, including the block that contains the midpoint are B_Blocks
	 * 	  No A_Block will exchange with a B_Block at this point in the sort.
	 *
	 *	Given that the keys may have changed, but that no A_Block needs to be
	 *	  sorted relative to a B_Block, means that the A_Blocks probably have
	 *	  to be sorted amongst themselves and the B_Blocks probably have to
	 *	  be sorted amongst themselves.
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
	ComparesAndMoves sortEachBlockTypeSeparately(T** array, index_t size,
			std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_blocks)
	{
		ComparesAndMoves result;
		//	calculate how many of each block are present
		int num_a_blocks = 0;
		for ( ; num_a_blocks < num_blocks; num_a_blocks++) {
			if (blocks[num_a_blocks].type == BlockType::B_BLOCK) {
				break;
			}
		}

		// insertion sort the A_Blocks
		for (int i = 1; i != num_a_blocks; i++) {
			for (int j = i; j > 0; j--) {
				result._compares++;
				if (*blocks[j].key >= *blocks[j-1].key) {
					break;
				}
				result += rotateBlocksRight(array, blocks, j-1, j, 1);
			}
		}

		// insertion sort the B_Blocks
		for (int i = num_a_blocks; i != num_blocks; i++) {
			for (int j = i; j > num_a_blocks; j--) {
				result._compares++;
				if (*blocks[j].key >= *blocks[j-1].key) {
					break;
				}
				result += rotateBlocksRight(array, blocks, j-1, j, 1);
			}
		}
		return result;
	}

	/*
	 *	ComparesAndMoves blockSwap(array, b1_start, b1_end, b2_start, bl2_end)
	 *
	 *	if the block sizes are not the same, it returns
	 *		(it is the caller's responsibility to determine if a rotate should
	 *		 occur instead of a swap)
	 *	if either of the indices are less than zero, it returns
	 *	if the blocks overlap (exchange is not defined), it returns
	 */

	template <typename T>
	ComparesAndMoves swapBlocks(T** array,
								index_t block1_start, index_t block1_end,
								index_t block2_start, index_t block2_end)
	{
		ComparesAndMoves result(0,0);

		//	if the indices are not credible, leave
		if (block1_start < 0 || block1_end < 0 || block2_start < 0 || block2_end < 0) {
			//	TODO - throw an exception
			return result;
		}
		//	if necessary, swap indices so that the block is defined left to right
		if (block1_start > block1_end) {
			index_t tmp  = block1_start;
			block1_start = block1_end;
			block1_end   = tmp;
		}
		if (block2_start > block2_end) {
			index_t tmp  = block2_start;
			block2_start = block2_end;
			block2_end   = tmp;
		}
		//	ensure the blocks are well formatted:
		//	1. same size
		//	2. if necessary, swap indices
		//	3. ensure the blocks don't overlap
		index_t block1_span = block1_end-block1_start+1;
		index_t block2_span = block2_end-block2_start+1;

		//	this is not an exception
		if (block1_span == 0) {
			return result;
		}
		//	if the blocks are different sizes, exit
		if (block1_span != block2_span) {
			// TODO - throw and exception
			return result;
		}
		//	if it is the same block, we are done
		if (block1_start == block2_start) {
			return result;
		}
		// do the blocks overlap?
		if (block1_start < block2_start) {
			//	block1 is the leftmost block
			//	  does block1 overlap with block2?`
			if (block1_end >= block2_start) {
				// TODO - throw exception
				return result;
			}
		} else {
			// 	block2 is the leftmost block
			//	  does block2 overlap with block1?
			if (block2_end >= block1_start) {
				// TODO - throw exception
				return result;
			}
		}

		T* temp;
		for (index_t i = block1_start, j = block2_start;
			 i <= block1_end; ++i, ++j) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result._moves += 3;
		}
		return result;
	}

	template<typename T>
	ComparesAndMoves swapBlockDescriptors(std::unique_ptr<BlockDescriptor<T[]>> &tags, int i, int j) {

		//	swapping the Tag.key, which is an array element, takes three moves
		ComparesAndMoves result(0,3);
		BlockDescriptor<T>tmp = tags[i];
		tags[i] = tags[j];
		tags[j] = tmp;
		return result;
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
	ComparesAndMoves sortPointerstoObjects(T **array, index_t size) {

		bool debug_verbose = false;
		bool print_error = true;
		std::stringstream msg;
		constexpr index_t minimum_block_size = 16;

		ComparesAndMoves result(0,0);

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

		assignKeys(array, block_descriptors, num_blocks);
		result += sortEachBlockTypeSeparately(array, size, block_descriptors, num_blocks);

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
				result += mergeContiguousBlocksByRotating(array, left_start,
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


	/*	**************************************************************************	*/
	/*	**************************************************************************	*/
	/*								Debugging resources								*/
	/*	**************************************************************************	*/
	/*	**************************************************************************	*/

	//	compares an array of block descriptors using the operator< overload

	template <typename T>
	bool areBlocksSorted(std::unique_ptr<BlockDescriptor<T>[]> &tags, int num_tags) {

		for (int i = 0; i != num_tags-1; i++) {
			if (tags[i+1] < tags[i])
				return false;
		}
		return true;
	}

	// 	prints a line where the blocks a represented graphically
	template <typename T>
	std::string blockDescriptorsToString(std::unique_ptr<BlockDescriptor<T>[]> &tags,
										 int num_tags,
										 int element_width) {

		//	restores ostream state with its destructor
		OStreamState ostream_state;

		std::stringstream result;
		if (element_width == 0) {
			result << "ERROR: printTags() called with element_width == 0";
			return result.str();
		}


		for (int i = 0; i != num_tags; i++) {

			int elements_remaining = tags[i].getWidth();

			switch (elements_remaining) {
			case 0:
				result << "!!!!ERROR: printTags() passed a block of size 0 elements: ";
				return result.str();
			case 1:
				result  << SingleElement(tags[i], element_width);
				break;
			default:
				result << OpeningElement(tags[i], element_width);
				elements_remaining--;
				while(elements_remaining-- > 1) {
					std::cout.fill(TAG_SPACE_CHAR);
					result << std::setw(element_width) << TAG_SPACE_CHAR;
				}
				result << ClosingElement(tags[i], element_width);
				break;
			}
		}
		return result.str();
	}

	/*
	 * 	create a string that prints the whole array, including the blockDescriptors
	 */
	template <typename T>
	std::string blockSortToString(T** array, index_t size, index_t v,
							 	  std::unique_ptr<BlockDescriptor<T>[]> &tags, int num_tags,
								  int value_width, int element_width) {
		OStreamState ostream_state;
		std::stringstream result;
		result << arrayIndicesToString(size, v, element_width) << std::endl;
		result << SortingUtilities::arrayElementsToString(array, size, value_width, element_width) << std::endl;
		result << blockDescriptorsToString(tags, num_tags, element_width);
		return result.str();
	}

	template <typename T>
	std::string blockDescriptorsSummaryToString(std::unique_ptr<BlockDescriptor<T>[]> &block_descriptors,
												int num_blocks) {
		std::stringstream msg;
		for (int i = 0; i != num_blocks; i++) {
			msg << (block_descriptors[i].type == BlockType::A_BLOCK ? "A" : "B");
			msg << "[" << std::setw(5) << block_descriptors[i].start_index << ":"
				<< std::setw(5) << block_descriptors[i].end_index << "] [";
			if (block_descriptors[i].type == BlockType::A_BLOCK) {
				msg << std::setw(5) << block_descriptors[i].start_index;
			} else {
				msg << std::setw(5) << block_descriptors[i].end_index;
			}
			msg << "] = " << *block_descriptors[i].key;
			if (i != num_blocks-1) {
				msg << " | ";
			}
		}
		return msg.str();
	}

	//	creates either	"     |" for an A_BLOCK or "   B|" for a B_BLOCK
	//		where width of the string is set by 'chars_remaining'

	template <typename T>
	std::string ClosingElement(BlockDescriptor<T> &tag, int chars_remaining) {
		std::stringstream result;
		result.fill(TAG_SPACE_CHAR);
		switch (chars_remaining) {
		case 0:
			result << __FUNCTION__ << " called with element_width == 0";
			break;
		case 1:
			result << to_char(tag.type);
			break;
		default:
			if (chars_remaining > 2) {
				result << std::setw(chars_remaining-2) << TAG_SPACE_CHAR;
			}
			if (tag.type == BlockType::B_BLOCK) {
				result << to_char(tag.type);
			} else {
				result << TAG_SPACE_CHAR;
			}
			result << TAG_BOUNDARY_CHAR;
			break;
		}
		return result.str();
	}

	//	creates either	"|A   " for an A_BLOCK or "|    " for a B_BLOCK
	//		where width of the string is set by 'chars_remaining'

	template <typename T>
	std::string OpeningElement(BlockDescriptor<T> &tag, int chars_remaining) {
		std::stringstream result;
		result.fill(TAG_SPACE_CHAR);
		switch (chars_remaining) {
		case 0:
			result << __FUNCTION__ << " called with element_width == 0";
			break;
		case 1:
			result << to_char(tag.type);
			break;
		default:
			result << TAG_BOUNDARY_CHAR;
			chars_remaining--;
			switch (tag.type) {
			case BlockType::A_BLOCK:
				result << to_char(tag.type);
				chars_remaining--;
				if (chars_remaining >0) {
					result << std::setw(chars_remaining) << TAG_SPACE_CHAR;
				}
				break;
			case BlockType::B_BLOCK:
				result << std::setw(chars_remaining) << TAG_SPACE_CHAR;
				break;
			case BlockType::UNSPECIFIED:
				result.fill('?');
				result << std::setw(chars_remaining) << '?';
			}
			break;
		}
		return result.str();
	}

	//	creates a string representation of a block
	//	  |A    |   or |    B|  or |?????|
	//	where the width of the representation
	//	is equal to the passed param 'element_width'

	template <typename T>
	std::string SingleElement(BlockDescriptor<T> &tag, int chars_remaining) {
		std::stringstream result;
		result.fill(TAG_SPACE_CHAR);

		switch(chars_remaining) {
		case 0:
			result << __FUNCTION__ << " called with element_width == 0";
			break;
		case 1:
			result << to_char(tag.type);
			break;
		case 2:
			result << TAG_BOUNDARY_CHAR
				   << to_char(tag.type);
			break;
		case 3:
			result << TAG_BOUNDARY_CHAR << to_char(tag.type)
				   << TAG_BOUNDARY_CHAR;
			break;
		default:
			result << TAG_BOUNDARY_CHAR;
			chars_remaining--;
			switch (tag.type) {
			case BlockType::A_BLOCK:
				result << to_char(tag.type);
				chars_remaining--;
				if (chars_remaining > 1) {
					result << std::setw(chars_remaining-1) << TAG_SPACE_CHAR;
				}
				break;
			case BlockType::B_BLOCK:
				if (chars_remaining > 2) {
					result << std::setw(chars_remaining-2) << TAG_SPACE_CHAR;
				}
				result << to_char(tag.type);
				break;
			default:
				result.fill('?');
				result << std::setw(chars_remaining-1) << '?';
				break;
			}
			result << TAG_BOUNDARY_CHAR;
			break;
		}
		return result.str();
	}

}	// namespace BlockSort

#pragma pop_macro("CREATE_BLOCKS_SYMMETRICALLY")

#endif /* BLOCKSORT_H_ */
