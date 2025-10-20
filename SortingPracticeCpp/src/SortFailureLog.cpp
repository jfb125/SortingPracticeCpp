/*
 * SortFailureLog.cpp
 *
 *  Created on: Jun 17, 2025
 *      Author: joe
 */

#include "SortFailureLog.h"
#if 0
void SortFailureLog::clear(void) {

	if (_input) {
		delete _input;
		_input = nullptr;
	}
	if (_result) {
		delete _result;
		_result = nullptr;
	}
	if (_message) {
		delete _message;
		_message = nullptr;
	}
	_diagnostics._is_sorted = false;
	_diagnostics._mismatched_index_i = 0;
	_diagnostics._mismatched_index_j = 0;
}
std::string SortFailureLog::str(void) const {

	std::stringstream retval;

	if (_diagnostics._is_sorted) {
		retval << "    no failure occurred" << std::endl;
	} else {
		retval << "!!!!!!!!!! **** failure occurred **** !!!!!!!!!!" << std::endl;
		if ( _result) {
			retval	<< "  result: " << std::endl
					<< "  " << _result->dump() << std::endl;
			retval 	<< "  elements out-of-place were " << std::endl
					<< "  [" << _diagnostics._mismatched_index_i<< "] "
					<< *(*_result)[_diagnostics._mismatched_index_i]
					<< std::endl
					<< "   vs "
					<< std::endl
					<< "  [" << _diagnostics._mismatched_index_j<< "] "
					<< *(*_result)[_diagnostics._mismatched_index_j]
				    << std::endl;
		} else {
			retval 	<< "  result: <nullptr>" << std::endl;
			retval 	<< "  elements out-of-place were ["
					<< _diagnostics._mismatched_index_i
					<< "] vs ["
					<< _diagnostics._mismatched_index_j << "]"
					<< std::endl;
		}
	}

	if (_message) {
		retval << "  " << *_message;
	} else {
		retval << "   (no failure log messages)";
	}

	return retval.str();
}
std::ostream& operator<<(std::ostream &out, const SortFailureLog &log) {
	out << log.str();
	return out;
}
SortFailureLog::SortFailureLog() {

	_diagnostics.clear();
	_input = nullptr;
	_result = nullptr;
	_message = nullptr;
}
SortFailureLog::~SortFailureLog() {

	if (_input) {
		delete _input;
		_input = nullptr;
	}
	if (_result) {
		delete _result;
		_result = nullptr;
	}
	if (_message) {
		delete _message;
		_message = nullptr;
	}
	_diagnostics.clear();
}
SortFailureLog::SortFailureLog(const IsSortedResult diagnostics,
							   StudentDataArray *input,
							   StudentDataArray *result,
							   std::string *message) {
	_diagnostics = diagnostics;
	if (input) {
		_input = new StudentDataArray(*input);
	} else {
		_input = nullptr;
	}
	if (result) {
		_result = new StudentDataArray(*result);
	} else {
		_result = nullptr;
	}
	if (message) {
		_message = new std::string(*message);
	} else {
		_message = nullptr;
	}
}
SortFailureLog::SortFailureLog(const SortFailureLog &other) {

	if (this != &other) {
		if (other._input) {
			_input = new StudentDataArray(*other._input);
		} else {
			_input = nullptr;
		}
		if (other._result) {
			_result = new StudentDataArray(*other._result);
		} else {
			_result = nullptr;
		}
		if (other._message) {
			_message = new std::string(*other._message);
		} else {
			_message = nullptr;
		}
		_diagnostics = other._diagnostics;
	}
}

SortFailureLog& SortFailureLog::operator=(const SortFailureLog &other) {

	if (this != &other) {
		clear();
		if (_input) {
			delete _input;
			_input = nullptr;
		}
		if (other._input) {
			_input = new StudentDataArray(*other._input);
		}

		if (_result) {
			delete _result;
			_result = nullptr;
		}
		if (other._result) {
			_result = new StudentDataArray(*other._result);
		}

		if (_message) {
			delete _message;
			_message = nullptr;
		}
		if (other._message) {
			_message = new std::string(*other._message);
		}

		_diagnostics = other._diagnostics;
	}
	return *this;
}
SortFailureLog::SortFailureLog(SortFailureLog &&other) noexcept {

	_input = other._input;
	other._input = nullptr;
	_result = other._result;
	other._result = nullptr;
	_message = other._message;
	other._message = nullptr;
	_diagnostics = other._diagnostics;
}

SortFailureLog& SortFailureLog::operator=(SortFailureLog &&other) noexcept {

	if (this != &other) {
		clear();
		if (_input) {
			delete _input;
			_input = nullptr;
		}
		_input = other._input;
		other._input = nullptr;

		if (_result) {
			delete _result;
			_result = nullptr;
		}
		_result = other._result;
		other._result = nullptr;

		if (_message) {
			delete _message;
			_message = nullptr;
		}
		_message = other._message;
		other._message = nullptr;

		_diagnostics = other._diagnostics;
	}
	return *this;
}
#endif
