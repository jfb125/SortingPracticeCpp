/*
 * ResultFormat.h
 *
 *  Created on: Oct 24, 2025
 *      Author: Joe Baker
 *      It is intended that this enum class can be used to pass information
 *        to future expansion of the ResultOutput.h functionality
 */

#ifndef RESULTFORMAT_H_
#define RESULTFORMAT_H_

#define ARRAY_SIZE_COLUMN_WIDTH 20

class ResultFormat {
public:
	enum class ResultOutputPositions {
		DO_NOT_PRINT,
		TABLE_ROWS,
		TABLE_COLUMNS,
		TABLE_TITLE
	};

private:
	ResultOutputPositions algorithm_position;
	ResultOutputPositions array_composition_position;
	ResultOutputPositions ordering_position;
	ResultOutputPositions sort_type_position;
public:

	ResultFormat() :
		algorithm_position(ResultOutputPositions::DO_NOT_PRINT),
		array_composition_position(ResultOutputPositions::DO_NOT_PRINT),
		ordering_position(ResultOutputPositions::DO_NOT_PRINT),
		sort_type_position(ResultOutputPositions::DO_NOT_PRINT){
	}

	ResultFormat( ResultOutputPositions x_alg,
				  ResultOutputPositions x_comp,
				  ResultOutputPositions x_ord,
				  ResultOutputPositions x_sort) :
					  algorithm_position(x_alg),
					  array_composition_position(x_comp),
					  ordering_position(x_ord),
					  sort_type_position(x_sort) {}

	ResultFormat(const ResultFormat &other) {
		algorithm_position 			= other.algorithm_position;
		array_composition_position	= other.array_composition_position;
		ordering_position			= other.ordering_position;
		sort_type_position			= other.sort_type_position;
	}

	ResultFormat& operator=(const ResultFormat &other) {
		if (this != &other) {
			algorithm_position 			= other.algorithm_position;
			array_composition_position	= other.array_composition_position;
			ordering_position			= other.ordering_position;
			sort_type_position			= other.sort_type_position;
		}
		return *this;
	}

	ResultOutputPositions algorithm(void) const {
		return algorithm_position;
	}
	ResultFormat& algorithm(const ResultOutputPositions &algo) {
		algorithm_position = algo;
		return *this;
	}
	ResultOutputPositions composition(void) const {
		return array_composition_position;
	}
	ResultFormat& composition(const ResultOutputPositions &comp) {
		array_composition_position = comp;
		return *this;

	}
	ResultOutputPositions ordering(void) const {
		return ordering_position;
	}
	ResultFormat& ordering(const ResultOutputPositions &ordrng) {
		ordering_position = ordrng;
		return *this;
	}
	ResultOutputPositions sort(void) const {
		return sort_type_position;
	}
	ResultFormat& sort(const ResultOutputPositions &srt) {
		sort_type_position = srt;
		return *this;
	}
};


#endif /* RESULTFORMAT_H_ */
