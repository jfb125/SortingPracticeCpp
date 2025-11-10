/*
 * InitialOrdering.h
 *
 *  Created on: Apr 16, 2025
 *      Author: joe
 */

#ifndef INITIALORDERING_H_
#define INITIALORDERING_H_

#include <iostream>
#include <sstream>
#include <string>
#include <inttypes.h>

//#include "SimpleRandomizer.h"
#include "SortingDataTypes.h"

/*	**************************************************************************	*/
/*	**************************************************************************	*/
/*						ordering of the array prior to sorting					*/
/*	**************************************************************************	*/
/*	**************************************************************************	*/

enum class InitialOrderings {
	IN_RANDOM_ORDER,
	IN_REVERSE_ORDER,	// 16 chars width
	FEW_CHANGES,
	NO_CHANGES,
};
const int max_initial_orderings_strlen = 16;
namespace std {
	std::string to_string(InitialOrderings const&);
}
int	strlen(InitialOrderings const&);
std::ostream& operator<<(std::ostream& out, InitialOrderings const &);

bool requiresNumOutOfPlace(InitialOrderings const&);
bool requiresSeed(InitialOrderings const&);

/*
 * 	These are necessary for ResultOutput.h to sort the table of results
 */
bool operator==(const InitialOrderings u, const InitialOrderings v);
bool operator< (const InitialOrderings u, const InitialOrderings v);
bool operator<=(const InitialOrderings u, const InitialOrderings v);
bool operator> (const InitialOrderings u, const InitialOrderings v);
bool operator>=(const InitialOrderings u, const InitialOrderings v);
bool operator!=(const InitialOrderings u, const InitialOrderings v);

constexpr int initial_ordering_default_num_out_of_place = 0;

class InitialOrdering {
private:
	InitialOrderings m_ordering;
	// these are private to manage initialization state
	//	when user is accessing 'num_out_of_place'
	bool 			m_num_out_of_place_is_initialized;
	array_size_t 	m_num_out_of_place;

public:

	InitialOrderings ordering(void) const {
		return m_ordering;
	}
	InitialOrdering& ordering(InitialOrderings& order) {
		m_ordering = order;
		return *this;
	}

	array_size_t 		num_out_of_place(void) const {
		return m_num_out_of_place;
	}
	InitialOrdering& 	num_out_of_place(array_size_t oop) {
		m_num_out_of_place = oop;
		m_num_out_of_place_is_initialized = true;
		return *this;
	}

	bool num_out_of_place_is_initialized(void) const {
		return m_num_out_of_place_is_initialized;
	}

	InitialOrdering() {
		m_ordering 							= InitialOrderings::NO_CHANGES;
		m_num_out_of_place 					= initial_ordering_default_num_out_of_place;
		m_num_out_of_place_is_initialized	= false;
	}
	InitialOrdering(InitialOrderings ordering, array_size_t num_out_of_place)
		: m_ordering(ordering), m_num_out_of_place(num_out_of_place) {
		m_num_out_of_place_is_initialized = true;
	}
	InitialOrdering(InitialOrderings ordering) : m_ordering(ordering) {
		m_num_out_of_place = initial_ordering_default_num_out_of_place;
		m_num_out_of_place_is_initialized = false;
	}
	InitialOrdering(const InitialOrdering &other) {
		m_ordering 							= other.m_ordering;
		m_num_out_of_place					= other.m_num_out_of_place;
		m_num_out_of_place_is_initialized	= other.m_num_out_of_place_is_initialized;
	}

	InitialOrdering& operator=(const InitialOrderings& ordering) {
		m_ordering	= ordering;
		return *this;
	}

	std::string to_string() {
		std::stringstream result;
		result << std::to_string(m_ordering);
		if (m_ordering == InitialOrderings::FEW_CHANGES) {
			result << " " << m_num_out_of_place;
		}
		return result.str();
	}

	//	These are needed to sort the table of outputs
	//	Fundamental operators
	bool operator==(InitialOrdering v) {
		if (!(m_ordering == v.ordering())) {
			return false;
		}
		if (m_num_out_of_place_is_initialized ||
			!v.num_out_of_place_is_initialized()) {
			return true;
		}
		return m_num_out_of_place == v.num_out_of_place();
	}
	bool operator< (InitialOrdering v) {
		if (m_ordering < v.ordering()) {
			return true;
		}
		if (!m_num_out_of_place_is_initialized ||
			!v.num_out_of_place_is_initialized()) {
			return true;
		}
		return m_num_out_of_place < v.num_out_of_place();
	}
	//	Derived operators
	bool operator<=(InitialOrdering v) {
		return (*this < v) || (*this == v);
	}
	bool operator>=(InitialOrdering v) {
		return !(*this < v);
	}
	bool operator> (InitialOrdering v) {
		return !(*this < v) && !(*this == v);
	}
	bool operator!=(InitialOrdering v) {
		return !(*this == v);
	}

};

bool requiresNumOutOfPlace(InitialOrdering);
std::ostream& operator<<(std::ostream&, InitialOrdering&);

#endif /* INITIALORDERING_H_ */
