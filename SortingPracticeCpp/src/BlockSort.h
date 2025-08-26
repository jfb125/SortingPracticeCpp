/*
 * BlockSort.h
 *
 *  Created on: Aug 11, 2025
 *      Author: joe
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

//#define DEBUG_VERBOSE_SORT_BLOCKS 1
//#define DEBUG_SUMMARY_SORT_BLOCKS
#define DEBUG_VERBOSE_BLOCK_SORT
#define SORT_BLOCKS_LEFT_TO_RIGHT 1
#define SORT_BLOCKS_RIGHT_TO_LEFT 2


array_size_t floorLog2(array_size_t num);
array_size_t blockSortModulo(array_size_t, array_size_t);
std::string printArrayIndices(array_size_t size, int value_width, int element_width);
std::string printLineArrayIndices(array_size_t size, int value_width, int element_width);
std::string printArrayIndices(std::string header, array_size_t size, int value_width, int element_width);

#define TAG_BOUNDARY_CHAR '|'
#define TAG_SPACE_CHAR ' '
#define ELEMENT_WIDTH 4
#define VALUE_WIDDTH 3

enum class BlockType {
	A_BLOCK,
	B_BLOCK,
	UNSPECIFIED
};
char to_char(BlockType type);

#define BLOCK_MERGE_BY_ROTATE

namespace BlockSort {
	using index_t = array_size_t;
	using amount_t	= array_size_t;
	template <typename T>
	class BlockTag {
	public:
		BlockType type;
		T* key;
		index_t start_index;
		index_t end_index;

		index_t	num_elements() const {
			return end_index - start_index + 1;
		}

		BlockTag() {
			type = BlockType::UNSPECIFIED;
			key = nullptr;
			start_index = 0;
			end_index = 0;
		}
		BlockTag(BlockType t, T* k, index_t s, index_t e) {
			type = t;
			key = k;
			start_index = s;
			end_index = e;
		}
		BlockTag(const BlockTag &other) {
			if (this != &other) {
				type = other.type;
				key = other.key;
				start_index = other.start_index;
				end_index = other.end_index;
			}
		}
		BlockTag& operator=(const BlockTag &other) {
			if (this != &other) {
				type = other.type;
				key = other.key;
				start_index = other.start_index;
				end_index = other.end_index;
			}
			return *this;
		}

		bool operator==(const BlockTag &other) const {
			if (key != nullptr && other.key != nullptr) {
				return *key == *other.key;
			}
			return false;
		}
		bool operator<(const BlockTag &other) const {
			if (key != nullptr && other.key != nullptr) {
				return *key < *other.key;
			}
			return false;
		}
		bool operator<=(const BlockTag &other) const {
			return *this == other || *this < other;
		}
		bool operator>(const BlockTag &other) const {
			return  !(*this == other || *this < other);
 		}
		bool operator>=(const BlockTag &other) const {
			return !(*this < other);
		}
		bool operator!=(const BlockTag &other) const {
			return !(*this == other);
		}
		std::string to_string(int index_width = 0) const {
			std::stringstream result;
			if (type != BlockType::UNSPECIFIED) {
				result 	<< (type == BlockType::A_BLOCK ? "A block " : "B block ");
				result 	<< "[" << std::setw(index_width) << start_index << ":"
						<< std::setw(index_width) << end_index << "] ";
				if (type == BlockType::A_BLOCK) {
					result << "[" << std::setw(index_width) << start_index << "] = ";
					if (key != nullptr) {
						result << *key;
					} else {
						result << "key is nullptr";
					}
				} else {
					result << "[" << std::setw(index_width) << end_index << "] = ";
					if (key != nullptr) {
						result << *key;
					} else {
						result << "key is nullptr";
					}
				}
			} else {
				result << "Unintialized block";
			}
			return result.str();
		}
	};

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							debugging resources							*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	/*	***********************************************	*/
	/*		outputting graphic representation of tags	*/
	/*	***********************************************	*/

	template <typename T>
	bool isSorted(BlockTag<T> *tags, int num_tags) {

		for (int i = 0; i != num_tags-1; i++) {
			if (tags[i+1] < tags[i])
				return false;
		}
		return true;
	}

	//	creates either	"|A   " for an A_BLOCK or "|    " for a B_BLOCK
	//		where width of the string is set by 'chars_remaining'

	template <typename T>
	std::string toStringTagOpeningElement(BlockTag<T> &tag, int chars_remaining) {
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

	//	creates either	"     |" for an A_BLOCK or "   B|" for a B_BLOCK
	//		where width of the string is set by 'chars_remaining'

	template <typename T>
	std::string toStringTagClosingElement(BlockTag<T> &tag, int chars_remaining) {
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


	//	creates a string representation of a block
	//	  |A    |   or |    B|  or |?????|
	//	where the width of the representation
	//	is equal to the passed param 'element_width'

	template <typename T>
	std::string toStringTagSingleElement(BlockTag<T> &tag, int chars_remaining) {
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

	// 	prints a line where the blocks a represented graphically

	template <typename T>
	std::string printTags(	std::string trailer, std::unique_ptr<BlockTag<T>[]> &tags,
					int num_tags, int element_width) {

		//	restores ostream state with its destructor
		OStreamState ostream_state;

		std::stringstream result;
		if (element_width == 0) {
			result << "ERROR: printTags() called with element_width == 0" << trailer;
			return result.str();
		}


		for (int i = 0; i != num_tags; i++) {

			int elements_remaining = tags[i].num_elements();

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
		result << trailer;
		return result.str();
	}

	/*	**************************************************	*/
	/*				print the values on a line				*/
	/*	**************************************************	*/

	template <typename T>
	void printElements(std::string trailer, T** array, array_size_t size, int value_width, int element_width) {
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

	template <typename T>
	void printBlockSortArray(std::string trailer, T** array, array_size_t size, index_t v, BlockTag<T> tags[], int num_tags) {
		constexpr const char * filler = "   ";
		constexpr const int num_width = 2;
		constexpr const char spacer = ' ';
		constexpr const int element_width = 3;

		std::cout << trailer;
		//	print out the index of the array
		for (int i = 0; i < size; i++) {
			std::cout << std::setw(num_width) << i << spacer;
		}
		std::cout << std::endl;

		//	print out the overhead chars
		for (int i = 0; i < size; i++) {
			if (i == v) {
				std::cout << std::setw(num_width) << " v ";
			} else {
				std::cout << filler;
			}
		}
		std::cout << std::endl;

		printElements(std::string(), array, size, num_width, element_width);
		std::cout << std::endl;
		printTags(std::string(), tags, num_tags, element_width);
		std::cout << trailer;
	}


	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							algorithm functions							*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	template <typename T>
	ComparesAndMoves blockMergeByRotate(T** array, index_t start, index_t mid, index_t end);
	template <typename T>
	int createTags( T** array, index_t start, index_t mid, index_t end,
			    	int block_size, std::unique_ptr<BlockTag<T>[]> &tags);
	template <typename T>
	ComparesAndMoves rotateArrayElements(T** array, index_t start, index_t end, index_t amount);
	template <typename T>
	ComparesAndMoves rotateTags(std::unique_ptr<BlockTag<T>[]> tags,
					int first_tag, int last_tag, int tag_rotate_count,
					index_t element_count);
	template <typename T>
	ComparesAndMoves sortBlocksLeftToRight(T **array, array_size_t size,
			std::unique_ptr<BlockTag<T>[]> &tags, int num_tags);
	template <typename T>
	ComparesAndMoves sortBlocksRightToLeft(T **array, array_size_t size,
			std::unique_ptr<BlockTag<T>[]> &tags, int num_tags);
	template <typename T>
	ComparesAndMoves swapBlocks(T** array,
								index_t block1_start, index_t block1_end,
								index_t block2_start, index_t block2_end);
	template<typename T>
	ComparesAndMoves swapTags(std::unique_ptr<BlockTag<T[]>> &tags, int i, int j);

	/*
	 * ComparesAndMoves blockMerge(arr, start, mid, end);
	 *
	 *   This uses rotate operations to put the { u[], v[n] } array in order
	 *   The requirement is that the sub portions of uv_array, u[] and v[]
	 *   	are in ascending order.
	 *   		Consider an array where start = 5, mid = 9 & end is equal to 12
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
	ComparesAndMoves blockMergeByRotate(T** array, index_t start, index_t mid, index_t end) {

		ComparesAndMoves result(0,0);

		index_t u = 0;
		index_t v = mid;
		amount_t rotate_count;

		while (1) {
			// find the first in u that is > v,
			//	which is the first u value that is out of order
			result._compares++;
			while (u < v && *array[u] <= *array[v]) {
				u++;
				result._compares++;
			}
			if (u >= v)
				break;

			// find the first value in v that > u
			rotate_count = 0;
			// search for an element in v that is greater than array[u]
			result._compares++;
			while (v <= end && *array[v] <= *array[u]) {
				v++;
				result._compares++;
				rotate_count++;
			}
			//	at this point `
			//	rotate u to the right in the array
			//	  past the last value in v that was equal to u_value
			result += rotateArrayElements(array, rotate_count, u, v-1);
			//  point to the element that is one past the element
			//    that u was pointing to which has now been rotated
			u = u + rotate_count + 1;
			if (v > end)
				break;
		}
		return result;
	}


	template <typename T>
	int  createTags(T** array, index_t start, index_t mid, index_t end,
			    	int block_size, std::unique_ptr<BlockTag<T>[]> &tags) {

		ComparesAndMoves result(0,0);
		index_t lower_span = mid-start;
		index_t upper_span = end-mid + 1;

		// calculate the total number of blocks
		int num_blocks = lower_span / block_size + upper_span / block_size;
		int first_block_size = lower_span % block_size;
		// if there is a partial first block
		if (first_block_size != 0)
			num_blocks++;
		//	if there is a partial last block
		if (upper_span % block_size)
			num_blocks++;

		// create the block storage
		tags = std::unique_ptr<BlockTag<T>[]>(new BlockTag<T>[num_blocks]);

		//	assign values to the blocks
		int block_number = 0;
		index_t start_of_block = start;
#if 0
		for (int i = 0; i != num_blocks; i++) {
			tags[i].type = BlockType::A_BLOCK;
			tags[i].start_index = 2*i;
			tags[i].end_index = 2*i+1;
			tags[i].key = new char('a'+i);
		}
		return num_blocks;
#endif
		//	do the A_Blocks first
		//	if the first A_Block is a partial
		if (first_block_size) {
			tags[block_number].type 		= BlockType::A_BLOCK;
			tags[block_number].start_index	= start;
			tags[block_number].end_index	= first_block_size-1;
			tags[block_number].key			= array[start_of_block];
			start_of_block += first_block_size;
			block_number = 1;
		}

		//	the full A_Blocks
		while (start_of_block < mid) {
			tags[block_number].type 		= BlockType::A_BLOCK;
			tags[block_number].start_index	= start_of_block;
			tags[block_number].end_index	= start_of_block+block_size-1;
			tags[block_number].key			= array[start_of_block];
			start_of_block += block_size;
			block_number++;
		}

		//	the full B_Blocks
		while (block_number < num_blocks-1) {
			tags[block_number].type 		= BlockType::B_BLOCK;
			tags[block_number].start_index	= start_of_block;
			tags[block_number].end_index	= start_of_block+block_size-1;
			tags[block_number].key			= array[start_of_block + block_size-1];
			start_of_block += block_size;
			block_number++;
		}

		//	the final B_Block may be a partial block,
		//	  but regardless of its size, it will extend to 'end'
		tags[block_number].type 		= BlockType::B_BLOCK;
		tags[block_number].start_index	= start_of_block;
		tags[block_number].end_index	= end;
		tags[block_number].key			= array[end];

		return num_blocks;
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
	 * 		amounts greater than the span are % to be within the span
	 */

	template <typename T>
	ComparesAndMoves rotateArrayElements(T** array, amount_t amount, index_t start, index_t end) {

		ComparesAndMoves result(0,0);

		if (end <= start)
			return result;

		index_t span = end - start + 1;
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
	 * 	ComparesAndMoves rotateTags(tags, first, last, tag_rotate_count, index_rotate_cnt);
	 *
	 * 	Takes an array of tags an rotates the tags on [first::last] by
	 * 	  distance element_count which refers to the array indices,
	 * 	  not the tag array
	 *
	 * 	Consider a tag array with the associated start:end values
	 * 		   0	  1		 2        3        4		5
	 * 		a[0:2] a[3:6] a[7:10] b[11:14] b[15:18] b[19:20]
	 *      key='d'key='e'key='f' key='b'  key='c'  key='g'
	 * 	rotateTags(tags, 0, 4, 8) results in
	 * 		   0	  1		  2        3        4		5
	 * 		b[0:3] b[4:7]  a[8:10] a[11:14] a[15:18] b[19:20]
	 * 		k='b'  key='c' key='d' key='e'  key='f'  key='g'
	 *	where 8 is the distance that each .start_index has moved
	 *	  tag 0 a[0:2] became tag 2 a[8:10]
	 *
	 *	1 update the start & end indices
	 *	  index_span_start = first.start
	 *	  index_span_end = last.end
	 *	  for each tag
	 *	    for start_index, end_index
	 *	      new_index = _index+rotate_count
	 *	      if (new_index > index_span_end)
	 *	        	overshoot = new_index - (index_end_index+1)
	 *	        	new_index = index_span_start + overshoot
	 *	      _index = new_index
	 *
	 *	2 rotate the tag types and keys
	 *		triple reverse tag type and key
	 *		for (i = start, j = end; i < j; i++, j--)
	 *			Tag<T> tmp = tags[i]
	 *			tags[i].type = tags[j].type
	 *			tags[i].key  = tags[j].key
	 *			tags[j].type = tmp.type
	 *			tags[j],key  = tmp.key
	 */

	template <typename T>
	ComparesAndMoves rotateTags(std::unique_ptr<BlockTag<T>[]> tags,
							    int first_tag, int last_tag, int tag_rotate_count,
								index_t element_rotate_count) {
		ComparesAndMoves result(0,0);

		if (first_tag == last_tag)
			return result;

		auto swapTypeAndKey = [&tags] (int i, int j) {
			BlockType tmp_type 	= tags[i].type;
			T*		  tmp_key	= tags[i].key;
			tags[i].type = tags[j].type;
			tags[i].key  = tags[j].key;
			tags[j].type = tmp_type;
			tags[j].key  = tmp_key;
		};

		 // update the start & end indices
		index_t start_of_span_index = tags[first_tag].start_index;
		index_t end_of_span_index	= tags[last_tag].end_index;

		for (int i = first_tag; i <= last_tag; i++) {
			index_t next_start_index = tags[i].start_index + element_rotate_count;
			// if this tag rotated to a position earlier in the array
			if (next_start_index > end_of_span_index) {
				//	calculate how many elements from the start of the span
				index_t overshoot = next_start_index - (end_of_span_index + 1);
				// the next start index will be 'overashoot' from the start of span
				next_start_index = start_of_span_index + overshoot;
			}
			tags[i].start_index = next_start_index;

			// do the sam calculation for the end_index
			index_t next_end_index = tags[i].end_index + element_rotate_count;
			if (next_end_index > end_of_span_index) {
				index_t overshoot = next_end_index - (end_of_span_index+1);
				next_end_index = tags[i].end_index + overshoot;
			}
			tags[i].end_index = next_end_index;
		}

		// rotate the types & keys
		//	 note that moving swapping a key is the same as
		//	   swapping an array element, so it counts as 3 moves

		for (int i = first_tag, j = last_tag; i < j; i++, j--) {
			swapTypeAndKey(i,j);
		}
		for (int i = first_tag, j = tag_rotate_count-1; i < j; i++, j--) {
			swapTypeAndKey(i,j);
		}
		for (int i = tag_rotate_count, j = last_tag; i < j; i++, j--) {
			swapTypeAndKey(i,j);
		}

		return result;
	}

	/*
	 * 	ComparesAndMoves sortBlocksInsertion(p_array, size, p_tags, num_tags);
	 *
	 *	This sorts the blocks, and thus moves the array elements using
	 *		an insertion sort, which means that out-of-order blocks will be
	 *		continuously swapped with their neighbor until they are in place
	 *
	 *	This assumes that an array of block tags containing information
	 *	  about block types, block keys (for sorting) are stored in p_tags
	 *
	 *	The result of this function is that the blocks are arranged
	 *	  in ascending order to facilitate a block merge which will result
	 *	  in the whole array being in order
	 *
	 *	Entry into this routine assumes that the values in the A_Blocks in the 'u' half of the array (left)
     *	  are in order and the values in the B_Blocks in the 'v' half of the array (right) are in order
     *	  although the total array may not be in order
     *
     *	  An A_Block's key value is the left most element and a B_Block's key value is the right most element
     *
     *	  (Note that the Block information is stored in a separate "Tag" array from the array "Values")
     *
     *	Name/Key: ALPHA='D'	    BRAVO='E'   CHARLIE='C'    DELTA='E'      ECHO='G'
     *	Tags:	  A_Block		A_Block		  B_Block		B_Block		  B_Block
     *	Values:	{ D, E, F } : { E, F, G } : { A, B, C } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *	If an A_Block of equal key value was placed to the right of a B_Block of equal key value, the
     *	  A_Block's key, which had been on the left side of the B_Block's key, would now be on the opposite
     *	  (right) side of the B_Block's key.  This reordering would result in the sort not being stable
     *	  because two keys of equal value had swapped their relative position
     *
     *	BRAVO='E' & CHARLIE='C' get swapped since 'C' is less than 'E'
     *
     *	Name/Key: ALPHA='D'	    CHARLIE='C'   BRAVO='E'    DELTA='E'      ECHO='G'
     *	Tags:	  A_Block		B_Block		  A_Block		B_Block		  B_Block
     *	Values:	{ D, E, F } : { A, B, C } : { E, F, G } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *	ALPHA='D' & CHARLIE='C' get swapped since 'C' is less than 'D'
     *
     *	Name/Key: CHARLIE='C'   ALPHA='D'     BRAVO='E'    DELTA='E'      ECHO='G'
     *	Tags:	  B_Block		A_Block		  A_Block		B_Block		  B_Block
     *	Values:	{ A, B, C } : { D, E, F } : { E, F, G } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *	This process is repeated for the next B_Block, which is DELTA, then for ECHO,
     *		until the last B_Block has been insertion sorted leftward to its correct place
     *	(Note that in the example, the array is sorted at the end of this step)
	 */

	template <typename T>
	ComparesAndMoves sortBlocksInsertion(T **array, array_size_t size,
											std::unique_ptr<BlockTag<T>[]> &tags, int num_tags) {

		ComparesAndMoves result(0,0);

		//	if the array consists of all A_Blocks, we are done
		if (tags[num_tags-1].type == BlockType::A_BLOCK) {
#if DEBUG_VERBOSE_SORT_BLOCKS
			std::cout << "All blocks are A Blocks" << std::endl;
#endif
			return result;
		}

		// 	if the array consists of all B_Blocks, we are done
		if (tags[0].type == BlockType::B_BLOCK) {
#if DEBUG_VERBOSE_SORT_BLOCKS
			std::cout << "All blocks are B Blocks" << std::endl;
#endif
			return result;
		}

		int num_a_blocks = 0;
		for (int i = 0; i != num_tags; i++) {
			if (tags[i].type == BlockType::A_BLOCK)
				num_a_blocks++;
			else
				break;
		}

#if DEBUG_VERBOSE_SORT_BLOCKS
		std::cout << "sortBlocksLeftToRight()" << std::endl;
		if (isSorted(tags, num_tags)) {
			std::cout << "Tags are initially sorted" << std::endl;
		} else {
			std::cout << "Tags are not sorted" << std::endl;
		}
#endif

		// process every a block
		for (int left_block = num_a_blocks-1; left_block >= 0; left_block--) {

			// the blocks to the right of the left_block are in order, although the
			//	left_block may need to be inserted somewhere in the blocks to the right
			int right_block = left_block+1;

			//	move through the blocks to the right until a block is found that is >= this block
			result._compares++;
			while (right_block < num_tags && *tags[right_block].key < *tags[left_block].key) {
				right_block++;
				result._compares++;
			}

			//	If the proper place for this block is immediately to the left of its neighbor, done
			if (left_block == right_block-1) {
				continue;
			}

			// move the right_block index back to the block that goes BEFORE the left_block
			right_block--;

			int start_of_rotated_span 	= tags[left_block].start_index;
			int end_of_rotated_span 	= tags[right_block].end_index;
			int rotate_count 			= tags[right_block].start_index - tags[left_block].start_index;

#if DEBUG_VERBOSE_SORT_BLOCKS
			printLineArrayIndices(size, 3, 4);
			printElements(std::string("\n"), array, size, 3, 4);
			std::stringstream debug;
			debug << " BEFORE a_block: " << left_block  << "[" << tags[left_block].start_index << ":" << tags[left_block].end_index << "]"
				  << " b_block: " 		 << right_block << "[" << tags[right_block].start_index << ":" << tags[right_block].end_index << "]"
				  << ", " << rotate_count << "\n\n";
			printTags(debug.str(), tags, num_tags, 4);
#endif

			result += rotateArray(array, rotate_count, start_of_rotated_span, end_of_rotated_span);
			//	update the block types in the block tag array
			BlockType left_block_type = tags[left_block].type;		// an A_Block
			for (int i = left_block; i < right_block; i++) {
				tags[i].type = tags[i+1].type;
				if (tags[i].type == BlockType::A_BLOCK) {
					tags[i].key = array[tags[i].start_index];
				} else {
					tags[i].key = array[tags[i].end_index];
				}
			}
			tags[right_block].type = left_block_type;
			if (tags[right_block].type == BlockType::A_BLOCK) {
				tags[right_block].key = array[tags[right_block].start_index];
			} else {
				tags[right_block].key = array[tags[right_block].end_index];
			}
#if DEBUG_VERBOSE_SORT_BLOCKS
			printLineArrayIndices(size, 3, 4);
			printTags(std::string(" AFTER\n"), tags, num_tags, 4);
			printElements(std::string("\n\n"), array, size, 3, 4);
#endif
		}

#if DEBUG_VERBOSE_SORT_BLOCKS
		if (isSorted(tags, num_tags)) {
			std::cout << "Tags are sorted" << std::endl;
		} else {
			std::cout << "Tags are NOT sorted" << std::endl;
		}
#endif
#ifdef DEBUG_SUMMARY_SORT_BLOCKS
		std::cout << "sortBlocksLefToRight() took " << result << std::endl;
#endif
		return result;
	}

	/*
	 * 	ComparesAndMoves sortBlocksLeftToRight(p_array, size, p_tags, num_tags);
	 *
	 *	(Note: this is called 'LeftToRight' because the B_Blocks are parsed left to right)
	 *
	 *	This assumes that an array of block tags containing information
	 *	  about block types, block keys (for sorting) are stored in p_tags
	 *
	 *	The result of this function is that the blocks are arranged
	 *	  in ascending order to facilitate a block merge which will result
	 *	  in the whole array being in order
	 *
	 *	Entry into this routine assumes that the values in the A_Blocks in the 'u' half of the array (left)
     *	  are in order and the values in the B_Blocks in the 'v' half of the array (right) are in order
     *	  although the total array may not be in order
     *
     *	  An A_Block's key value is the left most element and a B_Block's key value is the right most element
     *
     *	  (Note that the Block information is stored in a separate "Tag" array from the array "Values")
     *
     *	Name/Key: ALPHA='D'	    BRAVO='E'   CHARLIE='C'    DELTA='E'      ECHO='G'
     *	Tags:	  A_Block		A_Block		  B_Block		B_Block		  B_Block
     *	Values:	{ D, E, F } : { E, F, G } : { A, B, C } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *	Starting at the rightmost A_Block (BRAVO in the above example), move rightward through all of the
     *    B_Blocks until the first B_Block is found that is >= to the A_Block.  In the above example,
     *	  moving from BRAVO = 'E' > CHARLIE = 'C' onto BRAVO = 'E' <= DELTA = 'E'
     *
     *	The  BRAVO = 'E' will go to the left of DELTA = 'E' which is to the right of CHARLIE = 'C'
     *
     *	If an A_Block of equal key value was placed to the right of a B_Block of equal key value, the
     *	  A_Block's key, which had been on the left side of the B_Block's key, would now be on the opposite
     *	  (right) side of the B_Block's key.  This reordering would result in the sort not being stable
     *	  because two keys of equal value had swapped their relative position
     *
     *	  Elements 3:9 get rotated by a total of (CHARLIE.start = 6) - (BRAVO.start = 3) = 3 positions
     *
     *	Name/Key: ALPHA='D'	   CHARLIE='A'   BRAVO='G'    DELTA='E'      ECHO='G'
     *	Tags:	  A_Block		A_Block		  B_Block		B_Block		  B_Block
     *	Values:	{ D, E, F } : { A, B, C } : { E, F, G } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *	  As a result rotating the array, values [3:9] are in the correct position, but CHARLIE, which
     *		was a B_Block has now inherited an A_Block tag, and BRAVO, which was an A_Block has now
     *		inherited a B_Block's tag.  This causes the key values, which are determined by the Block type
     *	    to be incorrect.  This is fixed by changing the block types.  Stated alternatively, the Block type
     *		always moves with the array values during a rotation
     *
     *	Name/Key: ALPHA='D'	   CHARLIE='C'   BRAVO='E'      DELTA='E'      ECHO='G'
     *	Tags:	  A_Block		B_Block		  A_Block		B_Block		  B_Block
     *	Values:	{ D, E, F } : { A, B, C } : { E, F, G } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *	The process will be repeated for the A_Block ALPHA.  In this case, BRAVO = 'E' is the
     *	  first block >= ALPHA = 'D'.  ALPHA needs to be moved to the Left of BRAVO which is the right of CHARLIE
     *
     *	Name/Key: CHARLIE='C'   ALPHA='D'     BRAVO='E'    DELTA='E'      ECHO='G'
     *	Tags:	  B_Block		A_Block		  A_Block		B_Block		  B_Block
     *	Values:	{ A, B, C } : { D, E, F } : { E, F, G } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *	  The blocks are now in order, { 'C', 'D', 'E', 'E', 'G' }  A block-by-block merge can be performed
     *
     *	Note that it is possible that an A_Block goes at the end, which is why
     *	  the loop contains the check B_Block < num_tags
     *	  In that case, the last block
     *
     *	Name/Key: ALPHA='D'	    BRAVO='E'   CHARLIE='C'    DELTA='E'      ECHO='G'
     *	Tags:	  A_Block		A_Block		  B_Block		B_Block		  B_Block
     *	Values:	{ H, I, J } : { E, F, G } : { A, B, C } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
	 */

	template <typename T>
	ComparesAndMoves sortBlocksLeftToRight(T **array, array_size_t size,
											std::unique_ptr<BlockTag<T>[]> &tags, int num_tags) {

		ComparesAndMoves result(0,0);

		//	if the array consists of all A_Blocks, we are done
		if (tags[num_tags-1].type == BlockType::A_BLOCK) {
#if DEBUG_VERBOSE_SORT_BLOCKS
			std::cout << "All blocks are A Blocks" << std::endl;
#endif
			return result;
		}

		// 	if the array consists of all B_Blocks, we are done
		if (tags[0].type == BlockType::B_BLOCK) {
#if DEBUG_VERBOSE_SORT_BLOCKS
			std::cout << "All blocks are B Blocks" << std::endl;
#endif
			return result;
		}

		int num_a_blocks = 0;
		for (int i = 0; i != num_tags; i++) {
			if (tags[i].type == BlockType::A_BLOCK)
				num_a_blocks++;
			else
				break;
		}

#if DEBUG_VERBOSE_SORT_BLOCKS
		std::cout << "sortBlocksLeftToRight()" << std::endl;
		if (isSorted(tags, num_tags)) {
			std::cout << "Tags are initially sorted" << std::endl;
		} else {
			std::cout << "Tags are not sorted" << std::endl;
		}
#endif

		// process every A_Block starting with the block order:
		//	 								A0 A1 ...    Am B0 ... .......Bn
		// after the first pass, possibly	A0 A1 . Am-1 B0 B1 ... Am ... Bn
		//	thus, each new iteration has to start with the next left block
		//
		for (int a_block_index = num_a_blocks-1; a_block_index >= 0; a_block_index--) {

			// The blocks to the right of the left_block are in order,
			//	although there may be blocks to the right that are smaller than
			//	this block.  Note that it may be possible that block immediately
			//	to the right of this block is a B_Block
			int b_block_index = a_block_index+1;

			//	Find first b_block to the right of this a_block
			//	  that is greater than this block  That may entail
			//	  moving through  a number of B_Blocks.
			//	It is possible that there is no b_block that is greater
			//	   thatn this a_block
			while (b_block_index < num_tags) {
				result._compares++;
				if (*tags[a_block_index].key <= *tags[b_block_index].key) {
					break;
				}
				b_block_index++;
			}

			//	If the b_block that is greater than this a_block is
			//	  the next block, all of the blocks are in order
			//	  because all a_blocks to the left are smaller and
			//	  all b_blocks and a_blocks to the right are larger
			//	  because this algorithm starts with the biggest a_block,
			//	  performs the re-ordering, and then repeats by considering
			//	  the next a_block lower
			if (a_block_index == b_block_index-1) {
				break;
			}

			//	A b_block has been identified that is >= this a_block,
			//	  or the end of the b_blocks has been reached
			//	  and there is at least one intervening b_block
			//	  the portion of the array to be rotated is from
			//	  this a_block to the b_block before the found one
			//		consider the array { ... a[m], b[0], b[1] }
			//	a[m].key=3  b[0].key=0  b[1].key=3  b_block_index = 1 rotate a[m]:b[0]
			//	a[m].key=3  b[0].key=0  b[1].key=4  b_block_index = 1 rotate a[m]:b[0]
			//	a[m].key=3  b[0].key=0  b[1].key=1  b_block_index = 2 rotate a[m]:b[1]
			b_block_index--;

			int start_of_rotated_span 	= tags[a_block_index].start_index;
			int end_of_rotated_span 	= tags[b_block_index].end_index;
			int rotate_count 			= tags[b_block_index].start_index - tags[a_block_index].start_index;

#if DEBUG_VERBOSE_SORT_BLOCKS
			printLineArrayIndices(size, 3, 4);
			printElements(std::string("\n"), array, size, 3, 4);
			std::stringstream debug;
			debug << " BEFORE a_block: " << a_block_index  << "[" << tags[a_block_index].start_index << ":" << tags[a_block_index].end_index << "]"
				  << " b_block: " 		 << b_block_index << "[" << tags[b_block_index].start_index << ":" << tags[b_block_index].end_index << "]"
				  << ", " << rotate_count << "\n\n";
			printTags(debug.str(), tags, num_tags, 4);
#endif

			result += rotateArrayElements(array, rotate_count, start_of_rotated_span, end_of_rotated_span);

			//	The blocks tags no longer match the array because
			//	  the array has been rotated one block to the left
			//	  The tags themselves should not be rotated, because
			//	  the [start:end] has not changed
			//	  Update the block tags copy the next blocks TYPE
			//	    into this block, then updating the key value
			BlockType left_block_type = tags[a_block_index].type;		// an A_Block
			for (int i = a_block_index; i < b_block_index; i++) {
				tags[i].type = tags[i+1].type;
				if (tags[i].type == BlockType::A_BLOCK) {
					tags[i].key = array[tags[i].start_index];
				} else {
					tags[i].key = array[tags[i].end_index];
				}
			}
			tags[b_block_index].type = left_block_type;
			if (tags[b_block_index].type == BlockType::A_BLOCK) {
				tags[b_block_index].key = array[tags[b_block_index].start_index];
			} else {
				tags[b_block_index].key = array[tags[b_block_index].end_index];
			}

#if DEBUG_VERBOSE_SORT_BLOCKS
			printLineArrayIndices(size, 3, 4);
			printTags(std::string(" AFTER\n"), tags, num_tags, 4);
			printElements(std::string("\n\n"), array, size, 3, 4);
#endif
		}

#if DEBUG_VERBOSE_SORT_BLOCKS
		if (isSorted(tags, num_tags)) {
			std::cout << "Tags are sorted" << std::endl;
		} else {
			std::cout << "Tags are NOT sorted" << std::endl;
		}
#endif
#ifdef DEBUG_SUMMARY_SORT_BLOCKS
		std::cout << "sortBlocksLefToRight() took " << result << std::endl;
#endif
		return result;
	}

	/*
	 * 	ComparesAndMoves sortBlocksRightToLeft(p_array, size, p_tags, num_tags);
	 *
	 *	(Note: this is called 'RightToLeft' because the B_Blocks are parsed from the Right)
	 *
	 *	This assumes that an array of block tags containing information
	 *	  about block types, block keys (for sorting) are stored in p_tags
	 *
	 *	The result of this function is that the blocks are arranged
	 *	  in ascending order to facilitate a block merge which will result
	 *	  in the whole array being in order
	 *
	 *	Entry into this routine assumes that the values in the A_Blocks in the 'u' half of the array (left)
     *	  are in order and the values in the B_Blocks in the 'v' half of the array (right) are in order
     *	  although the total array may not be in order
     *
     *	  An A_Block's key value is the left most element and a B_Block's key value is the right most element
     *
     *	  (Note that the Block information is stored in a separate "Tag" array from the array "Values")
     *
     *	Name/Key: ALPHA='D'	    BRAVO='E'   CHARLIE='C'    DELTA='E'      ECHO='G'
     *	Tags:	  A_Block		A_Block		  B_Block		B_Block		  B_Block
     *	Values:	{ D, E, F } : { E, F, G } : { A, B, C } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *	right_index = num_tags-1;		// ECHO in this case
     *	Starting at left_most A_Block	// BRAVO in this case
     *	  while (right_index > left_index && tag[right_index] >= tag[left_index])
     *	    rightmost_index--
     *	  if (right_index < left_index)
     *	  	done
     *	  rotate span from left_most:right_most
     *	  rightmost--
     *	  leftmost--
     *
     *	starting with [leftmost=2]  CHARLIE='C' vs [rightmost=4]  ECHO.'G'
     *		ECHO.'G' >= BRAVO.'D', rightmost-- to 3
     *		DELTA.'E' >= BRAVO.'D' rightmost-- to 2
     *		CHARLIE.'C' >= BRAVO.'D'
     *		  rotate CHARLIE.'C' into position
     *		  update Block Types and keys
     *		  rightmost-- to 1
     *		  leftmost-- to 0
     *		continue
     *
     *	The CHARLIE='C' will go to the left of BRAVO='E'
     *
     *	Name/Key: ALPHA='D'	   CHARLIE='A'   BRAVO='G'    DELTA='E'      ECHO='G'
     *	Tags:	  A_Block		A_Block		  B_Block		B_Block		  B_Block
     *	Values:	{ D, E, F } : { A, B, C } : { E, F, G } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *	  As a result rotating the array, values [3:9] are in the correct position, but CHARLIE, which
     *		was a B_Block has now inherited an A_Block tag, and BRAVO, which was an A_Block has now
     *		inherited a B_Block's tag.  This causes the key values, which are determined by the Block type
     *	    to be incorrect.  This is fixed by changing the block types.  Stated alternatively, the Block type
     *		always moves with the array values during a rotation
     *
     *	The process continues with leftmost = 0 and rightmost = 1
     *	  [rightmost=1] CHARLIE.'C' vs [leftmost=0] ALPHA.'D'
     *
     *	Name/Key: ALPHA='D'	   CHARLIE='C'   BRAVO='E'      DELTA='E'      ECHO='G'
     *	Tags:	  A_Block		B_Block		  A_Block		B_Block		  B_Block
     *	Values:	{ D, E, F } : { A, B, C } : { E, F, G } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *
     *	Name/Key: CHARLIE='C'   ALPHA='D'     BRAVO='E'    DELTA='E'      ECHO='G'
     *	Tags:	  B_Block		A_Block		  A_Block		B_Block		  B_Block
     *	Values:	{ A, B, C } : { D, E, F } : { E, F, G } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
     *	  The blocks are now in order, { 'C', 'D', 'E', 'E', 'G' }  A block-by-block merge can be performed
     *
     *	  the loop contains the check B_Block < num_tags
     *	 In that case, [rightmost=4] ECHO.'G' >= [leftmost=0] ALPHA.'D' compares false
     *
     *	Name/Key: ALPHA='D'	    BRAVO='E'   CHARLIE='C'    DELTA='E'      ECHO='G'
     *	Tags:	  A_Block		A_Block		  B_Block		B_Block		  B_Block
     *	Values:	{ H, I, J } : { E, F, G } : { A, B, C } : { C, D, E } : { E, F, G }
     *	Index:    0  1  2       3  4  5       6  7  8       9  10 11      12 13 14
     *
	 */

	template <typename T>
	ComparesAndMoves sortBlocksRightToLeft(T **array, array_size_t size,
			std::unique_ptr<BlockTag<T>[]> &tags, int num_tags) {

		ComparesAndMoves result(0,0);

		//	if the array consists of all A_Blocks, we are done
		if (tags[num_tags-1].type == BlockType::A_BLOCK) {
#if DEBUG_VERBOSE_SORT_BLOCKS
			std::cout << "All blocks are A Blocks" << std::endl;
#endif
			return result;
		}

		// 	if the array consists of all B_Blocks, we are done
		if (tags[0].type == BlockType::B_BLOCK) {
#if DEBUG_VERBOSE_SORT_BLOCKS
			std::cout << "All blocks are B Blocks" << std::endl;
#endif
			return result;
		}

		int num_a_blocks = 0;
		for (int i = 0; i != num_tags; i++) {
			if (tags[i].type == BlockType::A_BLOCK)
				num_a_blocks++;
			else
				break;
		}

#ifdef DEBUG_VERBOSE_SORT_BLOCKS
		std::cout << "sortBlocksRightToLeft()" << std::endl;
		if (isSorted(tags, num_tags)) {
			std::cout << "Tags are initially sorted" << std::endl;
		} else {
			std::cout << "Tags are not sorted" << std::endl;
		}
#endif

		int right_block = num_tags-1;

		// process every A_Block
		for (int left_block = num_a_blocks-1; left_block >= 0; left_block--) {

			// find the rightmost B_Block that is < the A_Block
			result._compares++;
			while (right_block > left_block && tags[right_block] >= tags[left_block]) {
				right_block--;
				result._compares++;
			}

			if (right_block <= left_block) {
				break;
			}

			int start_of_rotated_span 	= tags[left_block].start_index;
			int end_of_rotated_span 	= tags[right_block].end_index;
			int rotate_count 			= tags[right_block].start_index - tags[left_block].start_index;

#if DEBUG_VERBOSE_SORT_BLOCKS
			printLineArrayIndices(size, 3, 4);
			printElements(std::string("\n"), array, size, 3, 4);
			std::stringstream debug;
			debug << " BEFORE a_block: " << left_block  << "[" << tags[left_block].start_index << ":" << tags[left_block].end_index << "]"
				  << " b_block: " 		 << right_block << "[" << tags[right_block].start_index << ":" << tags[right_block].end_index << "]"
				  << ", " << rotate_count << "\n\n";
			printTags(debug.str(), tags, num_tags, 4);
#endif

			result += rotateArray(array, rotate_count, start_of_rotated_span, end_of_rotated_span);
			//	update the block types in the block tag array
			BlockType left_block_type = tags[left_block].type;		// an A_Block
			for (int i = left_block; i < right_block; i++) {
				tags[i].type = tags[i+1].type;
				if (tags[i].type == BlockType::A_BLOCK) {
					tags[i].key = array[tags[i].start_index];
				} else {
					tags[i].key = array[tags[i].end_index];
				}
			}
			tags[right_block].type = left_block_type;
			if (tags[right_block].type == BlockType::A_BLOCK) {
				tags[right_block].key = array[tags[right_block].start_index];
			} else {
				tags[right_block].key = array[tags[right_block].end_index];
			}
#if DEBUG_VERBOSE_SORT_BLOCKS
			printLineArrayIndices(size, 3, 4);
			printTags(std::string(" AFTER\n"), tags, num_tags, 4);
			printElements(std::string("\n\n"), array, size, 3, 4);
#endif
		}

#ifdef DEBUG_VERBOSE_SORT_BLOCKS
		for (int i = 0; i != num_tags; i++) {
			std::cout << *tags[i].key << " ";
		}
		std::cout << std::endl;

		if (isSorted(tags, num_tags)) {
			std::cout << "Tags are sorted" << std::endl;
		} else {
			std::cout << "Tags are NOT sorted" << std::endl;
		}
#endif

#ifdef DEBUG_SUMMARY_SORT_BLOCKS
		std::cout << "sortBlocks() took " << result << std::endl;
#endif
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
	ComparesAndMoves swapTags(std::unique_ptr<BlockTag<T[]>> &tags, int i, int j) {

		//	swapping the Tag.key, which is an array element, takes three moves
		ComparesAndMoves result(0,3);
		BlockTag<T>tmp = tags[i];
		tags[i] = tags[j];
		tags[j] = tmp;
		return result;
	}


		/*	**************************************************	*/
		/*	**************************************************	*/
		/*						the sort						*/
		/*	**************************************************	*/
		/*	**************************************************	*/

	/*
	 * 	ComparesAndMoves sortArray(array, start, mid, end, block_size);
	 *
	 * 	This function sorts the blocks of the 'array' of size 'block_size'
	 *    S                          M                    E
	 * 	{ a, c, e, e, e, k, m, o, q, b, c, c, c, d, d, d, d }	block_size = 4
	 *
	 * 	  S        A Blocks                   M         B Blocks      E
	 * 	{ a } { c, e, e, e } { k, m, o, q } { b, c, c, c } { d, d, d, d }
	 *
	 * 	{ a } { c, e, e, e } { b, c, c, c } { d, d, d, d } { k, m, o, q }
	 *
	 */

	template <typename T>
	ComparesAndMoves sortArray(T** array, index_t start, index_t mid, index_t end, index_t block_size)
	{
		ComparesAndMoves result(0,0);

		return result;
	}

	/*	**************************	*/
	/*		the sorting function	*/
	/*	**************************	*/

	template <typename T>
	ComparesAndMoves sortPointersToObjects(T **array, array_size_t size) {
		ComparesAndMoves result(0,0);
		int element_width = 4;
		int value_width = 3;
		//	sort both the u half and the v half
		array_size_t v = size / 2;
		result += InsertionSort::sortPointersToObjects(array, v);
		result += InsertionSort::sortPointersToObjects(&array[v], size-v);

#ifdef DEBUG_VERBOSE_BLOCK_SORT
		printElements(std::string(" after insertion sorting halves\n"), array, size, value_width, element_width);
#endif

		array_size_t u_size = v;
		array_size_t block_size = static_cast<index_t>(std::sqrt(u_size));
		std::unique_ptr<BlockTag<T>[]> block_tags;
		int num_blocks = 0;

		num_blocks = createTags(array, 0, v, size-1, block_size, block_tags);
		std::cout << "Result of tagging blocks is:" << std::endl;
		printTags(std::string(" created tags\n"), block_tags, num_blocks, element_width);
		return result;
#ifdef SORT_BLOCKS_LEFT_TO_RIGHT
		result = sortBlocksLeftToRight(array, size, block_tags, num_blocks);
#else
		result = sortBlocksRightToLeft(array, size, block_tags, num_blocks);
#endif
		// merge consecutive blocks
		for (int right_block = num_blocks; right_block > 0; right_block--) {
			// two consecutive B_Blocks are in ascending order because
			//	they came out of the v side of the array that was sorted
			if (block_tags[right_block-1].type == BlockType::B_BLOCK) {
				continue;
			}
			// merge the A_Block
#ifdef BLOCK_MERGE_BY_ROTATE
			result +=
				blockMergeByRotate(array, block_tags[right_block-1].start_index,//	start
							  	  	      block_tags[right_block].start_index,	//  mid
										  block_tags[right_block].end_index);	//  end;
#endif
		}
		return result;
	}

}	// namespace BlockSort

#endif /* BLOCKSORT_H_ */
