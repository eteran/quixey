
#ifndef FUNCTION_20060121_H_
#define FUNCTION_20060121_H_

#include "script_types.h"
#include <string>

class function {
public:
	function();
	explicit function(const std::string &name);
	function(int type, address_t offset, unsigned int param_count);

public:
	address_t offset() const         { return offset_; }
	unsigned int param_count() const { return param_count_; }
	int type() const                 { return type_; }
	std::string name() const         { return name_; }

private:
	int          type_;
	address_t    offset_;
	unsigned int param_count_;
	std::string  name_;
};

#endif
