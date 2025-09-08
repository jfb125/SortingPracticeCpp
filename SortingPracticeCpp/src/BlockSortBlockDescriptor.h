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

	template <typename T>
	class BlockDescriptor {
	public:
		BlockType type;
		T* key;
		index_t start_index;
		index_t end_index;

		index_t	numElements() const {
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
