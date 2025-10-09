/*
 * blockSortManageBlocks.h
 *
 *  Created on: Oct 6, 2025
 *      Author: joe
 */

#ifndef BLOCKSORTDESCRIPTORS_H_
#define BLOCKSORTDESCRIPTORS_H_

#include <memory>
#include "SortingPracticeDataTypes.h"
//#include "BlockSortDataTypes.h"

namespace BlockSort {

	using index_t = long;

	template <typename T>
	class BlockDescriptor;

	/*	Assigns key values to the blocks.  This is useful if the underlying array
	 * has been sorted, such as initially sorting element within blocks. */

	template <typename T>
	ComparesAndMoves assignBlockKeys(T**array, std::unique_ptr<BlockDescriptor<T>[]> &descriptors, int num_descriptors);

	/*	Creates an array of block descriptors of types A & B {A[0]A[1]..A[m]B[0]..B[n]}
	 * 	  where A[0] is a full block but B[n] may not be a full block in the case where
	 * 	  the size of the array is not an integer multiple of the block size.  For example,
	 * 	  if (end-start+1) = 18 and block_size = 4, the last B_Block will contain 2 elements
	 * 	It is the caller's responsibility to ensure that mid is an integer multiple
	 * 	  of block_size from start. 	*/

	template <typename T>
	int createBlockDescriptors_A0_Full( T** array, index_t start, index_t mid, index_t end,
										int block_size,
										std::unique_ptr<BlockDescriptor<T>[]> &descriptors);


	/*	Creates an array of block descriptors of types A & B {[A0][A1]..[Am][B0]..[Bn]}
	 * 		where [A0] and B[n] may not be full blocks. m <= n, n is at most m+1.  For example
	 * 		if (end-start+1) = 18 and block size = 4, the first A_Block will have 1 element
	 * 		and the last B_Block will have 1 element.
	 * 	There is no restriction on the value of mid from start	*/

	template <typename T>
	int createBlockDescriptorsSymmetrically(T** array,
											index_t start, index_t mid, index_t end,
		    								int block_size,
											std::unique_ptr<BlockDescriptor<T>[]> &descriptors);

	/*
	 * 	Makes createBlock_______() more concise
	 */

	template <typename T>
	void assignBlockData(BlockDescriptor<T> &block, T**array, BlockType type, index_t start, index_t block_size);


	/*	***********************************************************************************	*/
	/*	***********************************************************************************	*/
	/*										functions										*/
	/*	***********************************************************************************	*/
	/*	***********************************************************************************	*/

	/*
	 * Assigns each block's key value based on the values in 'array'.  This
	 *	  is necessary when the underlying array has been reordered directly
	 *	  rather than using one of the blockDescriptor sorting functions
	 */

	template <typename T>
	ComparesAndMoves assignBlockKeys(T**array, std::unique_ptr<BlockDescriptor<T>[]> &descriptors, int num_descriptors) {

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
	 * 	createBlockDescriptors_A0_Full(array, start, mid, end, block_size, descriptor_dst&);
	 *
	 *		<<< !!! 'mid' must be an integer multiple of 'block_size' from 'start' !!! >>>
	 *
	 *	Inputs: array		- pointer to an array of pointers to type T
	 *			start		- index of first element in the span
	 *			mid			- the expected location of the left most block B[0]
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
		int block_number 		= 0;
		index_t start_of_block 	= start;

		//	the full A_Blocks where .end_index = (.start_index+block_size-1)
		while (start_of_block < mid) {
			assignBlockData(blocks[block_number], array,
							BlockType::A_BLOCK, start_of_block, block_size);
			start_of_block += block_size;
			block_number++;
		}

		//	If there are any B_Blocks
		if (block_number != num_blocks) {
			//	the full B_Blocks where .end_index = (.start_index+block_size-1)
			while (block_number < num_blocks-1) {
				assignBlockData(blocks[block_number], array,
								BlockType::B_BLOCK, start_of_block, block_size);
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
	 * 		                   0  1  2  3  4  5  6  7  8
	 * 		                 { a, c, e, g, i, b, d, f, h }
	 * 	descriptor types are  |A||A     ||       B||   B|
	 *  descriptor indices are [0] [1:3]       [4:6] [7:8]
	 *  descriptor keys are    'a' 'c'           'd'   'h'
	 *
	 * 	returns the count of tags which is 4
	 */

	template <typename T>
	int  createBlockDescriptorsSymmetrically(
			T** array,
			index_t start, index_t mid, index_t end,
			int block_size,
			std::unique_ptr<BlockDescriptor<T>[]> &blocks) {

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
		int block_number 		= 0;
		index_t start_of_block 	= start;

		//	do the A_Blocks first
		//	if the first A_Block is a partial, it's .end_index != (.start_index+block_size-1)
		if (first_block_size) {
			assignBlockData(blocks[block_number], array,
							BlockType::A_BLOCK, start_of_block, first_block_size);
			start_of_block += first_block_size;
			block_number = 1;
		}

		//	the full A_Blocks where .end_index = (.start_index+block_size-1)
		while (start_of_block < mid) {
			assignBlockData(blocks[block_number], array,
							BlockType::A_BLOCK, start_of_block, block_size);
			start_of_block += block_size;
			block_number++;
		}

		//	the full B_Blocks where .end_index = (.start_index+block_size-1)
		while (block_number < num_blocks-1) {
			assignBlockData(blocks[block_number], array,
							BlockType::B_BLOCK, start_of_block, block_size);
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
	 * 	Helper routine to make createBlock functions more concise
	 */

	template <typename T>
	void assignBlockData(BlockDescriptor<T> &block, T**array, BlockType type, index_t start, index_t block_size) {
		block.type 			= type;
		block.start_index 	= start;
		block.end_index		= start + block_size-1;
		if (type == BlockType::A_BLOCK) {
			block.key = array[block.start_index];
		} else {
			block.key = array[block.end_index];
		}
	}

}

namespace BlockSort
{
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

#endif /* BLOCKSORTDESCRIPTORS_H_ */
