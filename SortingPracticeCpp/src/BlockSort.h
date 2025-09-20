/*
 * BlockSort.h
 *
 *  Created on: Aug 11, 2025
 *      Author: joe
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

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <memory>

#include "SortingPracticeDataTypes.h"
#include "SortingUtilities.h"
#include "InsertionSort.h"

#include "BlockSortBlockDescriptor.h"

//#define DEBUG_VERBOSE_SORT_BLOCKS 1
//#define DEBUG_SUMMARY_SORT_BLOCKS
#define DEBUG_VERBOSE_BLOCK_SORT
	constexpr char 	TAG_BOUNDARY_CHAR 	= '|';
	constexpr char 	TAG_SPACE_CHAR 		= ' ';
	constexpr int 	ELEMENT_WIDTH 		= 5;
	constexpr int 	VALUE_WIDTH 		= 4;


using index_t = array_size_t;

index_t	floorLog2(index_t num);
index_t blockSortModulo(index_t rotation_count, index_t span);

std::string arrayIndicesToString(array_size_t size, array_size_t v,
								 int element_width = ELEMENT_WIDTH);
std::string arrayIndicesToString(std::string trailer,
								 array_size_t size, array_size_t v,
								 int element_width = ELEMENT_WIDTH);
std::string printArrayStartMiddleEnd(array_size_t size, array_size_t start, array_size_t mid, array_size_t end, int element_width);
std::string arrayIndicesToStringLine(array_size_t size, int value_width, int element_width);
std::string printLineArrayStartMiddleEnd(array_size_t size, array_size_t start, array_size_t mid, array_size_t end, int element_width);

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
	/*		outputting graphic representation of tags	*/
	template <typename T>
	std::string blockDescriptorsToString(std::unique_ptr<BlockDescriptor<T>[]> &tags,
										 int num_tags,
										 int element_width = ELEMENT_WIDTH);
	template <typename T>
	std::string blockDescriptorsToString(std::string trailer,
								 	 	 std::unique_ptr<BlockDescriptor<T>[]> &tags,
										 int num_tags,
										 int element_width = ELEMENT_WIDTH);
	template <typename T>
	std::string blockDescriptorsSummaryToString(std::unique_ptr<BlockDescriptor<T>[]> &descriptors,
												int num_tags);
	template <typename T>
	std::string blockSortToString(T** array, index_t size, index_t v,
							 	  std::unique_ptr<BlockDescriptor<T>[]> &tags,
								  int num_tags,
								  int value_width = VALUE_WIDTH,
								  int element_width = ELEMENT_WIDTH);
	template <typename T>
	void printBlockSortArray(std::string trailer,
							 T** array, index_t size, index_t v,
							 std::unique_ptr<BlockDescriptor<T>[]> &tags,
							 int num_tags);
	template <typename T>
	void printElements(std::string trailer, T** array, index_t size,
					   int value_width = VALUE_WIDTH,
					   int element_width = ELEMENT_WIDTH);
	template <typename T>
	std::string toStringTagClosingElement(BlockDescriptor<T> &tag, int chars_remaining);
	template <typename T>
	std::string toStringTagOpeningElement(BlockDescriptor<T> &tag, int chars_remaining);
	template <typename T>
	std::string toStringTagSingleElement(BlockDescriptor<T> &tag, int chars_remaining);


	/*	**************************************************************************	*/
	/*	**************************************************************************	*/
	/*							algorithm function declarations						*/
	/*	**************************************************************************	*/
	/*	**************************************************************************	*/

	/*	Assigns key values to the blocks if the underlying array has been reordered */
	template <typename T>
	ComparesAndMoves assignKeys(T**array, std::unique_ptr<BlockDescriptor<T>[]> &descriptors, int num_descriptors);

	template <typename T>
	int createBlockDescriptors( T** array, index_t start, index_t mid, index_t end,
			    				int block_size,
								std::unique_ptr<BlockDescriptor<T>[]> &descriptors);
	/*
	 * 	In an array of blockDescriptors, find the rightmost block that is < key
	 */
	template <typename T>
	ComparesAndMoves findRightmostSmallerBlock(std::unique_ptr<BlockDescriptor<T>[]> &blocks,
											   index_t first, index_t last,
								   	   	   	   T* key, index_t &key_location);
	//	this combines adjacent blocks into a single block
	template <typename T>
	int mergeAdajacentPairsOfDescriptors( T** array,
					std::unique_ptr<BlockDescriptor<T>[]> &descriptors,
					int num_descriptors);

	template <typename T>
	ComparesAndMoves mergeAllBlocks(T** array, index_t size,
					std::unique_ptr<BlockDescriptor<T>[]> &block_descriptors, int num_blocks);
	/*
	 * 	Blocks do not have to be contiguous,
	 */
	template <typename T>
	ComparesAndMoves mergeTwoBlocksByTable(T** array,
										index_t left_start, index_t left_end,
										index_t right_start, index_t right_end);
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


	template <typename T>
	ComparesAndMoves sortBlocksBinarySearch(T **array, index_t size,
			std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_tags);
	template <typename T>
	ComparesAndMoves sortBlocksHybrid(T **array, index_t size,
			std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_blocks);
	template <typename T>
	ComparesAndMoves sortBlocksRightToLeft(T **array, index_t size,
			std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_blocks);
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
	 * 	createBlockDescriptors(array, start, mid, end, block_size, descriptor_dst&);
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
	 * 		[end-1] is the end_index of the last A-type block and
	 * 		[end] is the start_index of the first B-type block
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

	template <typename T>
	int  createBlockDescriptors(T** array, index_t start, index_t mid, index_t end,
			    		 	 	int block_size, std::unique_ptr<BlockDescriptor<T>[]> &blocks) {

		ComparesAndMoves result(0,0);
		index_t lower_span = mid-start;
		index_t upper_span = end-mid + 1;

		// calculate the total number of blocks
		int num_blocks = lower_span / block_size + upper_span / block_size;
		int first_block_size = lower_span % block_size;
		// if there is a partial first block
		//	  it is necessary to calculate the first_block_size in order to
		//	  know where .end_index is in the first block
		if (first_block_size != 0)
			num_blocks++;
		//	if there is a partial last block
		//	  it is not necessary to calculate the block_size of the last block
		//	  the algorithm will make the .end_index of the last block = 'end'
		if (upper_span % block_size)
			num_blocks++;

		// create the block storage
		blocks = std::unique_ptr<BlockDescriptor<T>[]>(new BlockDescriptor<T>[num_blocks]);

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


	/*	**********************************************************	*/
	/*		Binary Search for the right-most block that is < key	*/
	/*	**********************************************************	*/

	constexpr index_t smaller_block_not_found = -1;

	template <typename T>
	ComparesAndMoves findRightmostSmallerBlock(std::unique_ptr<BlockDescriptor<T>[]> &blocks,
											   index_t first, index_t last,
								   	   	   	   T* key, index_t &key_location)
	{
		//	debugging resources
		constexpr bool debug_verbose = false;
		std::stringstream findRightMostSmallerValue_message;

		// return number of compares & moves(=0) which is a
		//	figure of merit in evaluating the algorithm
		ComparesAndMoves result(0,0);

		if (key == nullptr) {
			if (debug_verbose) {
				std::cout << __FUNCTION__ << " passed nullptr key\n";
			}
			return result;
		}

		//	find the midpoint of between [first:last]
		//	if the span is only 1, return first+1, which is last
		auto nextIndex = [] (index_t l_first, index_t l_last) -> index_t {

			//	the algorithm requires that first < last
			if (l_first == l_last) {
				//	first == last
				return l_first;
			}

			if (l_first < l_last) {
				//	first > last
				index_t tmp = l_first;
				l_first = l_last;
				l_last = tmp;
			}

			//	The algorithm itself
			if (l_last - l_first == 1) {
				return l_last;
			} else {
				//	in an odd span, return value less than span/2 = n.5 = n
				return l_first + (l_last-l_first)/2;
			}
		};

		/*
		 * 	The binary search
		 */

		//	first == last does not require any calculation
		//		if the key > block, return first, else return key not found
		if (first == last)
		{
			result._compares++;
			if (*key > *blocks[first].key) {
				key_location = first;
			} else {
				key_location = smaller_block_not_found;
			}
			if (debug_verbose) {
			//	debugging messages
				findRightMostSmallerValue_message
					<< "start == end == " << std::setw(2) << first
					<< ", trying guess [" << std::setw(2) << first
					<< "] = " << std::setw(2) << *blocks[first].key
					<< (*key > *blocks[first].key ? " is    " : " is NOT")
					<< " smaller than " << *key;

				findRightMostSmallerValue_message << " returning " << std::setw(2) << first;
				std::cout << findRightMostSmallerValue_message.str() << std::endl;
			}

			return result;
		}


		key_location = smaller_block_not_found;

		//	algorthm works when last > first
		if (last < first) {
			index_t tmp = first;
			first = last;
			last = tmp;
		}

		index_t guess = nextIndex(first, last);
		//	The algorithm searches through block values < key (if they exist) until it
		//	  finds a block value > key.  Once it finds a value that is > key,
		//	  it returns the location of the previous (right-most) value which key > block
		index_t smaller_value_index = smaller_block_not_found;

		while (1) {
			//	debug
			if (debug_verbose) {
				findRightMostSmallerValue_message
					<< "previous guess " << std::setw(2) << smaller_value_index
					<< ", trying guess [" << std::setw(2) << guess
					<< "] = " << std::setw(2) << *blocks[guess].key
					<< (*key > *blocks[guess].key ? " is    " : " is NOT")
					<< " smaller than " << *key;
			}

			//	compare *key to *blocks[guess].key
			result._compares++;
			if (*key > *blocks[guess].key) {
				// check to see if any elements further to the right
				//	 of this to see if there are larger values
				//	 in the array that are < the key
				smaller_value_index = guess;	// [guess] < key
				first = guess+1;
				if (first > last) {
					//	if there are no more blocks, done
					key_location = smaller_value_index;
					break;
				}
				guess = nextIndex(first, last);
			} else {
				// key <= array which means 'guess' is too far to the right
				//   look to the left for an element which is smaller than the key
				last = guess-1;
				if (last < first) {
					//	if no more blocks, done
					break;
				}
				guess = nextIndex(first, last);
			}
			if (debug_verbose) {
				//	debug stuff
				findRightMostSmallerValue_message
					<< " next guess = " << std::setw(2) << guess << std::endl;
			}
		}

		//	the previous found block < key is passed to the caller
		//	(if no value found, 'key_location' contains 'smaller_value_not_found')
		key_location = smaller_value_index;

		//	debug stuff
		if (debug_verbose) {
			findRightMostSmallerValue_message
				<< " returning " << key_location << std::endl;
			std::cout << findRightMostSmallerValue_message.str();
		}

		//	return diagnostices
		return result;
	}

	/*
	 * 	num_descriptors mergeAdajacentPairsOfDescriptors(descriptors, num_descriptors)
	 *
	 * 	in an array of BlockDescriptors [], merges every pair of descriptors [n:n+1]
	 */

	template <typename T>
	int mergeAdajacentPairsOfDescriptors( T** array,
								std::unique_ptr<BlockDescriptor<T>[]> &descriptors,
								int num_descriptors) {

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
	 * 	  each swap. This table of array indices allows the algorithm to use O(1) space
	 * 	  complexity.
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
	 *
	 * 	Note that it is possible to place all of the right block elements before all
	 * 		of the left block elements have been placed.  In that case, it is quite
	 * 		possible that the left block elements will not be in sequential order
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

	template <typename T>
	ComparesAndMoves mergeTwoBlocksByTable(T ** array,
										index_t left_start, index_t left_end,
										index_t right_start, index_t right_end) {
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
				std::cout << arrayElementsToString(&array[start], end-start+1, 3, 4)
						  << std::endl;
			}
			//	at this point `
			//	rotate u to the right in the array
			//	  past the last value in v that was equal to u_value
			result += rotateArrayElementsRight(array, u+1, v, rotate_count);

			if (debug_verbosely) {
				std::cout << "AFTER\n";
				std::cout << arrayElementsToString(&array[start], end-start+1, 3, 4)
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

	template <typename T>
	ComparesAndMoves sortBlocksBinarySearch(T **array, index_t size,
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

			//	Binary search for the largest B_Block that is less than this A_Block
			//	There may not be a B_Block that is less than this A_Block

			//	0	1	2	3	4	5	6   7
			//			ai  st              end
			//	A4, A6, A7, B5, B5, B6, B7, B8

			// b_block_end is passed by value to the function
			// b_block_end receives the value by reference
			result += findRightmostSmallerBlock(blocks, b_block_start, b_block_end,
											    blocks[a_block_index].key, b_block_end);

			//	0	1	2	3	4	5	6   7
			//			ai  st      end
			//	A4, A7, A8, B5, B5, B6, B9, B9

			//	If all B_Blocks are greater than this A_Block, the blocks are in order
			if (b_block_end == smaller_block_not_found) {
				break;
			}

			//	blocks[a_index] > blocks[b_end]
			//	determine how many smaller A_Blocks
			//	  are still larger than this B_Block

			while (a_block_index >= 0) {
				result._compares++;
				if (*blocks[a_block_index].key <= *blocks[b_block_end].key) {
					break;
				}
				a_block_index--;
			}

			//	0	1	2	3	4	5	6   7
			//	ai		    st      end
			//	A4, A7, A8, B5, B5, B6, B9, B9

			//	The above loop terminated because either
			//		[a_block_index] goes to the left of the [b_block_index]
			//	 or a_block_index < 0, which means all B_Blocks go to the
			//		are less than [0] and go to the left of [0]
			a_block_index++;

			//	0	1	2	3	4	5	6   7
			//	    ai	    st      end
			//	A4, A7, A8, B5, B5, B6, B9, B9	rotate count = -2

			// there will be at least one A_Block that is rotated **left** (-)
			//	 to the right-hand side of the B_Block < [a_block_index]
			int block_rotate_count = -(b_block_start - a_block_index);

			if (debug_verbose) {
				index_t array_size = blocks[num_tags-1].end_index+1;
				index_t v = blocks[num_a_blocks].start_index;
				std::cout << "BEFORE: "
						  << " b_index (end) = " << std::setw(2) << b_block_end
						  << " a_index (start) = " << std::setw(2) << a_block_index
						  << " rotation count = " << std::setw(2) << block_rotate_count
						  << "\n"
						  << blockSortToString(array, array_size, v, blocks, num_tags)
						  << std::endl;
			}
			result += rotateBlocksRight(array, blocks,
										a_block_index, b_block_end,
										block_rotate_count);

			//	0	1	2	3	4	5	6   7
			//	    ai	    st      end
			//	A4, B5, B5, B6, A7  A8  B9, B9	rotate count = -2

			// the B_Block has been shifted left by 'rotate_count'
			b_block_start += block_rotate_count;
			b_block_end += block_rotate_count;
			a_block_index--;

			//	0	1	2	3	4	5	6   7
			//	ai	st      end
			//	A4, B5, B5, B6, A7  A8  B9, B9

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

	template <typename T>
	ComparesAndMoves sortBlocksHybrid(T **array, index_t size,
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
				result += findRightmostSmallerBlock(blocks, b_block_index, num_blocks-1,
													blocks[a_block_index].key, b_block_index);
				//	0	 1	  2	   3	4	 5	  6    7
				//			  ai             bi
				//	A=4, A=6, A=7, B=5, B=5, B=6, B=7, B=8

				//	If all B_Blocks are greater than this A_Block, the blocks are in order
				//	0	 1	  2	   3	4	 5	  6    7
				//			  ai
				//	A=4, A=6, A=7, B=8, B=9, B=10,B=11,B=12
				if (b_block_index == smaller_block_not_found) {
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

	template <typename T>
	ComparesAndMoves sortBlocksRightToLeft(T **array, index_t size,
										   std::unique_ptr<BlockDescriptor<T>[]> &blocks, int num_blocks) {

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

	/*
	 * 	ComparesAndSwaps sortBlockTypes(array, size, blocks, num_blocks)
	 *
	 * 	Sorts the A_Blocks and the B_Blocks.  This is necessary to put the
	 * 		blocks in each half of the array into order immediately after
	 * 		the step which sorts the elements within each block
	 */

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
		int num_blocks = createBlockDescriptors(array, start, mid, end,
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
			result += mergeAllBlocks(array, block_descriptors, num_blocks);
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
				result += mergeTwoBlocksByTable(array, left_start, left_end,
													   right_start, right_end);
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
			num_blocks = mergeAdajacentPairsOfDescriptors(array,
														  block_descriptors,
														  num_blocks);
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

	template <typename T>
	std::string arrayElementsToString(T** array, index_t size, int value_width, int element_width) {
		OStreamState current_state;
		std::stringstream result;
		int spacer_width = element_width - value_width;
		for (int i = 0; i < size-1; i++) {
			result << std::right << std::setw(value_width) << *array[i];
			if (spacer_width) {
				result << std::setw(spacer_width) << ' ';
			}
		}
		result << std::right << std::setw(value_width) << *array[size-1];
		return result.str();
	}

	template <typename T>
	std::string arrayElementsToString(std::string trailer, T** array, index_t size, int value_width, int element_width) {
		OStreamState current_state;
		std::stringstream result;
		result << arrayElementsToString(array, size, value_width, element_width);
		result << trailer;
		return result.str();
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
				result  << toStringTagSingleElement(tags[i], element_width);
				break;
			default:
				result << toStringTagOpeningElement(tags[i], element_width);
				elements_remaining--;
				while(elements_remaining-- > 1) {
					std::cout.fill(TAG_SPACE_CHAR);
					result << std::setw(element_width) << TAG_SPACE_CHAR;
				}
				result << toStringTagClosingElement(tags[i], element_width);
				break;
			}
		}
		return result.str();
	}

	template <typename T>
	std::string blockDescriptorsToString(std::string trailer,
								 	 	 std::unique_ptr<BlockDescriptor<T>[]> &tags,
										 int num_tags,
										 int element_width)
	{
		OStreamState ostream_state;
		std::stringstream result;
		result << blockDescriptorsToString(tags, num_tags, element_width);
		result << trailer;
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
		result << arrayElementsToString(array, size, value_width, element_width) << std::endl;
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

	template <typename T>
	void printBlockSortArray(std::string trailer, T** array, index_t size, index_t v,
							 std::unique_ptr<BlockDescriptor<T>[]> &tags, int num_tags) {

		constexpr const int 	element_width = 4;
		constexpr const int 	value_width = element_width - 1;

		//	preserve state of ostream; destructor will restore state
		OStreamState ostream_state;
		std::cout << arrayIndicesToString(size, v, element_width) << std::endl;
		std::cout << arrayElementsToString(array, size, value_width, element_width) << std::endl;
		std::cout << blockDescriptorsToString(std::string(), tags, num_tags, element_width);
		std::cout << trailer;
	}

	template <typename T>
	void printElements(std::string trailer, T** array, index_t size, int value_width, int element_width) {
		OStreamState current_state;

		int spacer_width = element_width - value_width;
		for (int i = 0; i != size; i++) {
			if (spacer_width) {
				std::cout << std::setw(spacer_width) << ' ';
			std::cout << std::setw(value_width) << *array[i];
			}
		}
		std::cout << trailer;
	}

	//	creates either	"     |" for an A_BLOCK or "   B|" for a B_BLOCK
	//		where width of the string is set by 'chars_remaining'

	template <typename T>
	std::string toStringTagClosingElement(BlockDescriptor<T> &tag, int chars_remaining) {
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
	std::string toStringTagOpeningElement(BlockDescriptor<T> &tag, int chars_remaining) {
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
	std::string toStringTagSingleElement(BlockDescriptor<T> &tag, int chars_remaining) {
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

#endif /* BLOCKSORT_H_ */
