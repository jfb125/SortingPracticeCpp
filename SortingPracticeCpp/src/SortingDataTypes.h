#ifndef SORTINGPRACTICEDATATYPES
#define SORTINGPRACTICEDATATYPES

#include <inttypes.h>
#include <climits>
#include <iostream>
#include <iomanip>

using array_size_t = long long;
constexpr array_size_t ARRAY_SIZE_T_MIN = 0;
constexpr array_size_t ARRAY_SIZE_T_MAX = LONG_LONG_MAX;
constexpr array_size_t MIN_ARRAY_SIZE_T	= ARRAY_SIZE_T_MIN;
constexpr array_size_t MAX_ARRAY_SIZE_T	= ARRAY_SIZE_T_MAX;

using num_repetitions_t = long;
constexpr num_repetitions_t NUM_REPETITIONS_T_MIN = 0;
constexpr num_repetitions_t NUM_REPETITIONS_T_MAX = LONG_MAX;

using total_compares_t = long long;
constexpr total_compares_t TOTAL_COMPARES_T_MIN = 0;
constexpr total_compares_t TOTAL_COMPARES_T_MAX = LONG_LONG_MAX;

using total_moves_t = long long;
constexpr total_moves_t	TOTAL_MOVES_T_MIN = 0;
constexpr total_moves_t TOTAL_MOVES_T_MAX = LONG_LONG_MAX;


constexpr total_moves_t num_assignments_per_swap = 3;

/*	******************************************************************	*/
/*	class for storing the metrics that evaluate a sorting algorithm		*/
/*	******************************************************************	*/

struct SortMetrics {
	total_compares_t 	compares;
	total_moves_t 		assignments;
	SortMetrics()	: compares(0), assignments(0) {}
	SortMetrics(total_compares_t compares, total_moves_t moves) :
		compares(compares), assignments(moves) {}
};

//#define NUMBER_OF_MOVES_PER_SWAP 3
constexpr bool 			DEFAULT_ISSORTEDRESULT_IS_SORTED 			= false;
constexpr array_size_t 	DEFAULT_ISSORTEDRESULT_MISMATCHED_INDEX_I 	= 0;
constexpr array_size_t 	DEFAULT_ISSORTEDRESULT_MISMATCHED_INDEX_J 	= 0;


constexpr int compares_width 	= 8;
constexpr int assignments_width = 8;
#define compares_str 	"compares"
#define assignments_str	"assignments"
std::ostream& operator<<(std::ostream &out, const SortMetrics& object);
SortMetrics& operator+=(SortMetrics& lhs, const SortMetrics &rhs);
std::string averageMetricsToString(	const SortMetrics &metrics,
									num_repetitions_t num_tests,
									int precision = 1, int width =8);

class IsSortedResult {
public:
	bool _is_sorted;
	array_size_t _mismatched_index_i;
	array_size_t _mismatched_index_j;

	void clear(void) {
		_is_sorted = DEFAULT_ISSORTEDRESULT_IS_SORTED;
		_mismatched_index_i =
				DEFAULT_ISSORTEDRESULT_MISMATCHED_INDEX_I;
		_mismatched_index_j =
				DEFAULT_ISSORTEDRESULT_MISMATCHED_INDEX_J;
	}

	IsSortedResult() {
		clear();
	}

	IsSortedResult(bool is_sorted, array_size_t i, array_size_t j) :
		_is_sorted(is_sorted),
		_mismatched_index_i(i),
		_mismatched_index_j(j)
		{}

	IsSortedResult(const IsSortedResult &other) {
		_is_sorted = other._is_sorted;
		_mismatched_index_i = other._mismatched_index_i;
		_mismatched_index_j = other._mismatched_index_j;
	}

	IsSortedResult& operator=(const IsSortedResult &other) {
		if (this != &other) {
			_is_sorted = other._is_sorted;
			_mismatched_index_i = other._mismatched_index_i;
			_mismatched_index_j = other._mismatched_index_j;
		}
		return *this;
	}
};


/*	******************************************************************	*/
/*				wrapper around the user's data type						*/
/*	******************************************************************	*/

template <typename T>
class SortingDataType {
public:
	T 			value;
	long long 	index;
	int 		index_width;
//	static std::ostream operator<<(const SortingDataType<T> &object);
	std::string	(*to_string)(const T&);

	/*	constructors and copy / move operators	*/

	SortingDataType() {
		value 		= T();
		index 		= 0;
		index_width	= 8;
		to_string	= nullptr;
	}

	SortingDataType(const SortingDataType &other) {
		if (this != &other) {
			value 		= other.value;
			index 		= other.index;
			index_width	= other.index_width;
			to_string	= other.to_string;
		}
	}

	SortingDataType &operator=(const SortingDataType &other) {
		if (this != &other) {
			value 		= other.value;
			index 		= other.index;
			index_width	= other.index_width;
			to_string	= other.to_string;
		}
		return *this;
	}

	SortingDataType(SortingDataType &&other) {
		if (this != &other) {
			value 		= other.value;
			index 		= other.index;
			index_width	= other.index_width;
			to_string	= other.to_string;
			other.to_string = nullptr;
		}
	}

	SortingDataType& operator=(SortingDataType &&other) {
		if (this != &other) {
			value 		= other.value;
			index 		= other.index;
			index_width	= other.index_width;
			to_string	= other.to_string;
			other.to_string = nullptr;
		}
		return *this;
	}

	~SortingDataType() {}

	/*	individual object assignment operators	*/

	SortingDataType &operator=(const T val) {
		value = val;
		return *this;
	}

	SortingDataType &operator=(const array_size_t indx) {
		index = indx;
		return *this;
	}

	SortingDataType &operator=(std::string (*to_str)(const T&)) {
		to_string = to_str;
	}

	/*		individual setter methods	*/

	SortingDataType &set(const T val) {
		value = val;
		return *this;
	}

	SortingDataType &set(const array_size_t indx) {
		index = indx;
		return *this;
	}

	SortingDataType &set(std::string(*to_str)(const T&)) {
		to_string = to_str;
		return *this;
	}

	/*	comparing the underlying element	*/

		/*	fundamental operations	*/

	bool operator==(const SortingDataType &other) const {
		return value == other.value;
	}

	bool operator<(const SortingDataType &other) const {
		return value < other.value;
	}

		/*	derived operations	*/

	bool operator<=(const SortingDataType &other) const {
		return *this < other || *this == other;
	}

	bool operator>(const SortingDataType &other) const {
		return !(*this < other || *this == other);
	}

	bool operator>=(const SortingDataType &other) const {
		return !(*this < other);
	}

	bool operator!=(const SortingDataType &other) const {
		return !(*this == other);
	}
	std::ostream& operator<<(std::ostream&out);
	/*	output	*/
};

template <typename T>
std::ostream& SortingDataType<T>::operator<<(
			std::ostream &out) {
	out << value;
	return out;
}

std::ostream& operator<<(std::ostream &out, const SortingDataType<int> &object);
std::ostream& operator<<(std::ostream &out, const SortingDataType<char> &object);
std::ostream& operator<<(std::ostream &out, const SortingDataType<std::string> &object);

//	assigns the values in 'values' to the .value member in each dst
//	until all 'dst_size' members have been initialized.  .index = 0
//	b/c assigning indices is typically done after randomizing
//	Note that in the case of dst_size is not an integer multiple
//	of count_per_value, and dst_size is not an integer multiple
//	of val_size, and val_size is not an integer multiple of counts_per_value
//	for example	(dst_size = 13, val_size = 3, count_per_value = 2
//	The result will not be	{ A, A, A, A, B, B, B, B, B, C, C, C, C }
//	But instead				{ A, A, B, B, C, C, A, A, B, B, C, C, A }

namespace SortingDataTypes {

	constexpr int default_index_width = 5;

	template <typename T>
	array_size_t assignValues(
			SortingDataType<T> *dst, array_size_t dst_size,
			T *values, array_size_t val_size,
			array_size_t count_per_value = 1,
			int index_width = default_index_width,
			std::string (*to_str)(const T&) = nullptr)
	{
		array_size_t repetition_counter = count_per_value;
		array_size_t src_i = 0;
		for (array_size_t i = 0; i < dst_size; i++) {
			dst[i].value 		= values[src_i];
			dst[i].index 		= 0;
			dst[i].index_width 	= index_width;
			dst[i].to_string	= to_str;
			if (--repetition_counter == 0) {
				repetition_counter = count_per_value;
				if (++src_i == val_size) {
					src_i = 0;
				}
			}
		}
		return dst_size;
	}

	template <typename T>
	array_size_t assignSequenceNumbers(	SortingDataType<T> *array,
										array_size_t size,
										array_size_t start = 0) {
		for (array_size_t i = start; i != start+size; i++) {
			array[i].index = i;
		}

		return size;
	}

	template <typename T>
	bool isSorted(SortingDataType<T> *array, array_size_t size) {

		for (array_size_t i = 1; i != size; i++) {
			if (array[i-1].value > array[i].value) {
				return false;
			}
		}
		return true;
	}

	//	This only checks to see if identical elements in the array
	//	have sequentially ascending 'index' numbers
	template <typename T>
	bool isStable(SortingDataType<T> *array, array_size_t size) {

		for (array_size_t i = 1; i != size; i++) {
			if (array[i-1].value == array[i].value &&
			    array[i-1].index >= array[i].index) {
					return false;
			}
		}
		return true;
	}
}

bool sortingDataTypesTest();

/*	*************************************************************************	*/
/*						Generating specific data types							*/
/*	*************************************************************************	*/

std::string next_string(std::string &current, bool upper_then_lower = false);
array_size_t generateStrings(std::string *dst,
							 array_size_t size,
							 std::string &first_string,
							 std::string (*next_string)(std::string&, bool));

#endif
