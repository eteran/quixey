
#include "function_variable.h"
#include "error.h"
#include <cstdio>
#include <memory>

variable_base::pointer function_variable::clone() const {
	return std::make_shared<function_variable>(*this);
}

variable_base &function_variable::operator=(const variable_base &rhs) {
	value_ = rhs.to_function();
	return *this;
}

variable_base &function_variable::operator+=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &function_variable::operator-=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &function_variable::operator*=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &function_variable::operator/=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &function_variable::operator&=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &function_variable::operator|=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &function_variable::operator^=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &function_variable::operator%=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &function_variable::operator>>=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base &function_variable::operator<<=(const variable_base &rhs) {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base::pointer function_variable::operator+() const {
	throw invalid_operation_for_type();
}

variable_base::pointer function_variable::operator-() const {
	throw invalid_operation_for_type();
}

variable_base::pointer function_variable::operator!() const {
	throw invalid_operation_for_type();
}

variable_base::pointer function_variable::operator~() const {
	throw invalid_operation_for_type();
}

int function_variable::size() const {
	throw invalid_operation_for_type();
}

char function_variable::to_character() const {
	throw invalid_operation_for_type();
}

int function_variable::to_integer() const {
	throw invalid_operation_for_type();
}

std::string function_variable::to_string() const {
	throw invalid_operation_for_type();
}

function function_variable::to_function() const {
	return value_;
}

std::vector<variable_base::pointer> function_variable::to_array() const {
	throw invalid_type_conversion();
}

int function_variable::compare(const variable_base &rhs) const {
	(void)rhs;
	throw invalid_operation_for_type();
}

variable_base::pointer function_variable::operator[](const variable_base &index) const {
	(void)index;
	throw invalid_operation_for_type();
}

function function_variable::operator()() const {
	return value_;
}
