
#include "function.h"
#include "token.h"

function::function() : type_(token::UNKNOWN), offset_(-1), param_count_(0) {
}

function::function(int type, address_t offset, unsigned int param_count) : type_(type), offset_(offset), param_count_(param_count)  {
}

function::function(const std::string &name) : type_(token::UNKNOWN), offset_(-1), param_count_(0), name_(name) {
}
