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
#include <cstdint>

#include "SortingUtilities.h"
#include "SortingDebugOutput.h"
#include "IntegerArithmetic.h"
#include "nChoosek.h"

#include "BlockSort.h"

using namespace BlockSort;


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
#define TEST_BLOCK_SORT_MERGE_BLOCKS_EXHAUSTIVELY
//#define TEST_BLOCK_SORT_MERGE_BLOCKS_RANDOMLY
//#define TEST_BLOCK_SORT_ROTATE_ELEMENTS
//#define TEST_BLOCK_SORT_ROTATE_BLOCKS
//#define TEST_BLOCK_SORT_SWAP_BLOCKS
//#define TEST_BLOCK_SORT_SWAP_DESCRIPTORS
//#define TEST_BLOCK_SORT_SWAP_BLOCK_ELEMENTS
//#define TEST_BLOCK_SORT_SORT_BLOCKS
//#define TEST_BLOCK_SORT_SORT

bool testBlockSortBinarySearchFirstBlock();
bool testBlockSortBinarySearchLastBlock();
bool testBlockSortBinarySearchFirstElement();
bool testBlockSortBinarySearchLastElement();
bool testBlockSortCreateDescriptors();
bool testBlockSortMergeBlocksExhaustively();
bool testBlockSortMergeBlocksRandomly();
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
	std::cout << __FUNCTION__ << std::endl;

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
	runTest(all_tests_passed, testBlockSortMergeBlocksExhaustively, "function testBlockSortMergeBlocksExhaustively()");
	if (!all_tests_passed)
		return all_tests_passed;
	tests_passed++;
	std::cout << std::endl;
#endif

#ifdef TEST_BLOCK_SORT_MERGE_BLOCKS_RANDOMLY
	num_tests++;
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
		return all_tests_passed;
	tests_passed++;
#endif

#ifdef	TEST_BLOCK_SORT_BINARY_SEARCH_FIRST_ELEMENT
	num_tests++;
	runTest(all_tests_passed, testBlockSortBinarySearchFirstElement, "function testBlockSortBinaryFirst()");
	if (!all_tests_passed)
		return tests_passed;
	tests_passed++;
#endif
#ifdef	TEST_BLOCK_SORT_SWAP_DESCRIPTORS
	num_tests++;
	runTest(all_tests_passed, testBlockSortSwapDescriptors, "function testBlockSortSwapDescriptors()");
	if (!all_tests_passed)
		return tests_passed;
	tests_passed++;
#endif


#ifdef	TEST_BLOCK_SORT_BINARY_SEARCH_LAST_ELEMENT
	num_tests++;
	runTest(all_tests_passed, testBlockSortBinarySearchLastElement, "function testBlockSortBinaryLast()");
	if (!all_tests_passed)
		return tests_passed;
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
	SortMetrics dummy_metrics(0,0);
	for (int array_end = unique_values_size-2; array_end < unique_values_size; array_end++) {
		std::cout << "\nTesting binaryFirst() with an array with a "
				  << array_end << " unique elements\n";
		std::cout << toStringValueArrayIndices(0, array_end)
				  << std::endl;
		for (int i = minimum_unique_value-1; i <= array_end+1; i++) {
			std::cout << toStringValueArray(unique_values, 0, array_end, -1, 4);
			int *value = new int(i);
			index_t result =
				SortingUtilities::binarySearchFirstElement(
					unique_values, 0, array_end, value, dummy_metrics);
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
			index_t result =
				SortingUtilities::binarySearchFirstElement(
					triple_repeated_values, 0, array_end, value, dummy_metrics);
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
	SortMetrics metrics;

	for (int array_end = unique_values_size-2;
			  array_end < unique_values_size; array_end++) {
		std::cout << "\nTesting binaryLast() with an array with "
				  << array_end << " unique elements\n";
		std::cout << toStringValueArrayIndices(0, array_end)
				  << std::endl;
		for (int i = minimum_unique_value-1; i <= array_end+1; i++) {
			std::cout << toStringValueArray(unique_values, 0, array_end, -1, 4);
			int *value = new int(i);
			index_t result =
				SortingUtilities::binarySearchLastElement(
					unique_values, 0, array_end, value, metrics);
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
			index_t result =
				SortingUtilities::binarySearchLastElement (
					triple_repeated_values, 0, array_end, value, metrics);
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
		index_t array_size,
		index_t mid,
		int block_size,
		std::stringstream &msg)
{
	bool test_result = true;

	int block_i = 0;

	index_t expected_last_block_size = array_size % block_size;
	int expected_total_num_blocks = (array_size / block_size) +
									(expected_last_block_size ? 1 : 0);

	//	if there is no fractional final block, the final blockis full size
	if (expected_last_block_size == 0) {
		expected_last_block_size = block_size;
	}
	int expected_num_a_blocks = mid / block_size;
	int expected_num_b_blocks = expected_total_num_blocks - expected_num_a_blocks;

	if (mid % block_size) {
		test_result = false;
		msg << __FUNCTION__ << " returning false because there are not an "
			<< " integer number of blocks of size " << block_size
			<< " from start = 0 to mid = " << mid;
		goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
	}

	if (num_descriptors != expected_total_num_blocks) {
		test_result = false;
		msg << __FUNCTION__ << " returning false because"
			<< " expected num_blocks " << expected_total_num_blocks
			<< " does not match actual " << num_descriptors;
		goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
	}

	//	if there any A_Blocks
	for (; block_i != expected_num_a_blocks; block_i++) {
		int expected_block_size = block_size;
		if (descriptors[block_i].type != BlockType::A_BLOCK) {
			test_result = false;
			msg << __FUNCTION__
				<< " returns false because Block["
				<< block_i << "] is not an A_Block";
				goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
		}
		if (descriptors[block_i].getWidth() != expected_block_size) {
			test_result = false;
			msg << __FUNCTION__
				<< " returns false because block["
				<< block_i << "] which is an A_Block is wrong size: "
				<< "expected " << expected_block_size
				<< " vs actual " << descriptors[0].getWidth();
				goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
		}
	}

	if (block_i != expected_num_a_blocks) {
		test_result = false;
		msg << __FUNCTION__
			<< " returns false because expected number of A_Blocks "
			<< expected_num_a_blocks
			<< " does not match "
			<< block_i;
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
	}

	if (descriptors[block_i-1].end_index != mid-1) {
		test_result = false;
		msg << __FUNCTION__
			<< " returns false because end index of final A_Block "
			<< descriptors[block_i-1].end_index
			<< " does not match (mid-1) "
			<< mid-1;
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
	}

	if (descriptors[block_i].start_index != mid) {
		test_result = false;
		msg << __FUNCTION__
			<< " returns false because start index of first B_Block "
			<< descriptors[block_i].start_index
			<< " does not match mid "
			<< mid;
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
	}

	//	if there any B_Blocks
	for (; block_i != expected_total_num_blocks; block_i++) {
		int expected_block_size;
		// last B_Block ? or expected_last_block is a full block
		if (block_i != expected_total_num_blocks-1 || expected_last_block_size == 0) {
			expected_block_size = block_size;
		} else {
			expected_block_size = expected_last_block_size;
		}
		if (descriptors[block_i].type != BlockType::B_BLOCK) {
			test_result = false;
			msg << __FUNCTION__
				<< " returns false because block["
				<< block_i << "] is not a B_Block";
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
		}
		if (descriptors[block_i].getWidth() != expected_block_size) {
			test_result = false;
			msg << __FUNCTION__
				<< " returns false because block["
				<< block_i << "] which is a B_Block is wrong size: "
				<< "expected " << expected_block_size
				<< " vs uut " << descriptors[0].getWidth();
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
		}
	}

	if (block_i != expected_total_num_blocks) {
		test_result = false;
		msg << __FUNCTION__
			<< " returns false because expected number of B_Blocks "
			<< expected_num_b_blocks
			<< " does not match "
			<< block_i - expected_num_a_blocks;
		goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT;
	}

VALIDATE_CREATE_BLOCK_DESCRIPTORS_A0_FULL_RETURN_POINT:
	return test_result;
}

template <typename T>
bool validateCreateBlocksSymmetrically(
		std::unique_ptr<BlockDescriptor<T>[]> &descriptors,
		int num_descriptors,
		index_t array_size,
		index_t mid,
		int block_size,
		std::stringstream &msg)
{
	bool test_result = true;

	index_t left_span 					= mid;
	index_t right_span 					= array_size - mid;
	index_t expected_first_block_size	= left_span% block_size;
	index_t expected_last_block_size	= right_span % block_size;
	int	expected_num_a_blocks = left_span / block_size + (expected_first_block_size ? 1 : 0);
	int expected_num_b_blocks = right_span / block_size + (expected_last_block_size ? 1 : 0);
	int expected_total_num_blocks = expected_num_a_blocks + expected_num_b_blocks;
	//	These assignments have to come after these variables were used to
	//	indicated to add one to the number of blocks for fractional block size
	if (expected_first_block_size == 0) {
		//	if the first block is not a fractional block, it is full size
		expected_first_block_size = block_size;
	}
	if (expected_last_block_size == 0) {
		//	if the last block is not a fractional block, it is full size
		expected_last_block_size = block_size;
	}
	int block_i = 0;

	if (num_descriptors != expected_total_num_blocks) {
		test_result = false;
		msg << __FUNCTION__ << " returning false because"
			<< " expected num_blocks " << expected_total_num_blocks
			<< " does not match actual " << num_descriptors;
		goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT;
	}

	//	if there any A_Blocks
	for (; block_i != expected_num_a_blocks; block_i++) {
		int expected_block_size;
		// first A_Block or expected first block size is full block
		if (block_i != 0) {
			expected_block_size = block_size;
		} else {
			expected_block_size = expected_first_block_size;
		}
		if (descriptors[block_i].type != BlockType::A_BLOCK) {
			test_result = false;
			msg << __FUNCTION__
				<< " returns false because Block["
				<< block_i << "] is not an A_Block";
				goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT;
		}
		if (descriptors[block_i].getWidth() != expected_block_size) {
			test_result = false;
			msg << __FUNCTION__
				<< " returns false because block["
				<< block_i << "] which is an A_Block is wrong size: "
				<< "expected " << expected_block_size
				<< " vs actual " << descriptors[0].getWidth();
				goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT;
		}
	}

	if (block_i != expected_num_a_blocks) {
		test_result = false;
		msg << __FUNCTION__
			<< " returns false because expected number of A_Blocks "
			<< expected_num_a_blocks
			<< " does not match "
			<< block_i;
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT;
	}

	if (descriptors[block_i-1].end_index != mid-1) {
		test_result = false;
		msg << __FUNCTION__
			<< " returns false because end index of final A_Block "
			<< descriptors[block_i-1].end_index
			<< " does not match (mid-1) "
			<< mid-1;
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT;
	}

	if (descriptors[block_i].start_index != mid) {
		test_result = false;
		msg << __FUNCTION__
			<< " returns false because start index of first B_Block "
			<< descriptors[block_i].start_index
			<< " does not match mid "
			<< mid;
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT;
	}

	//	if there any B_Blocks
	for (; block_i != expected_total_num_blocks; block_i++) {
		int expected_block_size;
		// last B_Block ? or expected_last_block is a full block
		if (block_i != expected_total_num_blocks-1 || expected_last_block_size == 0) {
			expected_block_size = block_size;
		} else {
			expected_block_size = expected_last_block_size;
		}
		if (descriptors[block_i].type != BlockType::B_BLOCK) {
			test_result = false;
			msg << __FUNCTION__
				<< " returns false because block["
				<< block_i << "] is not a B_Block";
				goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT;
		}
		if (descriptors[block_i].getWidth() != expected_block_size) {
			test_result = false;
			msg << __FUNCTION__
				<< " returns false because block["
				<< block_i << "] which is a B_Block is wrong size: "
				<< "expected " << expected_block_size
				<< " vs uut " << descriptors[0].getWidth();
				goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT;
		}
	}

	if (block_i != expected_total_num_blocks) {
		test_result = false;
		msg << __FUNCTION__
			<< " returns false because expected number of B_Blocks "
			<< expected_num_b_blocks
			<< " does not match "
			<< block_i - expected_num_a_blocks;
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT;
	}

	if (descriptors[block_i-1].end_index != array_size-1) {
		test_result = false;
		msg << __FUNCTION__
			<< " returns false because end index of final B_Block "
			<< descriptors[block_i-1].end_index
			<< " does not match (array_size-1) "
			<< mid-1;
			goto VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT;
	}

VALIDATE_CREATE_BLOCK_DESCRIPTORS_SYMMETRICALLY_RETURN_POINT:
	return test_result;
}

bool testBlockSortCreateDescriptors() {

	constexpr bool verbose_output 	= false;
	constexpr int array_size_width 	= 3;
	constexpr int block_size_width 	= 3;
	constexpr int num_blocks_width 	= 3;
	std::stringstream test_result_msg;
	std::stringstream validation_msg;

	bool test_result = true;

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


	int minimum_block_size = 2;
	int maximum_block_size = 3;
	int nominal_array_size = 16;

	BlockOrganizations block_organizations[] = {
		BlockOrganizations::FULL_A0_BLOCK,
		BlockOrganizations::SYMMETRIC
	};

	int num_block_organizations = sizeof(block_organizations) / sizeof(BlockOrganizations);

	//	perform the test both on createBlockDescriptors_A0_Full()
	//	  					 and createBlocKDescriptorsSymmetrically()
	for (int block_organization_i = 0;
			 block_organization_i != num_block_organizations;
			 block_organization_i++)
	{
		BlockOrganizations block_organization = block_organizations[block_organization_i];

		for (int block_size  = minimum_block_size;
				 block_size <= maximum_block_size; block_size++) {

			//	to ensure in a symmetric array of blocks that the all
			//	possible sizes of the first and last block are tested
			int num_blocks_possible = nominal_array_size / block_size;
			int num_complete_blocks = num_blocks_possible;
			int minimum_array_size = num_complete_blocks * block_size;
			//	guarantee that all possible combinations of symmetric descriptors
			//	that have partial B_Blocks, and possibly A_Bloks, are tested
			int maximum_array_size = minimum_array_size+2*block_size-1;
			for (int array_size = minimum_array_size;
				 array_size <= maximum_array_size; array_size++) {
				// calculate how many full blocks there will be
				int minimum_num_blocks = array_size / block_size;
				// the mid, which is where the first B_Block starts
				//	is the the left of all the A_Blocks
				index_t start = 0;
				index_t end = array_size-1;
				std::unique_ptr<BlockDescriptor<char>[]> descriptors;
				index_t mid = array_size/2;
				int num_blocks = 0;

				switch (block_organization) {
				case BlockOrganizations::FULL_A0_BLOCK:
					mid = (minimum_num_blocks / 2) * block_size;
					num_blocks = createBlockDescriptors_A0_Full(test_vector,
																start, mid, end,
																block_size,
																descriptors);
					break;
				case BlockOrganizations::SYMMETRIC:
					mid = array_size / 2;
					num_blocks = createBlockDescriptorsSymmetrically(test_vector,
																	 start, mid, end,
																	 block_size,
																	 descriptors);
					break;
				default:
					std::cout << __FUNCTION__ << " unrecognized block organization" << std::endl;
					test_result = false;
					goto TEST_CREATE_BLOCKS_RETURN_POINT;
					break;
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
				test_result_msg << "Created blocks with "
								<< std::setw(BLOCK_ORGANIZATION_MAX_STRING_LENGTH)
								<< std::left << std::to_string(block_organization)
								<< " on an array size "
								<< std::setw(array_size_width) << array_size
						  	  	<< " block size "
								<< std::setw(block_size_width) << block_size
								<< " yields "
								<< std::setw(num_blocks_width) << num_blocks
								<< " blocks\n"
								<< arrayIndicesToString(array_size, mid) << std::endl
								<< SortingUtilities::arrayElementsToString(test_vector, array_size) << std::endl
								<< blockDescriptorsToString(descriptors, num_blocks) << std::endl;

				bool correct = false;
				switch (block_organization) {
				case BlockOrganizations::FULL_A0_BLOCK:
					correct = validateCreateBlocks_A0_Full(descriptors, num_blocks,
														   array_size, mid, block_size,
														   validation_msg);
					break;
				case BlockOrganizations::SYMMETRIC:
					correct = validateCreateBlocksSymmetrically(descriptors, num_blocks,
																array_size, mid, block_size,
																validation_msg);
					break;
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
	}

TEST_CREATE_BLOCKS_RETURN_POINT:
	return test_result;
}


	/*	*******************************************************	*/
	/*	*******************************************************	*/
	/*						floorLog2							*/
	/*	*******************************************************	*/
	/*	*******************************************************	*/

bool testFloorLog2() {

	constexpr bool announce_each_test_result = false;
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
	//	do 0 as a separate case
	array_size_t value = highestPowerOf2(0);
	array_size_t expected = calc_expected(0);
	if (announce_each_test_result) {
		std::cout << "floor(log2(" << std::setw(num_width) << 0 << ")) = "
				<< std::setw(num_width) << value;
	}
	if (value != expected) {
		std::cout << " does not match expected " << std::setw(num_width)
				<< expected << std::endl;
		passed = false;
		goto TEST_FLOOR_LOG_2_RETURN_LABEL;
	}
	if (announce_each_test_result) {
		std::cout << std::endl;
	}

	for (array_size_t i = 2; i < (1 << 30); i <<= 1) {
		// one less than a power of 2
		value = highestPowerOf2(i - 1);
		expected = calc_expected(i - 1);
		if (announce_each_test_result) {
			std::cout << "floor(log2(" << std::setw(num_width) << i - 1 << ")) = "
					<< std::setw(num_width) << value;
		}
		if (value != expected) {
			std::cout << " does not match expected " << std::setw(num_width)
					<< expected << std::endl;
			passed = false;
			goto TEST_FLOOR_LOG_2_RETURN_LABEL;
		}
		if (announce_each_test_result) {
			std::cout << std::endl;
		}

		// a power of 2
		value = highestPowerOf2(i);
		expected = calc_expected(i);
		if (announce_each_test_result) {
			std::cout << "floor(log2(" << std::setw(num_width) << i << ")) = "
					<< std::setw(num_width) << value;
		}
		if (value != expected) {
			std::cout << " does not match expected " << std::setw(num_width)
					<< expected << std::endl;
			passed = false;
			goto TEST_FLOOR_LOG_2_RETURN_LABEL;
		}
		if (announce_each_test_result) {
			std::cout << std::endl;
		}
	}
TEST_FLOOR_LOG_2_RETURN_LABEL:
	return passed;
}


/*	*******************************************************	*/
/*	*******************************************************	*/
/*			binary search through an array of tags			*/
/*	*******************************************************	*/
/*	*******************************************************	*/

bool testBlockSortBinarySearchFirstBlock() {
	std::cout << __FUNCTION__ << " not implemented\n";
	return false;
}


bool testBlockSortBinarySearchLastBlock() {

	constexpr bool debug_verbose 				= false;
	constexpr bool announce_each_test_result 	= false;
	constexpr bool announce_total_test_result	= false;
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

			SortMetrics result(0,0);
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
				<< " which took " << std::setw(2) << result.compares
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
	if (debug_verbose || announce_total_test_result) {
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

	std::cout << __FUNCTION__ << std::endl;

	bool debug_verbose 	= false;
	bool echo_result 	= true;
	bool test_passed 	= true;

//	int test_vector_sizes[] = { 16, 17, 18, 19, 20, 21, 22 };
	int test_vector_sizes[] = { 16 };
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

		if (debug_verbose) {
			std::cout << "Test Vector Size = " << test_vector_size << std::endl;
		}
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
	    if (debug_verbose) {
	    	std::cout << "Generating " << num_test_vectors << " vectors ... " << std::endl;
	    }
		if (!generateAllCombinationsOfValues(test_vectors, test_values, num_test_vectors, test_vector_size, mid)) {
			std::cout << "Unable to generate " << num_test_vectors << " test vectors" << std::endl;
			test_passed = false;
			goto TEST_BLOCK_MERGE_EXHAUSTIVELY_RETURN;
		}
		if (debug_verbose) {
			std::cout << " generated " << num_test_vectors << " vectors" << std::endl;
		}

		array_size_t left_start = 0;
		array_size_t left_end = mid - 1;
		array_size_t right_start = mid;
		array_size_t right_end = test_vector_size-1;

		SortMetrics total_results(0,0);
		SortMetrics least_moves		(100000000,100000000);
		SortMetrics least_compares	(100000000,100000000);
		SortMetrics most_moves		(        0,        0);
		SortMetrics most_compares	(        0,        0);

		int num_tests_run = 0;

		MergeStrategy merge_strategies[] =
		{
//				MergeStrategy::AUXILLIARY,
				MergeStrategy::ROTATE,
				MergeStrategy::TABLE
		};

		int num_merge_strategies = sizeof(merge_strategies)/sizeof(MergeStrategy);

		for (int merge_strategy_i = 0;
			     merge_strategy_i != num_merge_strategies; merge_strategy_i++) {

			MergeStrategy merge_strategy = merge_strategies[merge_strategy_i];
			for (int i = 0; i != num_test_vectors; i++) {
				std::stringstream test_message;
				test_message.clear();
				test_message.str("");
				test_message << std::setw(5) << num_tests_run << " ";
				InsertionSort::sortPointersToObjects(&test_vectors[i][left_start], mid);
				InsertionSort::sortPointersToObjects(&test_vectors[i][mid], right_end-right_start+1);

				num_tests_run++;
				test_message << " when divided into two subarrays, each sorted: "
							 << testVectorToString(test_vectors[i], test_vector_size);

				SortMetrics result;
				switch(merge_strategy) {
				case MergeStrategy::TABLE:
					result =
						SortingUtilities::mergeTwoBlocksElementsByTable(
													 test_vectors[i],
													 left_start, left_end,
													 right_start, right_end);
					break;
				case MergeStrategy::ROTATE:
					result =
						SortingUtilities::mergeTwoAdjacentBlocksByRotation(
													 test_vectors[i],
													 left_start, left_end,
													 right_start, right_end);
					break;
				case MergeStrategy::AUXILLIARY:
				default:
					break;
				}
				total_results += result;
				if (result.compares < least_compares.compares) {
					least_compares = result;
				}
				if (result.assignments < least_moves.assignments) {
					least_moves = result;
				}
				if (result.compares > most_compares.compares) {
					most_compares = result;
				}
				if (result.assignments > most_moves.assignments) {
					most_moves = result;
				}

				test_message << " merged using strategy "
							 << std::to_string(merge_strategy) << " to "
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
				std::cout << "Merging all " << std::setw(3) << num_test_vectors
						  << " pairs of unique arrays of size "
						  << test_vector_size << " where mid = " << mid << " using strategy "
						  << std::to_string(merge_strategy)
						  << std::endl
						  << " took average of  "
						  << std::fixed
						  << std::setprecision(1)
						  << std::setw(8)
						  << static_cast<double>(total_results.compares) / num_tests_run
						  << " compares and "
						  << std::setw(4)
						  << static_cast<double>(total_results.assignments) / num_tests_run
						  << " moves\n"
						  << " worst case moves " << std::setw(8) << most_moves.compares << " compares and "
						  << std::setw(4) << most_moves.assignments << " moves\n"
						  << " worst case cmprs " << std::setw(8) << most_compares.compares << " compares and "
						  << std::setw(4) << most_compares.assignments << " moves\n"
						  << "  best case moves " << std::setw(8) << least_moves.compares << " compares and "
						  << std::setw(4) << least_moves.assignments << " moves\n"
						  << "  best case cmprs " << std::setw(8) << least_compares.compares << " compares and "
						  << std::setw(4) << least_moves.assignments << " moves\n";
			}
		}
	}

	TEST_BLOCK_MERGE_EXHAUSTIVELY_RETURN:
	return test_passed;

	#pragma pop_macro("DATA_TYPE")
}

bool testBlockSortMergeBlocksRandomly() {

	std::cout << __FUNCTION__ << std::endl;

	bool test_passed = true;
//	SimpleRandomizer randomizer;
	bool debug_verbose 		= false;
	bool echo_test_result 	= true;
	std::stringstream message;

	using data_type = int;

	int element_width;

	auto _arrayToString = [&] (data_type **l_array, int l_size) -> std::string {
		std::stringstream result;
		result << "[";
		for (int i = 0; i != l_size; i++) {
			result << std::setw(element_width) << *l_array[i] << " ";
		}
		result << "]";
		return result.str();
	};

	SortMetrics (*sortArray)(data_type**, int, int) = [] (data_type **l_array, int l_start, int l_end) {
		SortMetrics result(0,0);
		for (int i = l_start+1; i <= l_end; i++) {
			for (int j = i; j != l_start; j--) {
				result.compares++;
				if (*l_array[j-1] > *l_array[j]) {
					data_type *tmp = l_array[j-1];
					l_array[j-1] = l_array[j];
					l_array[j] = tmp;
					result.assignments += 3;
					result.compares++;
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

		array_size_t width = array_size;

		element_width = 1;
		while (width > 9) {
			width /= 10;
			element_width++;
		}

		data_type *test_array[array_size];
		data_type *reference_array[array_size];
		data_type *initial_array[array_size];

		for (int i = 0; i != array_size; i++) {
			reference_array[i] = new int(i);
		}

		sortArray(reference_array, 0, array_size-1);

		MergeStrategy merge_strategies[] = {
//				MergeStrategy::AUXILLIARY,
				MergeStrategy::ROTATE,
				MergeStrategy::TABLE
		};

		int num_merge_strategies = sizeof(merge_strategies) / sizeof(MergeStrategy);

		for (int merge_strategy_num = 0;
				 merge_strategy_num != num_merge_strategies;
				 merge_strategy_num++)
		{
			//  create a randomizer which will always be intialized to the
			//	same default key
			SimpleRandomizer randomizer;

			MergeStrategy merge_strategy = merge_strategies[merge_strategy_num];

			SortMetrics total_results(0,0);

			for (int test_number = 0; test_number != num_test_passes; test_number++) {

				SortMetrics result(0, 0);
				//	create a linear array
				for (int i = 0; i != array_size; i++) {
					test_array[i] 		= reference_array[i];
				}

				//	randomize the array using the default randomizer
				for (int i = 0; i != array_size; i++) {
					int r = randomizer.rand(i, array_size);
					data_type *temp = test_array[i];
					test_array[i] = test_array[r];
					test_array[r] = temp;
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

				for (int i = 0; i != array_size; i++) {
					initial_array[i] = test_array[i];
				}

				message.clear();
				message.str("");
				if (debug_verbose) {
					message << "  indices      : ";
					for (int i = 0; i != array_size; i++) {
						message << std::setw(element_width) << i;
					}
					message << std::endl;
					message << "  initial_array: "
							<< _arrayToString(initial_array, array_size)
							<< std::endl;
				}

				switch(merge_strategy) {
				case MergeStrategy::ROTATE:
					result +=
						SortingUtilities::mergeTwoAdjacentBlocksByRotation(
													test_array,
													left_start, left_end,
													right_start, right_end);
					break;
				case MergeStrategy::TABLE:
					result +=
						SortingUtilities::mergeTwoBlocksElementsByTable(
													test_array,
												 	left_start, left_end,
													right_start, right_end);
					break;
				case MergeStrategy::AUXILLIARY:
					std::cout << __FUNCTION__ << " using strategy "
							  << std::to_string(merge_strategy)
							  << " which is not implemented\n";
					test_passed = false;
					goto TEST_BLOCK_SORT_MERGE_BLOCKS_RETURN_LABEL;
				}
				total_results += result;

				if (debug_verbose) {
					message << "          after: "
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
								<< test_number << std::endl;
						message << " indices        ";
						for (int i = 0; i != array_size; i++) {
							message << std::setw(element_width+1) << i;
						}
						message << std::endl;
						message	<< " initial_array  "
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
						 << " using stategy "
						 << std::setw(MERGE_STRATEGY_MAX_STRING_LENGTH)
						 << std::left << merge_strategy
						 << std::right
						 << " took on average "
						 << std::fixed << std::setprecision(1) << std::setw(8)
						 << static_cast<double>(total_results.compares) / num_test_passes
						 << " compares and "
						 << std::fixed << std::setprecision(1) << std::setw(12)
						 << static_cast<double>(total_results.assignments) / num_test_passes
						 << " moves" << std::endl;
			}
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

	bool announce_results 	= false;
	bool show_results 		= false;

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
	SortMetrics rotate_result(0,0);

	//	list all the array sizes to be tested
	//	array_size_t array_sizes[] = {	8, 9, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };
	array_size_t array_sizes[] = {	8, 9, 16, 32 };
	int num_array_sizes = sizeof(array_sizes) / sizeof(array_size_t);

	//	Determining the array counts to be tested for each array size
	array_size_t rotate_counts[] = { 3 };
	int  num_rotate_counts = sizeof(rotate_counts)/sizeof(array_size_t);
	//	Setting this will ignore 'rotate_counts'
	constexpr bool exhaustively_test_rotate_counts = true;
	array_size_t min_rotate_count;	//	= -(2*array_size-1)
	array_size_t max_rotate_count;	//	= +(2*array_size+1);
	array_size_t current_rotate_count;

	for (int array_size_i = 0; array_size_i != num_array_sizes; array_size_i++)
	{
		array_size_t array_size = array_sizes[array_size_i];
		int *reference_array[array_size];
		for (array_size_t i = 0; i < array_size; i++) {
			reference_array[i] = new int(static_cast<int>(i));
		}

		num_rotate_counts = sizeof(rotate_counts)/sizeof(array_size_t);
		if (exhaustively_test_rotate_counts) {
			min_rotate_count = -2 * array_size - 1;
			max_rotate_count  = 2 * array_size + 1;
			current_rotate_count =  min_rotate_count;
		}

		int rotate_count_i = 0;
		while (rotate_count_i != num_rotate_counts)
		{
			//	rotate_count is either specified, or calculated
			array_size_t rotate_count = rotate_counts[rotate_count_i];
			if (!exhaustively_test_rotate_counts) {
				rotate_count = rotate_counts[rotate_count_i];
				rotate_count_i++;
			} else {
				rotate_count_i = 0;
				//	force the loop counter to exit
				if (current_rotate_count > max_rotate_count) {
					break;
				}
				//	force the loop counter to continue
				rotate_count = current_rotate_count++;
				//	if there are no more test runs, break
			}

			// 	create an array to test rotation and an array of expecteds
			int *rotated_array[array_size];
			int *expected_array[array_size];
			for (array_size_t i = 0; i != array_size; i++) {
				rotated_array[i]  = reference_array[i];
				expected_array[i] = reference_array[i];
			}

			rotateArrayElementsLongWay(expected_array, 0, array_size-1, rotate_count);
			rotate_result =
				SortingUtilities::rotateArrayElementsRight<int>(rotated_array, 0, array_size-1, rotate_count);
			//	check results
			for (int i = 0 ; i < array_size; i++) {
				if (rotated_array[i] != expected_array[i]) {
					std::cout << "ERROR: expected[" << i << "] " << *expected_array[i]
							  << " vs actual[" << i << "] " << *rotated_array[i] << std::endl;
					test_passed = false;
					break;
				}
			}
			if (announce_results || show_results) {
				std::cout << (test_passed ? "PASSED " : "FAILED ");
				//	print out before / after
				std::cout << "array size " << std::setw(4) << array_size << " ";
				std::cout << "rotate(" << std::setw(3) << rotate_count << ") ";
				if (show_results) {
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
				}
				std::cout << " took:" << std::setw(7) << rotate_result.assignments
						  << " moves\n";
			}
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

	using DataType = int;

	/*	***************************************************	*/
	/*		lambda that rotates simply but inefficiently	*/
	/*	***************************************************	*/

	auto copyArray = [](int **_dst, int **_src, array_size_t _size) {
		for (int i = 0; i != _size; i++) {
			_dst[i] = _src[i];
		}
	};

	auto copyDescriptors = [](Descriptors<DataType> &dst,
							  	  	  Descriptors<DataType> &src,
									  int num_blocks) {
		for (int i = 0; i != num_blocks; i++) {
			dst[i] = src[i];
		}
	};

	auto generateReferenceArray = [] (DataType* (&dst_array)[],index_t size, DataType first_value) {
		for (array_size_t i = 0; i != size; i++) {
			dst_array[i] = new DataType(first_value + i);
		}
	};

	auto generateExpected = [] (DataType* (&array)[],
								Descriptors<DataType> &descriptors,
								index_t window_start, index_t window_end,
								int block_rotate_count)
	{
#pragma push_macro("debug")
#define debug(msg)	msg
		index_t span_start 	= descriptors[window_start].start_index;
		index_t span_end	= descriptors[window_end].end_index;

		if (block_rotate_count < 0) {
			//	rotate left the number of times requested
			for (int rotate_num = 0; rotate_num > block_rotate_count; rotate_num--) {
				//	rotate left the underlying array the width of the left-most block
				for (int element_shift_count = descriptors[window_start].getWidth();
						 element_shift_count; element_shift_count--) {
					//	store the first element in the span which will be overwritten
					DataType* first_element = array[span_start];
					//	move each element one position to the left
					for (int i = span_start; i < span_end; i++) {
						array[i] = array[i+1];
					}
					//	write the element that was at the span_start to the span_end
					array[span_end] = first_element;
				}
				//	rotate the descriptors over by 1
				BlockDescriptor<DataType> first_descriptor = descriptors[window_start];
				for(int i = window_start; i < window_end; i++) {
					descriptors[i] = descriptors[i+1];
				}
				descriptors[window_end] = first_descriptor;

				//	update the indices and thus the keys of the descriptors
//				std::cout << "The underlying array: " << std::endl;
//				for (int i = span_start; i <= span_end; i++) {
//					std::cout << std::setw(3) << *array[i] << " ";
//				}
//				std::cout << std::endl;
				index_t block_start = span_start;
				index_t block_end;
				//	reassign the descriptors' values to the updated array
				for (int i = window_start; i <= window_end; i++) {
					index_t block_size = descriptors[i].getWidth();
					block_end = block_start + block_size-1;
					descriptors[i].start_index 	= block_start;
					descriptors[i].end_index	= block_end;
					if (descriptors[i].type == BlockSort::BlockType::A_BLOCK)
						descriptors[i].key	= array[block_start];
					else
						descriptors[i].key	= array[block_end];
					block_start = block_end+1;
				}
				if (block_start != span_end+1) {
					std::cout << "Something went seriously wrong\n";
				}
//				std::cout << "The descriptors: " << std::endl;
//				for (int i = window_start; i <= window_end; i++) {
//					std::cout << descriptors[i] << " ";
//				}
//				std::cout << std::endl;
			}
		} else {
			//	rotate right
			for (int rotate_num = 0; rotate_num < block_rotate_count; rotate_num++) {
				//	rotate left the underlying array the width of the rightmost block
				for (int element_shift_count = descriptors[window_end].getWidth();
						 element_shift_count; element_shift_count--) {
					//	store the last element in the span which will be overwritten
					DataType* last_element = array[span_end];
					//	move each element one position to the right
					for (int i = span_end; i > span_start; i--) {
						array[i] = array[i-1];
					}
					//	write the element that was at the span_start to the span_end
					array[span_start] = last_element;
				}

				//	rotate the descriptors to the right by 1
				BlockDescriptor<DataType> last_descriptor = descriptors[window_end];
				for(int i = window_end; i > window_start; i--) {
					descriptors[i] = descriptors[i-1];
				}
				descriptors[window_start] = last_descriptor;

				//	update the indices and thus the keys of the descriptors
//				std::cout << "The underlying array: " << std::endl;
//				for (int i = span_start; i <= span_end; i++) {
//					std::cout << std::setw(3) << *array[i] << " ";
//				}
//				std::cout << std::endl;
				index_t block_start = span_start;
				index_t block_end;
				//	reassign the descriptors' values to the updated array
				for (int i = window_start; i <= window_end; i++) {
					//	the width of the block is correct
					index_t block_size = descriptors[i].getWidth();
					block_end = block_start + block_size-1;
					descriptors[i].start_index 	= block_start;
					descriptors[i].end_index	= block_end;
					if (descriptors[i].type == BlockSort::BlockType::A_BLOCK)
						descriptors[i].key	= array[block_start];
					else
						descriptors[i].key	= array[block_end];
					block_start = block_end+1;
				}
				if (block_start != span_end+1) {
					std::cout << "Something went seriously wrong\n";
				}
//				std::cout << "The descriptors: " << std::endl;
//				for (int i = window_start; i <= window_end; i++) {
//					std::cout << descriptors[i] << " ";
//				}
//				std::cout << std::endl;
			}
		}
#pragma pop_macro("debug")
	};

	auto verifyResult = [] (DataType* (&test_array)[], DataType* (&expected_array)[],
							index_t array_size,
							Descriptors<DataType> &test_descriptors,
							Descriptors<DataType> &expected_descriptors,
							int num_descriptors) -> bool {
		//	check the array
		for (index_t i = 0; i != array_size; i++) {
			if (*test_array[i] != *expected_array[i]) {
				return false;
			}
		}
		//	check the descriptors
		for (int i = 0; i != num_descriptors; i++) {
			if (test_descriptors[i].start_index != expected_descriptors[i].start_index)
				return false;
			if (test_descriptors[i].end_index != expected_descriptors[i].end_index)
				return false;
			if (test_descriptors[i].key != expected_descriptors[i].key)
				return false;
			if (*test_descriptors[i].key != *expected_descriptors[i].key)
				return false;
			if (test_descriptors[i].type != expected_descriptors[i].type)
				return false;
		}
		return true;
	};

	auto debug = [] (DataType *r[], DataType *e[], DataType *t[],
					 index_t a_sz, index_t mid,
					 Descriptors<DataType> &rd, Descriptors<DataType> &ed,
					 Descriptors<DataType> &td, int n_d) -> std::string {
		std::stringstream result;
		result << BlockSort::blockSortToString(r, a_sz, mid, rd, n_d, "Reference ")
			   << std::endl;
		result << BlockSort::blockSortToString(e, a_sz, mid, ed, n_d, "Expected  ")
			   << std::endl;
		result << BlockSort::blockSortToString(t, a_sz, mid, td, n_d, "Resultant ")
			   << std::endl;
		return result.str();
	};

	/* ***********************************************************************	*/
	/* 								test code									*/
	/* ***********************************************************************	*/

	constexpr bool debug_verbose 	= false;
	constexpr bool echo_test_params = false;

	bool test_passed = true;
	int test_count = 0;

	DataType first_value = 0;

	BlockSort::BlockOrganizations block_organizations[] = {
//			BlockSort::BlockOrganizations::FULL_A0_BLOCK,
			BlockSort::BlockOrganizations::SYMMETRIC,
	};
	int num_block_organizations = sizeof(block_organizations) / sizeof (BlockOrganizations);

	int block_sizes[] = { 3, 4, 5 };
	int num_block_sizes = sizeof(block_sizes)/sizeof(int);
	int block_counts[]	= { 2, 3, 4, 5, 6, 7, 8 };
	int num_block_counts = sizeof(block_counts)/sizeof(int);

	for (int block_organization_i = 0;
			 block_organization_i != num_block_organizations;
			 block_organization_i++)
	{
		BlockSort::BlockOrganizations block_organization =
				block_organizations[block_organization_i];
		for (int block_size_i = 0; block_size_i < num_block_sizes; block_size_i++)
		{
			int block_size = block_sizes[block_size_i];
			for (int block_count_i = 0; block_count_i < num_block_counts; block_count_i++)
			{
				int block_count 	= block_counts[block_count_i];
				int min_array_size 	= block_size * block_count;
				int max_array_size 	= min_array_size;
				switch(block_organization) {
				case BlockSort::BlockOrganizations::FULL_A0_BLOCK:
					//	the right most B_Block may be anywhere from 1 to block_size-1
					//	A[0:2]B[3:5]	A[0:2]B[3:5]B[6]	A[0:2]B[3:5]B[6:7]
					max_array_size = min_array_size + block_size-1;
					break;
				case BlockSort::BlockOrganizations::SYMMETRIC:
					//	there may be partial blocks on both A & B
					//	A[0:2]B[3:5] 			A[0:2]B[3:5]B[6:6]
					//	A[0]A[1:3][B[4:6]B[7]	A[0]A[1:3]B[4:6]B[7:8]
					//	A[0:1]A[2:4]B[5:7]B[8:9]
					max_array_size = min_array_size + 2*block_size-1;
					break;
				default:
					std::cout << __FUNCTION__ << " ERROR undefined BlockOrganization\n";
					return false;
				}
				for (index_t array_size = min_array_size; array_size <= max_array_size; array_size++)
				{
					Descriptors<DataType> reference_descriptors;
					DataType *reference_array[array_size];
					int num_blocks = 0;
					generateReferenceArray(reference_array, array_size, first_value);
					index_t start = 0;
					index_t mid;
					index_t end = array_size-1;
					//	at this point, block_organization is valid
					switch(block_organization) {
					case BlockSort::BlockOrganizations::FULL_A0_BLOCK:
						//	guarantee that there are an integer number of A blocks
						mid = (block_count / 2) * block_size;
						num_blocks =
							createBlockDescriptors_A0_Full(reference_array,
														   start, mid, end,
														   block_size,
														   reference_descriptors);
						break;
					default:
					case BlockSort::BlockOrganizations::SYMMETRIC:
						//	there can be partial A and partial B blocks
						mid = array_size/2;
						num_blocks =
							createBlockDescriptorsSymmetrically(reference_array,
																start, mid, end,
																block_size,
																reference_descriptors);
					}
					if (debug_verbose) {
						std::cout << "Organization " << block_organization
								  << " array_size " << array_size
								  << " mid " << mid
								  << " block_size " << block_size
								  << " num_blocks " << num_blocks
								  << std::endl;
						std::cout << blockSortToString(reference_array, array_size, mid,
													   reference_descriptors, num_blocks)
								  << std::endl;
					}

//					for (int window_size = num_blocks; window_size > 0; window_size--) {
					for (int window_size = 1; window_size <= num_blocks; window_size++)
					{
						int window_start 	= 0;
						int window_end 		= window_start + window_size;
						for (; window_end < num_blocks; window_start++, window_end++) {
							//	rotate lefts up to slightly more than window size
							int min_rotate_count = -window_size-1;
							//	rotate right up to slightly more than window size
							int max_rotate_count =  window_size+1;
							//min_rotate_count = -1;
							//max_rotate_count = +1;
							for (int rotate_count  = min_rotate_count;
									 rotate_count <= max_rotate_count;
									 rotate_count++) {
								test_count++;
								std::stringstream test_name_msg;
								std::stringstream test_debug_msg;
								DataType *test_array[array_size];
								DataType *expected_array[array_size];
								Descriptors<DataType> test_descriptors
									= std::unique_ptr<BlockDescriptor<DataType>[]>(new BlockDescriptor<DataType>[num_blocks]);
								Descriptors<DataType> expected_descriptors
									= std::unique_ptr<BlockDescriptor<DataType>[]>(new BlockDescriptor<DataType>[num_blocks]);

								copyArray(test_array, reference_array, array_size);
								copyArray(expected_array, reference_array, array_size);
								for (int i = 0; i != num_blocks; i++) {
									test_descriptors[i] = reference_descriptors[i];
									expected_descriptors[i] = reference_descriptors[i];
								}
								copyDescriptors(expected_descriptors, reference_descriptors, num_blocks);
								copyDescriptors(test_descriptors, reference_descriptors, num_blocks);
								generateExpected(expected_array, expected_descriptors,
												 window_start, window_end,
												 rotate_count);
								if (debug_verbose) {
									std::cout << "reference [" << reference_descriptors[0].start_index << ":" << reference_descriptors[num_blocks-1].end_index << "]"
											  << " expected [" << expected_descriptors[0].start_index << ":" << expected_descriptors[num_blocks-1].end_index << "]"
											  << " test [" << test_descriptors[0].start_index << ":" << test_descriptors[num_blocks-1].end_index << "]"
											  << " window_start " << window_start
											  << " window_end " << window_end
											  << " num_blocks " << num_blocks
											  << std::endl;
								}
								rotateBlocksRight(test_array, test_descriptors,
												  window_start, window_end,
												  rotate_count);
								bool pass_passed = verifyResult(test_array, expected_array, array_size,
														   	    test_descriptors, expected_descriptors,
																num_blocks);
								test_name_msg
									<< "Test Number " << std::setw(4) << test_count
								 	<< " Block organization "
									<< std::setw(BLOCK_ORGANIZATION_MAX_STRING_LENGTH)
									<< std::left << block_organization
									<< std::right
									<< " array_size " 	<< std::setw(5) << array_size
									<< " block size "   << std::setw(3) << block_size
									<< " block count " 	<< std::setw(3) << num_blocks
									<< " window start " << std::setw(3) << window_start
									<< " window end " 	<< std::setw(3) << window_end
									<< " rotate_count " << std::setw(3) << rotate_count
									<< std::endl;
								test_debug_msg
									<< debug(reference_array,
											 expected_array,
											 test_array,
											 array_size, mid,
											 reference_descriptors,
											 expected_descriptors,
											 test_descriptors,
											 num_blocks);
								if (echo_test_params || debug_verbose) {
									std::cout << test_name_msg.str();
								}
								if (debug_verbose && (window_end - window_start != 0)) {
									std::cout << test_debug_msg.str();
								}
								if (!pass_passed) {
									if (!echo_test_params && !debug_verbose)
										std::cout << test_name_msg.str();
									if (!debug_verbose) {
										std::cout << test_debug_msg.str();
									}
									std::cout << "!!! ERROR !!!" << std::endl;
									test_passed = false;
									goto TEST_ROTATE_BLOCKS_RETURN_POINT;
								}
								if (debug_verbose) {
									std::cout << std::endl;
								}
							}
						}
					}
				}
			}
		}
	}

	std::cout << " All " << test_count << " tests passed\n";
TEST_ROTATE_BLOCKS_RETURN_POINT:
	return test_passed;
}

bool testBlockSortSortBlocks() {

	//	The desstructor restores the state of ostream
	OStreamState ostream_state;

	std::cout << __FUNCTION__ << "()" << std::endl;
#if 0
	enum SortingStrategy { BINARY, HYBRID, RIGHT_TO_LEFT, TABLE };
	std::string (*strategyToString)(SortingStrategy) =
			[] (SortingStrategy strategy) -> std::string {
		switch(strategy) {
		case BINARY:		return std::string("Binary       ");
		case HYBRID:		return std::string("Hybrid       ");
		case RIGHT_TO_LEFT: return std::string("Right to Left");
		case TABLE:			return std::string("Table        ");
		default:			return std::string("?????????????");
		}
	};
#endif
	using BlockArray_t = std::unique_ptr<BlockSort::BlockDescriptor<int>[]>;

	constexpr bool echo_every_test_step 			= false;
	constexpr bool echo_every_test_result 			= false;
	constexpr bool echo_test_configuration_summary	= true;

	//	constants related to output number format
//	constexpr const int object_width = 3;
//	constexpr const int element_width = 4;
//	constexpr const char separator = ' ';
	constexpr const int compares_precision 	= 1;
	constexpr const int moves_precision 	= 1;
	constexpr const int num_unique_values 	= 5;

//	auto out = [object_width, element_width, separator]
//					(int ** array, array_size_t array_size,
//					 std::string trailer) {
//		std::stringstream result;
//		result << SortingUtilities::arrayElementsToString(array, array_size, object_width, element_width)
//			   << trailer;
//		return result.str();
//	};

	bool test_passed = true;

	bool (*areBlocksSorted)(BlockArray_t &blocks, int num_blocks) =
		[] (BlockArray_t &_blocks, int _num_blocks) {
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
			30, 31, 32, 33, 34, 35,
			48, 49, 50, 51, 52, 53, 54,
			62, 63, 64, 65, 66, 67, 68, 69
			};
//	constexpr array_size_t array_sizes[] = {
//			32, 50, 72, 98, 128
//			};
//	constexpr array_size_t array_sizes[] = {
//			32, 128, 512, 2048, 8192, 32768
//	};
//	constexpr array_size_t array_sizes[] = {
//			32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768
//	};

	constexpr int num_array_sizes = sizeof(array_sizes) / sizeof(array_size_t);

	bool all_unique_elements = true;
	bool few_unique_elements = false;

	for (int array_size_i = 0; array_size_i < num_array_sizes; ++array_size_i)
	{
		array_size_t array_size = array_sizes[array_size_i];
//		int min_block_size = static_cast<array_size_t>(sqrt(array_size/2))-1;
//		int max_block_size = min_block_size+2;
		int min_block_size = static_cast<array_size_t>(sqrt(array_size/2));
		int max_block_size = min_block_size;
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

		constexpr BlockSort::SortingStrategy sorting_strategies[] = {
			BlockSort::SortingStrategy::BINARY,
			BlockSort::SortingStrategy::HYBRID,
			BlockSort::SortingStrategy::RIGHT_TO_LEFT,
			BlockSort::SortingStrategy::TABLE,
		};

		int num_sorting_strategies = sizeof(sorting_strategies) / sizeof(SortingStrategy);

		for (int block_size = min_block_size; block_size <= max_block_size; ++block_size)
		{
			int num_tests = 10;
			SortMetrics total_results[num_sorting_strategies];

			// run the test with the same reference_array (input)
			//	using however many strategies desired
			for (int test_num = 0; test_num != num_tests; test_num++)
			{
				randomizeArray(reference_array, array_size);
				randomizeArray(reference_array, array_size);
				randomizeArray(reference_array, array_size);

				for (int strategy_i = 0; strategy_i < num_sorting_strategies; strategy_i++)
				{
					SortMetrics result(0,0);
					std::stringstream messages;
					BlockSort::SortingStrategy sorting_strategy
						= sorting_strategies[strategy_i];
					int* array[array_size];
					for (int i = 0; i != array_size; i++) {
						array[i] = reference_array[i];
					}
					int start = 0;
					int mid = array_size / 2;
					int end = array_size - 1;
					int num_blocks = 0;
					BlockArray_t blocks;

					//	the randomizing algorithm leaves most of the larger elements in the
					//	  right had side of the array.  Swap the two halves so that the
					//	  larger elements are in the left side of the array
					InsertionSort::sortPointersToObjects(array, mid);
					InsertionSort::sortPointersToObjects(&array[mid], end-mid+1);

					num_blocks = BlockSort::createBlockDescriptorsSymmetrically(array, start, mid, end, block_size, blocks);
					messages << "Test run of strategy " << sorting_strategy << "\n";
					messages << blockSortToString(array, array_size, mid,
												  blocks, num_blocks)
							 << std::endl;
					switch(sorting_strategy) {
					case BlockSort::SortingStrategy::RIGHT_TO_LEFT:
						result = BlockSort::sortBlocksRightToLeft(array, array_size, blocks, num_blocks);
						break;
					case BlockSort::SortingStrategy::BINARY:
						result = BlockSort::sortBlocksBinarySearch(array, array_size, blocks, num_blocks);
						break;
					case BlockSort::SortingStrategy::HYBRID:
						result = BlockSort::sortBlocksHybrid(array, array_size, blocks, num_blocks);
						break;
					case BlockSort::SortingStrategy::TABLE:
						result = BlockSort::sortBlocksByTable(array, blocks, num_blocks);
						break;
					default:
						break;
					}
					total_results[strategy_i] += result;

					messages << "Result:\n";
					messages << blockSortToString(array, array_size, mid,
												  blocks, num_blocks)
							 << std::endl;

					if (!areBlocksSorted(blocks, num_blocks)) {
						test_passed = false;
						std::cout << messages.str();
						std::cout << " !!!! FAILED !!!! test run " << test_num << std::endl;
						goto TEST_BLOCK_SORT_SORT_BLOCKS_RETURN_LABEL;
					}
					if (echo_every_test_step) {
						std::cout << messages.str();
					}
					if (echo_every_test_result) { // && (test_num % 1000 == 0)) {
						std::cout << std::setw(SORTING_STRATEGY_MAX_STRING_LENGTH)
								  << std::left << sorting_strategy << std::right
								  << " sorting an array of " << array_size << " elements "
								  << " with block size " << block_size
								  << " took "
								  << std::setw(5) << result.compares << " compares and"
								  << std::setw(8) << result.assignments    << " moves\n";
					}
				}
			}
			if (echo_test_configuration_summary)
			{
				for (int strategy_i = 0; strategy_i < num_sorting_strategies; strategy_i++)
				{
					std::cout  << std::setw(6) << num_tests << " tests of sorting blocks using strategy "
							   << std::setw(SORTING_STRATEGY_MAX_STRING_LENGTH)
							   << std::left << sorting_strategies[strategy_i] << std::right
							   << " on an array with " << std::setw(6) << this_passes_unique_value_count
							   << " unique values and a block size of "
							   << std::setw(3) << block_size << " took on average "
							   << std::fixed << std::setprecision(compares_precision) << std::setw(8)
							   << static_cast<double>(total_results[strategy_i].compares) / (num_tests)
							   << " compares and "
							   << std::fixed << std::setprecision(moves_precision) << std::setw(10)
							   << static_cast<double>(total_results[strategy_i].assignments) / (num_tests)
							   << " moves\n";
				}
			}
//			if (all_unique_elements) {
//				few_unique_elements = true;
//				all_unique_elements = false;
//			} else {
//				few_unique_elements = false;
//				all_unique_elements = true;
//			}
		}
		if (num_sorting_strategies != 1) {
			std::cout << "\n";
		}
	}
	TEST_BLOCK_SORT_SORT_BLOCKS_RETURN_LABEL:
	return test_passed;
}


bool testBlockSortSwapBlocks() {

	OStreamState ostream_state;	// restores ostream flags in destructor

	std::cout << __FUNCTION__ << std::endl;

	using DataType = char;

	/*	******************************************************************	*/
	/*								lambdas									*/
	/*	******************************************************************	*/

	auto copy_array = [] (DataType **dst, DataType **src, index_t size) {

		for (int i = 0; i != size; i++) {
			dst[i] = new char(*src[i]);
		}
	};

	auto copy_descriptors = [] (DataType** expected_array,
			 	 	 	 	 	Descriptors<DataType> &dst,
								Descriptors<DataType> &src,
								int num_descriptors) {
		//	ensure that the expected descriptors point to the
		//	  expected arrray
		for (int i = 0; i != num_descriptors; i++) {
			dst[i] = src[i];
			dst[i].assignKey(expected_array);
		}
	};

	auto compare_arrays = [] (DataType **u, DataType **v, index_t size) -> bool {
		bool identical = true;
		for (int i = 0; i != size; i++) {
			if (*u[i] != *v[i]) {
				identical = false;
				break;
			}
		}
		return identical;
	};

	auto compare_descriptors = [] (Descriptors<DataType> &expected,
							  Descriptors<DataType> &result,
							  int num_descriptors) -> bool{

		bool identical = true;
		int i = 0;
		while (i != num_descriptors) {
			if (expected[i].type != result[i].type)	{
				std::cout << "Block Types do not match: expected " << expected[i] << " vs result " << result[i] << std::endl;
				identical = false;
				break;
			}
			if (*expected[i].key != *result[i].key) {
				std::cout << "Block keys do not match: expected " << expected[i] << " vs result " << result[i] << std::endl;
				identical = false;
				break;
			}
			i++;
		}
		return identical;
	};

	auto generate_expected_array = [] (DataType **expected,
								 	   Descriptors<DataType> &descriptors,
									   int u, int v) {

		if (u == v)	return;
		if (u > v) {
			int temp = u;
			u = v;
			v = temp;
		}

		index_t u_start = descriptors[u].start_index;
		index_t u_end	= descriptors[u].end_index;
		index_t v_start	= descriptors[v].start_index;
		index_t v_end	= descriptors[v].end_index;
		index_t u_size 	= u_end - u_start + 1;
		index_t v_size 	= v_end - v_start + 1;
		if (u_size == v_size) {
			index_t u_p = u_start;
			index_t v_p = v_start;
			for (int i = 0; i != u_size; i++, u_p++, v_p++) {
				DataType *temp = expected[u_p];
				expected[u_p] = expected[v_p];
				expected[v_p] = temp;
			}
		} else {
			//	set aside the values for the blocks that are going to be swapped
			DataType* u_values[u_size];
			DataType* v_values[v_size];
			for (int i = 0; i != u_size; i++) {
				u_values[i] = expected[u_start+i];
			}
			for (int i = 0; i != v_size; i++) {
				v_values[i] = expected[v_start+i];
			}

			if (u_size < v_size) {
				//	make room in the left hand size for all of v
				for (index_t shift_right_count = v_size - u_size;
						     shift_right_count != 0;
						     shift_right_count--) {
					for (index_t p = v_end; p > u_start; p--) {
						expected[p] = expected[p-1];
					}
				}
			} else {
				//  make room in the right hand size for all of u
				for (index_t shift_left_count = u_size - v_size;
							 shift_left_count != 0;
							 shift_left_count--) {
					for (index_t p = u_start; p < v_end; p++) {
						expected[p] = expected[p+1];
					}
				}
			}
			index_t right_dst = v_end;
			for (int i = u_size-1; i >= 0; i--, right_dst--) {
				expected[right_dst] = u_values[i];
			}
			index_t left_dst = u_start;
			for (int i = 0; i < v_size; i++, left_dst++) {
				expected[left_dst] = v_values[i];
			}
		}
	};

	auto generate_expected_descriptors = [] (
			DataType **expected_array, Descriptors<DataType> &expected, int u, int v)
	{
		if (u == v)	return;
		if (u > v) {
			int temp = u;
			u = v;
			v = temp;
		}
		index_t start_span 				= expected[u].start_index;
		BlockDescriptor<DataType> temp = expected[u];
		expected[u]						= expected[v];
		expected[v]						= temp;
		for (int i = u; i <= v; i++) {
			index_t size = expected[i].getWidth();
			expected[i].start_index = start_span;
			expected[i].end_index	= start_span + size - 1;
			expected[i].assignKey(expected_array);
			start_span += size;
		}
	};

	auto make_test_vector_1 = [] (DataType **dst, index_t size, index_t mid) {
		char left_start = 'A';
		char right_start = 'a';
		int i = 0;
		for (; i != mid; i++) {
			dst[i] = new char(left_start + ((i)%26));
		}
		for (; i != size; i++) {
			dst[i] = new char(right_start + ((i)%26));
		}
	};

	SimpleRandomizer randomizer(getChronoSeed());

	auto randomize = [&randomizer] (DataType **array, index_t length) {
		for (int i = 0; i != length; i++) {
			index_t r = randomizer.rand(i, length);
			DataType* temp = array[i];
			array[i] = array[r];
			array[r] = temp;
		}
	};

//	auto crc_to_string = [] (uint16_t _crc) -> std::string {
//		OStreamState ostream_state;
//		std::stringstream msg;
//		msg << "0x" << std::hex << std::setw(4) << std::setfill('0') << _crc;
//		return msg.str();
//	};

	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*							function logic starts here						*/
	/*	**********************************************************************	*/
	/*	**********************************************************************	*/

	constexpr bool announce_each_result = false;
	constexpr bool debug_verbose 		= false;
	constexpr bool output_metrics 		= false;

	bool test_passed = true;

	index_t array_sizes[] = { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
						      42, 43, 44, 45, 46, 47, 48, 49, 50 };
//	index_t array_sizes[] = { 32, 64, 128, 256 };
	int num_array_sizes = sizeof(array_sizes)/sizeof(index_t);

	BlockOrganizations organizations[] = {
			BlockOrganizations::FULL_A0_BLOCK,
			BlockOrganizations::SYMMETRIC
	};
	int num_organizations = sizeof(organizations)/sizeof(BlockOrganizations);

	int num_repeats_per_test = 1000;
	int test_number 		 = 0;

	for (int array_size_i = 0;
			 array_size_i < num_array_sizes;
			 array_size_i++) {
		for (int block_organization_i = 0;
				 block_organization_i != num_organizations;
				 block_organization_i++) {
			BlockOrganizations organization = organizations[block_organization_i];
			index_t array_size 	= array_sizes[array_size_i];
			index_t span_end 	= array_size-1;
			index_t	block_size 	= static_cast<index_t>(std::sqrt(array_size/2));
			index_t mid;
			switch(organization) {
			case BlockOrganizations::FULL_A0_BLOCK:
				// 	the left half of the array has an integer number of blocks
				//	the right half of the array may have single partial block
				//	array_size = 35, block_size = 4, mid = 4*4 = 16
				//	a0 has 4 elements, b8 has 3 elements
				//	block_sizes = {4:4:4:4 _ 4:4:4;4:3}
				{
				index_t num_complete_blocks = array_size / block_size;
				mid = (num_complete_blocks /2)*block_size;
				}
				break;
			case BlockOrganizations::SYMMETRIC:
				//	both the left half and the right half may have partial blocks
				//	array_size = 35, block_size = 4, mid = 35/2 = 17,
				//	a0 has 1 element, b9 has 2 elements
				// 	blocks sizes = {1:4:4:4:4 _ 4:4:4:4:2}
				mid	= array_size/2;
				break;
			default:
				std::cout << "UNSPECIFIED BLOCK ORGANIZATION" << std::endl;
				test_passed = false;
				goto TEST_BLOCK_SORT_SWAP_BLOCKS_TEST_RETURN;

			}

			if (debug_verbose || output_metrics) {
				std::cout << "Test number " << std::setw(12) << test_number
						  << " with array size " << array_size
						  << " block size " << block_size
						  << " organization "
						  << std::setw(BLOCK_ORGANIZATION_MAX_STRING_LENGTH)
						  << std::left << organization
						  << std::right;
				if (!output_metrics)
					std::cout << std::endl;
				else
					std::cout << " ";
				std::cout << std::flush;
			}
			bool first_test_pass = true;
			total_moves_t max_moves = 0;

			for (int test_counter = 0; test_counter != num_repeats_per_test; test_counter++) {
				std::vector<std::vector<DataType*>> 	test_vectors_log;
				DataType *test_vector[array_size];
				make_test_vector_1(test_vector, array_size, mid);
				randomize(test_vector, array_size);
				InsertionSort::sortPointersToObjects(test_vector, mid);
				InsertionSort::sortPointersToObjects(&test_vector[mid], array_size-mid);

				Descriptors<DataType> test_descriptors;
				int num_descriptors;

				switch(organization) {
				case BlockOrganizations::FULL_A0_BLOCK:
					num_descriptors =
						BlockSort::createBlockDescriptors_A0_Full(
							test_vector, 0, mid, span_end,
							block_size, test_descriptors);
					break;
				case BlockOrganizations::SYMMETRIC:
					num_descriptors =
						BlockSort::createBlockDescriptorsSymmetrically(
							test_vector, 0, mid, span_end,
							block_size, test_descriptors);
					break;
				}

				// Go through all possible combinations, including passing
				//	a second (right) index that is less than the first (left) index
				for (int i = 0; i < num_descriptors-1; i++) {
					if (debug_verbose && first_test_pass) {
						std::cout << "   array size " << array_size
								  << " block size " << block_size
								  << " i = " << i << std::endl;
//						first_test_pass = false;
					}
					for (int j = 0; j < num_descriptors; j++) {
						test_number++;
						if (debug_verbose && first_test_pass) {
							std::cout << "     *** TEST START : " << std::setw(12) << test_number
									  << ": array size " << array_size
									  << " with blocks size " << block_size
									  << " " << std::to_string(organization) << " "
									  << " swapping " << i
									  << " vs " << j
									  << std::endl;;
//							first_test_pass = false;
						}

						SortMetrics metrics(0,0);
						DataType *array_under_test[array_size];
						Descriptors<DataType> descriptors_under_test =
							Descriptors<DataType>(new BlockDescriptor<DataType>[num_descriptors]);
						DataType *expected_array[array_size];
						Descriptors<DataType> expected_descriptors =
							Descriptors<DataType>(new BlockDescriptor<DataType>[num_descriptors]);
						copy_array(array_under_test, test_vector, array_size);
						copy_array(expected_array, test_vector, array_size);
						copy_descriptors(array_under_test,
										 descriptors_under_test, test_descriptors,
										 num_descriptors);
						copy_descriptors(expected_array,
										 expected_descriptors, test_descriptors,
										 num_descriptors);

						//	first swap the underlying elements into their final positions
						generate_expected_array(expected_array, expected_descriptors, i, j);
						//	then update the .type and .key members
						generate_expected_descriptors(expected_array,
													  expected_descriptors, i, j);

						metrics = BlockSort::swapBlocks(array_under_test, descriptors_under_test, i, j);

						if (metrics.assignments > max_moves) {
							max_moves = metrics.assignments;
						}

						bool arrays_passed = compare_arrays(expected_array, array_under_test, array_size);
						bool descriptors_passed = compare_descriptors(expected_descriptors, descriptors_under_test, num_descriptors);
						if (announce_each_result) {
							std::stringstream msg;
							msg << BlockSort::blockSortToString(
											test_vector, array_size, mid,
											test_descriptors, num_descriptors)
								<< "\n\n";
							msg << std::setw(5) << test_number << ": swapped " << i << " vs " << j
								<< " which took " << metrics << " yielded:\n\n";
							msg << BlockSort::blockSortToString(
											array_under_test, array_size, mid,
											descriptors_under_test, num_descriptors)
								<< "\n\n";

							std::cout << msg.str() << std::endl;
						}
						if (!arrays_passed || !descriptors_passed) {
							test_passed = false;
							std::stringstream msg;
							if (!announce_each_result) {
								msg << BlockSort::blockSortToString(
												test_vector, array_size, mid,
												test_descriptors, num_descriptors)
									<< "\n\n";
								msg << std::setw(5) << test_number << ": swapped " << i << " vs " << j
									<< " which took " << metrics << " yielded:\n\n";
								msg << BlockSort::blockSortToString(
												array_under_test, array_size, mid,
												descriptors_under_test, num_descriptors)
									<< "\n\n";

								std::cout << msg.str() << std::endl;
							}
							if (!arrays_passed) {
								msg << "ERROR: resultant does not match expected_array" << std::endl;
							}
							if (!descriptors_passed) {
								msg << "ERROR: resultant descriptors do not match expected descriptors" << std::endl;
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
			if (debug_verbose || output_metrics) {
				std::cout << " maximum number of moves was " << std::setw(5) << max_moves << std::endl;
			}
		}
	}
TEST_BLOCK_SORT_SWAP_BLOCKS_TEST_RETURN:
	return test_passed;
}


bool testBlockSortSort() {

	constexpr bool debug_verbose 			= false;
	constexpr bool announce_each_test_result= false;
	constexpr bool present_summary 			= true;

	int element_width = 4;
	int value_width = element_width-1;

	using data_type = long;

	bool test_passed = true;

	SimpleRandomizer randomizer;

	constexpr int num_test_runs = 1000;

//	index_t array_sizes[] = { 16 };
	index_t array_sizes[] = { 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };
//	index_t array_sizes[] = { 32, 33, 34, 35,
//							  36, 37, 38, 39, 40, 41,
//	  	  	  	  	  	  	  42, 43, 44, 45, 46, 47, 48, 49,  50
//	};
	int num_array_sizes = sizeof(array_sizes) / sizeof(index_t);

	SortMetrics total_metrics[num_array_sizes];

	double nlogn[num_array_sizes];
	double ave_compares[num_array_sizes];
	double ave_moves[num_array_sizes];

	BlockOrganizations block_organizations[] = {
//			BlockOrganizations::FULL_A0_BLOCK,
			BlockOrganizations::SYMMETRIC
	};

	int num_block_organizations = sizeof(block_organizations) / sizeof(BlockOrganizations);

	int 		a_increment	= 1;
	int copies_per_a_value	= 1;
	int 		b_increment = 1;
	int copies_per_b_value	= 1;

	for (int block_organization_i = 0;
			 block_organization_i != num_block_organizations;
			 block_organization_i++)
	{
		std::stringstream array_config_msg;

		BlockOrganizations block_organization = block_organizations[block_organization_i];
		if (debug_verbose || announce_each_test_result || present_summary) {
			array_config_msg.clear();
			array_config_msg.str("");
			std::string repeated_copies_string;
			if (copies_per_a_value > 0) {
				repeated_copies_string += std::to_string(copies_per_a_value);
			} else {
				repeated_copies_string += " ((array_size/2) / ";
				repeated_copies_string += std::to_string(-1*copies_per_a_value);
				repeated_copies_string += ") ";
			}
			array_config_msg << "Arrays configured with "
							 << repeated_copies_string
							 << " copies of each value"
							 << " using block_configuration "
							 << std::to_string(block_organization)
							 << std::endl;
			std::cout << array_config_msg.str();
		}

		for (int array_size_i = 0;
				 array_size_i != num_array_sizes;
				 array_size_i++)
		{
			index_t array_size = array_sizes[array_size_i];

			nlogn[array_size_i] = array_size * std::log2(array_size);

			index_t array_start = 0;
			index_t array_end 	= array_size-1;

			index_t block_size 	= static_cast<index_t>(std::sqrt(array_size/2));
			index_t num_blocks 	= array_size / block_size;
			index_t num_A_blocks= num_blocks / 2;
			index_t array_mid 	= num_A_blocks * block_size;

			data_type* reference_array[array_size];

			data_type 	a_value 	= 0;
			data_type 	b_value		= array_mid;

			//	negative values are used to indicate that the
			//	halves of the array should each have
			//	array_mid / (-copies_per_a_value)
			//	unique values

			int a_copy_count = copies_per_a_value;
			if (copies_per_a_value < 0) {
				a_copy_count = array_mid / (-copies_per_a_value);
			}
			int b_copy_count = copies_per_b_value;
			if (copies_per_b_value < 0) {
				b_copy_count = (array_end-array_mid+1)/(-copies_per_b_value);
			}

			int copy_count = a_copy_count;
			for (int i = 0; i < array_mid; i++) {
				reference_array[i] = new data_type(a_value);
				if (--copy_count == 0) {
					a_value 	+= a_increment;
					copy_count 	 = copies_per_a_value;
				}
			}
			copy_count = b_copy_count;
			for (int i = array_mid; i < array_size; i++)  {
				reference_array[i] = new data_type(b_value);
				if (--copy_count == 0) {
					b_value		+= b_increment;
					copy_count	 = copies_per_b_value;
				}
			}

			total_metrics[array_size_i] = SortMetrics(0,0);

			for (int test_number = 1; test_number <= num_test_runs; test_number++)
			{
				SortMetrics test_metrics(0,0);

				data_type* test_array[array_size];
				for (int i = 0; i != array_size; i++) {
					test_array[i] = reference_array[i];
				}


				std::stringstream msg;
				msg << "indices   :" << arrayIndicesToString(	array_size, array_mid,
																element_width)
					<< std::endl;

				msg << "randomized:"
					<< SortingUtilities::arrayElementsToString( test_array, array_size,
																value_width, element_width)
					<< std::endl;

				for (int i = 0; i != array_size; i++) {
					index_t r = randomizer.rand(i, array_size);
					data_type* temp = test_array[i];
					test_array[i] = test_array[r];
					test_array[r] = temp;
				}

				InsertionSort::sortPointersToObjects(test_array, array_mid);
				InsertionSort::sortPointersToObjects(&test_array[array_mid], array_end-array_mid+1);

				msg << "half sort :"
					<< SortingUtilities::arrayElementsToString( test_array, array_size,
																value_width, element_width)
					<< std::endl;

	//			std::cout << msg.str() << std::endl << std::endl;
	//			continue;

				std::unique_ptr<BlockDescriptor<data_type>[]> descriptors;
				int num_blocks = 0;
				switch(block_organization) {
				case BlockOrganizations::FULL_A0_BLOCK:
					num_blocks = createBlockDescriptors_A0_Full(
									test_array,
									array_start, array_mid, array_end,
									block_size, descriptors);
				break;
				case BlockOrganizations::SYMMETRIC:
					num_blocks = createBlockDescriptorsSymmetrically(
									test_array,
									array_start, array_mid, array_end,
									block_size, descriptors);
					break;
				default:
					std::cout << "Unknown block organization requested" << std::endl;
					test_passed = false;
					goto TEST_BLOCK_SORT_SORT_RETURN_LABEL;
				}
	//			std::cout << "block descriptors:\n";
	//			std::cout << blockDescriptorsToString(descriptors, block_size) << std::endl;
				msg << blockSortToString<data_type>(test_array, array_size, array_mid,
													descriptors, num_blocks, "randomized:",
													value_width, element_width) << std::endl;

				test_metrics = sortAndMergeBlocks(test_array, descriptors, num_blocks);
				total_metrics[array_size_i] += test_metrics;
				msg << blockSortToString<data_type>(test_array, array_size, array_mid,
													descriptors, num_blocks, "sorted    :",
													value_width, element_width) << std::endl;

				index_t mismatched_i;
				index_t mismatched_j;
				if (!SortingUtilities::isSorted(test_array, array_size,
											   mismatched_i, mismatched_j)) {
					if (!debug_verbose) {
						std::cout << msg.str() << std::endl;
					}
					test_passed = false;
					std::cout << "BlockSort failed ["
							  << mismatched_i << "] = "
							  << *reference_array[mismatched_i]
							  << " vs [" << mismatched_j << "] = "
							  << *reference_array[mismatched_j] << std::endl;
					goto TEST_BLOCK_SORT_SORT_RETURN_LABEL;
				}
				if (announce_each_test_result) {
					if (debug_verbose) {
						std::cout << msg.str();
					}
					std::cout << "test number " << std::setw(5) << test_number
							  << " took " 		<< test_metrics.compares
							  << " compares and " << test_metrics.assignments
							  << " moves" 		<< std::endl;
					if (debug_verbose) {
						//	there is a lot of info in the console if debugging,
						//	so add extra white space after the test
						std::cout << std::endl;
					}
				}
			}
			ave_moves[array_size_i] 	= static_cast<double>(total_metrics[array_size_i].assignments)/num_test_runs;
			ave_compares[array_size_i]	= static_cast<double>(total_metrics[array_size_i].compares)/num_test_runs;
			if (present_summary || announce_each_test_result || debug_verbose) {
				std::cout << "Array size " << std::setw(6) << array_size
						  << " repeated " << std::setw(6) << num_test_runs
						  << " ave time complexity "
						  << averageMetricsToString(total_metrics[array_size_i],num_test_runs);
				if (array_size_i) {
					int 	this_i	  	= array_size_i;
					int		prev_i		= array_size_i-1;
//					index_t this_size 	= array_sizes[this_i];
//					index_t prev_size 	= array_sizes[prev_i];
					double 	this_nlogn	= nlogn[this_i];
					double	prev_nlogn	= nlogn[prev_i];
					double	nlogn_ratio	= this_nlogn/prev_nlogn;
					double 	compares_ratio = 0;
					double	moves_ratio	= 0;
					if (ave_compares[prev_i] > 0.1)
						compares_ratio	= ave_compares[this_i] / ave_compares[prev_i];
					if (ave_moves[prev_i] > 0.1)
						moves_ratio		= ave_moves[this_i] / ave_moves[prev_i];
					int ratio_width = 5;
					int ratio_prec = 2;
		//			std::cout << "  ("  << std::setw(num_width) << this_size
		//					  << "log(" << std::setw(num_width) << this_size
		//					  << ")="   << std::setw(num_width) << std::setprecision(1) << std::fixed << this_nlogn
		//					  << ") / "
		//					  << "("    << std::setw(num_width) << prev_size
		//					  << "log(" << std::setw(num_width) << prev_size << ")="
		//					  << ")="   << std::setw(num_width) << std::setprecision(1) << std::fixed << prev_nlogn
		//					  << ") = " << std::setw(ratio_width) << std::setprecision(1) << std::fixed << nlogn_ratio;
					std::cout << "; nlog ratio "
							  << std::setw(ratio_width) << std::setprecision(ratio_prec) << std::fixed << nlogn_ratio;
					std::cout << " compares ratio "
							  << std::setw(ratio_width) << std::setprecision(ratio_prec) << std::fixed << compares_ratio
							  << " moves ratio "
							  << std::setw(ratio_width) << std::setprecision(ratio_prec) << std::fixed << moves_ratio;
				}
				std::cout << std::endl;
			}
		}
	}
TEST_BLOCK_SORT_SORT_RETURN_LABEL:
	return test_passed;
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

	OStreamState ostream_state;

	constexpr bool verbose 		= false;
	constexpr int value_width 	= 3;
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
						if (verbose) {
							std::cout << messages.str() << " !!! right block end index > array_size-1" << std::endl;
						}
						continue;
					}
					if (left_block_begin <0) {
						if (verbose) {
							std::cout << messages.str() << " !!! left block start index < 0" << std::endl;
						}
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
						BlockSort::swapBlockElementsOfEqualSize(array,
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
	std::cout << __FUNCTION__ << std::endl;
	std::cout << "NOT IMPLEMENTED\n";
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

