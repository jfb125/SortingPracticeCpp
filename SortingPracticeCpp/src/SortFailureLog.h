/*
 * SortFailureLog.h
 *
 *  Created on: Jun 17, 2025
 *      Author: joe
 */

#ifndef SORTFAILURELOG_H_
#define SORTFAILURELOG_H_

#include <iostream>
#include <iomanip>
#include <sstream>

#include "StudentDataArray.h"
#include "SortingDataTypes.h"

class SortFailureLog {

public:
	IsSortedResult _diagnostics;
	StudentDataArray *_input;
	StudentDataArray *_result;
	std::string *_message;


	std::string str(void) const;
	void clear(void);

	SortFailureLog();
	virtual ~SortFailureLog();
	// deep copies of pointer objects
	SortFailureLog(IsSortedResult diagnostices,
				   StudentDataArray *input,
				   StudentDataArray *result,
				   std::string *message);
	SortFailureLog(const SortFailureLog &other);
	SortFailureLog& operator=(const SortFailureLog &other);
	SortFailureLog(SortFailureLog &&other) noexcept;
	SortFailureLog& operator=(SortFailureLog &&other) noexcept;
};

std::ostream& operator<<(std::ostream&, const SortFailureLog&);

#endif /* SORTFAILURELOG_H_ */
