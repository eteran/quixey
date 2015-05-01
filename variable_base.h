
#ifndef VARIABLE_BASE_10012011_H_
#define VARIABLE_BASE_10012011_H_

#include "function.h"
#include <memory>
#include <string>
#include <vector>

class variable_base {
public:
	typedef std::shared_ptr<variable_base> pointer;
	
public:
	virtual ~variable_base() {
	}
	
public:
	virtual pointer clone() const = 0;
	
public:
	virtual variable_base &operator=(const variable_base &rhs) = 0;
	virtual variable_base &operator+=(const variable_base &rhs) = 0;
	virtual variable_base &operator-=(const variable_base &rhs) = 0;
	virtual variable_base &operator*=(const variable_base &rhs) = 0;
	virtual variable_base &operator/=(const variable_base &rhs) = 0;
	virtual variable_base &operator&=(const variable_base &rhs) = 0;
	virtual variable_base &operator|=(const variable_base &rhs) = 0;
	virtual variable_base &operator^=(const variable_base &rhs) = 0;
	virtual variable_base &operator%=(const variable_base &rhs) = 0;
	virtual variable_base &operator>>=(const variable_base &rhs) = 0;
	virtual variable_base &operator<<=(const variable_base &rhs) = 0;
	
public:
	virtual pointer operator[](const variable_base &index) const = 0;
	
public:
	virtual int size() const = 0;
	
public:
	virtual pointer operator+() const = 0;
	virtual pointer operator-() const = 0;
	virtual pointer operator!() const = 0;
	virtual pointer operator~() const = 0;

public:
	virtual function operator()() const = 0;
	
public:
	virtual int compare(const variable_base &rhs) const = 0;
	
public:
	virtual char                                to_character() const = 0;
	virtual int                                 to_integer() const = 0;
	virtual std::string                         to_string() const = 0;
	virtual function                            to_function() const = 0;
	virtual std::vector<variable_base::pointer> to_array() const = 0;

public:
	virtual bool is_character() const = 0;
	virtual bool is_integer() const = 0;
	virtual bool is_string() const = 0;
	virtual bool is_function() const = 0;
	virtual bool is_array() const = 0;
};

#endif
