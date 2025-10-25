/*
 * Disorganizer.h
 *
 *  Created on: Jun 13, 2025
 *      Author: joe
 */

#ifndef DISORGANIZER_H_
#define DISORGANIZER_H_

#include <iostream>
#include <iomanip>
#include <inttypes.h>

#include "InitialOrdering.h"
#include "SimpleRandomizer.h"

class Disorganizer {
private:
	InitialOrdering  	m_ordering;
	SimpleRandomizer& 	m_randomizer;

public:

	Disorganizer& 		randomizer(SimpleRandomizer& rdmizer);
	SimpleRandomizer& 	randomizer(void) const;

	Disorganizer& ordering(InitialOrderings ordr);
	InitialOrdering ordering(void) const;

	Disorganizer& seed(uint64_t);
	uint64_t 	seed(void) const;

	uint64_t	recent(void) const;

	Disorganizer() = delete;
	Disorganizer(InitialOrdering ord, SimpleRandomizer& rndmzr) :
					m_ordering(ord),
					m_randomizer(rndmzr) {}
	Disorganizer& operator=(const Disorganizer&);
};

#endif /* DISORGANIZER_H_ */
