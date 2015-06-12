
#ifndef VARIABLE_20060121_H_
#define VARIABLE_20060121_H_

#include "variable_base.h"

class function;

class variable {
	friend char                                to_character(const variable &);
	friend int                                 to_integer(const variable &);
	friend std::string                         to_string(const variable &);
	friend function                            to_function(const variable &);
	friend std::vector<variable_base::pointer> to_array(const variable &);
	
	friend bool is_character(const variable &);
	friend bool is_integer(const variable &);
	friend bool is_string(const variable &);
	friend bool is_function(const variable &);
	friend bool is_array(const variable &);
	
	friend int size(const variable &);
	
	friend variable operator&&(const variable &lhs, const variable &rhs);
	friend variable operator||(const variable &lhs, const variable &rhs);
	friend variable operator<(const variable &lhs, const variable &rhs);
	friend variable operator>(const variable &lhs, const variable &rhs);
	friend variable operator<=(const variable &lhs, const variable &rhs);
	friend variable operator>=(const variable &lhs, const variable &rhs);
	friend variable operator!=(const variable &lhs, const variable &rhs);
	friend variable operator==(const variable &lhs, const variable &rhs);
	
private:
	explicit variable(const variable_base::pointer &p);
	
public:
	explicit variable(char value);
	explicit variable(int value);
	explicit variable(const std::string &value);
	explicit variable(const function &value);
	explicit variable(const std::vector<variable_base::pointer> &value);
	
public:
	static variable create_character();
	static variable create_integer();
	static variable create_string();
	static variable create_function();
	static variable create_array();
		
public:
	variable();
	variable(const variable &other);
	variable(variable &&other);
	variable& operator=(variable &&rhs);
	
public:
	variable &operator=(const variable &rhs);
	variable &operator+=(const variable &rhs);
	variable &operator-=(const variable &rhs);
	variable &operator*=(const variable &rhs);
	variable &operator/=(const variable &rhs);
	variable &operator&=(const variable &rhs);
	variable &operator|=(const variable &rhs);
	variable &operator^=(const variable &rhs);
	variable &operator%=(const variable &rhs);
	variable &operator>>=(const variable &rhs);
	variable &operator<<=(const variable &rhs);
	
public:
	variable operator[](const variable &index) const;
	
public:
	variable operator+() const;
	variable operator-() const;
	variable operator!() const;
	variable operator~() const;
	
public:
	function operator()() const;

public:
	void swap(variable &other);
	void reset(const variable &other);
	
public:
	variable_base::pointer get() const { return var_; }

private:
	variable_base::pointer var_;
};

char                                to_character(const variable &var);
int                                 to_integer(const variable &var);
std::string                         to_string(const variable &var);
function                            to_function(const variable &var);
std::vector<variable_base::pointer> to_array(const variable &var);

bool is_character(const variable &);
bool is_integer(const variable &);
bool is_string(const variable &);
bool is_function(const variable &);
bool is_array(const variable &);

int size(const variable &);

inline variable operator&&(const variable &lhs, const variable &rhs) { return variable(to_integer(lhs) && to_integer(rhs)); }
inline variable operator||(const variable &lhs, const variable &rhs) { return variable(to_integer(lhs) && to_integer(rhs)); }

inline variable operator<(const variable &lhs, const variable &rhs)  { return variable(lhs.var_->compare(*rhs.var_) < 0); }
inline variable operator>(const variable &lhs, const variable &rhs)  { return variable(lhs.var_->compare(*rhs.var_) > 0); }
inline variable operator<=(const variable &lhs, const variable &rhs) { return variable(lhs.var_->compare(*rhs.var_) <= 0); }
inline variable operator>=(const variable &lhs, const variable &rhs) { return variable(lhs.var_->compare(*rhs.var_) >= 0); }
inline variable operator!=(const variable &lhs, const variable &rhs) { return variable(lhs.var_->compare(*rhs.var_) != 0); }
inline variable operator==(const variable &lhs, const variable &rhs) { return variable(lhs.var_->compare(*rhs.var_) == 0); }

#endif
