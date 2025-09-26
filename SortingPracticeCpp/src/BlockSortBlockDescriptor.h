/*
 * BlockSortBlockDescriptor.h
 *
 *  Created on: Sep 8, 2025
 *      Author: joe
 */

#ifndef BLOCKSORTBLOCKDESCRIPTOR_H_
#define BLOCKSORTBLOCKDESCRIPTOR_H_


#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <memory>

#include "SortingPracticeDataTypes.h"

using index_t = array_size_t;

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

namespace BlockSort {

	/*
	 * class BlockDescriptor
	 *
	 *   Keeps track of relevant traits of a block of the array where 'block'
	 * refers to the blocks used in the Block Sort algorithm.
	 *
	 *   There are some implementations that claim that this information can
	 * be kept within the array itself.  This class is created so that debugging
	 * the algorithm itself can be decoupled from sorting in place.  This is because
	 * sorting in place requires using portions of the array to keep track of block
	 * type (A vs B) and block order.  This information is implied by putting elements
	 * of the array out-of-sequence to represent block movement.  The elements used
	 * have to be unique an monotonically ascending.  It is possible that there are
	 * fewer unique values in the array than there are blocks, which requires a
	 * modification in how the algorithm proceeds.
	 *
	 * 	 The relevant traits kept are:
	 * 	 - the type, A_Block or B_Block indicating whether the underlying elements
	 * 	   were initially in the left half, or right half + 1 side of the array
	 * 	 - the start index & end index, which is useful when the blocks are not
	 * 	   all of the same size
	 * 	 - the key, which in an A_Block is the value at [start] and
	 * 	                  in a  B_Block is teh value at [end]
	 *
	 * 	 Block Sort algorithm guarantees stability, which means that in the final
	 * sorted array, any element in the left half of the array (A_Block)
	 * must come before an element with an equivalent key in the right half of the
	 * array (B_Block).  When positioning / sorting the blocks by key, an A_Block
	 * may only go to the right of a B_Block if the least (left) element
	 * (and all other elements) in the A_Block are greater than the largest (right)
	 * element in the B_Block.
	 *
	 *   Consider the two blocks if the first element in both blocks was the sort key
	 *
	 *               key                       key
	 *           A [ da  ea  fa  ga ] and  B [ cb  db  eb  gb ]
	 *
	 * The "ga" in A may NOT move to the right of the "gb" in B, which would reverse
	 * the order of the two 'f' elements and thus violate stability
	 *
	 *               key         |< relative order change >|
	 *           B [ cb  db  eb  gb ]      A [ da  eb  fa  ga ]
	 *
	 *   Consider the two blocks where the left block's key (A_Block) is the first
	 * element athe the right block's key (B_Block) is the last element
	 *
	 *               key                                   key
	 *           A [ ga  ha  ia  ja  ] and B [ cb  db  eb  fb  ]
	 *
	 * Swapping the blocks does not violate stability because the smallest element in
	 * A, "ga", which is A's key, is greater than the largest element in B, "fb"
	 * which is B's key
	 *                           key           key
	 *           B [ cb  db  eb  fb ]      A [ ga  ha  ia  ja ]
	 */

	template <typename T>
	class BlockDescriptor {
	public:
		BlockType type;
		T* key;
		index_t start_index;
		index_t end_index;

		index_t	getWidth() const {
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

		BlockDescriptor() {
			type = BlockType::UNSPECIFIED;
			key = nullptr;
			start_index = 0;
			end_index = 0;
		}
		BlockDescriptor(BlockType t, T* k, index_t s, index_t e) {
			type = t;
			key = k;
			start_index = s;
			end_index = e;
		}
		BlockDescriptor(const BlockDescriptor &other) {
			if (this != &other) {
				type = other.type;
				key = other.key;
				start_index = other.start_index;
				end_index = other.end_index;
			}
		}
		BlockDescriptor& operator=(const BlockDescriptor &other) {
			if (this != &other) {
				type = other.type;
				key = other.key;
				start_index = other.start_index;
				end_index = other.end_index;
			}
			return *this;
		}

		bool operator==(const BlockDescriptor &other) const {
			if (key != nullptr && other.key != nullptr) {
				return *key == *other.key;
			}
			return false;
		}
		bool operator<(const BlockDescriptor &other) const {
			if (key != nullptr && other.key != nullptr) {
				return *key < *other.key;
			}
			return false;
		}
		bool operator<=(const BlockDescriptor &other) const {
			return *this == other || *this < other;
		}
		bool operator>(const BlockDescriptor &other) const {
			return  !(*this == other || *this < other);
		}
		bool operator>=(const BlockDescriptor &other) const {
			return !(*this < other);
		}
		bool operator!=(const BlockDescriptor &other) const {
			return !(*this == other);
		}

		bool isExactlyEqual(const BlockDescriptor &other) const {
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
	std::ostream& operator<<(std::ostream& out, const BlockDescriptor<T> &object) {
		out << object.to_string();
		return out;
	}
}

#endif /* BLOCKSORTBLOCKDESCRIPTOR_H_ */
