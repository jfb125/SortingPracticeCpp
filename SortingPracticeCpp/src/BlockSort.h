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

#include "SortingPracticeDataTypes.h"
#include "SortingUtilities.h"
#include "InsertionSort.h"

bool	testBlockSort();

array_size_t floorLog2(array_size_t num);
void printArrayIndices(array_size_t size, int value_width, int element_width);
void printLineArrayIndices(array_size_t size, int value_width, int element_width);
void printArrayIndices(std::string header, array_size_t size, int value_width, int element_width);

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
			start_index = 1;
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
	void printTags(std::string trailer, BlockTag<T> tags[], int num_tags, int element_width) {

		if (element_width == 0) {
			std::cout << "ERROR: printTags() called with element_width == 0" << trailer;
			return;
		}

		OStreamState current_state;	// restores ostream state in destructor

		for (int i = 0; i != num_tags; i++) {

			int elements_remaining = tags[i].num_elements();

			switch (elements_remaining) {
			case 0:
				std::cout << "!!!!ERROR: printTags() passed a block of size 0 elements: ";
				return;
			case 1:
				std::cout << toStringTagSingleElement(tags[i], element_width);
				break;
			default:
				std::cout << toStringTagOpeningElement(tags[i], element_width);
				elements_remaining--;
				while(elements_remaining-- > 1) {
					std::cout.fill(TAG_SPACE_CHAR);
					std::cout << std::setw(element_width) << TAG_SPACE_CHAR;
				}
				std::cout << toStringTagClosingElement(tags[i], element_width);
				break;
			}
		}
		std::cout << trailer;
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

	/*
	 * 	void rotateArray(array, amount, start, end);
	 *
	 * 	rotate the contents of the array by 'amount' positions
	 * 	  where the array is defined as array[start]:array[end] (inclusive)
	 *
	 * 	algorithm:
	 * 		reverse the array
	 * 		reverse the sub array [start:amount-1]
	 * 		reverse the sub array [amount:end]
	 *
	 * 	note that amount can be negative or greater than the span
	 * 		negative amounts are converted into the equivalent positive amount
	 * 		amounts greater than the span are % to be within the span
	 */

	template <typename T>
	ComparesAndMoves rotateArray(T** array, amount_t amount, index_t start, index_t end) {

		ComparesAndMoves result(0,0);

		if (end <= start)
			return result;

		index_t span = end - start + 1;

		amount %= span;
		if (amount < 0) {
			amount = span+amount;
		}

		if (amount == 0)
			return result;

		T* temp;
		for (index_t i = start, j = end; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result._moves += 3;
		}

		for (index_t i = start, j = start+amount-1; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result._moves += 3;
		}

		for (index_t i = start + amount, j = end; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result._moves += 3;
		}
		return result;
	}

	/*
	 * ComparesAndMoves blockMerge(uv_array, size, v);
	 *
	 *   This uses rotate operations to put the { u[], v[n] } array in order
	 *   The requirement is that the sub portions of uv_array, u[] and v[]
	 *   	are in ascending order.
	 *        e.g. if  u[] = { 2, 3, 3, 8 } and v[] = { 1, 3, 7, 7 }
	 *      then uv_array[] = { 2, 3, 3, 8, 1, 3, 7, 7 }
	 *
	 *   The position of the first element in v, in this case [4] = 1,
	 *   	must be provided by the caller
	 *
	 *   The algorithm looks for the first element in v that is > u
	 *      the first element in u, then rotates
	 * 		{ 2, 3, 3, 8, 1, 3, 7, 7 }	[u=0] is 2, [v=4] is 1, count = 0
	 * 		{ 2, 3, 3, 8, 1, 3, 7, 7 }  [u=0] is 2, [v=5] is 3, count = 1
	 * 		arrayRotate(uv_array, count = 1, start = u = 0, end = v-1 = 4
	 * 		{ 1, 2, 3, 3, 8, 3, 7, 7 }
	 * 			u = u + count = 1
	 * 			search u for the first element != '2' which is [u=2] is 3
	 * 			search v for the first element < [u=2] is 3, [v=6] is 7
	 * 	    arrayRotate(uv_array, count = 1, start = 2, end = 7-1 = 6)
	 * 	    { 1, 2, 3, 3, 3, 8, 7, 7 }
	 * 	    	search u for the first element != 3 which is [u=5] is 8
	 * 	    	search v for the first element > 8 v stops at size
	 * 	    arrayRotate(uv_array, count = 2, start = 5, end = size-1)
	 * 	    { 1, 2, 3, 3, 3, 7, 7, 8 }
	 */

	template <typename T>
	ComparesAndMoves blockMerge(T** uv_array, index_t size, index_t v) {

		ComparesAndMoves result(0,0);

		index_t u = 0;
		amount_t rotate_count;

		while (1) {
			rotate_count = 0;
			T* u_value = uv_array[u];
			// search for an element in v that is greater than array[u]
			while (v < size && *uv_array[v++] <= *u_value) {
				result._compares++;
				rotate_count++;
			}
			//	rotate u to the right in the array
			//	  past the last value in v that was equal to u_value
			result += rotateArray(uv_array, rotate_count, u, v+rotate_count-1);

			if (v == size)
				break;

			//	the u portion of the array has been rotated 'rotate_count'
			//	  to the right in the array. All values to the left of
			//	  u are < or = to the start of the u segment
			//	  find the next value to the right in u that is >
			//	  the start of u
			u += rotate_count;
			//	advance u until a value is found that is greater than
			//	  the current u_value
			while (u < size && *uv_array[u] <= *u_value) {
				result._compares++;
				u++;
			}
			//	if there were no values found > the current u value,
			//	  no more rotates need to be performed -> done
			if (u == size)
				break;
		}
		return result;
	}

	/*
	 * 	ComparesAndMoves sortBlocks(p_array, size, p_tags, num_tags);
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
	ComparesAndMoves sortBlocks(T **array, array_size_t size, BlockTag<T> *tags, int num_tags) {

		ComparesAndMoves result(0,0);

		//	if the array consists of all A_Blocks, we are done
		if (tags[num_tags-1].type == BlockType::A_BLOCK)
			return result;

		// 	if the array consists of all B_Blocks, we are done
		if (tags[0].type == BlockType::B_BLOCK)
			return result;

		int num_a_blocks = 0;
		for (int i = 0; i != num_tags; i++) {
			if (tags[i].type == BlockType::A_BLOCK)
				num_a_blocks++;
			else
				break;
		}

		std::cout << "sortBlocks()" << std::endl;

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

			printLineArrayIndices(size, 3, 4);
			printElements(std::string("\n"), array, size, 3, 4);
			std::stringstream debug;
			debug << " BEFORE a_block: " << left_block  << "[" << tags[left_block].start_index << ":" << tags[left_block].end_index << "]"
				  << " b_block: " 		 << right_block << "[" << tags[right_block].start_index << ":" << tags[right_block].end_index << "]"
				  << ", " << rotate_count << "\n\n";
			printTags(debug.str(), tags, num_tags, 4);
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
			if (tags[right_block].type == BlockType::B_BLOCK) {
				tags[right_block].key = array[tags[right_block].start_index];
			} else {
				tags[right_block].key = array[tags[right_block].end_index];
			}

			printLineArrayIndices(size, 3, 4);
			printTags(std::string(" AFTER\n"), tags, num_tags, 4);
			printElements(std::string("\n\n"), array, size, 3, 4);
		}
		std::cout << "sortBlocks() took " << result << std::endl;
		return result;
	}

	template <typename T>
	ComparesAndMoves sortPointersToObjects(T **array, array_size_t size) {
		ComparesAndMoves result(0,0);
		int element_width = 4;
		int value_width = 3;
		//	sort both the u half and the v half
		array_size_t v = size / 2;
		result += InsertionSort::sortPointersToObjects(array, v);
		result += InsertionSort::sortPointersToObjects(&array[v], size-v);

		printElements(std::string(" after insertion sorting halves\n"), array, size, value_width, element_width);

		array_size_t u_size = v;
		array_size_t v_size = (size - u_size);
		array_size_t block_size = static_cast<index_t>(std::sqrt(u_size));
		array_size_t first_u_width = u_size % block_size;
		array_size_t last_v_width = v_size % block_size;
		int num_A_blocks = u_size / block_size + (first_u_width != 0 ? 1: 0);
		int num_B_blocks = v_size / block_size + (last_v_width != 0 ? 1 : 0);
		int num_blocks = num_A_blocks + num_B_blocks;
//		std::cout << "u_size " << u_size
//				  << " v_size " << v_size
//				  << " block_width " << block_size
//				  << " first_u_width " << first_u_width
//				  << " last_v_width " << last_v_width
//				  << " num_blocks " << num_blocks << std::endl;
		BlockTag<T> tags[num_blocks];
		array_size_t start_of_block = 0;
		//	if the first block is less than a full block size, otherwise full size
		array_size_t end_of_block = first_u_width != 0 ? first_u_width-1 : block_size-1;
		for (int i = 0; i < num_blocks; i++) {
			tags[i].start_index = start_of_block;
			tags[i].end_index	= end_of_block;
			if (start_of_block < v) {
				tags[i].type = BlockType::A_BLOCK;
				tags[i].key = array[start_of_block];
			} else {
				tags[i].type = BlockType::B_BLOCK;
				tags[i].key = array[end_of_block];
			}
			start_of_block = end_of_block+1;
			end_of_block = end_of_block + block_size;
			//	this happens with the last_v_block is non-zero
			if (end_of_block >= size)
				end_of_block = size-1;
		}
//		printTags(std::string(" tags\n"), tags, num_blocks, element_width);
		result = sortBlocks(array, size, tags, num_blocks);
//		printTags(std::string(" tags\n"), tags, num_blocks, element_width);
//		printElements(std::string("\n"), array, size, value_width, element_width);
//		std::cout << std::endl;
		return result;
	}

}	// namespace BlockSort

#endif /* BLOCKSORT_H_ */
