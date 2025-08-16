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
	 * 	ComparesAndMoves sortBlocks(array, size, p_tags, num_tags);
	 *
	 * 		organizes the array such that every A_Block (left side block)
	 * 			is moved to the right of every B_Block (right side block)
	 * 			that is less than it where 'is less' is determined by comparing
	 * 			the left-most element of the a_block (which is tag.key)
	 * 			against the right-most element of the b block (tag.key)
	 * 		this ensures that no element in the A
	 *
	 *
	 */
	template <typename T>
	ComparesAndMoves sortBlocks(T **array, array_size_t size, BlockTag<T> *tags, int num_tags) {

		ComparesAndMoves result(0,0);
		int num_a_blocks = [](BlockTag<T>*tags, int num_tags) {
			int result = 0;
			for (int i = 0; i != num_tags; i++) {
				if (tags[i].type == BlockType::A_BLOCK) {
					result++;
				} else {
					break;
				}
			}
			return result;
		};
		int lowest_b_block = num_a_blocks;
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
		std::cout << "u_size " << u_size
				  << " v_size " << v_size
				  << " block_width " << block_size
				  << " first_u_width " << first_u_width
				  << " last_v_width " << last_v_width
				  << " num_blocks " << num_blocks << std::endl;
		BlockTag<T> tags[num_blocks];
		array_size_t start_of_block = 0;
		//	if the first block is less than a full block size, othewisze full size
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
		printTags(std::string(" tags\n"), tags, num_blocks, element_width);
		printElements(std::string("\n"), array, size, value_width, element_width);
//		std::cout << std::endl;
		return result;
	}

}	// namespace BlockSort

#endif /* BLOCKSORT_H_ */
