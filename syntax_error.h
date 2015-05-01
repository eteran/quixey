
#ifndef SYTAX_ERROR_20111021_H_
#define SYTAX_ERROR_20111021_H_

#include "error.h"

struct syntax_error : error {
	virtual const char *what() const throw() {
		return "syntax_error";
	}
};

struct unexpected_eof : syntax_error {
	virtual const char *what() const throw() {
		return "unexpected_eof";
	}
};

struct semicolon_expected : syntax_error {
	virtual const char *what() const throw() {
		return "semicolon_expected";
	}
};

struct type_expected : syntax_error {
	virtual const char *what() const throw() {
		return "type_expected";
	}
};

struct paren_expected : syntax_error {
	virtual const char *what() const throw() {
		return "paren_expected";
	}
};

struct colon_expected : syntax_error {
	virtual const char *what() const throw() {
		return "colon_expected";
	}
};

struct brace_expected : syntax_error {
	virtual const char *what() const throw() {
		return "brace_expected";
	}
};

struct bracket_expected : syntax_error {
	virtual const char *what() const throw() {
		return "bracket_expected";
	}
};

struct comma_expected : syntax_error {
	virtual const char *what() const throw() {
		return "comma_expected";
	}
};

struct while_expected : syntax_error {
	virtual const char *what() const throw() {
		return "while_expected";
	}
};

struct quote_expected : syntax_error {
	virtual const char *what() const throw() {
		return "quote_expected";
	}
};

struct string_expected : syntax_error {
	virtual const char *what() const throw() {
		return "string_expected";
	}
};

struct identifier_expected : syntax_error {
	virtual const char *what() const throw() {
		return "identifier_expected";
	}
};

struct multiline_string_literal : syntax_error {
	virtual const char *what() const throw() {
		return "multiline_string_literal";
	}
};

struct unimplemented_keyword : syntax_error {
	virtual const char *what() const throw() {
		return "unimplemented_keyword";
	}
};

struct unimplemented_operator : syntax_error {
	virtual const char *what() const throw() {
		return "unimplemented_operator";
	}
};

struct invalid_operation_for_type : syntax_error {
	virtual const char *what() const throw() {
		return "invalid_operation_for_type";
	}
};

struct invalid_type_conversion : syntax_error {
	virtual const char *what() const throw() {
		return "invalid_type_conversion";
	}
};

struct return_outside_call : syntax_error {
	virtual const char *what() const throw() {
		return "return_outside_call";
	}
};

struct function_during_prescan : syntax_error {
	virtual const char *what() const throw() {
		return "function_during_prescan";
	}
};

struct function_name_is_keyword : syntax_error {
	virtual const char *what() const throw() {
		return "function_name_is_keyword";
	}
};

struct variable_name_is_keyword : syntax_error {
	virtual const char *what() const throw() {
		return "variable_name_is_keyword";
	}
};

#endif
