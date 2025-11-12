/*
 * OStreamState.h
 *
 *  Created on: Sep 25, 2025
 *      Author: Joe Baker
 */

#ifndef OSTREAMSTATE_H_
#define OSTREAMSTATE_H_

#include <iostream>
#include <iomanip>

/*
 * 	Creating an object of this class stores stdio's format flags
 * 	The destructor restores flags to state they were when the constructors was called
 * 	This is useful to restoring the state of 'left' 'right' etc after function exits
 */

class OStreamState {
private:
	std::ios::fmtflags flags;
	char fill;
public:
	void init() {
		flags = std::cout.flags();
		fill = std::cout.fill();
	}
	void restore() {
		std::cout.flags(flags);
		std::cout.fill(fill);
	}
	OStreamState() {
		init();
	}
	~OStreamState() {
		restore();
	}
	OStreamState(const OStreamState &other) {
		flags = other.flags;
		fill = other.fill;
	}
	OStreamState& operator=(const OStreamState &other) {
		flags = other.flags;
		fill = other.fill;
		return *this;
	}
};

#endif /* OSTREAMSTATE_H_ */
