
#include "int_variable.h"
#include "error.h"
#include <memory>

variable_base::pointer int_variable::clone() const {
	return std::make_shared<int_variable>(*this);
}

variable_base &int_variable::operator=(const variable_base &rhs) {
	value_ = rhs.to_integer();
	return *this;
}

variable_base &int_variable::operator+=(const variable_base &rhs) {
	value_ += rhs.to_integer();
	return *this;
}

variable_base &int_variable::operator-=(const variable_base &rhs) {
	value_ -= rhs.to_integer();
	return *this;
}

variable_base &int_variable::operator*=(const variable_base &rhs) {
	value_ *= rhs.to_integer();
	return *this;
}

variable_base &int_variable::operator/=(const variable_base &rhs) {
	value_ /= rhs.to_integer();
	return *this;
}

variable_base &int_variable::operator&=(const variable_base &rhs) {
	value_ &= rhs.to_integer();
	return *this;
}

variable_base &int_variable::operator|=(const variable_base &rhs) {
	value_ |= rhs.to_integer();
	return *this;
}

variable_base &int_variable::operator^=(const variable_base &rhs) {
	value_ ^= rhs.to_integer();
	return *this;
}

variable_base &int_variable::operator%=(const variable_base &rhs) {
	value_ %= rhs.to_integer();
	return *this;
}

variable_base &int_variable::operator>>=(const variable_base &rhs) {
	value_ >>= rhs.to_integer();
	return *this;
}

variable_base &int_variable::operator<<=(const variable_base &rhs) {
	value_ <<= rhs.to_integer();
	return *this;
}

variable_base::pointer int_variable::operator+() const {
	return std::make_shared<int_variable>(+value_);
}

variable_base::pointer int_variable::operator-() const {
	return std::make_shared<int_variable>(-value_);
}

variable_base::pointer int_variable::operator!() const {
	return std::make_shared<int_variable>(!value_);
}

variable_base::pointer int_variable::operator~() const {
	return std::make_shared<int_variable>(~value_);
}

int int_variable::size() const {
	throw invalid_operation_for_type();
}

int int_variable::to_integer() const {
	return value_;
}

char int_variable::to_character() const {
	throw invalid_type_conversion();
}

std::string int_variable::to_string() const {
	throw invalid_type_conversion();
}

function int_variable::to_function() const {
	throw invalid_type_conversion();
}

std::vector<variable_base::pointer> int_variable::to_array() const {
	throw invalid_type_conversion();
}

int int_variable::compare(const variable_base &rhs) const {
	if(const int_variable *const x = dynamic_cast<const int_variable*>(&rhs)) {
		return value_ - x->value_;
	}
	throw invalid_type_conversion();
}

variable_base::pointer int_variable::operator[](const variable_base &index) const {
	(void)index;
	throw invalid_operation_for_type();
}

function int_variable::operator()() const {
	throw invalid_operation_for_type();
}
