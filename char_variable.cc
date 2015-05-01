
#include "char_variable.h"
#include "error.h"
#include <memory>

variable_base::pointer char_variable::clone() const {
	return std::make_shared<char_variable>(*this);
}

variable_base &char_variable::operator=(const variable_base &rhs) {
	value_ = rhs.to_integer();
	return *this;
}

variable_base &char_variable::operator+=(const variable_base &rhs) {
	value_ += rhs.to_integer();
	return *this;
}

variable_base &char_variable::operator-=(const variable_base &rhs) {
	value_ -= rhs.to_integer();
	return *this;
}

variable_base &char_variable::operator*=(const variable_base &rhs) {
	value_ *= rhs.to_integer();
	return *this;
}

variable_base &char_variable::operator/=(const variable_base &rhs) {
	value_ /= rhs.to_integer();
	return *this;
}

variable_base &char_variable::operator&=(const variable_base &rhs) {
	value_ &= rhs.to_integer();
	return *this;
}

variable_base &char_variable::operator|=(const variable_base &rhs) {
	value_ |= rhs.to_integer();
	return *this;
}

variable_base &char_variable::operator^=(const variable_base &rhs) {
	value_ ^= rhs.to_integer();
	return *this;
}

variable_base &char_variable::operator%=(const variable_base &rhs) {
	value_ %= rhs.to_integer();
	return *this;
}

variable_base &char_variable::operator>>=(const variable_base &rhs) {
	value_ >>= rhs.to_integer();
	return *this;
}

variable_base &char_variable::operator<<=(const variable_base &rhs) {
	value_ <<= rhs.to_integer();
	return *this;
}

variable_base::pointer char_variable::operator+() const {
	return std::make_shared<char_variable>(+value_);
}

variable_base::pointer char_variable::operator-() const {
	return std::make_shared<char_variable>(-value_);
}

variable_base::pointer char_variable::operator!() const {
	return std::make_shared<char_variable>(!value_);
}

variable_base::pointer char_variable::operator~() const {
	return std::make_shared<char_variable>(~value_);
}

int char_variable::size() const {
	throw invalid_operation_for_type();
}

char char_variable::to_character() const {
	return value_;
}

int char_variable::to_integer() const {
	return static_cast<int>(value_);
}

std::string char_variable::to_string() const {
	return std::string(1, value_);
}

function char_variable::to_function() const {
	throw invalid_type_conversion();
}

std::vector<variable_base::pointer> char_variable::to_array() const {
	throw invalid_type_conversion();
}

int char_variable::compare(const variable_base &rhs) const {
	if(const char_variable *const x = dynamic_cast<const char_variable*>(&rhs)) {
		return value_ - x->value_;
	}
	throw invalid_type_conversion();
}

variable_base::pointer char_variable::operator[](const variable_base &index) const {
	(void)index;
	throw invalid_operation_for_type();
}

function char_variable::operator()() const {
	throw invalid_operation_for_type();
}
