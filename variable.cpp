
#include "variable.h"
#include "array_variable.h"
#include "char_variable.h"
#include "error.h"
#include "function_variable.h"
#include "int_variable.h"
#include "string_variable.h"
#include <algorithm>
#include <memory>

variable variable::create_array() {
	return variable(std::make_shared<array_variable>());
}

variable variable::create_function() {
	return variable(std::make_shared<function_variable>(function()));
}

variable variable::create_character() {
	return variable(std::make_shared<char_variable>('\0'));
}

variable variable::create_integer() {
	return variable(std::make_shared<int_variable>(0));
}

variable variable::create_string() {
	return variable(std::make_shared<string_variable>(""));
}

void variable::swap(variable &other) {
	using std::swap;
	swap(var_, other.var_);
}

void variable::reset(const variable &other) {
	variable(other).swap(*this);
}

variable::variable(int value) : var_(std::make_shared<int_variable>(value)) {
}

variable::variable(const std::string &value) : var_(std::make_shared<string_variable>(value)) {
}

variable::variable(char value) : var_(std::make_shared<char_variable>(value)) {
}

variable::variable(const function &value) : var_(std::make_shared<function_variable>(value)) {
}

variable::variable(const std::vector<variable_base::pointer> &value) : var_(std::make_shared<array_variable>(value)) {
}

variable::variable(const variable_base::pointer &p) : var_(p) {

}

variable::variable() {
}

variable::variable(variable &&other) : var_(std::move(other.var_)) {
}

variable::variable(const variable &other) {
	if(other.var_) {
		var_ = other.var_->clone();
	}
}

variable& variable::operator=(variable &&rhs) {
	if(&rhs != this) {
		var_ = std::move(rhs.var_);
	}
	return *this;

}

variable &variable::operator=(const variable &rhs) {
	if(!var_ && rhs.var_) {
		var_ = rhs.var_->clone();
	} else if(var_ && rhs.var_) {
		*var_ = *rhs.var_;
	} else {
		variable(rhs.var_).swap(*this);
	}
	return *this;
}

variable &variable::operator+=(const variable &rhs) {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	
	// TODO: is there a better approach to this?
	// this code makes things like 'A' + "B" work
		
	if(var_->is_character() && rhs.var_->is_string()) {
		var_ = std::make_shared<string_variable>(var_->to_string());
	}
	
	*var_ += *rhs.var_;
	return *this;
}

variable &variable::operator-=(const variable &rhs) {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	*var_ -= *rhs.var_;
	return *this;
}

variable &variable::operator*=(const variable &rhs) {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	*var_ *= *rhs.var_;
	return *this;
}

variable &variable::operator/=(const variable &rhs) {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	*var_ /= *rhs.var_;
	return *this;
}

variable &variable::operator&=(const variable &rhs) {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	*var_ &= *rhs.var_;
	return *this;
}

variable &variable::operator|=(const variable &rhs) {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	*var_ |= *rhs.var_;
	return *this;
}

variable &variable::operator^=(const variable &rhs) {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	*var_ ^= *rhs.var_;
	return *this;
}

variable &variable::operator%=(const variable &rhs) {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	*var_ %= *rhs.var_;
	return *this;
}

variable &variable::operator>>=(const variable &rhs) {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	*var_ >>= *rhs.var_;
	return *this;
}

variable &variable::operator<<=(const variable &rhs) {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	*var_ <<= *rhs.var_;
	return *this;
}

variable variable::operator+() const {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	return variable(+(*var_));
}

variable variable::operator-() const {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	return variable(-(*var_));
}

variable variable::operator!() const {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	return variable(!(*var_));
}

variable variable::operator~() const {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	return variable(~(*var_));
}

function variable::operator()() const {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	return (*var_)();
}

variable variable::operator[](const variable &index) const {
	if(!var_) {
		throw uninitialized_variable_used();
	}
	return variable((*var_)[*(index.var_)]);
}

char to_character(const variable &var) {
	if(!var.var_) {
		throw uninitialized_variable_used();
	}
	
	return var.var_->to_character();
}

int to_integer(const variable &var) {
	if(!var.var_) {
		throw uninitialized_variable_used();
	}
	
	return var.var_->to_integer();
}

std::string to_string(const variable &var) {
	if(!var.var_) {
		throw uninitialized_variable_used();
	}
	
	return var.var_->to_string();
}

function to_function(const variable &var) {
	if(!var.var_) {
		throw uninitialized_variable_used();
	}
	
	return var.var_->to_function();
}

std::vector<variable_base::pointer> to_array(const variable &var) {
	if(!var.var_) {
		throw uninitialized_variable_used();
	}
	
	return var.var_->to_array();
}


bool is_character(const variable &var) {
	if(!var.var_) {
		return false;
	}
	
	return var.var_->is_character();
}

bool is_integer(const variable &var) {
	if(!var.var_) {
		return false;
	}
	
	return var.var_->is_integer();
}

bool is_string(const variable &var) {
	if(!var.var_) {
		return false;
	}
	
	return var.var_->is_string();
}


bool is_function(const variable &var) {
	if(!var.var_) {
		return false;
	}
	
	return var.var_->is_function();
}

bool is_array(const variable &var) {
	if(!var.var_) {
		return false;
	}
	
	return var.var_->is_array();
}

int size(const variable &var) {
	if(!var.var_) {
		throw uninitialized_variable_used();
	}
	
	return var.var_->size();
}
