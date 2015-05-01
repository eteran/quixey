
#ifndef STRING_VARIABLE_10012011_H_
#define STRING_VARIABLE_10012011_H_

#include "variable_base.h"

class string_variable : public variable_base {
public:
	explicit string_variable(const std::string &value) : value_(value) {
	}

public:
	virtual pointer clone() const;
	
public:
	virtual variable_base &operator=(const variable_base &rhs);
	virtual variable_base &operator+=(const variable_base &rhs);
	virtual variable_base &operator-=(const variable_base &rhs);
	virtual variable_base &operator*=(const variable_base &rhs);
	virtual variable_base &operator/=(const variable_base &rhs);
	virtual variable_base &operator&=(const variable_base &rhs);
	virtual variable_base &operator|=(const variable_base &rhs);
	virtual variable_base &operator^=(const variable_base &rhs);
	virtual variable_base &operator%=(const variable_base &rhs);
	virtual variable_base &operator>>=(const variable_base &rhs);
	virtual variable_base &operator<<=(const variable_base &rhs);

public:
	virtual pointer operator[](const variable_base &index) const;
	
public:
	virtual int size() const;

public:
	virtual pointer operator+() const;
	virtual pointer operator-() const;
	virtual pointer operator!() const;
	virtual pointer operator~() const;
	
public:
	virtual function operator()() const;
	
public:
	int compare(const variable_base &rhs) const;

public:
	virtual char                                to_character() const;
	virtual int                                 to_integer() const;
	virtual std::string                         to_string() const;
	virtual function                            to_function() const;
	virtual std::vector<variable_base::pointer> to_array() const;

public:
	virtual bool is_character() const { return false; }
	virtual bool is_integer() const   { return false; }
	virtual bool is_string() const    { return true; }
	virtual bool is_function() const  { return false; }
	virtual bool is_array() const     { return false; }

private:
	std::string value_;
};

#endif
