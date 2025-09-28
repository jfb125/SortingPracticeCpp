/*
 * 	BlockSortTest.cpp
 *
 *  Created on: Aug 14, 2025
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <numeric>
#include <cmath>
#include <vector>
#include <algorithm>

#include "IntegerArithmetic.h"
#include "nChoosek.h"

#include "BlockSort.h"
//	BlockSortUnused.h contains functions that are no longer used to implement
//	the BlockSort, but have been retained to avoid losing all the work that went
//	into creating them.  They can be accessed for regression testing by prepending
//	'regressionTestOnly_' to their function names.
#include "BlockSortUnused.h"

using namespace BlockSort;


enum class MergeStrategy {
	 TABLE, AUXILLIARY, ROTATE
};

std::string toString(MergeStrategy strategy) {
	switch (strategy) {
	case MergeStrategy::AUXILLIARY:
		return std::string("AUXILIARY");
	case MergeStrategy::TABLE:
		return std::string("TABLE    ");
	case MergeStrategy::ROTATE:
		return std::string("ROTATE   ");
	default:
		return std::string("UKNOWN   ");
	};
}

MergeStrategy merge_strategy = MergeStrategy::ROTATE;


/*	**********************************************	*/
/*	**********************************************	*/
/*		forward declaration of test functions		*/
/*	**********************************************	*/
/*	**********************************************	*/

//#define TEST_MODULO
//#define TEST_BLOCK_SORT_BINARY_SEARCH_FIRST_ELEMENT
//#define TEST_BLOCK_SORT_BINARY_SEARCH_LAST_ELEMENT
//#define TEST_BLOCK_SORT_BINARY_SEARCH_FIRST_BLOCK
//#define TEST_BLOCK_SORT_BINARY_SEARCH_LAST_BLOCK
//#define TEST_BLOCK_SORT_BINARY_SEARCH_DESCRIPTOR_SEARCH
//#define TEST_BLOCK_SORT_CREATE_DESCRIPTORS
//#define TEST_BLOCK_SORT_FLOOR_LOG_2
//#define TEST_BLOCK_SORT_MERGE_BLOCKS_RANDOMLY
//#define TEST_BLOCK_SORT_MERGE_BLOCKS_EXHAUSTIVELY
//#define TEST_BLOCK_SORT_ROTATE_ELEMENTS
//#define TEST_BLOCK_SORT_ROTATE_BLOCKS
#define TEST_BLOCK_SORT_SWAP_BLOCKS
//#define TEST_BLOCK_SORT_SWAP_DESCRIPTORS
//#define TEST_BLOCK_SORT_SWAP_BLOCK_ELEMENTS
//#define TEST_BLOCK_SORT_SORT_BLOCKS
//#define TEST_BLOCK_SORT_SORT

bool testBlockSortBinarySearchFirstBlock();
bool testBlockSortBinarySearchLastBlock();
bool testBlockSortBinarySearchFirstElement();
bool testBlockSortBinarySearchLastElement();
bool testBlockSortCreateDescriptors();
bool testBlockSortMergeBlocksRandomly();
bool testBlockSortMergeBlocksExhaustively();
bool testBlockSortModulo();
bool testBlockSortRotateArrayElements();
bool testBlockSortRotateBlocks();
bool testBlockSortSort();
bool testBlockSortSortBlocks();
bool testBlockSortSwapBlockElements();
bool testBlockSortSwapDescriptors();
bool testBlockSortSwapBlocks();
bool testBlockSort();
bool testFloorLog2();

#define runTest(result, func, name_str) do {\
	if (!func()) {\
		std::cout << "ERROR: " << name_str << " returns FALSE" << std::endl;\
		result = false;\
	} else {\
		std::cout << "PASS:  " << name_str << " returns TRUE" << std::endl;\
	}\
} while(false)

template <typename T>
bool generateAllCombinationsOfValues(T*** test_vectors,
									 T**  test_values,
									 int num_test_vectors,
									 int test_vector_size,
									 int mid);

template <typename T>
bool generateAllPermutationsOfValues(T** test_vectors[],
									 T** test_values,
						 	 	 	 int num_test_vectors,
									 int test_vector_size);

template<typename T>
void randomizeArray(T **array, array_size_t size);


/*	**********************************************	*/
/*	**********************************************	*/
/*					testBlockSort()					*/
/*	**********************************************	*/
/*	**********************************************	*/

bool testBlockSort() {
	bool all_tests_passed = true;
	int num_tests = 0;
	int tests_passed = 0;
	std::cout << "testBlockSort()" << std::endl;

#ifdef TEST_BLOCK_SORT_CREATE_DESCRIPTORS
	num_tests++;
	runTest(all_tests_passed, testBlockSortCreateDescriptors, "function blockSortCreateDescriptors()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
#endif

#ifdef TEST_MODULO
	num_tests++;
	runTest(all_tests_passed, testBlockSortModulo, "function blockSortModulo()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
#endif

#ifdef TEST_BLOCK_SORT_FLOOR_LOG_2
	num_tests++;
	runTest(all_tests_passed, testFloorLog2, "function floorLog2()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
#endif

#ifdef TEST_BLOCK_SORT_BINARY_SEARCH_LAST_BLOCK
	num_tests++;
	runTest(all_tests_passed, testBlockSortBinarySearchLastBlock, "function testBlockSortBinarySearchLastBlock()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
#endif

#ifdef	TEST_BLOCK_SORT_ROTATE_ELEMENTS
	num_tests++;
	runTest(all_tests_passed, testBlockSortRotateArrayElements, "function testBlockSortRotateArrayElements()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
#endif

#ifdef	TEST_BLOCK_SORT_ROTATE_BLOCKS
	num_tests++;
	runTest(all_tests_passed, testBlockSortRotateBlocks,
			"function testBlockSortRotateBlocks()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
#endif

#ifdef TEST_BLOCK_SORT_SORT_BLOCKS
	num_tests++;
	runTest(all_tests_passed, testBlockSortSortBlocks, "function testBlockSortSortBlocks()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
#endif

#ifdef TEST_BLOCK_SORT_MERGE_BLOCKS_EXHAUSTIVELY
	num_tests++;
	merge_strategy = MergeStrategy::ROTATE;
	runTest(all_tests_passed, testBlockSortMergeBlocksExhaustively, "function testBlockSortMergeBlocksExhaustively()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
	std::cout << std::endl;
	num_tests++;
	merge_strategy = MergeStrategy::TABLE;
	runTest(all_tests_passed, testBlockSortMergeBlocksExhaustively, "function testBlockSortMergeBlocksExhaustively()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
#endif

#ifdef TEST_BLOCK_SORT_MERGE_BLOCKS_RANDOMLY
	num_tests++;
	merge_strategy = MergeStrategy::ROTATE;
	runTest(all_tests_passed, testBlockSortMergeBlocksRandomly, "function testBlockSortMergeBlocksRandomly()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
	num_tests++;
	merge_strategy = MergeStrategy::TABLE;
	runTest(all_tests_passed, testBlockSortMergeBlocksRandomly, "function testBlockSortMergeBlocksRandomly()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
#endif

#ifdef TEST_BLOCK_SORT_SWAP_BLOCK_ELEMENTS
	num_tests++;
	runTest(all_tests_passed, testBlockSortSwapBlockElements, "function testBlockSwapElements()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
#endif

#ifdef TEST_BLOCK_SORT_SWAP_BLOCKS
	num_tests++;
	runTest(all_tests_passed, testBlockSortSwapBlocks, "function testBlockSortSwapBlocks()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
#endif

#ifdef	TEST_BLOCK_SORT_SORT
	num_tests++;
	runTest(all_tests_passed, testBlockSortSort, "function testBlockSortSort()");
	if (!all_tests_passed)
		return passed;
	tests_passed++;
#endif

#ifdef	TEST_BLOCK_SORT_BINARY_SEARCH_FIRST_ELEMENT
	num_tests++;
	runTest(all_tests_passed, testBlockSortBinarySearchFirstElement, "function testBlockSortBinaryFirst()");
	if (!all_tests_passed)
		return passed;
	tests_passed++;
#endif

#ifdef	TEST_BLOCK_SORT_BINARY_SEARCH_LAST_ELEMENT
	num_tests++;
	runTest(all_tests_passed, testBlockSortBinarySearchLastElement, "function testBlockSortBinaryLast()");
	if (!all_tests_passed)
		return passed;
	tests_passed++;
#endif

	std::cout << "testBlockSort() ran " << tests_passed << " successful tests\n";
	return all_tests_passed;
}

/*	**********************************************	*/
/*	**********************************************	*/
/*					individual tests				*/
/*	**********************************************	*/
/*	**********************************************	*/

	/*	***************************************	*/
	/*				Binary searching			*/
	/*	***************************************	*/

std::string toStringValueArrayIndices(int start, int end, int width = 4){
	std::stringstream result;
	for (int i = start; i < end; i++) {
		result << std::setw(width-1) << i << ' ';
	}
	result << std::setw(width-1) << end;
	return result.str();
}

template <typename T>
std::string toStringValueArray(T** array, int start, int end, int highlight = -1, int width = 4) {

	std::stringstream result;
	for (int i = start; i < end; i++) {
		if (i != highlight) {
			result << std::setw(width-1) << *array[i] << ' ';
		} else {
			result << '_' << std::setw(width-1) << *array[i] << '_';
		}
	}
	result << std::setw(width-1) << *array[end];
	return result.str();
}

int *unique_values[] = {
		new int(0), new int(1), new int(2), new int(3),
		new int(4), new int(5), new int(6), new int(7)
};
int unique_values_size = sizeof(unique_values)/sizeof(int*);
int minimum_unique_value = 0;
int maximum_unique_value = 7;

int *triple_repeated_values[] = {
		new int(0), new int(0), new int(0),
		new int(1), new int(1), new int(1),
		new int(2), new int(2), new int(2),
		new int(3), new int(3), new int(3),
};
int triple_repeated_values_size = sizeof(triple_repeated_values)/sizeof(int*);
int minimum_repeated_value = 0;
int maximum_repeated_value = 3;
int num_times_repeated = 3;

bool testBlockSortBinarySearchFirstElement() {
	bool test_passed = true;

	for (int array_end = unique_values_size-2; array_end < unique_values_size; array_end++) {
		std::cout << "\nTesting binaryFirst() with an array with a "
				  << array_end << " unique elements\n";
		std::cout << toStringValueArrayIndices(0, array_end)
				  << std::endl;
		for (int i = minimum_unique_value-1; i <= array_end+1; i++) {
			std::cout << toStringValueArray(unique_values, 0, array_end, -1, 4);
			int *value = new int(i);
			index_t result = binarySearchFirstElement(unique_values, 0, array_end, value);
			std::cout << " insert " << std::setw(2) << *value << " before " << result;
			if ((i <= minimum_unique_value && result != minimum_unique_value) ||
				(i > minimum_unique_value && result != *value)) {
				std::cout << " WHICH IS AN ERROR\n";
				test_passed = false;
				goto TEST_BINARY_SEARCH_FIRST_RETURN_LABEL;
			}
			std::cout << std::endl;
		}
	}

	for (int array_end = triple_repeated_values_size-2;
			 array_end < triple_repeated_values_size; array_end++) {
		std::cout << "\nTesting binaryFirst() with an array with an "
				  << ((array_end+1) & 1 ? "odd" : "even")
				  << " number of unique elements with each repeated "
				  << num_times_repeated << "\n";
		std::cout << toStringValueArrayIndices(0, array_end)
				  << std::endl;
		for (int i = minimum_repeated_value-1; i <= maximum_repeated_value+1; i++) {
			std::cout << toStringValueArray(triple_repeated_values, 0, array_end, -1, 4);
			int *value = new int(i);
			index_t result = binarySearchFirstElement(triple_repeated_values, 0, array_end, value);
			std::cout << " insert " << std::setw(2) << *value << " before " << result;
			if (i <= minimum_repeated_value) {
				if (result != 0) {
					std::cout << " WHICH IS AN ERROR\n";
					test_passed = false;
					goto TEST_BINARY_SEARCH_FIRST_RETURN_LABEL;
				}
			} else {
				if (i <= maximum_repeated_value) {
					if (result != num_times_repeated * i) {
						std::cout << " WHICH IS AN ERROR\n";
						test_passed = false;
						goto TEST_BINARY_SEARCH_FIRST_RETURN_LABEL;
					}
				} else {
					if (result != array_end+1) {
						std::cout << " WHICH IS AN ERROR\n";
						test_passed = false;
						goto TEST_BINARY_SEARCH_FIRST_RETURN_LABEL;
					}
				}
			}
			std::cout << std::endl;
		}
	}
TEST_BINARY_SEARCH_FIRST_RETURN_LABEL:
	return test_passed;
}

bool testBlockSortBinarySearchLastElement() {

	bool test_passed = true;

	for (int array_end = unique_values_size-2;
			  array_end < unique_values_size; array_end++) {
		std::cout << "\nTesting binaryLast() with an array with "
				  << array_end << " unique elements\n";
		std::cout << toStringValueArrayIndices(0, array_end)
				  << std::endl;
		for (int i = minimum_unique_value-1; i <= array_end+1; i++) {
			std::cout << toStringValueArray(unique_values, 0, array_end, -1, 4);
			int *value = new int(i);
			index_t result = binarySearchLastElement(unique_values, 0, array_end, value);
			std::cout << " insert " << std::setw(2) << *value << " before " << result;
			if ((i <  minimum_unique_value && result != minimum_unique_value) ||
				(i >= minimum_unique_value && i < maximum_unique_value && result != *value+1) ||
				(i >=  maximum_unique_value && result != array_end+1)) {
				std::cout << " WHICH IS AN ERROR\n";
				test_passed = false;
				goto TEST_BINARY_SEARCH_LAST_RETURN_LABEL;
			}
			std::cout << std::endl;
		}
	}

	for (int array_end = triple_repeated_values_size-2;
			 array_end < triple_repeated_values_size; array_end++) {
		std::cout << "\nTesting binaryLast() with an array with an "
				  << ((array_end+1) & 1 ? "odd" : "even")
				  << " elements each repeated "
				  << num_times_repeated << "\n";
		std::cout << toStringValueArrayIndices(0, array_end)
				  << std::endl;
		for (int i = minimum_repeated_value-1; i <= maximum_repeated_value+1; i++) {
			std::cout << toStringValueArray(triple_repeated_values, 0, array_end, -1, 4);
			int *value = new int(i);
			index_t result = binarySearchLastElement
					(triple_repeated_values, 0, array_end, value);
			std::cout << " insert " << std::setw(2) << *value << " before " << result;
			if (i < minimum_repeated_value) {
				if (result != 0) {
					std::cout << " WHICH IS AN ERROR\n";
					test_passed = false;
					goto TEST_BINARY_SEARCH_LAST_RETURN_LABEL;
				}
			}
			if (i >= minimum_repeated_value && i < maximum_repeated_value) {
				if (result != 3*(i+1)) {
					std::cout << " WHICH IS AN ERROR\n";
					test_passed = false;
					goto TEST_BINARY_SEARCH_LAST_RETURN_LABEL;
				}
			}
			if (i >= maximum_repeated_value) {
				if (result != array_end+1) {
					std::cout << " WHICH IS AN ERROR\n";
					test_passed = false;
					goto TEST_BINARY_SEARCH_LAST_RETURN_LABEL;
				}
			}
			std::cout << std::endl;
		}
	}

TEST_BINARY_SEARCH_LAST_RETURN_LABEL:
	return test_passed;
}


	/*	*******************************************************	*/
	/*	*******************************************************	*/
	/*						createDescriptors					*/
	/*	*******************************************************	*/
	/*	*******************************************************	*/

template <typename T>
bool validateCreateBlocks_A0_Full(
		std::unique_ptr<BlockDescriptor<T>[]> &descriptors,
		int num_descriptors,
		index_t mid,
		int block_size,
		std::stringstream &msg)
{
	bool test_result = true;
	msg.str("");

	int block_number = 0;

	//	it is possible that the descriptors are only A_BLOCKs
	for (; block_number != num_descriptors; block_number++) {
		if (descriptors[block_number].type == BlockType::B_BLOCK) {
			break;
		}
		if (descriptors[block_number].getWidth() != block_size) {
			test_result = false;
			msg << "ERROR - A_BLOCK[" << block_number << "] is not full size = " << block_size;
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
		}
	}

	// first B_Block must start at 'mid'
	if (descriptors[block_number].start_index != mid) {
		test_result = false;
		msg << "ERROR - the first B_Block.start_index " << descriptors[block_number].start_index
			<< " is not = " << mid;
		goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
	}

	//	all but the final B_Block has to be 'block_size'
	for (; block_number < num_descriptors-1; block_number++) {
		if (descriptors[block_number].getWidth() != block_size) {
			test_result = false;
			msg << "ERROR - B_BLOCK[" << block_number << "] is not full size = " << block_size;
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
		}
	}

VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT:
	return test_result;
}

template <typename T>
bool validateCreateBlocksSymmetrically(
		std::unique_ptr<BlockDescriptor<T>[]> &descriptors,
		int num_descriptors,
		index_t mid,
		int block_size,
		std::stringstream &msg)
{
	bool test_result = true;
	msg.str("");

	//	the first A_Block may not be a full block
	int block_number = 1;

	//	it is possible that the descriptors are only A_BLOCKs
	for (; block_number != num_descriptors; block_number++) {
		if (descriptors[block_number].type == BlockType::B_BLOCK) {
			break;
		}
	}

	// first B_Block must start at 'mid'
	if (descriptors[block_number].start_index != mid) {
		test_result = false;
		msg << "ERROR - the first B_Block.start_index " << descriptors[block_number].start_index
			<< " is not = " << mid;
		goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT;
	}

	//	all but the final B_Block has to be 'block_size'
	for (; block_number < num_descriptors-1; block_number++) {
		if (descriptors[block_number].getWidth() != block_size) {
			test_result = false;
			msg << "ERROR - B_BLOCK[" << block_number << "] is not full size = " << block_size;
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT;
		}
	}

VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT:
	return test_result;
}

bool testBlockSortCreateDescriptors() {

	constexpr bool verbose_output = true;

	bool test_result = true;
	bool test_a0_full = true;
	std::stringstream test_result_msg;
	std::stringstream validation_msg;

	char *test_vector[] = {
		new char('a'), new char('b'), new char('c'), new char('d'),
		new char('e'), new char('f'), new char('g'), new char('h'),
		new char('i'), new char('j'), new char('k'), new char('l'),
		new char('m'), new char('n'), new char('o'), new char('p'),
		new char('q'), new char('r'), new char('s'), new char('t'),
		new char('u'), new char('v'), new char('w'), new char('x'),
		new char('y'), new char('z'),
		new char('A'), new char('B'), new char('C'), new char('D'),
		new char('E'), new char('F'), new char('G'), new char('H'),
		new char('H'), new char('J'), new char('K'), new char('L'),
		new char('M'), new char('N'), new char('O'), new char('P'),
		new char('Q'), new char('R'), new char('S'), new char('T'),
		new char('U'), new char('V'), new char('W'), new char('X'),
		new char('Y'), new char('Z')
		};

//	int test_vector_size = sizeof(test_vector) / sizeof(char*);

	int minimum_block_size = 2;
	int maximum_block_size = 2;
	int nominal_array_size = 16;

	//	perform the test both on createBlockDescriptors_A0_Full()
	//	  					 and createBlocKDescriptorsSymmetrically()
	do {
		for (int block_size  = minimum_block_size;
				 block_size <= maximum_block_size; block_size++) {

			//	try all necessary array sizes to generate all fractional
			//	  sizes of the final B_Block, from 0 to block_size-1
			int num_blocks_possible = nominal_array_size / block_size;
			int num_complete_blocks = num_blocks_possible;
			int minimum_array_size = num_complete_blocks * block_size;
			int maximum_array_size = minimum_array_size+block_size-1;
			for (int array_size = minimum_array_size;
				 array_size <= maximum_array_size; array_size++) {
				// calculate how many full blocks there will be
				int minimum_num_blocks = array_size / block_size;
				// the mid, which is where the first B_Block starts
				//	is the the left of all the A_Blocks
				int start = 0;
				int end = array_size-1;
				std::unique_ptr<BlockDescriptor<char>[]> descriptors;
				int mid = 0;
				int num_blocks = 0;
				if (test_a0_full) {
					mid = (minimum_num_blocks / 2) * block_size;
					num_blocks = createBlockDescriptors_A0_Full(test_vector,
																start, mid, end,
																block_size,
																descriptors);
				} else {
					mid = array_size / 2;
					num_blocks = regressionTestOnly_CreateBlockDescriptorsSymmetrically(test_vector,
																	 start, mid, end,
																	 block_size,
																	 descriptors);
				}

	//			/* ways to force errors */
	//			/* force a[0] to not be a full block */
	//			descriptors[0].end_index--;
	//			/* force b[0] to not start at 'mid' */
	//			int b0_location = 0;
	//			for (; b0_location < num_blocks; b0_location++) {
	//				if (descriptors[b0_location].type == BlockType::B_BLOCK)
	//					break;
	//			}
	//			if (b0_location != num_blocks) {
	//				descriptors[b0_location].start_index++;
	//			}
	//			/* force the 2nd to last b_block to not be a full block	*;
	//			descriptors[num_blocks-2].end_index--;

				test_result_msg.str("");
				if (test_a0_full) {
					test_result_msg << "Created blocks with A0 full on ";
				} else {
					test_result_msg << "Created blocks symmetrically ";
				}
				test_result_msg	<< "array size " << std::setw(3) << array_size
						  	  	<< " block size " << std::setw(3) << block_size
								<< " yields " << std::setw(3) << num_blocks
								<< std::endl
								<< arrayIndicesToString(array_size, mid) << std::endl
								<< SortingUtilities::arrayElementsToString(test_vector, array_size) << std::endl
								<< blockDescriptorsToString(descriptors, num_blocks) << std::endl;
				bool correct;
				if (test_a0_full) {
					correct = validateCreateBlocks_A0_Full(descriptors, num_blocks,
														   mid, block_size,
														   validation_msg);
				} else {
					correct = validateCreateBlocksSymmetrically(descriptors, num_blocks,
																mid, block_size,
																validation_msg);
				}
				if (verbose_output || !correct) {
					std::cout << test_result_msg.str();
				}
				if (!correct) {
					std::cout << " which is in ERROR for the following reason:" << std::endl;
					std::cout << validation_msg.str() << std::endl;
					test_result = false;
					goto TEST_CREATE_BLOCKS_RETURN_POINT;
				}
				if (verbose_output) {
					std::cout << std::endl;
				}
			}
		}
		if (test_a0_full) {
			test_a0_full = false;
		} else {
			test_a0_full = true;
		}
	} while (false && test_a0_full == false);

TEST_CREATE_BLOCKS_RETURN_POINT:
	return test_result;
}


/*	*******************************************************	*/
	/*	*******************************************************	*/
	/*						floorLog2							*/
	/*	*******************************************************	*/
	/*	*******************************************************	*/

bool testFloorLog2() {

	//	a way of calculating that uses a loop,
	//		which is slow due to branching
	auto calc_expected = [](array_size_t num) {
		if (num == 0)
			return 0;
		for (int i = 1; i <= 1 << 30; i *= 2) {
			if (i == num) {
				return i;
			}
			if (i > num) {
				return i >> 1;
			}
		}
		return 1 << 30;
	};

	constexpr int num_width = 10;
	bool passed = true;
	array_size_t value = highestPowerOf2(0);
	array_size_t expected = calc_expected(0);
	std::cout << "floor(log2(" << std::setw(num_width) << 0 << ")) = "
			<< std::setw(num_width) << value;
	if (value != expected) {
		std::cout << " does not match expected " << std::setw(num_width)
				<< expected << std::endl;
		passed = false;
		goto TEST_FLOOR_LOG_2_RETURN_LABEL;
	}
	std::cout << std::endl;

	for (array_size_t i = 2; i < (1 << 30); i <<= 1) {
		// one less than a power of 2
		value = highestPowerOf2(i - 1);
		expected = calc_expected(i - 1);
		std::cout << "floor(log2(" << std::setw(num_width) << i - 1 << ")) = "
				<< std::setw(num_width) << value;
		if (value != expected) {
			std::cout << " does not match expected " << std::setw(num_width)
					<< expected << std::endl;
			passed = false;
			goto TEST_FLOOR_LOG_2_RETURN_LABEL;
		}
		std::cout << std::endl;

		// a power of 2
		value = highestPowerOf2(i);
		expected = calc_expected(i);
		std::cout << "floor(log2(" << std::setw(num_width) << i << ")) = "
				<< std::setw(num_width) << value;
		if (value != expected) {
			std::cout << " does not match expected " << std::setw(num_width)
					<< expected << std::endl;
			passed = false;
			goto TEST_FLOOR_LOG_2_RETURN_LABEL;
		}
		std::cout << std::endl;
	}
	TEST_FLOOR_LOG_2_RETURN_LABEL: return passed;
}


/*	*******************************************************	*/
/*	*******************************************************	*/
/*			binary search through an array of tags			*/
/*	*******************************************************	*/
/*	*******************************************************	*/

bool testBlockSortBinarySearchFirstBlock() {
	return false;
}


bool testBlockSortBinarySearchLastBlock() {

	constexpr bool debug_verbose = false;
	constexpr bool announce_each_test_result = true;
	constexpr int element_width = ELEMENT_WIDTH;

	OStreamState ostream_state;	// restores state in its destructor
	std::stringstream msg;

	bool test_passed = true;

	using datatype = int;

	struct TestVector {
		datatype *array;
		size_t size_of_array;
	};

	/*
	 * 	Arrays of values that will be assigned as keys to each block
	 */
	datatype test_array_0_7[] 			= {  0,  1,  2,  3,  4,  5,  6,  7 };
	datatype test_array_1_15_odd[] 		= {  1,  3,  5,  7,  9, 11, 13, 15 };
	datatype test_array_2_16_even[] 	= {  2,  4,  6,  8, 10, 12, 14, 16 };
	datatype test_array_1_17_odd[]		= {  1,  3,  5,  7,  9, 11, 13, 15, 17 };
	datatype test_array_2_18_even[]		= {  2,  4,  6,  8, 10, 12, 14, 16, 18 };
	datatype test_array_even_repeated[]	= {  1,  1,  2,  2,  3,  3,  4, 4 };
	datatype test_array_odd_repeated[]	= {  1,  1,  1,  2,  2,  2,  3, 3, 3 };
	datatype test_array_1[]				= {  1  };
	datatype test_array_2[]				= {  1,  2  };

	TestVector test_vector_0_7 			= {test_array_0_7,
										   sizeof(test_array_0_7) / sizeof(datatype) };
	TestVector test_vector_1_15_odd 	= {test_array_1_15_odd,
										   sizeof(test_array_1_15_odd) / sizeof(datatype) };
	TestVector test_vector_2_16_even 	= {test_array_2_16_even,
										   sizeof(test_array_2_16_even) / sizeof(datatype) };
	TestVector test_vector_1_17_odd 	= {test_array_1_17_odd,
										   sizeof(test_array_1_17_odd) / sizeof(datatype) };
	TestVector test_vector_2_18_even 	= {test_array_2_18_even,
										   sizeof(test_array_2_18_even) / sizeof(datatype) };
	TestVector test_vector_even_repeated= {test_array_even_repeated,
										   sizeof(test_array_even_repeated) / sizeof(datatype) };
	TestVector test_vector_odd_repeated = {test_array_odd_repeated,
										   sizeof(test_array_odd_repeated) / sizeof(datatype) };
	TestVector test_vector_1 			= {test_array_1,
										   sizeof(test_array_1) / sizeof(datatype) };
	TestVector test_vector_2 			= {test_array_2,
										   sizeof(test_array_2) / sizeof(datatype) };

	TestVector *test_vectors [] {
		&test_vector_0_7,
		&test_vector_1_15_odd,
		&test_vector_2_16_even,
		&test_vector_1_17_odd,
		&test_vector_2_18_even,
		&test_vector_even_repeated,
		&test_vector_odd_repeated,
		&test_vector_1,
		&test_vector_2
	};

	int num_test_vectors = sizeof(test_vectors) / sizeof(TestVector*);

	int test_number = 0;

	for (int test_array_i = 0; test_array_i != num_test_vectors; test_array_i++)
	{
		/*	 Create an array of block descriptors without creating an
		 * underlying array. Force 'key' in each block to point to a new int 	*/
		index_t haystack_size = test_vectors[test_array_i]->size_of_array;
		auto haystack =
		std::unique_ptr<BlockDescriptor<int>[]>(new BlockDescriptor<int>[haystack_size]);

		for (int i = 0; i != haystack_size; i++) {
			haystack[i].type = BlockType::A_BLOCK;
			haystack[i].start_index = i * 4;
			haystack[i].end_index 	= haystack[i].start_index + 3;
			haystack[i].key = new datatype(test_vectors[test_array_i]->array[i]);
		}

		//	  Search each array for key values from the first block's key -1
		//	to the last block's key+1
		for (index_t needle = test_vectors[test_array_i]->array[0]-1;
					 needle <= test_vectors[test_array_i]->array[haystack_size-1]+1;
					 needle++) {
			msg.str("");
			test_number++;
			//	test input parameters
			index_t haystack_start = 0;
			index_t haystack_end = haystack_size-1;
			datatype *key = new datatype(needle);

			//	determine the expected output
			//	assume that *key is > all elements in the array
			index_t expected_answer = haystack_size;
			for (index_t i = 0; i != haystack_size; i++) {
				//	if an element is found that is > key, it is the answer
				if (*haystack[i].key > *key) {
					expected_answer = i;
					break;
				}
			}

			ComparesAndMoves result(0,0);
			index_t result_index = 0;
			result += binarySearchLastBlock(haystack, haystack_start, haystack_end,
										  	key, result_index);

			msg << std::setw(3) << test_number
				<< " binarySearchLastBlock() on array: " << std::endl;
			msg << "Blocks " << arrayIndicesToString(haystack_size, -1) << std::endl;
			msg << "Keys   ";
			for (int i = 0; i != haystack_size; i++) {
				msg << std::setw(element_width-1) << *haystack[i].key << " ";
			}
			msg << std::endl;
			msg << "returned " 	  << std::setw(2) << result_index
				<< " when passed "<< std::setw(2) << *key
				<< " which took " << std::setw(2) << result._compares
				<< " compares\n"
				<< std::endl;
			//	check the result of the function
			if (result_index != expected_answer) {
				msg.str("");
				msg << "\n!!! ERROR:"
					<< " returned " 	<< std::setw(2) << result_index
					<< " when passed "<< std::setw(2) << *key
					<< " expected " 	<< std::setw(2) << expected_answer
					<< " !!!\n";
				msg << "Blocks " << arrayIndicesToString(haystack_size, -1) << std::endl;
				msg << "Keys   ";
				for (int i = 0; i != haystack_size; i++) {
					msg << std::setw(element_width-1) << *haystack[i].key << " ";
				}
				msg << "\n\n";
				test_passed = false;
				std::cout << msg.str();
				goto TEST_BLOCK_SORT_BINARY_SEARCH_SEARCH_EXIT;
			}
			if (debug_verbose || announce_each_test_result) {
				std::cout << msg.str();
			}
		}
	}
TEST_BLOCK_SORT_BINARY_SEARCH_SEARCH_EXIT:
	if (true || debug_verbose) {
		std::cout << "After " << test_number << " tests "
				  << __FUNCTION__ << "() returns "
				  << (test_passed ? "passed\n" : "failed\n");
	}
	return test_passed;
}


/*	***********************************************************	*/
/*		Tests merging by presenting all possible permutations	*/
/* 			of a given sequence of length 'test_vector_size'	*/
/*	***********************************************************	*/

bool testBlockSortMergeBlocksExhaustively() {

	OStreamState ostream_state;

	bool debug_verbose = false;
	bool echo_result = true;
	std::stringstream test_message;
	bool test_passed = true;

	int test_vector_sizes[] = { 7, 8, 16 };
	int num_test_vectors_sizes = sizeof(test_vector_sizes) / sizeof(int);

	#pragma push_macro("DATA_TYPE")
	#define DATA_TYPE c
	using data_type = char;
//	#define DATA_TYPE i
//	using data_type = int;

	/*		lambdas		*/

	auto testVectorToString = [] (data_type** array, int num) -> std::string {
		std::stringstream result;
		result << '\'';
		for (int i = 0; i < num; i++) {
			result << *array[i];
		}
		result << '\'';
		return result.str();
	};

	bool (*isSorted)(data_type**, int) = [](data_type** array, int num) -> bool {
		for (int i = 0; i != num-1; i++) {
			if (*array[i+1] < *array[i])
				return false;
		}
		return true;
	};

	/*		test code	*/

	for (int test_vector_i = 0; test_vector_i < num_test_vectors_sizes; test_vector_i++) {
		int test_vector_size = test_vector_sizes[test_vector_i];

		//	build the array of test values
		char *test_values[test_vector_size];
		for (int i = 0; i != test_vector_size; i++) {
			#if (DATA_TYPE == c)
				test_values[i] = new char('A'+(i % 26));
			#elif (DATA_TYPE == i)
				test_values[i] = new int(i);
			#endif
		}

		int mid = test_vector_size/2;

		//	calculate the binomial coefficient which is (n choose k)
		int num_test_vectors = 1;
	    for (int i = 1; i <= mid; ++i) {
	    	int num_test_vectors_was = num_test_vectors;
	        num_test_vectors = num_test_vectors * (test_vector_size - i + 1) / i;
	        if (num_test_vectors_was > num_test_vectors) {
	        	std::cout << "Unable to generate " << num_test_vectors << " test vectors" << std::endl;
	        	test_passed = false;
	        	goto TEST_BLOCK_MERGE_EXHAUSTIVELY_RETURN;
	        }
	    }

	    char **test_vectors[num_test_vectors];

		if (!generateAllCombinationsOfValues(test_vectors, test_values, num_test_vectors, test_vector_size, mid)) {
			std::cout << "Unable to generate " << num_test_vectors << " test vectors" << std::endl;
			test_passed = false;
			goto TEST_BLOCK_MERGE_EXHAUSTIVELY_RETURN;
		}

		array_size_t left_start = 0;
		array_size_t left_end = mid - 1;
		array_size_t right_start = mid;
		array_size_t right_end = test_vector_size-1;

		ComparesAndMoves total_results(0,0);
		ComparesAndMoves least_moves	(100000000,100000000);
		ComparesAndMoves least_compares	(100000000,100000000);
		ComparesAndMoves most_moves		(        0,        0);
		ComparesAndMoves most_compares	(        0,        0);

		int num_tests_run = 0;

		for (int i = 0; i != num_test_vectors; i++) {
			test_message.clear();
			test_message.str("");
			test_message << std::setw(5) << num_tests_run << " ";
			InsertionSort::sortPointersToObjects(&test_vectors[i][left_start], mid);
			InsertionSort::sortPointersToObjects(&test_vectors[i][mid], right_end-right_start+1);

			num_tests_run++;
			test_message << " when divided into two subarrays, each sorted: "
						 << testVectorToString(test_vectors[i], test_vector_size);

			ComparesAndMoves result;
			switch(merge_strategy) {
			case MergeStrategy::TABLE:
				result = regressionTestOnly_mergeTwoBlocksByTable(test_vectors[i], left_start, left_end, right_start, right_end);
				break;
			case MergeStrategy::ROTATE:
				result = mergeContiguousBlocksByRotating(test_vectors[i], 0, test_vector_size / 2, test_vector_size - 1);
				break;
			case MergeStrategy::AUXILLIARY:
			default:
				break;
			}
			total_results += result;
			if (result._compares < least_compares._compares) {
				least_compares = result;
			}
			if (result._moves < least_moves._moves) {
				least_moves = result;
			}
			if (result._compares > most_compares._compares) {
				most_compares = result;
			}
			if (result._moves > most_moves._moves) {
				most_moves = result;
			}

			test_message << " merged using strategy " << toString(merge_strategy) << " to "
						 << testVectorToString(test_vectors[i], test_vector_size)
						 << " which took "
						 << result;
			if (isSorted(test_vectors[i], test_vector_size)) {
				test_message << " which is correct" << std::endl;
			} else {
				test_message << " which is in ERROR" << std::endl;
				std::cout << test_message.str();
				test_passed = false;
				goto TEST_BLOCK_MERGE_EXHAUSTIVELY_RETURN;
				break;
			}
			if (debug_verbose) {
				std::cout << test_message.str();
			}
		}
		if (echo_result) {
			std::cout << "Sorting all " << std::setw(3) << num_tests_run
					  << " unique arrays of size "
					  << test_vector_size << " where mid = " << mid << " using strategy "
					  << toString(merge_strategy)
					  << std::endl
					  << " took average of  "
					  << std::fixed
					  << std::setprecision(1)
					  << std::setw(8)
					  << static_cast<double>(total_results._compares) / num_tests_run
					  << " compares and "
					  << std::setw(4)
					  << static_cast<double>(total_results._moves) / num_tests_run
					  << " moves\n"
					  << " worst case moves " << std::setw(8) << most_moves._compares << " compares and "
					  << std::setw(4) << most_moves._moves << " moves\n"
					  << " worst case cmprs " << std::setw(8) << most_compares._compares << " compares and "
					  << std::setw(4) << most_compares._moves << " moves\n"
					  << "  best case moves " << std::setw(8) << least_moves._compares << " compares and "
					  << std::setw(4) << least_moves._moves << " moves\n"
					  << "  best case cmprs " << std::setw(8) << least_compares._compares << " compares and "
					  << std::setw(4) << least_moves._moves << " moves\n";
		}
	}

	TEST_BLOCK_MERGE_EXHAUSTIVELY_RETURN:
	return test_passed;

	#pragma pop_macro("DATA_TYPE")
}

bool testBlockSortMergeBlocksRandomly() {

	bool test_passed = true;
//	SimpleRandomizer randomizer;
	bool debug_verbose = false;
	bool echo_test_result = true;
	std::stringstream message;

	int element_width;

	auto _arrayToString = [&] (int **l_array, int l_size) -> std::string {
		std::stringstream result;
		result << "[";
		for (int i = 0; i != l_size; i++) {
			result << std::setw(element_width) << *l_array[i] << " ";
		}
		result << "]";
		return result.str();
	};

	ComparesAndMoves (*sortArray)(int**, int, int) = [] (int **l_array, int l_start, int l_end) {
		ComparesAndMoves result(0,0);
		for (int i = l_start+1; i <= l_end; i++) {
			for (int j = i; j != l_start; j--) {
				result._compares++;
				if (*l_array[j-1] > *l_array[j]) {
					int *tmp = l_array[j-1];
					l_array[j-1] = l_array[j];
					l_array[j] = tmp;
					result._moves += 3;
					result._compares++;
				} else {
					break;
				}
			}
		}
		return result;
	};

	int num_test_passes = 1000;
//	array_size_t array_sizes[] = { 8, 16, 31, 32, 33, 127, 128, 129, 16, 32, 64, 128};
	array_size_t array_sizes[] = { 512, 1024, 2048 };
	int num_array_sizes = sizeof(array_sizes) / sizeof(array_size_t);

	for (int array_size_i = 0; array_size_i != num_array_sizes; ++array_size_i) {
		array_size_t array_size = array_sizes[array_size_i];
		ComparesAndMoves total_result(0, 0);
		int num_tests = num_test_passes;
		array_size_t width = array_size;

		element_width = 1;
		while (width > 9) {
			width /= 10;
			element_width++;
		}

		int *test_array[array_size];
		int *reference_array[array_size];
		int *initial_array[array_size];

		for (int i = 0; i != array_size; i++) {
			reference_array[i] = new int(i);
		}

		sortArray(reference_array, 0, array_size-1);

		for (int test_number = 0; test_number != num_tests; test_number++) {

			ComparesAndMoves result(0, 0);
			//	create a linear array
			for (int i = 0; i != array_size; i++) {
				test_array[i] 		= reference_array[i];
			}

			randomizeArray(test_array, array_size);

			for (int i = 0; i != array_size; i++) {
				initial_array[i] = test_array[i];
			}
			//	sort each array, u & v, using an insertion sort
			index_t mid = array_size/2;
			index_t left_start = 0;
			index_t left_end = mid-1;
			index_t left_span = mid;
			index_t right_start = mid;
			index_t right_end = array_size-1;
			index_t right_span = array_size-mid;

			InsertionSort::sortPointersToObjects(test_array, left_span);
			InsertionSort::sortPointersToObjects(&test_array[mid], right_span);
			message.clear();
			message.str("");
			if (debug_verbose) {
				message << "  initial_array: "
						<< _arrayToString(initial_array, array_size);
			}

			switch(merge_strategy) {
			case MergeStrategy::ROTATE:
				result += mergeContiguousBlocksByRotating(test_array,
												left_start, mid, right_end);
				break;
			case MergeStrategy::TABLE:
				result += regressionTestOnly_mergeTwoBlocksByTable(test_array,
											 left_start, left_end,
											 right_start, right_end);
				break;
			case MergeStrategy::AUXILLIARY:
				std::cout << __FUNCTION__ << " using strategy "
						  << toString(merge_strategy)
						  << " which is not implemented\n";
				test_passed = false;
				goto TEST_BLOCK_SORT_MERGE_BLOCKS_RETURN_LABEL;
			}
			total_result += result;

			if (debug_verbose) {
				message << " after: "
						<< _arrayToString(test_array, array_size)
						<< " used: " << result
						<< "\n";
				std::cout << message.str();
			}

			for (int i = 0; i != array_size; i++) {
				if (*test_array[i] != *reference_array[i]) {
					test_passed = false;
					message.clear();
					message.str();
					message << "array size " << array_size << " test pass "
							<< test_number << std::endl
							<< " initial_array  "
							<< _arrayToString(initial_array, array_size)
							<< std::endl
							<< " result array   "
							<< _arrayToString(test_array, array_size)
							<< std::endl
							<< " expected array "
							<< _arrayToString(reference_array, array_size)
							<< std::endl;

					message << " FAILED: [" << std::setw(3) << i << "]"
							<< " expected_array " << initial_array[i] << " vs actual "
							<< *test_array[i] << std::endl;
					std::cout << message.str();
					goto TEST_BLOCK_SORT_MERGE_BLOCKS_RETURN_LABEL;
				}
			}
		}
		if (echo_test_result) {
			std::cout << "Merging blocks " << num_test_passes
					 << " times on an test_array of size "
					 << std::setw(4) << array_size
					 << " using stategy " << toString(merge_strategy)
					 << " took on average "
					 << std::fixed << std::setprecision(1) << std::setw(8)
					 << static_cast<double>(total_result._compares) / num_tests
					 << " compares and "
					 << std::fixed << std::setprecision(1) << std::setw(12)
					 << static_cast<double>(total_result._moves) / num_tests
					 << " moves" << std::endl;
		}
	}

	TEST_BLOCK_SORT_MERGE_BLOCKS_RETURN_LABEL: return test_passed;
}

bool testBlockSortModulo() {
	bool test_passed = true;

	for (array_size_t span = 7; span <= 9; span++) {
		array_size_t dividend = span+1;
		array_size_t expected = 1;
		for (; dividend >= -span-1; dividend--) {
			array_size_t calculated = blockSortModulo(dividend, span);
			std::cout << std::setw(2) << dividend << " % " << span << " = " << expected
					<< " vs " << calculated << " | ";
			if (expected != calculated) {
				std::cout << " ERROR ";
				test_passed = false;
				break;
			}
			if (--expected < 0) {
				expected = span-1;
			}
		}
		std::cout << std::endl;
		if (!test_passed)
			break;
	}
	return test_passed;
}

bool testBlockSortRotateArrayElements() {

	std::cout << __FUNCTION__ << std::endl;

	auto rotateArrayElementsLongWay =
		[](int** array,array_size_t start, array_size_t end, array_size_t count)
		{
			if (count == 0) {
				return;
			}
			if (count < 0) {
				// shift left
				for (array_size_t i = count; i != 0; i++) {
					int* element_zero = array[start];
					for (array_size_t j = start; j != end; j++) {
						array[j] = array[j + 1];
					}
					array[end] = element_zero;
				}
				return;
			}
			for (array_size_t i = 0; i != count; i++) {
				int* element_end = array[end];
				for (array_size_t j = end; j != 0; j--) {
					array[j] = array[j-1];
				}
				array[start] = element_end;
			}
		};

	bool test_passed = true;
	ComparesAndMoves rotate_result;

	array_size_t array_sizes[] = {	8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };
	int num_array_sizes = sizeof(array_sizes)/sizeof(array_size_t);

	array_size_t rotate_counts[] = { 2 };
	int num_rotate_counts = sizeof(rotate_counts)/sizeof(array_size_t);

	// test an array of even size and an array o]f odd size
	for (int array_size_i = 0; array_size_i != num_array_sizes; array_size_i++)
	{
		array_size_t array_size = array_sizes[array_size_i];
		int *reference_array[array_size];
		for (array_size_t i = 0; i < array_size; i++) {
			reference_array[i] = new int(static_cast<int>(i));
		}

		for (int rotate_count_i = 0; rotate_count_i != num_rotate_counts; rotate_count_i++)
		{
			array_size_t rotate_count = rotate_counts[rotate_count_i];
			// 	create an array to test rotation and an array of expecteds
			int *rotated_array[array_size];
			int *expected_array[array_size];
			for (array_size_t i = 0; i != array_size; i++) {
				rotated_array[i]  = reference_array[i];
				expected_array[i] = reference_array[i];
			}

			rotateArrayElementsLongWay(expected_array, 0, array_size-1, rotate_count);
			rotate_result = rotateArrayElementsRight<int>(rotated_array, 0, array_size-1, rotate_count);
			//	check results
			for (int i = 0 ; i < array_size; i++) {
				if (rotated_array[i] != expected_array[i]) {
					std::cout << "ERROR: expected[" << i << "] " << *expected_array[i]
							  << " vs actual[" << i << "] " << *rotated_array[i] << std::endl;
					test_passed = false;
					break;
				}
			}
			std::cout << (test_passed ? "PASSED " : "FAILED ");
			//	print out before / after
			std::cout << "array size " << std::setw(4) << array_size << " ";
			std::cout << "rotate(" << std::setw(3) << rotate_count << ") ";
			if (array_size < 32) {
				std::cout << "\"";
				for (int j = 0; j != array_size; j++) {
					std::cout << *reference_array[j];
				}
				std::cout << "\" yields \"";
				for (int j = 0; j != array_size; j++) {
					std::cout << *rotated_array[j];
				}
				std::cout << "\" vs expected \"";
				for (int j = 0; j != array_size; j++) {
					std::cout << *expected_array[j];
				}
				std::cout << "\"";
			}
			std::cout << " took:" << std::setw(7) << rotate_result._moves
					  << " moves\n";

			if (!test_passed) {
				break;
			}
		}
		if (!test_passed) {
			break;
		}
	}

	return test_passed;
}

bool testBlockSortRotateBlocks() {

	std::cout << __FUNCTION__ << std::endl;

	bool test_result = true;

	constexpr bool verbose = false;
	constexpr bool echo_test_name = true;

	/*	******************************	*/
	/*		helpful lambdas				*/
	/*	******************************	*/

	auto calculateNumberOfTags = [](array_size_t _size, array_size_t _v,
			int block_size) -> int {
		array_size_t left_span = _v;
		array_size_t right_span = _size - _v;
		int num_blocks = left_span / block_size
				+ (left_span % block_size ? 1 : 0);
		num_blocks += right_span / block_size
				+ (right_span % block_size ? 1 : 0);
		return num_blocks;
	};

	auto copyArray = [](int **_dst, int **_src, array_size_t _size) {
		for (int i = 0; i != _size; i++) {
			_dst[i] = _src[i];
		}
	};
	auto copyTags = [](std::unique_ptr<BlockDescriptor<int> []> &_dst,
					   std::unique_ptr<BlockDescriptor<int> []> &_src, int num_tags) {
		for (int i = 0; i != num_tags; i++) {
			_dst[i] = _src[i];
		}
	};
	auto compareResult = [](int **_test_array, int **_expected_array, int _array_size,
			std::unique_ptr<BlockDescriptor<int> []> &_test_tags,
			std::unique_ptr<BlockDescriptor<int> []> &_expected_tags, int _num_tags,
			std::stringstream &message) -> bool {

		for (int i = 0; i != _array_size; i++) {
			if (*_test_array[i] != *_expected_array[i]) {
				message << "ERROR: test value[" << i << "] '" << *_test_array[i]
						<< "' does not match expected value[" << i << "] '"
						<< _expected_array[i] << "'";
				return false;
			}
		}
		for (int i = 0; i != _num_tags; i++) {
			if (!_test_tags[i].isExactlyEqual(_expected_tags[i])) {
				message << "ERROR: test tag[" << i << "] '"
						<< _test_tags[i]
						<< "' does not match expected tag[" << i << "] '"
						<< _expected_tags[i] << "'";
				return false;
			}
		}
		return true;
	};

	/*	***************************************************	*/
	/*		lambda that rotates simply but inefficiently	*/
	/*	***************************************************	*/

	auto rotateExpectedBlocks = [](int **x_array,
			std::unique_ptr<BlockDescriptor<int> []> &x_tags, int x_array_size,
			int x_tag_rotate_count, int x_first_tag, int x_last_tag)
	{
		int array_start = x_tags[x_first_tag].start_index;
		int array_end	= x_tags[x_last_tag].end_index;

		for (; x_tag_rotate_count > 0; --x_tag_rotate_count) {
			// rotate the elements in the underlying array
			int tag_span = x_tags[x_last_tag].getWidth();
			for (int element_counter = tag_span; element_counter != 0; element_counter--) {
				// store the last element.  it will become the first element
				int *end_key = x_array[array_end];
				// move every element one position to the right
				for (int element_ptr = array_end; element_ptr > array_start; --element_ptr) {
					x_array[element_ptr] = x_array[element_ptr - 1];
				}
				x_array[array_start] = end_key;
			}
			// rotate each of the tags by one position
			// 	the start_index of [x_first_tag] will get written into
			//	the tag that is coming off of the right side of the array
			int first_start_index 	= x_tags[x_first_tag].start_index;
			// make a copy of the tag on the right side of the array
			//	  so that it can be put into 'first_tag' position in the array
			BlockDescriptor<int> tmp_block;
			tmp_block = x_tags[x_last_tag];
			for (int tag_ptr = x_last_tag; tag_ptr > x_first_tag; --tag_ptr) {
				// copy the tag that is to the left into her
				x_tags[tag_ptr] 			= x_tags[tag_ptr - 1];
				// add the amount that the underlying array was shifted
				//	to the indices of the tag when it was to the left
				x_tags[tag_ptr].start_index += tag_span;
				x_tags[tag_ptr].end_index 	+= tag_span;
			}
			x_tags[x_first_tag] 			= tmp_block;
			x_tags[x_first_tag].start_index = first_start_index;
			x_tags[x_first_tag].end_index 	= first_start_index + tag_span - 1;
		}
	};

	/* ***********************************************************************	*/
	/* 								test code									*/
	/* ***********************************************************************	*/

	int array_sizes[] = { //15, 16, 17, 31, 32, 33, 63, 64, 65, 127, 128, 129, 255, 256, 257,
						  16, 32, 64, 128, 256 };
	int num_array_sizes = sizeof(array_sizes)/ sizeof(int);

	int num_test_permutations = 0;

	int previous_array_size = 0;
	int previous_block_size = 0;

	for (int array_size_i = 0; array_size_i != num_array_sizes; array_size_i++)
	{
		int array_size = array_sizes[array_size_i];
		// divide the array up such that the size of right side of the array, 'v'
		//	will be equal or larger than the size of left side of the array, 'u'
		int v = array_size / 2;

		//	'v' also is the number of elements in the smaller 'u' half of the array
		//	The algorithm authors define the block size as <= sqrt(sizeof(u))
		int max_block_size = static_cast<int>(std::sqrt(v)+1);
		int min_block_size = max_block_size-2;

		for (int block_size = min_block_size; block_size <= max_block_size; block_size++)
		{
			int num_tags = calculateNumberOfTags(array_size, v, block_size);
			int *reference_array[array_size];

			for (int i = 0; i != array_size; i++) {
				reference_array[i] = new int(i);
			}

			//	The algorithm requires the two portions of the array, left & right,
			//	  are sorted although the total array may not be sorted
			//	  For this test, that invariant need not be true, however, it
			//	  makes reading dumps of the operation easier if it is true
			randomizeArray(reference_array, array_size);
			InsertionSort::sortPointersToObjects(reference_array, v);
			InsertionSort::sortPointersToObjects(&reference_array[v], array_size - v);

			std::unique_ptr<BlockDescriptor<int> []> reference_tags;
			num_tags = createBlockDescriptors_A0_Full(
					reference_array, 0, v, array_size - 1,
					block_size, reference_tags);

			//	anchor span [left:right] rotate_count
			//	0		5		[0:4]	  { 0, 1, 2, 3, 4, 5 }
			//	0		4		[0:3]	  { 0, 1, 2, 3, 4 }
			//	0       4       [1:4]	  { 0, 1, 2, 3, 4 }
			//	0       3		[0:2]	  { 0, 1, 2, 3 }
			//	0       3		[1:3]	  { 0, 1, 2, 3 }
			//	0		3		[2:4]	  { 0, 1, 2, 3 }
			//  0		2		[0:1]	  { 0, 1, 2 }
			//  0       2       [1:2]     { 0, 1, 2 }
			//  0       2       [2:3]     { 0, 1, 2 }
			//  0       2       [3:4]     { 0, 1, 2 }
			//  0       1       [0:0]     { 0, 1 }
			//  0       1       [1:1]     { 0, 1 }
			//  0       1       [2:2]     { 0, 1 }
			//  0       1       [3:3]     { 0, 1 }
			//  0       1       [4:4]     { 0, 1 }
			//	1		4		[1:4]	  { 0, 1, 2, 3, 4 }
			//  1       3       [1:3]     { 0, 1, 2, 3 }
			//  1       3       [2:4]     { 0, 1, 2, 3 }
			//				etc
			//  3       1       [3:4]     { 0, 1 }
			//  4       1		[4:4]	  { 0, 1 }

			for (int anchor_block = 0; anchor_block < num_tags; ++anchor_block)
			{
				for (array_size_t block_span = num_tags-anchor_block; block_span > 0; --block_span)
				{
					array_size_t first_block = anchor_block;
					array_size_t last_block  = anchor_block + block_span - 1;
					//	move right the group of blocks [left:right]that will be rotated
					for ( ; last_block < num_tags; ++first_block, ++last_block)
					{
						// rotate blocks [first:last] within [anchor:anchor+span] by { 0, 1, ... }
						for (array_size_t rotate_count = block_span; rotate_count >= 0; --rotate_count)
						{
							num_test_permutations++;
							ComparesAndMoves compares_and_moves(0,0);
							std::stringstream test_name_message;
							std::stringstream test_debug_message;

							int *test_array[array_size];
							int *expected_array[array_size];
							std::unique_ptr<BlockDescriptor<int> []> test_tags(
									new BlockDescriptor<int> [num_tags]);
							std::unique_ptr<BlockDescriptor<int> []> expected_tags(
									new BlockDescriptor<int> [num_tags]);

							copyArray(test_array, reference_array, array_size);
							copyTags(test_tags, reference_tags, num_tags);
							copyArray(expected_array, test_array, array_size);
							copyTags(expected_tags, test_tags, num_tags);
							rotateExpectedBlocks(expected_array, expected_tags,
												 array_size, rotate_count,
												 first_block, last_block);

							test_name_message << "on an array of size " << std::setw(4) << array_size
											  << " with " << std::setw(3) << num_tags << " blocks total "
											  << "rotating blocks [" << std::setw(3) << first_block
											  << ":" << std::setw(3) << last_block
											  << "] of maximum size " << std::setw(3) << block_size
											  << " by " << std::setw(2) << rotate_count;
							test_debug_message << blockSortToString(test_array, array_size,
																	v, test_tags, num_tags)
											   << std::endl;

							// perform the operation under test
							compares_and_moves +=
							rotateBlocksRight(test_array, test_tags,
										 	  first_block, last_block, rotate_count);

							test_debug_message << "result:\n"
											   << blockSortToString(test_array, array_size,
													   	   	   	    v, test_tags, num_tags)
											   << "\nexpected:\n"
											   << blockSortToString(expected_array, array_size,
													   	   	        v, expected_tags, num_tags);

							if (!compareResult(test_array, expected_array,
											   array_size, test_tags, expected_tags,
											   num_tags, test_debug_message))
							{
								std::cout << "*************** ERROR ***************" << std::endl;
								std::cout << "On test number " << num_test_permutations << std::endl;
								std::cout << test_name_message.str();
								std::cout << test_debug_message.str() <<  std::endl;
								std::cout << "*************** ERROR ***************" << std::endl;
								test_result = false;
								return test_result;
							}

							if (verbose) {
								std::cout << test_name_message.str() << std::endl;
								std::cout << test_debug_message.str() << std::endl << std::endl;
							} else {
								if (echo_test_name) {
									if ((false || (block_size != previous_block_size ||
												   array_size != previous_array_size)) &&
										(false || compares_and_moves._moves != 0)) {
										previous_array_size = array_size;
										previous_block_size = block_size;
										std::cout << "Test run "
												  << std::setw(9) << num_test_permutations
												  << " " << test_name_message.str()
												  << " took " << compares_and_moves._compares
												  << " compares and " << std::setw(4)
												  << compares_and_moves._moves << " moves\n";
									}
								}
							}
						}
					}
				}
			}
		}
	}
	std::cout << " All " << num_test_permutations << " tests passed\n";
	return test_result;
}

bool testBlockSortSortBlocks() {

	std::cout << __FUNCTION__ << "()" << std::endl;

	enum SortingStrategy { BINARY, HYBRID, RIGHT_TO_LEFT };
	std::string (*strategy_string)(SortingStrategy) =
			[] (SortingStrategy strategy) -> std::string {
		switch(strategy) {
		case BINARY:		return std::string("Binary       ");
		case HYBRID:		return std::string("Hybrid       ");
		case RIGHT_TO_LEFT: return std::string("Right to Left");
		default:			return std::string("?????????????");
		}
	};

	using BlockArrayPointer = std::unique_ptr<BlockSort::BlockDescriptor<int>[]>;

	constexpr bool echo_every_test_step = false;
	constexpr bool echo_every_test_result = false;
	constexpr bool echo_test_configuration_summary = true;

	constexpr const int object_width = 3;
	constexpr const int element_width = 4;
	constexpr const char separator = ' ';
	constexpr const int compares_precision = 1;
	constexpr const int moves_precision = 1;
	constexpr const int num_unique_values = 5;

	auto out = [object_width, element_width, separator]
					(int ** array, array_size_t array_size,
					 std::string trailer) {
		std::stringstream result;
		result << SortingUtilities::arrayElementsToString(array, array_size, object_width, element_width)
			   << trailer;
		return result.str();
	};

	bool test_passed = true;

	bool (*areBlocksSorted)(BlockArrayPointer &blocks, int num_blocks) =
		[] (BlockArrayPointer &_blocks, int _num_blocks) {
		for (int i = 1; i < _num_blocks; i++) {
			//	if the earlier block is > this block
			if (*_blocks[i-1].key > *_blocks[i].key)
				return false;
			//	if the two blocks are equal, they have to be in order:
			//		A_Block, A_Block
			//		A_Block, B_Block
			//		B_Block, B_Block
			//	to ensure stability
			//		B_Block, A_Block
			//	violates stability because it puts equivalent values
			///	  that were in the left side of the array (A_Blocks) to the right of
			//	  values that were in the right side of the array (B_Blocks)
			if (*_blocks[i-1].key  == *_blocks[i].key &&
			     _blocks[i-1].type == BlockType::B_BLOCK &&
				 _blocks[i].type   == BlockType::A_BLOCK) {
					return false;
				}
		}
		return true;
	};

	constexpr array_size_t array_sizes[] = {
			/* 31,  31,*/  32,  32,//  33,  33,
			/* 63,  63,*/  64,  64,//  65,  65,
			/*127, 127,*/ 128, 128,// 129, 128,
			/*255, 255,*/ 256, 256,// 257, 257,
			/*511, 511,*/ 512, 512,// 513, 513
			};
	constexpr int num_array_sizes = sizeof(array_sizes) / sizeof(array_size_t);

	//	OStreamState destructor will restore state of ostream
	OStreamState ostream_state;
	bool all_unique_elements = true;
	bool few_unique_elements = false;

	for (int array_size_i = 0; array_size_i < 1 + 0*num_array_sizes; ++array_size_i)
	{
		array_size_t array_size = array_sizes[array_size_i];
		int min_block_size = static_cast<array_size_t>(sqrt(array_size/2))-1;
		int max_block_size = min_block_size+2;
		int num_unique = num_unique_values;
		int* reference_array[array_size];
		int this_passes_unique_value_count;

		if (all_unique_elements && !few_unique_elements) {
			for (int i = 0; i != array_size; i++) {
				reference_array[i] = new int(i);
			}
			this_passes_unique_value_count = array_size;
		} else
		if (!all_unique_elements && few_unique_elements) {
			for (int i = 0, unique_counter = 0; i != array_size; i++) {
				reference_array[i] = new int(unique_counter);
				if (++unique_counter == num_unique) {
					unique_counter = 0;
				}
			}
			this_passes_unique_value_count = num_unique;
		} else {
			std::cout << "ERROR: conflicted array composition directives \n";
			test_passed = false;
			goto TEST_BLOCK_SORT_SORT_BLOCKS_RETURN_LABEL;
		}

		constexpr SortingStrategy sorting_strategies[] = {
				BINARY,
//				RIGHT_TO_LEFT,
//				HYBRID,
		};

		int num_sorting_strategies = sizeof(sorting_strategies) / sizeof(SortingStrategy);

		for (int block_size = min_block_size; block_size <= max_block_size; ++block_size)
		{
			int num_tests = 1;
			ComparesAndMoves total_results[num_sorting_strategies];

			// run the test with the same reference_array (input)
			//	using however many strategies desired
			for (int test_num = 0; test_num != num_tests; test_num++)
			{
				std::stringstream messages;
				randomizeArray(reference_array, array_size);
				randomizeArray(reference_array, array_size);
				randomizeArray(reference_array, array_size);


				for (int strategy_i = 0; strategy_i < 1 + 0*num_sorting_strategies; strategy_i++)
				{
					ComparesAndMoves result(0,0);
					int* array[array_size];
					for (int i = 0; i != array_size; i++) {
						array[i] = reference_array[i];
					}
					int start = 0;
					int mid = array_size / 2;
					int end = array_size - 1;
					int num_blocks = 0;
					BlockArrayPointer blocks;

					messages << arrayIndicesToString(array_size, object_width, element_width) << std::endl;
					messages << out(array, array_size, " generated\n");

					//	the randomizing algorithm leaves most of the larger elements in the
					//	  right had side of the array.  Swap the two halves so that the
					//	  larger elements are in the left side of the array
					InsertionSort::sortPointersToObjects(array, mid);
					InsertionSort::sortPointersToObjects(&array[mid], end-mid+1);

					messages << out(array, array_size, " after randomizing\n");
					num_blocks = BlockSort::regressionTestOnly_CreateBlockDescriptorsSymmetrically(array, start, mid, end, block_size, blocks);
					messages << arrayStartMiddleEndToString(array_size, start, mid, end, element_width);
					messages << BlockSort::blockDescriptorsToString(blocks, num_blocks, element_width)
							 << std::endl;
					messages << out(array, array_size, "\n");

					switch(sorting_strategies[strategy_i]) {
					case RIGHT_TO_LEFT:
						std::cout << "strategy RIGHT_TO_LEFT" << std::endl;
						result = BlockSort::regressionTestOnly_sortBlocksRightToLeft(array, array_size, blocks, num_blocks);
						break;
					case BINARY:
						std::cout << "strategy BINARY " << std::endl;
						result = BlockSort::regressionTestOnly_sortBlocksBinarySearch(array, array_size, blocks, num_blocks);
						std::cout << "stregaty BINARY DONE" << std::endl;
						break;
					case HYBRID:
						std::cout << "strategy HYBRID " << std::endl;
						result = BlockSort::regressionTestOnly_sortBlocksHybrid(array, array_size, blocks, num_blocks);
						break;
					default:
						break;
					}
					total_results[strategy_i] += result;

					messages << BlockSort::blockDescriptorsToString(blocks, num_blocks, element_width)
							 << std::endl;
					messages << out(array, array_size, " after sorting blocks\n");

					if (!areBlocksSorted(blocks, num_blocks)) {
						test_passed = false;
						std::cout << " !!!! FAILED !!!! test run " << test_num << std::endl;
						std::cout << messages.str() << std::endl;
						goto TEST_BLOCK_SORT_SORT_BLOCKS_RETURN_LABEL;
					}
					if (echo_every_test_step) {
						std::cout << messages.str();
					}
					if (echo_every_test_result) { // && (test_num % 1000 == 0)) {
						std::cout << strategy_string(sorting_strategies[strategy_i])
								  << " sorting an array of " << array_size << " elements "
								  << " with block size " << block_size
								  << " took "
								  << std::setw(8) << result._moves    << " moves"
								  << std::setw(5) << result._compares << " compares and\n";
					}
				}
			}
			if (echo_test_configuration_summary)
			{
				for (int strategy_i = 0; strategy_i < num_sorting_strategies; strategy_i++)
				{
					std::cout  << std::setw(6) << num_tests << " tests of "
							   << strategy_string(sorting_strategies[strategy_i])
							   << " sorting blocks of " << std::setw(4) << array_size
							   << " array with " << std::setw(3) << this_passes_unique_value_count
							   << " unique values and a block size of "
							   << std::setw(3) << block_size << " took on average "
							   << std::fixed << std::setprecision(moves_precision) << std::setw(10)
							   << static_cast<double>(total_results[strategy_i]._moves) / (num_tests)
							   << " moves and "
							   << std::fixed << std::setprecision(compares_precision) << std::setw(8)
							   << static_cast<double>(total_results[strategy_i]._compares) / (num_tests)
							   << " compares\n";
				}
			}
			if (all_unique_elements) {
				few_unique_elements = true;
				all_unique_elements = false;
			} else {
				few_unique_elements = false;
				all_unique_elements = true;
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
	TEST_BLOCK_SORT_SORT_BLOCKS_RETURN_LABEL:
	return test_passed;
}

bool testBlockSortSwapBlocks() {

	OStreamState ostream_state;	// restores ostream flags in destructor

	constexpr bool announce_each_result = true;
	constexpr bool debug_verbose = false;

	bool test_result = true;
	index_t array_sizes[] = { 32, 64 };

	using data_type = char;

	auto copy_array = [] (data_type **dst, data_type **src, index_t size) {

		for (int i = 0; i != size; i++) {
			dst[i] = new char(*src[i]);
		}
	};

	auto copy_descriptors = [] (data_type** expected_array,
			 	 	 	 	 	Descriptors<data_type> &dst,
								Descriptors<data_type> &src,
								int num_descriptors) {
		//	ensure that the expected descriptors point to the
		//	  expected arrray
		for (int i = 0; i != num_descriptors; i++) {
			dst[i] = src[i];
			dst[i].assignKey(expected_array);
		}
	};

	auto compare_arrays = [] (data_type **u, data_type **v, index_t size) -> bool {
		bool identical = true;
		for (int i = 0; i != size; i++) {
			if (*u[i] != *v[i]) {
				identical = false;
				break;
			}
		}
		return identical;
	};

	auto compare_descriptors = [] (Descriptors<data_type> &expected,
							  Descriptors<data_type> &result,
							  int num_descriptors) -> bool{
		for (int i = 0; i != num_descriptors; i++) {
			if (expected[i].type != result[i].type)	return false;
			if (*expected[i].key != *result[i].key) return false;
		}
		return true;
	};

	auto generate_expected_array = [] (data_type **expected,
								 	   Descriptors<data_type> &descriptors,
									   int u, int v) {

		index_t u_start = descriptors[u].start_index;
		index_t u_end	= descriptors[u].end_index;
		index_t v_start	= descriptors[u].start_index;
		index_t v_end	= descriptors[v].end_index;
		index_t u_size 	= u_end - u_start + 1;
		index_t v_size 	= v_end - v_start + 1;
		if (u_size == v_size) {
			BlockSort::swapBlockElements(expected, u_start, v_start, v_size);
		} else {
			BlockSort::rotateArrayElementsRight(expected, u_start, v_start, v_size);
		}
	};

	auto generate_expected_descriptors = [] (data_type **expected_array,
											 Descriptors<data_type> &expected,
											 int u, int v) {
		BlockType temp 	= expected[u].type;
		expected[u].type= expected[v].type;
		expected[v].type=temp;
		expected[u].assignKey(expected_array);
		expected[v].assignKey(expected_array);
	};

	auto make_test_vector_1 = [] (data_type **dst, index_t size) {
		char left_start = 'A';
		char right_start = 'A';
		for (int i = 0; i != size/2; i++) {
			dst[i] = new char(left_start + ((2*i)%26));
		}
		for (int i = size/2; i != size; i++) {
			dst[i] = new char(right_start + ((2*i+1)%26));
		}
	};

	int num_array_sizes = sizeof(array_sizes)/sizeof(index_t);

	for (int array_size_i = 0;
			 array_size_i != 1 + 0*num_array_sizes;
			 array_size_i++) {
		index_t array_size 	= array_sizes[array_size_i];
		index_t mid 		= array_size/2;
		index_t span_end 	= array_size-1;
		index_t block_size 	= static_cast<index_t>(std::sqrt(mid));

		data_type *test_vector[array_size];
		make_test_vector_1(test_vector, array_size);
		InsertionSort::sortPointersToObjects(test_vector, mid);
		InsertionSort::sortPointersToObjects(&test_vector[mid], array_size-mid);

		std::unique_ptr<BlockDescriptor<char>[]> test_descriptors;
		int num_descriptors = BlockSort::createBlockDescriptors_A0_Full(
				test_vector, 0, mid, span_end,
				block_size, test_descriptors);

		for (int i = 0; i < num_descriptors-1; i++) {
			for (int j = i+1; j < num_descriptors; j++) {
				std::stringstream msg;
				ComparesAndMoves metrics(0,0);
				data_type *array_under_test[array_size];
				std::unique_ptr<BlockDescriptor<char>[]> descriptors_under_test;
					std::unique_ptr<BlockDescriptor<char>[]>(new BlockDescriptor<char>[num_descriptors]);
				data_type *expected_array[array_size];
				std::unique_ptr<BlockDescriptor<char>[]> expected_descriptors;
					std::unique_ptr<BlockDescriptor<char>[]>(new BlockDescriptor<char>[num_descriptors]);
				copy_array(array_under_test, test_vector, array_size);
				copy_array(expected_array, test_vector, array_size);
				copy_descriptors(array_under_test,
								 descriptors_under_test, test_descriptors,
								 num_descriptors);
				copy_descriptors(expected_array,
								 expected_descriptors, test_descriptors,
								 num_descriptors);
#if 0
				//	first swap the underlying elmeents into their final positions
				generate_expected_array(expected_array, expected_descriptors, i, j);
				//	then update the .type and .key members
				generate_expected_descriptors(expected_array,
											  expected_descriptors, i, j);

				metrics = BlockSort::swapBlocks(array_under_test, descriptors_under_test, i, j);
#endif
				msg << BlockSort::blockSortToString(
								test_vector, array_size, mid,
								test_descriptors, num_descriptors)
					<< std::endl;
				msg << "Swapped " << i << " vs " << j
					<< " which took " << metrics << "and yielded \n";
				msg << BlockSort::blockSortToString(
								array_under_test, array_size, mid,
								descriptors_under_test, num_descriptors)
					<< std::endl;

				if (!compare_arrays(expected_array, array_under_test, array_size)) {
					msg << "ERROR: resultant does not match expected_array" << std::endl;
					test_result = false;

				}
				if (!compare_descriptors(expected_descriptors, descriptors_under_test, num_descriptors)) {
					msg << "ERROR: resultant descriptors do not match expected descriptors" << std::endl;
					test_result = false;
				}
				if (announce_each_result) {
					std::cout << msg.str() << std::endl;
				}
				if (true || !test_result) {
					if (!announce_each_result) {
						std::cout << msg.str() << std::endl;
					}
					std::cout << "Expected\n"
							  << BlockSort::blockSortToString(expected_array,
									  	  	  	  	  	  	  array_size, mid,
															  expected_descriptors,
															  num_descriptors)
							  << std::endl
							  << "\nversus result:\n"
							  << BlockSort::blockSortToString(array_under_test,
									  	  	  	  	  	  	  array_size, mid,
															  descriptors_under_test,
															  num_descriptors)
							  << std::endl;
					goto TEST_BLOCK_SORT_SWAP_BLOCKS_TEST_RETURN;
				}
			}
		}
	}
TEST_BLOCK_SORT_SWAP_BLOCKS_TEST_RETURN:
	return test_result;
}


bool testBlockSortSort() {
	bool passed = true;
	ComparesAndMoves sort_result;
	constexpr int num_tests = 1000;
	constexpr array_size_t array_size = 1024;
	array_size_t v = array_size/2;
	int *reference_array[array_size];

	for (int i = 0, val = 0; i < v; val += 2, i++) {
		reference_array[i] = new int(val);
	}
	for (int i = v, val = 1; i < array_size; val += 2, i++)  {
		reference_array[i] = new int(val);
	}

	int element_width = 5;
	int value_width = 4;

	for (int test_number = 0; test_number != num_tests; test_number++) {
		int *test_array[array_size];
		for (int i = 0; i != array_size; i++) {
			test_array[i] = reference_array[i];
		}
		std::stringstream msg;
		msg << "            " << arrayIndicesToString(array_size, v, element_width)
		    << std::endl;
		msg << "initially : " << SortingUtilities::arrayElementsToString(test_array,
																		 array_size,
																		 value_width,
																		 element_width)
			<< std::endl;
		SortingUtilities::randomizeArray(test_array, array_size);
		msg << "randomized: " << SortingUtilities::arrayElementsToString(test_array,
																		 array_size,
																		 value_width,
																		 element_width)
			<< std::endl;
		BlockSort::sortPointerstoObjects(test_array, array_size);
		msg << "sorted    : " << SortingUtilities::arrayElementsToString(test_array,
																		 array_size,
																		 value_width,
																		 element_width)
			<< std::endl;
		index_t mismatched_i;
		index_t mismatched_j;
		if (!SortingUtilities::isSorted(test_array, array_size,
									   mismatched_i, mismatched_j)) {
			passed = false;
			std::cout << msg.str() << std::endl;
			std::cout << "BlockSort failed ["
					  << mismatched_i << "] = "
					  << *reference_array[mismatched_i]
					  << " vs [" << mismatched_j << "] = "
					  << *reference_array[mismatched_j] << std::endl;
			goto TEST_BLOCK_SORT_SORT_RETURN_LABEL;
		}
	}
TEST_BLOCK_SORT_SORT_RETURN_LABEL:
	return passed;
}

/*	**********************************************************	*/
/*						testBlockSwap							*/
/*	**********************************************************	*/

//	for array_size = even_number, then odd_number
//	  for block_size = 2 to array_size / 2
//		for block distances -2, -1, 0, 1, 2
//		   for array positions [0:block_size-1],
//							   [block_size:2*block_size-1],
//							   [array_size-blocK_size+1:array_size-1]
//			  builds a test array
//			  if there is room in the array for the given gap
//	    		builds an expected array
//				calls block swap on the test array
//				verifies the result of the swap
//			  else
//				calls block swap and verifies that the return value is 0
//
//	'if there is room in the array':
//	  consider the case where the array is 20 elements long
//	    and the block size is 9.  To swap a gap of 2,
//		array[0:8] would swap with array[0+2*9=18:8+2*9=26]
//		which is outside the array and thus blockSwap should return
//		without any moves

bool testBlockSortSwapBlockElements() {

	constexpr bool verbose = true;
	constexpr int value_width = 3;
	constexpr int element_width = 4;

	constexpr int min_array_size = 20;
	constexpr int max_array_size = 21;
	constexpr int min_block_size = 2;
	constexpr int min_block_gap = -2;
	constexpr int max_block_gap =  2;

	struct BlockBoundary {
		int begin;
		int end;
		BlockBoundary() {
			begin = 0;
			end = 0;
		}
	};

	bool test_passed = true;
	int test_case = 0;
	for (int array_size = min_array_size; array_size <= max_array_size; array_size++) {
		int max_block_size = array_size / 2;
		for (int block_size = min_block_size; block_size <= max_block_size; block_size++) {
			for (int block_gap = min_block_gap; block_gap <= max_block_gap; block_gap++) {
				//	create a block on the left that will cause the block
				//	  on the right to end at the last element in tha array
				int last_block_start =
						(array_size-1) -					// right most end
						(abs(block_gap) * block_size) -		// left end
						block_size + 1;						// left begin_
				int beginnings[] = { 0, 1, last_block_start };
				int num_beginnings = sizeof(beginnings) / sizeof(int);
				for (int i = 0; i != num_beginnings; i++) {
					std::stringstream messages;
					//	delineate two blocks, left & right
					int left_block_begin = beginnings[i];
//					int left_block_end = left_block_begin + block_size-1;
					int right_block_begin =
							left_block_begin + std::abs(block_gap) * block_size;
//					int right_block_end =
//							right_block_begin + block_size-1;
					messages << "test case " << test_case
							 << " array_size " 	<< std::setw(2) << array_size
							 << ", block_size " << std::setw(2) << block_size
							 << ", block_gap " << std::setw(2) << block_gap
							 << ", block_beginning " << std::setw(2) << beginnings[i]
							 << " left_block [" << std::setw(2) << left_block_begin
							 << ":" << std::setw(2) << left_block_begin + block_size - 1
							 << "], right block [" << std::setw(2) << right_block_begin
							 << ":" << std::setw(2) << right_block_begin + block_size - 1 << "]";
					test_case++;
					//	if the bounds of the right block are outside the array
					if (right_block_begin + block_size >= array_size) {
						std::cout << messages.str() << " !!! right block end index > array_size-1" << std::endl;
						continue;
					}
					if (left_block_begin <0) {
						std::cout << messages.str() << " !!! left block start index < 0" << std::endl;
						continue;
					}
					messages << std::endl;
					// create the array to be tested
					char *array[array_size];
					char *expected[array_size];
					for (int i = 0; i != array_size; i++) {
						array[i] = new char('a' + (i % 26));
						expected[i] = new char('a' + (i %26));
					}
					for (int i = left_block_begin, j = right_block_begin;
							 i < left_block_begin + block_size; ++i, ++j) {
						char *tmp = expected[i];
						expected[i] = expected[j];
						expected[j] = tmp;
					}
					messages << " testing:  "
							 << SortingUtilities::arrayElementsToString(array, array_size, value_width, element_width)
							 << std::endl;
					if (true || block_gap >= 0) {
						//	if the block gap is not negative,
						//	   the blocks can be swapped
						BlockSort::swapBlockElements( array,
											  	  	  left_block_begin,
													  right_block_begin,
													  block_size);
					}
					messages << " expected: "
							 << SortingUtilities::arrayElementsToString(expected, array_size,
												      value_width, element_width)
							 << std::endl;
					messages << " received: "
							 << SortingUtilities::arrayElementsToString(array, array_size,
												      value_width, element_width)
							 << std::endl;
					for (int i = 0; i != array_size; i++) {
						if (*array[i] != *expected[i]) {
							messages << " ERROR: [" << std::setw(2) << i << "] "
									 << " expected " << *expected[i]
									 << " vs received " << *array[i]
									 << std::endl;
							test_passed = false;
							std::cout << messages.str();
							goto TEST_BLOCK_SORT_SWAP_BLOCKS_RETURN_LABEL;
						}
					}
					messages << std::endl;
					if (verbose)
						std::cout << messages.str();
				}	// while (start & end positions < array_size-1
			}	// for swap gaps -2, -1, 0, 1, 2
		}	// for block size
	}	// for array size
TEST_BLOCK_SORT_SWAP_BLOCKS_RETURN_LABEL:
	return test_passed;
}

bool testBlockSortSwapDescriptors() {
	bool test_passed = true;
	return test_passed;
}

/*
 * 	 Generates a series of test vectors that are split from [0:mid-1] [mid:size-1]
 * 	such that any given distince combination of 'mid' values appears only once
 * 	in the [0:mid-1].  This is useful for testing functions that sort each half
 * 	of the test vector before operating on the array
 * 		Consider the values { A, B, C, D, E, F } with mid = 3
 * 		The test patterns	{ A, C, E, B, D, F } and { E, C, A, F, D, B }
 * 		will be equivalent vectors after sorting [0:2] and [3:5]
 */

template <typename T>
bool generateAllCombinationsOfValues(T*** test_vectors,
									 T**  test_values,
									 int num_test_vectors,
									 int test_vector_size,
									 int mid) {

	constexpr bool debug_verbose = false;

	/*	******************************************	*/
	/*					lambdas						*/
	/*	******************************************	*/

	auto displayVector = [] (std::vector<T*>vector) {
		std::cout << "{ ";
		for (size_t i = 0; i != vector.size(); i++) {
			std::cout << *vector.at(i) << " ";
		}
		std::cout << "}";
	};

	auto displayArray = [] (T**array, int num_elements) {
		std::cout << "{ ";
		for (int i = 0; i != num_elements; i++) {
			std::cout << *array[i] << " ";
		}
		std::cout << "}";
	};

	//	append any allowed_values that are not already in test_vector
	auto finishBuildingTestVector = [] (T**test_vector,
										int test_vector_size, int dst_i,
										std::vector<T*> &values) {
		//	for all of the values in 'allowed'
		for (auto it : values) {
			int i = 0;
			bool found = false;
			//	see if the allowed value is already in test_vector
			for (; i != dst_i; i++) {
				if (*test_vector[i] == *it) {
					found = true;
					break;
				}
			}
			//	if the value is not in the test_vector, add it
			if (!found) {
				test_vector[dst_i] = it;
				dst_i++;
			}
			//	if we have completely filled the test vector, done
			if (dst_i == test_vector_size)
				break;
		}
	};

	/*	******************************************	*/
	/*			main code starts here				*/
	/*	******************************************	*/

	//	create a vector of test_values for generator
	std::vector<T*> allowed_values;
	for (int i = 0; i != test_vector_size; i++) {
		allowed_values.emplace_back(test_values[i]);
	}

	nChoosek generator(test_vector_size, mid, allowed_values);

	//	create the storage for the test vectors
	for (int i = 0; i != num_test_vectors; i++) {
		test_vectors[i] = new T*[test_vector_size];
	}

	//	create each test vector

	for (int vector_num = 0; vector_num != num_test_vectors; ++vector_num) {
		std::vector<T*>combo;
		generator.next(combo);
		if (debug_verbose) {
			std::cout << std::setw(4) << vector_num << ": ";
			displayVector(combo);
			std::cout << " generates ";
		}
		//	copy elements for left side of test vector
		nCk_index_t elem_num = 0;
		for ( ; elem_num != combo.size(); ++elem_num) {
			test_vectors[vector_num][elem_num] = combo.at(elem_num);
		}
		//	Append values not aready in the test vector to the test vector
		finishBuildingTestVector(test_vectors[vector_num], test_vector_size, elem_num, allowed_values);
		if (debug_verbose) {
			displayArray(test_vectors[vector_num], test_vector_size);
			std::cout << std::endl;
		}
	}
	return true;
}

template <typename T>
bool generateAllPermutationsOfValues(T** test_vectors[],
									 T **test_values,
									 int num_test_vectors,
									 int test_vector_size) {

	bool result = true;

	constexpr bool verbose_messages = false;

	auto testVectorToString = [] (char** array, int num) -> std::string {
		std::stringstream result;
		result << '"';
		for (int i = num-1; i >= 0; i--) {
			result << *array[i];
		}
		result << '"';
		return result.str();
	};

	auto rotateValuesRight = [] (char **array, int start, int end, int amount)
	{
		auto xchng = [] (char **array, int i, int j) {
			char *tmp = array[i]; 	array[i] = array[j]; 	array[j] = tmp;
		};

		for (int i = start, j = end; i < j; i++, j--)  				xchng(array, i, j);
		for (int i = start, j = start+amount-1; i < j; i++, j--)  	xchng(array, i, j);
		for (int i = start+amount, j = end; i < j; i++, j--) 		xchng(array, i, j);
	};

	auto validateTestVectors = [testVectorToString] (char *** &_vectors, int _num, int _size) -> bool {
		for (int i = 0; i < _num-1; i++) {
			for (int j = i+1; j < _num; j++) {
				bool different = false;
				for (int k = 0; k != _size; k++) {
					if (*_vectors[i][k] != *_vectors[j][k]) {
						different = true;
						break;
					}
				}
				if (!different) {
					std::cout << "ERROR: vector[" << i << "] "
							  << testVectorToString(_vectors[i], _size)
							  << " == vector[" << j << "] "
							  << testVectorToString(_vectors[j], _size)
							  << std::endl;
					return false;;
				}
			}
		}
		return true;
	};

	auto copyTestVector = [ ](char** &dst, char**src, int size) {
		for (int i = 0; i != size; i++) {
			dst[i] = src[i];
		}
	};

	auto rotateCounterToString = [] (int *array, int size) -> std::string {
		std::stringstream result;
		result << "<";
		for (int i = size-1; i > 0; i--) {
			result << std::setw(2) << array[i] << " :";
		}
		result <<  std::setw(2) << array[0] << " >";
		return result.str();
	};

	/*	**********************************************	*/
	/*				code starts here					*/
	/*	**********************************************	*/

	//	consider an test space of 6 possible values for a digit
	//	there will be 6! = 720 vectors
	//	6 5 4 3 2 1  indices
	//	A B C D E F  digits
	//  7 6 5 4 3 2  counters
	//	every time change digit 0
	//	every second time change digit 1

	int num_rotate_digits = test_vector_size-1;

	int rotate_counts[num_rotate_digits];
	int rotate_counters[num_rotate_digits];
	for (int i = 0; i < num_rotate_digits; i++) {
		rotate_counts[i] 	= i+1;
		rotate_counters[i] 	= i+1;
	}

	for (int i = 0; i != num_test_vectors; i++) {
		(test_vectors)[i] = new char*[test_vector_size];
	}

	int test_vector_number = 0;
	int active_digit = 0;
	while(test_vector_number < num_test_vectors) {
		if (verbose_messages) {
			std::cout << "test_vector[" << std::setw(3) << test_vector_number << "] = "
					  << testVectorToString(test_values, test_vector_size)
					  << " " << rotateCounterToString(rotate_counters, num_rotate_digits) << std::endl;
		}
		copyTestVector(test_vectors[test_vector_number++], test_values, test_vector_size);
		while(active_digit < num_rotate_digits) {
			rotateValuesRight(test_values, 0, active_digit+1, 1);
			if (--rotate_counters[active_digit] < 0) {
				// if the current digit's count is 0, all permutations have been stored
				rotate_counters[active_digit] = rotate_counts[active_digit];
				active_digit++;
			} else {
				// the current digit's count is not 0, so we do not need to rotate the next count
				break;
			}
		}
		active_digit = 0;
	}
	if (verbose_messages) {
		std::cout << "Validating that " << num_test_vectors << " are unique\n";
	}
	if (!validateTestVectors(test_vectors, num_test_vectors, test_vector_size)) {
		result = false;
	}
	if (verbose_messages) {
		std::cout << __FUNCTION__ << " returns " << result << std::endl;
	}
	return result;
}

template<typename T>
void randomizeArray(T **array, array_size_t size) {

	static SimpleRandomizer randomizer;

	for (array_size_t i = 0; i != size; i++) {
		array_size_t r = randomizer.rand(i, size);
		T *temp = array[i];
		array[i] = array[r];
		array[r] = temp;
	}
}

