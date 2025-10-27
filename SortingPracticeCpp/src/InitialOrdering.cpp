/*
 * InitialOrdering.cpp
 *
 *  Created on: Apr 16, 2025
 *      Author: joe
 */

#include "InitialOrdering.h"

/* ***********************************************************************	*/
/* ***********************************************************************	*/
/*						enum InitialOrderings								*/
/* ***********************************************************************	*/
/* ***********************************************************************	*/


/*	**********************************************************************	*/
/*		enum class InitialOrderings	functions								*/
/*	**********************************************************************	*/

bool requiresNumOutOfPlace(InitialOrderings const& ordering) {
	return ordering == InitialOrderings::FEW_CHANGES;
}

bool requiresSeed(InitialOrderings& ordering) {
	return ordering == InitialOrderings::IN_RANDOM_ORDER;
}

namespace std {
std::string to_string(InitialOrderings const& ordering) {

	switch (ordering) {
	case InitialOrderings::FEW_CHANGES:
		return std::string("FEW_CHANGES");
	case InitialOrderings::IN_RANDOM_ORDER:
		return std::string("IN_RANDOM_ORDER");
	case InitialOrderings::IN_REVERSE_ORDER:
		return std::string("IN_REVERSE_ORDER");
	case InitialOrderings::NO_CHANGES:
		return std::string("NO_CHANGES");
	default:
		return std::string("UNRECOGNIZED ORDERING");
	}
}
}

int strlen(InitialOrderings &ordering) {
	return std::to_string(ordering).length();
}

std::ostream& operator<<(std::ostream &out, InitialOrderings ordering) {

	out << std::to_string(ordering);
	return out;
}

/*
 * 	class InitialOrdering
 */

bool requiresNumOutOfPlace(const InitialOrdering& ordering) {
	return requiresNumOutOfPlace(ordering.ordering());
}

std::ostream& operator<<(std::ostream& out, InitialOrdering& object) {
	out << object.to_string();
	return out;
}
