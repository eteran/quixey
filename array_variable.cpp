
#include "array_variable.h"
#include "function.h"
#include "error.h"
#include <memory>

variable_base::pointer array_variable::clone() const {
	return std::make_shared<array_variable>(*this);
}

variable_base &array_variable::operator=(const variable_base &rhs) {
	value_ = rhs.to_array();
	return *this;
}

variable_base &array_variable::operator+=(const variable_base &rhs) {

	std::vector<variable_base::pointer> other = rhs.to_array();
	value_.insert(value_.end(), other.begin(), other.end());
	return *this;
}

variable_base &array_variable::operator-=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &array_variable::operator*=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &array_variable::operator/=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &array_variable::operator&=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &array_variable::operator|=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &array_variable::operator^=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &array_variable::operator%=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &array_variable::operator>>=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &array_variable::operator<<=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base::pointer array_variable::operator+() const {
	throw invalid_operation_for_type();
}

variable_base::pointer array_variable::operator-() const {
	throw invalid_operation_for_type();
}

variable_base::pointer array_variable::operator!() const {
	throw invalid_operation_for_type();
}

variable_base::pointer array_variable::operator~() const {
	throw invalid_operation_for_type();
}

int array_variable::size() const {
	return value_.size();
}

char array_variable::to_character() const {
	throw invalid_operation_for_type();
}

int array_variable::to_integer() const {
	throw invalid_operation_for_type();
}

std::string array_variable::to_string() const {
	throw invalid_operation_for_type();
}

function array_variable::to_function() const {
	throw invalid_operation_for_type();
}

std::vector<variable_base::pointer> array_variable::to_array() const {
	return value_;
}

int array_variable::compare(const variable_base &rhs) const {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base::pointer array_variable::operator[](const variable_base &index) const {
	size_t n = index.to_integer();
	if(n >= value_.size()) {
		throw out_of_bounds();
	}
	
	return value_[n];
}

function array_variable::operator()() const {
	throw invalid_operation_for_type();
}
