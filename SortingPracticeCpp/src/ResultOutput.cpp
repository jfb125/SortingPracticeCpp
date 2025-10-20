/*
 * ResultOutputParameters.cpp
 *
 *  Created on: Jun 21, 2025
 *      Author: joe
 */

#include "ResultOutput.h"

// TestResultMember is defined in the header file that contains
//	the class OneTestResult, which has the members

#define MIN_TEST_RESULT_INTEGER static_cast<int>(MIN_TEST_RESULT_MEMBER)
#define MAX_TEST_RESULT_INTEGER static_cast<int>(MAX_TEST_RESULT_MEMBER)

constexpr char test_result_table_header[] = PRINT_TEST_RESULT_TABLE_HEADER;
constexpr char average_compares_string[] = "avg cmp";
constexpr char average_moves_string[] = "avg mov";
constexpr char colon_separator[] =  ": ";
constexpr char space_separator[] = "  ";
constexpr char slash_separator[] = " / ";
constexpr int compare_moves_fudge_factor = 4;
constexpr int average_compares_strlen = 7 + compare_moves_fudge_factor;
constexpr int average_moves_strlen = 7 + compare_moves_fudge_factor;
constexpr int comma_separator_strlen = 2;
constexpr int colon_separator_strlen = 2;
constexpr int space_separator_strlen = 2;
constexpr int slash_separator_strlen = 3;

class ArraySizeData {
public:
	int num_sizes;
	array_size_t min_size;
	array_size_t max_size;
	array_size_t *sizes;

	ArraySizeData() {
		num_sizes = 0;
		min_size = MAX_ARRAY_SIZE_T;
		max_size = MIN_ARRAY_SIZE_T;
		sizes = nullptr;
	}

	std::string str() {
		std::stringstream retval;
		retval << "size list contains " << num_sizes << " sizes "
			   << " from " << min_size
			   << " to "  << max_size
			   << ": ";
		if (sizes) {
			for (int i = 0; i != num_sizes-1; i++) {
				retval << sizes[i] << ", ";
			}
			retval << sizes[num_sizes-1];
		} else {
			retval << "<nullptr>";
		}
		return retval.str();
	}
};


/* ****************************************************************************	*/
/* ****************************************************************************	*/
/*								forward declaration								*/
/* ****************************************************************************	*/
/* ****************************************************************************	*/
#if 0
bool isValid(TestResultMember object);

TestResultMember& operator++(TestResultMember&);
TestResultMember  operator++(TestResultMember&, int unused);

bool operator==(TestResultMember u, TestResultMember v);
bool operator<(TestResultMember u, TestResultMember v);
bool operator<=(TestResultMember u, TestResultMember v);
bool operator>(TestResultMember u, TestResultMember v);
bool operator>=(TestResultMember u, TestResultMember v);
bool operator!=(TestResultMember u, TestResultMember v);

bool isSameLine(OneTestResult* result, SortAlgorithms &algorithm, ArrayComposition &composition, InitialOrdering &ordering);
void printRowStart(SortAlgorithms &algorithm, ArrayComposition &composition, InitialOrdering &ordering,
					int algorithm_strlen, int composition_strlen, int ordering_strlen);

void printRowsAlgorithm_ColumnsSize_CellsAverages(OneTestResult** result,
												  int num_test_results,
												  bool (*isLess)(OneTestResult*, OneTestResult*));

int	getstrlenAlgorithm(OneTestResult ** result, int num_tests);
int getstrlenComposition(OneTestResult** result, int num_tests);
int getstrlenOrdering(OneTestResult** result, int num_tests);
int getMaxDigitsCompares(OneTestResult** result, int num_tests);
int getMaxDigitsMoves(OneTestResult** result, int num_tests);
int getMaxDigitsArraySize(OneTestResult** result, int num_tests);
void getArraySizeData(ArraySizeData &dst, OneTestResult** results, int num_test_results);

bool isLess_AlgorithmCompostionOrderingSize(OneTestResult* u, OneTestResult* v);
bool isLess_CompostionOrderingAlgorithmSize(OneTestResult* u, OneTestResult* v);
void sortResultsArray(OneTestResult** results, int num_tests,
					  bool (*isULessThanV)(OneTestResult* u, OneTestResult* v));


/* ****************************************************************************	*/
/* ****************************************************************************	*/
/*						different table output functions						*/
/* ****************************************************************************	*/
/* ****************************************************************************	*/

void printTestResults(OneTestResult** results, int num_test_results) {

	// print the title of the table
	std::cout << test_result_table_header << std::endl
			  << "Sort Test Results" << std::endl
			  << test_result_table_header << std::endl;
	bool (*isLess)(OneTestResult* u, OneTestResult*v);
	isLess = isLess_CompostionOrderingAlgorithmSize;
	printRowsAlgorithm_ColumnsSize_CellsAverages(results, num_test_results, isLess);
}

void terseDump(OneTestResult** results, int num_test_results) {

	for (int i = 0; i != num_test_results-1; i++) {
		std::cout << std::setw(2) << i << ": "
				  << results[i]->_algorithm << ", "
				  << results[i]->_composition << ", "
				  << results[i]->_ordering << ", "
				  << results[i]->_size
				  << std::endl;
	}
	int i = num_test_results-1;
	std::cout << std::setw(2) << i << ": "
			  << results[i]->_algorithm << ", "
			  << results[i]->_composition << ", "
			  << results[i]->_ordering << ", "
			  << results[i]->_size;
}

/* ****************************************************************************	*/
/* ****************************************************************************	*/
/*							different table styles								*/
/* ****************************************************************************	*/
/* ****************************************************************************	*/

void printRowsAlgorithm_ColumnsSize_CellsAverages(OneTestResult** results,
												  int num_test_results,
												  bool (*isLess)(OneTestResult*, OneTestResult*)) {

	std::ios_base::fmtflags _flags = std::cout.flags();

//	terseDump(results, num_test_results);
//	std::cout << std::endl;

	sortResultsArray(results, num_test_results, isLess);

	ArraySizeData size_data;
	getArraySizeData(size_data, results, num_test_results);

	int max_algorithm_strlen = getstrlenAlgorithm(results, num_test_results);
	int max_composition_strlen = getstrlenComposition(results, num_test_results);
	int max_ordering_strlen = getstrlenOrdering(results, num_test_results);
	int max_digits_compares = getMaxDigitsCompares(results, num_test_results);
	int max_digits_moves = getMaxDigitsMoves(results, num_test_results);
	int compares_strlen	= max_digits_compares;
	int moves_strlen	= max_digits_moves;

	if (compares_strlen < average_compares_strlen) {
		compares_strlen = average_compares_strlen;
	}
	if (moves_strlen < average_moves_strlen) {
		moves_strlen = average_moves_strlen;
	}

	SortAlgorithms current_sort = SortAlgorithms::SORT_ALGORITHMS_COUNT;
	ArrayComposition current_composition = ArrayCompositions::ARRAY_COMPOSITIONS_COUNT;
	InitialOrdering current_ordering = InitialOrderings::INITIAL_ORDERINGS_COUNT;

	int array_size_header_width =
			max_algorithm_strlen + max_composition_strlen + max_ordering_strlen +
			comma_separator_strlen + comma_separator_strlen + colon_separator_strlen;

	std::cout << std::setw(array_size_header_width) << std::right << "Array sizes: ";
	for (int i = 0; i != size_data.num_sizes; i++) {
		std::cout << std::setw(compares_strlen + slash_separator_strlen) << size_data.sizes[i];
		std::cout << std::setw(moves_strlen) << "" << space_separator;
	}
	std::cout << std::endl;

	std::stringstream rep_str;
	rep_str << "number of repetitions: " << results[0]->_sort_metrics._num_repetitions;
	std::cout << std::setw(array_size_header_width) << std::left << rep_str.str();
	for (int i = 0; i != size_data.num_sizes; i++) {
		std::cout << std::setw(compares_strlen) << std::right << average_compares_string
				  << std::setw(0) << slash_separator
				  << std::setw(moves_strlen) << std::left << average_moves_string
				  << space_separator;
	}
	std::cout << std::endl;

	SortAlgorithms	first_sort_in_group = results[0]->_algorithm;
	for (int i = 0; i != num_test_results; i++) {
		if(!isSameLine(results[i], current_sort, current_composition, current_ordering)) {
			current_sort = results[i]->_algorithm;
			current_composition = results[i]->_composition;
			current_ordering = results[i]->_ordering;
			if (i != 0) {
				std::cout << std::endl;
				if (current_sort == first_sort_in_group) {
					std::cout << std::endl;
				}
			}
			printRowStart(current_sort, current_composition, current_ordering,
						  max_algorithm_strlen, max_composition_strlen, max_ordering_strlen);
		}
		std::cout << std::setw(compares_strlen) << std::right
				  << results[i]->_sort_metrics.comparesStr()
				  << std::setw(0) << slash_separator
				  << std::setw(moves_strlen) << std::right
				  << results[i]->_sort_metrics.movesStr()
				  << space_separator;
		// HACK - get rid of '-2' and "  "
	}
	std::cout << std::endl << std::endl;

	if (size_data.sizes) {
		delete size_data.sizes;
		size_data.sizes = nullptr;
	}

	// restore io flags
	std::cout.flags(_flags);
}


void printRowStart(SortAlgorithms &algorithm, ArrayComposition &composition, InitialOrdering &ordering,
					int algorithm_strlen, int composition_strlen, int ordering_strlen) {

	std::cout << std::setw(algorithm_strlen) << std::left << algorithm << space_separator
			  << std::setw(composition_strlen) << std::left << composition << space_separator
			  << std::setw(ordering_strlen) << std::left << ordering << colon_separator;
}


/* ****************************************************************************	*/
/* ****************************************************************************	*/
/*							supporting functions								*/
/* ****************************************************************************	*/
/* ****************************************************************************	*/

int	getstrlenAlgorithm(OneTestResult** results, int num_tests) {

	int max_len = 0;

	for (int i = 0; i != num_tests; i++) {
		std::string algstr = toString(results[i]->_algorithm);
		int len = strlen(algstr.c_str());
		if (len > max_len) {
			max_len = len;
		}
	}
	return max_len;
}

int getstrlenComposition(OneTestResult** result, int num_tests) {

	int max_len = 0;

	for (int i = 0; i != num_tests; i++) {
		int len = result[i]->_composition.str().length();
		if (len > max_len) {
			max_len = len;
		}
	}
	return max_len;
}

int getstrlenOrdering(OneTestResult** result, int num_tests) {

	int max_len = 0;

	for (int i = 0; i != num_tests; i++) {
		int len = result[i]->_ordering.str().length();
		if (len > max_len) {
			max_len = len;
		}
	}
	return max_len;
}

int getMaxDigitsCompares(OneTestResult** result, int num_tests) {

	int max_digits = 1;

	// determine the largest number of compares
	total_compares_t max_compares = 0;
	for (int i = 0; i != num_tests; i++) {
		// trying to improve readability
		total_compares_t this_compares = result[i]->_sort_metrics.compares;
		if (this_compares > max_compares) {
			max_compares = this_compares;
		}
	}

	// determine the number of decimal digits
	//	in the largest number of compares
	while (max_compares != 0) {
		max_digits++;
		max_compares /= 10;
	}

	return max_digits;
}

int getMaxDigitsMoves(OneTestResult** result, int num_tests) {

	int max_digits = 1;

	// determine the largest number of compares
	total_moves_t max_moves = 0;
	for (int i = 0; i != num_tests; i++) {
		// trying to improve readability
		total_moves_t this_moves = result[i]->_sort_metrics.assignments;
		if (this_moves > max_moves) {
			max_moves = this_moves;
		}
	}

	// determine the number of decimal digits
	//	in the largest number of compares
	while (max_moves != 0) {
		max_digits++;
		max_moves /= 10;
	}

	return max_digits;
}

int getMaxDigitsArraySize(OneTestResult** result, int num_tests) {

	int max_array_digits = 1;

	// determine largest array size
	array_size_t max_array_size = 0;
	for (int i = 0; i != num_tests; i++) {
		// improve readability
		array_size_t this_size = (*result)[i]._size;
		if (this_size > max_array_size) {
			max_array_size = this_size;
		}
	}

	// determine num of decimal digits of max array size

	while (max_array_size) {
		max_array_digits++;
		max_array_size /= 10;
	}
	return max_array_digits;
}

void getArraySizeData(ArraySizeData &dst,
					  OneTestResult** results, int num_test_results) {

	// linked list is used to identify unique values of '_size' in results
	struct ArraySizeNode {
		array_size_t m_size;
		ArraySizeNode *m_next;
		ArraySizeNode *m_prev;
		ArraySizeNode(array_size_t size) {
			m_size = size;
			m_next = nullptr;
			m_prev = nullptr;
		}
	};

	dst.num_sizes = 0;
	dst.min_size = MAX_ARRAY_SIZE_T;
	dst.max_size = MIN_ARRAY_SIZE_T;
	dst.sizes = nullptr;

	ArraySizeNode *head = nullptr;
	ArraySizeNode *tail = nullptr;
	ArraySizeNode *seeker = nullptr;

	for (int i = 0; i != num_test_results; i++) {
		seeker = head;
		array_size_t this_size = results[i]->_size;
		// determine if this_size represents a new min or max
		if (this_size < dst.min_size) {
			dst.min_size = this_size;
		}
		if (this_size > dst.max_size) {
			dst.max_size = this_size;
		}
		// try to find 'this_size' in the list of known sizes
		if (seeker == nullptr) {
			// if the list is empty, enter this_size into head
			head = new ArraySizeNode(this_size);
			tail = head;
			dst.num_sizes++;
		} else {
			while (seeker != nullptr) {
				// if the value is found in the list,
				//	break out to the for loop to look at next result
				if (seeker->m_size == this_size) {
					break;
				}
				// the current node does not contain the value
				if (seeker->m_next == nullptr) {
					// the current node is the last value
					// create a new node with unique this_value
					seeker->m_next = new ArraySizeNode(this_size);
					seeker->m_next->m_prev = seeker;
					tail = seeker->m_next;
					//  increment the number of unique values
					dst.num_sizes++;
				}	// if seeker->m_next == nullptr
				seeker = seeker->m_next;
			}	// while seeker != nullptr
		}	// else list is not empty
	}	// for i <= num_sizes

	// copy the unique size values that were stored in the linked list
	dst.sizes = new array_size_t[dst.num_sizes];
	seeker = head;
	int size_index = 0;
	while (seeker) {
		dst.sizes[size_index++] = seeker->m_size;
		seeker = seeker->m_next;
	}

	// sort the unique size values that are sorted in the linked list
	for (int last_unsorted = dst.num_sizes-1; last_unsorted != 0; last_unsorted--) {
		bool was_swap = false;
		for (int j = 0; j < last_unsorted; j++) {
			if (dst.sizes[j] > dst.sizes[j+1]) {
				array_size_t tmp = dst.sizes[j];
				dst.sizes[j] = dst.sizes[j+1];
				dst.sizes[j+1] = tmp;
				was_swap = true;
			}
		}
		if (!was_swap)
			break;
	}

	// free the list
	while (tail->m_prev) {
		tail = tail->m_prev;
		delete tail->m_next;
		tail->m_next = nullptr;
	}
	delete tail;
	tail = nullptr;
}


/* ****************************************************************************	*/
/* ****************************************************************************	*/
/*							sorting the array									*/
/* ****************************************************************************	*/
/* ****************************************************************************	*/

bool isLess_AlgorithmCompostionOrderingSize(OneTestResult* u, OneTestResult* v) {

	if (u->_algorithm < v->_algorithm)		return true;
	if (u->_algorithm > v->_algorithm)		return false;
	if (u->_composition < v->_composition)	return true;
	if (u->_composition > v->_composition)	return false;
	if (u->_ordering < v->_ordering)		return true;
	if (u->_ordering > v->_ordering)		return false;
	if (u->_size < v->_size)				return true;
											return false;
}

bool isLess_CompostionOrderingAlgorithmSize(OneTestResult* u, OneTestResult* v) {

	if (u->_composition < v->_composition)	return true;
	if (u->_composition > v->_composition)	return false;
	if (u->_ordering < v->_ordering)		return true;
	if (u->_ordering > v->_ordering)		return false;
	if (u->_algorithm < v->_algorithm)		return true;
	if (u->_algorithm > v->_algorithm)		return false;
	if (u->_size < v->_size)				return true;
											return false;
}
/*
 *	bubble sort with  !was_swap  optimization
 */
void sortResultsArray(OneTestResult** results, int num_tests,
					  bool (*isULessThanV)(OneTestResult* u, OneTestResult* v)) {

	if (results == nullptr)
		return;
	if (num_tests <= 1)
		return;

	for (int closest_unordered = 0;
		 closest_unordered != num_tests;
		 closest_unordered++) {
		bool was_swap = false;
		for (int bubble_up = num_tests-1;
			 bubble_up > closest_unordered; bubble_up--) {
			// if the element earlier in the array is not less than this element
			if (!isULessThanV(results[bubble_up-1], results[bubble_up])) {
				OneTestResult *tmp = results[bubble_up];
				results[bubble_up] = results[bubble_up-1];
				results[bubble_up-1] = tmp;
				was_swap = true;
			}
		}
		if (!was_swap)
			break;
	}
}


/* ****************************************************************************	*/
/* ****************************************************************************	*/
/*							enum TestResultMember operators						*/
/* ****************************************************************************	*/
/* ****************************************************************************	*/

bool isSameLine(OneTestResult* result,
				SortAlgorithms &algorithm,
				ArrayComposition &composition,
				InitialOrdering &ordering) {

		if (result->_algorithm != algorithm)		return false;
		if (result->_composition != composition) 	return false;
		if (result->_ordering != ordering)			return false;
		return true;
}


/* ****************************************************************************	*/
/* ****************************************************************************	*/
/*							enum TestResultMember operators						*/
/* ****************************************************************************	*/
/* ****************************************************************************	*/

	using Format = ResultParameterFormat;
	Format ResultFormat::algorithm(void) const {
		return m_algorithm;
	}
	Format& ResultFormat::algorithm(Format &format) {
		m_algorithm = format;
		return m_algorithm;
	}
	Format ResultFormat::composition(void) const {
		return m_composition;
	}
	Format& ResultFormat::composition(Format &format) {
		m_composition = format;
		return m_composition;
	}
	Format ResultFormat::ordering(void) const {
		return m_ordering;
	}
	Format& ResultFormat::ordering(Format &format) {
		m_ordering = format;
		return m_ordering;
	}
	Format ResultFormat::sort(void) const {
		return m_sort;
	}
	Format& ResultFormat::sort(Format &format) {
		m_sort = format;
		return format;
	}

	ResultFormat::ResultFormat() {
		m_algorithm = DEFAULT_ALGORITHM_FORMAT_SPECIFIER;
		m_composition = DEFAULT_COMPOSITION_FORMAT_SPECIFIER;
		m_ordering = DEFAULT_ORDERING_FORMAT_SPECIFIER;
		m_sort = DEFAULT_SORT_FORMAT_SPECIFIER;
	}

	ResultFormat::ResultFormat(
			Format algorithm,
			Format composition,
			Format ordering,
			Format sort) :
					m_algorithm(algorithm),
					m_composition(composition),
					m_ordering(ordering),
					m_sort(sort) {}

	ResultFormat::ResultFormat(const ResultFormat &other) {

		m_algorithm = other.m_algorithm;
		m_composition = other.m_composition;
		m_ordering = other.m_ordering;
		m_sort = other.m_sort;
	}

	ResultFormat& ResultFormat::operator=(const ResultFormat &other) {

		if (this != &other) {
			m_algorithm = other.m_algorithm;
			m_composition = other.m_composition;
			m_ordering = other.m_ordering;
			m_sort = other.m_sort;
		}
		return *this;
	}


/* ****************************************************************************	*/
/* ****************************************************************************	*/
/*							enum TestResultMember operators						*/
/* ****************************************************************************	*/
/* ****************************************************************************	*/

bool isValid(TestResultMember object) {
	return 	(static_cast<int>(object) >= MIN_TEST_RESULT_INTEGER) &&
			(static_cast<int>(object) <= MAX_TEST_RESULT_INTEGER);
}

TestResultMember& operator++(TestResultMember &value) {

	if (!isValid(value)) {
		value = MAX_TEST_RESULT_MEMBER;
	}
	int currently = static_cast<int>(value);
	if (currently >= MIN_TEST_RESULT_INTEGER &&
		currently < MAX_TEST_RESULT_INTEGER) {
		currently++;
	} else {
		currently = MAX_TEST_RESULT_INTEGER;
	}

	value = static_cast<TestResultMember>(currently);
	return value;
}

TestResultMember operator++(TestResultMember &value, int unused) {

	TestResultMember retval = value;

	if (isValid(value)) {
		++value;
	} else {
		value = MAX_TEST_RESULT_MEMBER;
	}

	return retval;
}

//	explicitly defined comparison operators

bool operator==(TestResultMember u, TestResultMember v) {

	if (!isValid(u))	return false;
	if (!isValid(v))	return false;
	return static_cast<int>(u) == static_cast<int>(v);
}

bool operator<(TestResultMember u, TestResultMember(v)) {

	if (!isValid(u))	return false;
	if (!isValid(v))	return false;
	return static_cast<int>(u) < static_cast<int>(v);
}

//	derived comparison operators

bool operator<=(TestResultMember u, TestResultMember v) {

	if (!isValid(u))	return false;
	if (!isValid(v))	return false;
	return (u < v) || (u == v);
}

bool operator>(TestResultMember u, TestResultMember v) {

	if (!isValid(u))	return false;
	if (!isValid(v))	return false;
	return !(u == v || u < v);
}

bool operator>=(TestResultMember u, TestResultMember v) {

	if (!isValid(u))	return false;
	if (!isValid(v))	return false;
	return !(u < v);
}

bool operator!=(TestResultMember u, TestResultMember v) {

	if (!isValid(u))	return false;
	if (!isValid(v))	return false;
	return !(u == v);
}
#endif
