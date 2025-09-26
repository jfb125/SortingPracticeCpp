#ifndef SORTINGPRACTICEDATATYPES
#define SORTINGPRACTICEDATATYPES

#include <inttypes.h>
#include <climits>

//	these needs to be signed to handle cases where indices are reversed
typedef long array_size_t;
#define ARRAY_SIZE_T_MIN		0
#define ARRAY_SIZE_T_MAX		LONG_MAX

typedef long num_repetitions_t;
#define NUM_REPETITIONS_T_MIN	0
#define NUM_REPETITIONS_T_MAX	LONG_MAX

typedef unsigned long long total_compares_t;
#define TOTAL_COMPARES_T_MIN	0
#define TOTAL_COMPARES_T_MAX	ULLONG_MAX

typedef unsigned long long total_moves_t;
#define TOTAL_MOVES_T_MIN		0
#define TOTAL_MOVES_T_MAX		ULLONG_MAX

struct ComparesAndMoves {
	total_compares_t _compares;
	total_moves_t 	_moves;
	ComparesAndMoves()	: _compares(0), _moves(0) {}
	ComparesAndMoves(total_compares_t compares, total_moves_t moves) :
		_compares(compares), _moves(moves) {}
};

//#define NUMBER_OF_MOVES_PER_SWAP 3
#define _DEFAULT_ISSORTEDRESULT_IS_SORTED	false
#define _DEFAULT_ISSORTEDRESULT_MISMATCHED_INDEX_I	0
#define _DEFAULT_ISSORTEDRESULT_MISMATCHED_INDEX_J	0

std::ostream& operator<<(std::ostream &out, const ComparesAndMoves& object);
ComparesAndMoves& operator+=(ComparesAndMoves& lhs, const ComparesAndMoves &rhs);

class IsSortedResult {
public:
	bool _is_sorted;
	array_size_t _mismatched_index_i;
	array_size_t _mismatched_index_j;

	void clear(void) {
		_is_sorted = _DEFAULT_ISSORTEDRESULT_IS_SORTED;
		_mismatched_index_i =
				_DEFAULT_ISSORTEDRESULT_MISMATCHED_INDEX_I;
		_mismatched_index_j =
				_DEFAULT_ISSORTEDRESULT_MISMATCHED_INDEX_J;
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

#define MAX_ARRAY_SIZE_T	ULONG_MAX
#define MIN_ARRAY_SIZE_T	0

#endif
