/*
 * blockSortDebugging.h
 *
 *  Created on: Sep 27, 2025
 *      Author: joe
 */

#ifndef BLOCKSORTDEBUGGING_H_
#define BLOCKSORTDEBUGGING_H_
//	for VALUE_WIDTH and ELEMENT_WIDTH
#include "SortingUtilities.h"

namespace BlockSort {

	constexpr char 	TAG_BOUNDARY_CHAR 	= '|';
	constexpr char 	TAG_SPACE_CHAR 		= ' ';

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
										 int element_width = ELEMENT_WIDTH) {

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
								  int value_width = VALUE_WIDTH,
								  int element_width = ELEMENT_WIDTH) {
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


#endif /* BLOCKSORTDEBUGGING_H_ */
