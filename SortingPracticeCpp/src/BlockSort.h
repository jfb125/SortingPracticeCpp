/*
 * BlockSort.h
 *
 *  Created on: Aug 11, 2025
 *      Author: joe
 */

/*	TODO - List
 *		<< Remember in all merging algorithms, you do not have to merge
 *		   consecutive B Blocks - their elements are in ascending order >>
 *		<< Optimization: if you go to merge two blocks, and the last element
 *		   of the block on the left <= the first element of the right block
 *		   there is no need to merge - all of the elements are in order >>
 *
 *	Change 'BlockTag' to be 'BlockInfo'
 *	Change 'key' to be be a function that returns the underlying array element
 *	Create a 'rollAndDrop() for sorting the blocks.  Probably won't be
 *
 *		any more efficient as the method I am using
 *
 * 	Fix rotateMerge() to use a binary sort & to keep track of least A element
 * 		which will reduce the amount of time it takes for the binary search
 *
 * 	Create a sqrtMerge() that uses an auxiliary array the size of an A_Block
 * 		to hold the A_Block, then merges the A_Block & B_Block into the
 * 		place in the original array where the A_Block is.
 *  Aux                Aux                Aux            	Aux
 *  0 3 5 6            x 3 5 6            x x x 6           x x x x
 * 	A	    B          A       B          A       B         A       B
 * 	0 3 5 6 1 2 4 7    0 1 2 3 1 2 4 7    0 1 2 3 4 5 4 7   0 1 2 3 4 5 6 7
 *
 *	Create a merge that, given an A_Block of size m, uses the m strictly
 *		ascending values to the left of the A_Block as the Aux array
 *
 *	Grail sort strictly uses merging where the auxialliary array is
 *		the left 'block_size' distincet elements
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

#define TAG_BOUNDARY_CHAR '|'
#define TAG_SPACE_CHAR ' '
#define ELEMENT_WIDTH 4
#define VALUE_WIDTH 3


array_size_t floorLog2(array_size_t num);

std::string arrayIndicesToString(array_size_t size, int value_width, int element_width);
std::string printLineArrayIndices(array_size_t size, int value_width, int element_width);
//std::string printArrayIndices(std::string header, array_size_t size, int value_width, int element_width);
std::string arrayIndicesToString(std::string trailer, array_size_t size, array_size_t v, int element_width);
std::string arrayIndicesToString(array_size_t size, array_size_t v, int element_width);


enum class BlockType {
	A_BLOCK,
	B_BLOCK,
	UNSPECIFIED
};
#define A_BLOCK_CHAR 'A'
#define B_BLOCK_CHAR 'B'
#define U_BLOCK_CHAR 'U'

char to_char(BlockType type);
std::ostream& operator<<(std::ostream& out, BlockType object);

#define BLOCK_MERGE_BY_ROTATE

namespace BlockSort {
	using array_size_t = array_size_t;

	template <typename T>
	class BlockTag {
	public:
		BlockType type;
		T* key;
		array_size_t start_index;
		array_size_t end_index;

		array_size_t	numElements() const {
			return end_index - start_index + 1;
		}

		T* assignKey(T**array) {
			switch(type) {
			case BlockType::A_BLOCK:
				key = array[start_index];
				break;
			case BlockType::B_BLOCK:
				key = array[end_index];
				break;
			case BlockType::UNSPECIFIED:
				//	TODO - throw exception
				key = nullptr;
				break;
			default:
				key = nullptr;
				//	TODO - throw exception
				break;
			}
			return key;
		}

		std::string spanString(int index_width = 1) const {
			std::stringstream sstring;
			sstring << "[" << start_index << ":" << end_index << "]";
			return sstring.str();
		}

		BlockTag() {
			type = BlockType::UNSPECIFIED;
			key = nullptr;
			start_index = 0;
			end_index = 0;
		}
		BlockTag(BlockType t, T* k, array_size_t s, array_size_t e) {
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

		bool isExactlyEqual(const BlockTag &other) const {
			if (type != other.type) 				return false;
			if (key != other.key)					return false;
			if (start_index != other.start_index)	return false;
			if (end_index != other.end_index)		return false;
			return true;
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

	template <typename T>
	std::ostream& operator<<(std::ostream& out, const BlockTag<T> &object) {
		out << object.to_string();
		return out;
	}

	template <typename T>
	array_size_t blockSortModulo(array_size_t rotation_count, array_size_t span) {
		while (rotation_count < 0) {
			rotation_count += span;
		}
		rotation_count %= span;
		return rotation_count;
	}

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							debugging resources							*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	/*	***********************************************	*/
	/*		outputting graphic representation of tags	*/
	/*	***********************************************	*/

	template <typename T>
	bool areTagsSorted(std::unique_ptr<BlockTag<T>[]> &tags, int num_tags) {

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

	/*	**************************************************	*/
	/*				print the values on a line				*/
	/*	**************************************************	*/

	// 	prints a line where the blocks a represented graphically
	template <typename T>
	std::string tagArrayToString(std::unique_ptr<BlockTag<T>[]> &tags, int num_tags,
								 int element_width = ELEMENT_WIDTH) {

		//	restores ostream state with its destructor
		OStreamState ostream_state;

		std::stringstream result;
		if (element_width == 0) {
			result << "ERROR: printTags() called with element_width == 0";
			return result.str();
		}


		for (int i = 0; i != num_tags; i++) {

			int elements_remaining = tags[i].numElements();

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
	std::string tagArrayToString(std::string trailer,
								 std::unique_ptr<BlockTag<T>[]> &tags, int num_tags,
								 int element_width = ELEMENT_WIDTH)
	{
		OStreamState ostream_state;
		std::stringstream result;
		result << tagArrayToString(tags, num_tags, element_width);
		result << trailer;
		return result.str();
	}

	template <typename T>
	std::string arrayElementsToString(T** array, array_size_t size, int value_width, int element_width) {
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
	std::string arrayElementsToString(std::string trailer, T** array, array_size_t size, int value_width, int element_width) {
		OStreamState current_state;
		std::stringstream result;
		result << arrayElementsToString(array, size, value_width, element_width);
		result << trailer;
		return result.str();
	}

	template <typename T>
	std::string blockSortToString(T** array, array_size_t size, array_size_t v,
							 	  std::unique_ptr<BlockTag<T>[]> &tags, int num_tags,
								  int value_width = 3, int element_width = 4) {
		OStreamState ostream_state;
		std::stringstream result;
		result << arrayIndicesToString(size, v, element_width) << std::endl;
		result << arrayElementsToString(array, size, value_width, element_width) << std::endl;
		result << tagArrayToString(tags, num_tags, element_width);
		return result.str();
	}

	/*	**************************************************	*/
	/*				print the entire structure				*/
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
	void printBlockSortArray(std::string trailer, T** array, array_size_t size, array_size_t v,
							 std::unique_ptr<BlockTag<T>[]> &tags, int num_tags) {

		constexpr const int 	element_width = 3;
		constexpr const int 	value_width = element_width - 1;
		constexpr const char 	spacer = ' ';
		// hand edit these to make them 'element_width' wide
		constexpr const char*   midpoint 	= "\\V/";

		//	preserve state of ostream; destructor will restore state
		OStreamState ostream_state;

		//	print out the index of the array
		for (int i = 0; i < size; i++) {
			if (i != v) std::cout << std::setw(value_width) << i << spacer;
			else std::cout << midpoint;
		}
		std::cout << std::endl;

		printElements(std::string(), array, size, value_width, element_width);
		std::cout << std::endl;
		std::cout << tagArrayToString(std::string(), tags, num_tags, element_width);
		std::cout << trailer;
	}


	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							algorithm functions							*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	template <typename T>
	ComparesAndMoves mergeBlocksByRotating(T** array, array_size_t start, array_size_t mid, array_size_t end);
	template <typename T>
	int createBlockTags( T** array, array_size_t start, array_size_t mid, array_size_t end,
			    	int block_size, std::unique_ptr<BlockTag<T>[]> &tags);
	template <typename T>
	ComparesAndMoves rotateArrayElementsRight(T** array, array_size_t start, array_size_t end, array_size_t amount);
	template <typename T>
	ComparesAndMoves rotateBlocksRight(T** array, std::unique_ptr<BlockTag<T>[]> &tags,
					array_size_t first_tag, array_size_t last_tag, int tag_rotate_count);
	template <typename T>
	ComparesAndMoves sortBlocksRightToLeft(T **array, array_size_t size,
			std::unique_ptr<BlockTag<T>[]> &tags, int num_tags);
	template <typename T>
	ComparesAndMoves swapBlocks(T** array,
								array_size_t block1_start, array_size_t block1_end,
								array_size_t block2_start, array_size_t block2_end);
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
	ComparesAndMoves mergeBlocksByRotating(T** array, array_size_t start, array_size_t mid, array_size_t end) {

		bool debug_verbosely = false;

		ComparesAndMoves result(0,0);

		array_size_t u = mid-1;
		array_size_t v = end;
		array_size_t rotate_count;

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
				std::cout << arrayIndicesToString(26, 3, 4) << std::endl;
				std::cout << arrayElementsToString(array, end-start+1, 3, 4)
						  << std::endl;
			}
			//	at this point `
			//	rotate u to the right in the array
			//	  past the last value in v that was equal to u_value
			result += rotateArrayElementsRight(array, rotate_count, u+1, v);

			if (debug_verbosely) {
				std::cout << "AFTER\n";
				std::cout << arrayElementsToString(array, end-start+1, 3, 4)
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

	/*
	 * 	createBlockTags(array, start, mid, end, block_size, tags_dst&);
	 *
	 * 	parses an array from [start:end] into blocks of span <= block_size
	 * 	 	to create a list of blocks of elements which is stored in tags_dst.
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
	int  createBlockTags(T** array, array_size_t start, array_size_t mid, array_size_t end,
			    		 int block_size, std::unique_ptr<BlockTag<T>[]> &tags) {

		ComparesAndMoves result(0,0);
		array_size_t lower_span = mid-start;
		array_size_t upper_span = end-mid + 1;

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
		array_size_t start_of_block = start;

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
	 * 		amounts greater than the span are modulo-division to be within the span
	 */

	template <typename T>
	ComparesAndMoves rotateArrayElementsRight(T** array, array_size_t amount,
											array_size_t start, array_size_t end) {

		ComparesAndMoves result(0,0);

		if (end <= start)
			return result;

		array_size_t span = end - start + 1;

		//	converts amounts that are not in [0,span) to in range
		amount = blockSortModulo<T>(amount, span);

		if (amount == 0)
			return result;

		//	reverse the entire array
		T* temp;
		for (array_size_t i = start, j = end; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result._moves += 3;
		}

		//	reverse the left hand side
		for (array_size_t i = start, j = start+amount-1; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result._moves += 3;
		}

		//	reverse the right hand side
		for (array_size_t i = start + amount, j = end; i < j; i++, j--) {
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
	 * 	rotateTags(array, tags, 2, 0, 4) results in
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
									   std::unique_ptr<BlockTag<T>[]> &tags,
							           array_size_t first, array_size_t last,
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

		auto rotateParametersToString = [&](array_size_t array_rotation) -> std::string {
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
			BlockSort::BlockTag<T> tmp = tags[i];
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
			array_size_t tmp = first;
			first = last;
			last = tmp;
		}

		array_size_t tag_span = last - first + 1;
		tag_rotate_count = blockSortModulo<T>(tag_rotate_count, tag_span);
		if (tag_rotate_count == 0) {
			return compares_and_moves;
		}

		// make a record of the span of the array covered by the tags
		//	 rotating the array needs these two indices
		//	 updating the indices of the rotated tags needs to know the start_index
		array_size_t first_start_index = tags[first].start_index;
		array_size_t last_end_index	= tags[last].end_index;
		//	calculate how far the array elements on the right of the array span
		//	  will be rotated off of the end of the span back to the start left
		int array_rotate_count = 0;
		for (int count = tag_rotate_count, tag_i = last; count > 0; --count, --tag_i) {
			array_rotate_count += tags[tag_i].numElements();
		}

		/*	rotate the underlying array	*/

		compares_and_moves += rotateArrayElementsRight(array, array_rotate_count,
												  	   first_start_index, last_end_index);

		/*	rotate the tags	*/

		compares_and_moves += rotateTagPersonalities();

		/*	assign indices	*/

		_debug(rotateParametersToString(array_rotate_count));

		//	The tag.type & tag.key are in the correct place in the tag array
		//	The indices still point the the original place in the underlying array
		//	Although each tag's indices are incorrect, the span between the
		//	  incorrect indices has not changed.  Use this to assign new indices

		array_size_t _start_index = first_start_index;
		for (int tag_i = first; tag_i <= last; tag_i++) {
			array_size_t _end_index = _start_index + tags[tag_i].numElements() - 1;
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
		if (areTagsSorted(tags, num_tags)) {
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
			tagArrayToString(debug.str(), tags, num_tags, 4);
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
			tagArrayToString(std::string(" AFTER\n"), tags, num_tags, 4);
			printElements(std::string("\n\n"), array, size, 3, 4);
#endif
		}

#if DEBUG_VERBOSE_SORT_BLOCKS
		if (areTagsSorted(tags, num_tags)) {
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
	 */

	template <typename T>
	ComparesAndMoves sortBlocksRightToLeft(T **array, array_size_t size,
										   std::unique_ptr<BlockTag<T>[]> &tags, int num_tags) {

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
		if (tags[num_tags-1].type == BlockType::A_BLOCK) {
			_debug("All blocks are A Blocks");
			return result;
		}

		// 	if the array consists of all B_Blocks, we are done
		if (tags[0].type == BlockType::B_BLOCK) {
			 _debug("All blocks are B Blocks");
			return result;
		}

		int num_a_blocks = 0;
		for (int i = 0; i != num_tags; i++) {
			if (tags[i].type == BlockType::A_BLOCK)
				num_a_blocks++;
			else
				break;
		}

		bool tagsAreSorted = true;
		for (int i = 1; i != num_tags; i++) {
			result._compares++;
			if (tags[i-1] > tags[i]) {
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
		array_size_t a_block_index = num_a_blocks-1;
		array_size_t b_block_index = num_tags-1;
		while (a_block_index >= 0) {

			//	Find first B_Block that is less than this A_Block
			//	There may not be a B_Block that is greater than this A_Block
			while (b_block_index > a_block_index) {
				result._compares++;
				if (*tags[b_block_index].key < *tags[a_block_index].key) {
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
				if (*tags[a_block_index].key <= *tags[b_block_index].key)
					break;
				a_block_index--;
				tag_rotate_count--;
			}

			if (debug_verbose) {
				array_size_t array_size = tags[num_tags-1].end_index+1;
				array_size_t v = tags[num_a_blocks].start_index;
				std::cout << "BEFORE: "
						  << " b_index (end) = " << std::setw(2) << b_block_index
						  << " a_index+1 (start) = " << std::setw(2) << a_block_index+1
						  << " rotation count = " << std::setw(2) << tag_rotate_count
						  << "\n"
						  << blockSortToString(array, array_size, v, tags, num_tags)
						  << std::endl;
			}
			// At this point 'a_block_index' points to the largest A_Block
			//	that is <= [b_block_index],  but [a_block_index+1] is greater
			//	than [b_block_index] b/c the previous while loop exited
			//	because a [b_block_index] that was smaller was found, and
			//	the comparison b_block_index == a_block_index return false
			result += rotateBlocksRight(array, tags,
										a_block_index+1, b_block_index,
										tag_rotate_count);
			// the B_Block has been shifted left by 'tag_rotate_count'
			b_block_index += tag_rotate_count;
			// a_block_index points to the block to the left of the span
			//	that was rotated, and the span that was rotated is now in order

			if (debug_verbose) {
				array_size_t array_size = tags[num_tags-1].end_index+1;
				array_size_t v = tags[num_a_blocks].start_index;
				std::cout << "AFTER: \n"
						  << blockSortToString(array, array_size, v, tags, num_tags)
						  << std::endl;
			}
		}

		if (debug_verbose) {
			array_size_t array_size = tags[num_tags-1].end_index+1;
			array_size_t v = tags[num_a_blocks].start_index;
			std::cout << "EXITING: \n"
					  << blockSortToString(array, array_size, v, tags, num_tags)
					  << std::endl;
			if (areTagsSorted(tags, num_tags)) {
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
#if 0
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
		if (areTagsSorted(tags, num_tags)) {
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
			tagArrayToString(debug.str(), tags, num_tags, 4);
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
			tagArrayToString(std::string(" AFTER\n"), tags, num_tags, 4);
			printElements(std::string("\n\n"), array, size, 3, 4);
#endif
		}

#ifdef DEBUG_VERBOSE_SORT_BLOCKS
		for (int i = 0; i != num_tags; i++) {
			std::cout << *tags[i].key << " ";
		}
		std::cout << std::endl;

		if (areTagsSorted(tags, num_tags)) {
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
#endif
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
								array_size_t block1_start, array_size_t block1_end,
								array_size_t block2_start, array_size_t block2_end)
	{
		ComparesAndMoves result(0,0);

		//	if the indices are not credible, leave
		if (block1_start < 0 || block1_end < 0 || block2_start < 0 || block2_end < 0) {
			//	TODO - throw an exception
			return result;
		}
		//	if necessary, swap indices so that the block is defined left to right
		if (block1_start > block1_end) {
			array_size_t tmp  = block1_start;
			block1_start = block1_end;
			block1_end   = tmp;
		}
		if (block2_start > block2_end) {
			array_size_t tmp  = block2_start;
			block2_start = block2_end;
			block2_end   = tmp;
		}
		//	ensure the blocks are well formatted:
		//	1. same size
		//	2. if necessary, swap indices
		//	3. ensure the blocks don't overlap
		array_size_t block1_span = block1_end-block1_start+1;
		array_size_t block2_span = block2_end-block2_start+1;

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
		for (array_size_t i = block1_start, j = block2_start;
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
	ComparesAndMoves sortArray(T** array, array_size_t start, array_size_t mid, array_size_t end, array_size_t block_size)
	{
		ComparesAndMoves result(0,0);

		return result;
	}

	/*	**************************	*/
	/*		the sorting function	*/
	/*	**************************	*/

	template <typename T>
	ComparesAndMoves sortPointersToObjects(T **array, array_size_t size) {
//		constexpr int debug_element_width = 4;
//		constexpr int debug_value_width = 3;
		constexpr array_size_t min_array_size_to_sort = 32;

		if (size < min_array_size_to_sort) {
			return InsertionSort::sortPointersToObjects(array, size);
		}

		//	The first time tags are created on the unsorted array,
		//	  the elements within the tags will need to be sorted.
		//	  This sorting may cause the 'key' value pointer
		//	  of the BlockTag to no longer point to the correct value,
		//	  which needs to be corrected by going through the tags
		bool sort_within_blocks = true;

		ComparesAndMoves result(0,0);

		for (array_size_t span = min_array_size_to_sort; span < size/2; span *= 2) {
			for (array_size_t start = 0; start < size; start++) {
 				std::unique_ptr<BlockTag<T>[]> tags;
				int num_tags;
				array_size_t end = start + span - 1;
				if (end >= size)
					end = size-1;
				array_size_t mid = start + (end-start+1) / 2;
				array_size_t block_size = static_cast<array_size_t>(std::sqrt(span/2));
				num_tags = createBlockTags(array, start, mid, end, block_size, tags);
				if (sort_within_blocks) {
					sort_within_blocks = false;
					for (int i = 0; i != num_tags; i++) {
						InsertionSort::sortPointersToObjects(&array[tags[i].start_index], tags[i].numElements());
					}
					//	The underlying array has changed, so the keys are state
					for (int i = 0; i != num_tags; i++) {
						tags[i].assignKey(array);
					}
				}
				sortBlocksRightToLeft(array, span, tags, num_tags);
				for (int i = num_tags-1; i > 0; i--) {
					//	TODO - this merges the entire array, not the blocks
					//		 - need to create a block merge
					mergeBlocksByRotating(array, start, mid, end);
				}
			}
		}

		return result;
	}

}	// namespace BlockSort

#endif /* BLOCKSORT_H_ */
