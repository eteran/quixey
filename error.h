
#ifndef ERROR_20111021_H_
#define ERROR_20111021_H_

#include <exception>

struct error : std::exception {
	virtual const char *what() const throw() {
		return "error";
	}
};

#include "syntax_error.h"
#include "runtime_error.h"


#endif
