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

#undef _INITIAL_ORDERING
#define _INITIAL_ORDERINGS\
	_INITIAL_ORDERING(NO_CHANGES)\
	_INITIAL_ORDERING(IN_REVERSE_ORDER)\
	_INITIAL_ORDERING(IN_RANDOM_ORDER)\
	_INITIAL_ORDERING(FEW_CHANGES)\
	_INITIAL_ORDERING(INITIAL_ORDERINGS_COUNT)

#define _INITIAL_ORDERING(order)	order,
enum InitialOrderings : uint8_t {	_INITIAL_ORDERINGS 	};

#define MIN_INITIAL_ORDERING	InitialOrderings::NO_CHANGES
#define MAX_INITIAL_ORDERING	InitialOrderings::FEW_CHANGES
#define NUM_INITIAL_ORDERINGS	InitialOrderings::INITIAL_ORDERINGS_COUNT

std::ostream& operator<<(std::ostream&, InitialOrderings);
std::string toString(InitialOrderings);

bool isValid(InitialOrderings);
bool requiresSeed(InitialOrderings);

InitialOrderings& operator++(InitialOrderings& current_ordering);
InitialOrderings operator++(InitialOrderings& current_ordering, int unused);
int operator-(InitialOrderings, InitialOrderings);

bool operator==(InitialOrderings, InitialOrderings);
bool operator<(InitialOrderings, InitialOrderings);
bool operator!=(InitialOrderings, InitialOrderings);
bool operator<=(InitialOrderings, InitialOrderings);
bool operator>(InitialOrderings, InitialOrderings);
bool operator>=(InitialOrderings, InitialOrderings);

#define DEFAULT_INITIAL_ORDERING_INITIAL_ORDERINGS\
				InitialOrderings::NO_CHANGES
#define DEFAULT_INITIAL_ORDERING_NUM_OUT_OF_PLACE ULONG_MAX
#define DEFAULT_NUM_OUT_OF_PLACE_IS_INITIALIZED false

class InitialOrdering {
private:
	InitialOrderings _ordering;
	// these are private to manage initialization state
	//	when user is accessing '_num_out_of_place'
	bool _num_out_of_place_is_initialized;
	array_size_t _num_out_of_place;

public:

	InitialOrderings order(void) const;
	InitialOrdering& order(InitialOrderings);

	array_size_t num_out_of_place(void) const;
	InitialOrdering& num_out_of_place(array_size_t);

	bool num_out_of_place_is_initialized(void) const;

	//	only check _ordering; _num_out_of_place is not compared
	bool operator==(const InitialOrdering&) const;
	bool operator<(const InitialOrdering&) const;
	bool operator!=(const InitialOrdering&) const;
	bool operator <=(const InitialOrdering&) const;
	bool operator>(const InitialOrdering&) const;
	bool operator>=(const InitialOrdering&) const;


	InitialOrdering& operator++();
	InitialOrdering operator++(int unused);
	int operator-(const InitialOrdering&) const;

	std::string str(void) const;

	InitialOrdering();
	~InitialOrdering();
	InitialOrdering(InitialOrderings ordering, array_size_t num_out_of_place);
	InitialOrdering(InitialOrderings ordering);
	InitialOrdering(const InitialOrdering& other);
	InitialOrdering& operator=(const InitialOrdering& other);
	//	this does not change the value of '_num_out_of_place'
	InitialOrdering& operator=(InitialOrderings ordering);
};

bool requiresNumOutOfPlace(InitialOrderings);
bool isValid(InitialOrderings);

std::ostream& operator<<(std::ostream&, const InitialOrdering&);

#endif /* INITIALORDERING_H_ */
