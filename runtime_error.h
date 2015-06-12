
#ifndef RUNTIME_ERROR_20111021_H_
#define RUNTIME_ERROR_20111021_H_

#include "error.h"

struct builtins_cannot_be_entry_points : error {
	virtual const char *what() const throw() {
		return "builtins_cannot_be_entry_points";
	}
};

struct runtime_error : error {
	virtual const char *what() const throw() {
		return "runtime_error";
	}
};

struct divide_by_zero : runtime_error {
	virtual const char *what() const throw() {
		return "divide_by_zero";
	}
};

struct uninitialized_variable_used : runtime_error {
	virtual const char *what() const throw() {
		return "uninitialized_variable_used";
	}
};

struct variable_not_found : runtime_error {
	virtual const char *what() const throw() {
		return "variable_not_found";
	}
};

struct incorrect_param_count : runtime_error {
	virtual const char *what() const throw() {
		return "incorrect_param_count";
	}
};

struct duplicate_function : runtime_error {
	virtual const char *what() const throw() {
		return "duplicate_function";
	}
};

struct duplicate_global : runtime_error {
	virtual const char *what() const throw() {
		return "duplicate_global";
	}
};

struct duplicate_local : runtime_error {
	virtual const char *what() const throw() {
		return "duplicate_local";
	}
};

struct undefined_function : syntax_error {
	virtual const char *what() const throw() {
		return "undefined_function";
	}
};

struct unknown_identifier : syntax_error {
	virtual const char *what() const throw() {
		return "unknown_identifier";
	}
};

struct out_of_bounds : runtime_error {
	virtual const char *what() const throw() {
		return "out_of_bounds";
	}
};

#endif
