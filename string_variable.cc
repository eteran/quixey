
#include "string_variable.h"
#include "char_variable.h"
#include "error.h"
#include <cstdio>
#include <memory>

variable_base::pointer string_variable::clone() const {
	return std::make_shared<string_variable>(*this);
}

variable_base &string_variable::operator=(const variable_base &rhs) {
	value_ = rhs.to_string();
	return *this;
}

variable_base &string_variable::operator+=(const variable_base &rhs) {
	value_ += rhs.to_string();
	return *this;
}

variable_base &string_variable::operator-=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &string_variable::operator*=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &string_variable::operator/=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &string_variable::operator&=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &string_variable::operator|=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &string_variable::operator^=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &string_variable::operator%=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &string_variable::operator>>=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &string_variable::operator<<=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base::pointer string_variable::operator+() const {
	throw invalid_operation_for_type();
}

variable_base::pointer string_variable::operator-() const {
	throw invalid_operation_for_type();
}

variable_base::pointer string_variable::operator!() const {
	throw invalid_operation_for_type();
}

variable_base::pointer string_variable::operator~() const {
	throw invalid_operation_for_type();
}

int string_variable::size() const {
	return value_.size();
}

char string_variable::to_character() const {
	throw invalid_type_conversion();
}

int string_variable::to_integer() const {
	throw invalid_type_conversion();
}

std::string string_variable::to_string() const {
	return value_;
}

function string_variable::to_function() const {
	throw invalid_type_conversion();
}

std::vector<variable_base::pointer> string_variable::to_array() const {
	throw invalid_type_conversion();
}

int string_variable::compare(const variable_base &rhs) const {
	if(const string_variable *const x = dynamic_cast<const string_variable*>(&rhs)) {
		if(value_ < x->value_) {
			return -1;
		} else if(value_ > x->value_) {
			return 1;
		} else {
			return 0;
		}
	}
	
	throw invalid_type_conversion();
}

variable_base::pointer string_variable::operator[](const variable_base &index) const {
	size_t n = index.to_integer();
	if(n >= value_.size()) {
		throw out_of_bounds();
	}
	return std::make_shared<char_variable>(value_[n]);
}


function string_variable::operator()() const {
	throw invalid_operation_for_type();
}
