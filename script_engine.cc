
#include "script_engine.h"
#include "error.h"
#include "library_adaptor.h"
#include "string_util.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace {

//-----------------------------------------------------------------------------
// Name: is_delim
//-----------------------------------------------------------------------------
bool is_delim(char c) {
	using std::strchr;
	return strchr(" !:;,+-<>'/*%^=()[].\r\n\t", c) != nullptr;
}

//-----------------------------------------------------------------------------
// Name: is_odigit
//-----------------------------------------------------------------------------
bool is_odigit(char c) {
	return (c >= '0' && c <= '7');
}

//-----------------------------------------------------------------------------
// Name: skip_whitespace
//-----------------------------------------------------------------------------
template <class In>
void skip_whitespace(In &it, In end) {

	using std::isspace;

	// skip over white space
	while(it != end && isspace(*it)) {
		++it;
	}
}

//-----------------------------------------------------------------------------
// Name: skip_comments
//-----------------------------------------------------------------------------
template <class In>
bool skip_comments(In &it, In end) {

	if(it == end) {
		return false;
	}

	const In original_pc = it;

	if(*it == '/') {
		++it;

		// c++ style comments
		if(it != end && *it == '/') {
			while(it != end && *it != '\n') {
				++it;
			}
			return true;


		// c style comments
		} else if(it != end && *it == '*') {
			do {
				// find end of comment
				while(it != end && *it != '*') {
					++it;
				}

				++it;
			} while(it != end && *it != '/');

			if(it == end) {
				throw unexpected_eof();
			}

			++it;
			return true;

		// not a comment
		} else {
			it = original_pc;
		}
	} else if(*it == '#') {
		++it;

		// shell style comments
		while(it != end && *it != '\n') {
			++it;
		}
		return true;	
	}

	return false;
}

//-----------------------------------------------------------------------------
// Name: process_char
// Desc: processes a character constant (assumes is either in a string or a
//       quoted char constant)
//-----------------------------------------------------------------------------
template <class In>
char process_char(In &it, In end) {

	using std::isxdigit;

	if(it == end) {
		return '\0';
	}

	char temp_char;

	// is it an escape sequence?
	if(*it == '\\') {

		std::string temp_string;

		// skip past escape char
		++it;

		if(it == end) {
			throw unexpected_eof();
		}

		// which special sequence is it?
		switch(*it) {
		case '\'': temp_char = '\''; break;
		case '\"': temp_char = '\"'; break;
		case '\\': temp_char = '\\'; break;
		case '0':  temp_char = '\0'; break;
		case 'a':  temp_char = '\a'; break;
		case 'b':  temp_char = '\b'; break;
		case 'f':  temp_char = '\f'; break;
		case 'n':  temp_char = '\n'; break;
		case 'r':  temp_char = '\r'; break;
		case 't':  temp_char = '\t'; break;
		case 'v':  temp_char = '\v'; break;
		case 'x':
			++it;
			while(it != end && isxdigit(*it)) {
				temp_string += *it;
				++it;
			}

			if(it == end) {
				throw unexpected_eof();
			}

			temp_char = static_cast<char>(stoi(temp_string, nullptr, 16));
			--it;
			break;
		default:
			while(it != end && is_odigit(*it)) {
				temp_string += *it;
				++it;
			}

			if(it == end) {
				throw unexpected_eof();
			}

			temp_char = static_cast<char>(stoi(temp_string, nullptr, 8));
			--it;
			break;
		}

	} else {
		temp_char = *it;
	}

	++it;

	return temp_char;
}

//-----------------------------------------------------------------------------
// Name: call_size
//-----------------------------------------------------------------------------
int call_size(std::string s) {
	return s.size();
}

//-----------------------------------------------------------------------------
// Name: call_puts
//-----------------------------------------------------------------------------
int call_puts(std::string s) {
	std::cout << s << '\n';
	return 0;
}

//-----------------------------------------------------------------------------
// Name: call_getch
//-----------------------------------------------------------------------------
int call_getch() {
	char ch;
	std::cin >> ch;
	return ch;
}

//-----------------------------------------------------------------------------
// Name: call_getnum
//-----------------------------------------------------------------------------
int call_getnum() {
	int num;
	std::cin >> num;
	return num;
}

//-----------------------------------------------------------------------------
// Name: call_putchar
//-----------------------------------------------------------------------------
int call_putchar(char ch) {
	std::cout << ch;
	return 0;
}

//-----------------------------------------------------------------------------
// Name: call_is_integer
//-----------------------------------------------------------------------------
int call_is_integer(variable x) {
	return is_integer(x);
}

//-----------------------------------------------------------------------------
// Name: call_is_character
//-----------------------------------------------------------------------------
int call_is_character(variable x) {
	return is_character(x);
}

//-----------------------------------------------------------------------------
// Name: call_is_string
//-----------------------------------------------------------------------------
int call_is_string(variable x) {
	return is_string(x);
}

//-----------------------------------------------------------------------------
// Name: call_is_function
//-----------------------------------------------------------------------------
int call_is_function(variable x) {
	return is_function(x);
}

//-----------------------------------------------------------------------------
// Name: call_is_array
//-----------------------------------------------------------------------------
int call_is_array(variable x) {
	return is_array(x);
}

// example of a variadic built in function
// much more complicated, but works great :)
// by only taking a single param of type
// script_engine * the function system knows
// this is a variadic function, and will leave it
// up to the implementer to pull arguments as needed
// BE CAREFUL!
//-----------------------------------------------------------------------------
// Name: call_printf
//-----------------------------------------------------------------------------
int call_printf(script_engine *engine) {
	
	assert(engine);

	variable x;
	engine->eval_exp(x);

	auto format = to_string(x);

	for(auto it = format.begin(); it != format.end(); ++it) {
		if(*it == '%') {
			if(++it != format.end()) {
				switch(*it) {
				case 'c':
					do {
						engine->get_token();
						engine->test_token<comma_expected>(token::COMMA);

						variable x;
						engine->eval_exp(x);
						std::cout << to_character(x);
					} while(0);
					break;

				case 'u':
					do {
						engine->get_token();
						engine->test_token<comma_expected>(token::COMMA);

						variable x;
						engine->eval_exp(x);
						std::cout << std::dec << static_cast<unsigned long>(to_integer(x));
					} while(0);
					break;

				case 'd':
					do {
						engine->get_token();
						engine->test_token<comma_expected>(token::COMMA);

						variable x;
						engine->eval_exp(x);
						std::cout << std::dec << to_integer(x);
					} while(0);
					break;

				case 'x':
					do {
						engine->get_token();
						engine->test_token<comma_expected>(token::COMMA);

						variable x;
						engine->eval_exp(x);
						std::cout << std::hex << to_integer(x);
					} while(0);
					break;

				case 's':
					do {
						engine->get_token();
						engine->test_token<comma_expected>(token::COMMA);

						variable x;
						engine->eval_exp(x);
						std::cout << to_string(x);
					} while(0);
					break;
				default:
					std::cout << *it;
					break;
				}
			} else {
				std::cout << '%';
			}
		} else {
			std::cout << *it;
		}
	}
	return 0;
}


}

//-----------------------------------------------------------------------------
// Name: script_engine
//-----------------------------------------------------------------------------
script_engine::script_engine() : block_depth_(0), prescan_(false) {

	// setup keywords
	keywords_.emplace("break",    token::BREAK);
	keywords_.emplace("case",     token::CASE);
	keywords_.emplace("char",     token::CHAR);
	keywords_.emplace("continue", token::CONTINUE);
	keywords_.emplace("do",       token::DO);
	keywords_.emplace("else",     token::ELSE);
	keywords_.emplace("for",      token::FOR);
	keywords_.emplace("if",       token::IF);
	keywords_.emplace("int",      token::INT);
	keywords_.emplace("auto",     token::AUTO);
	keywords_.emplace("return",   token::RETURN);
	keywords_.emplace("string",   token::STRING);
	keywords_.emplace("switch",   token::SWITCH);
	keywords_.emplace("while",    token::WHILE);
	keywords_.emplace("function", token::FUNCTION);

	// setup types
	types_.insert("char");
	types_.insert("int");
	types_.insert("string");
	types_.insert("auto");

	// setup built in functions
	register_function("getch",        call_getch);
	register_function("getnum",       call_getnum);
	register_function("is_array",     call_is_array);
	register_function("is_character", call_is_character);
	register_function("is_function",  call_is_function);
	register_function("is_integer",   call_is_integer);
	register_function("is_string",    call_is_string);
	register_function("printf",       call_printf);
	register_function("putchar",      call_putchar);
	register_function("puts",         call_puts);
	register_function("size",         call_size);
	
	reset();
}

//-----------------------------------------------------------------------------
// Name: create_scope
//-----------------------------------------------------------------------------
void script_engine::create_scope() {
	assert(!function_variables_.empty());
	function_variables_.top().push_front({});
}

//-----------------------------------------------------------------------------
// Name: destroy_scope
//-----------------------------------------------------------------------------
void script_engine::destroy_scope() {
	assert(!function_variables_.empty());
	function_variables_.top().pop_front();
}

//-----------------------------------------------------------------------------
// Name: register_function
//-----------------------------------------------------------------------------
template <class F>
void script_engine::register_function(const std::string &name, F func) {
	builtin_functions_.emplace(name, wrap_function(func));
}

//-----------------------------------------------------------------------------
// Name: tokenize
// Desc: tokenizes the whole program array and places it
//       into program_
//-----------------------------------------------------------------------------
void script_engine::tokenize() {
	// this will quickly run through the program
	// tokenizing as it goes and pushing it the tokens onto
	// our list... this is our sort of "compiling"
	auto it = source_.begin();
	do {
		token_ = process_token(it, source_.end());
		program_.push_back(token_);
	} while(token_.type() != token::FINISHED);
}

//-----------------------------------------------------------------------------
// Name: is_type
// Desc: returns true if the string is a typename
//-----------------------------------------------------------------------------
bool script_engine::is_type(const std::string &s) const {
	return types_.find(s) != types_.end();
}

//-----------------------------------------------------------------------------
// Name: is_type
// Desc: returns true if the token is a typename
//-----------------------------------------------------------------------------
bool script_engine::is_type(const token &token) const {
	return is_type(to_string(token));
}

//-----------------------------------------------------------------------------
// Name: is_keyword
// Desc: returns true if the string is a keyword
//-----------------------------------------------------------------------------
bool script_engine::is_keyword(const std::string &s) const {
	return get_keyword(s) != token::UNKNOWN;
}

//-----------------------------------------------------------------------------
// Name: declare_function
// Desc:
//-----------------------------------------------------------------------------
void script_engine::declare_function(const std::string &name, int return_type, int &brace) {

	if(is_keyword(name)) {
		throw function_name_is_keyword();
	}

	// track the program counter's original location
	const address_t function_location = program_counter_;

	// we just saw an lparen, get next token...
	// it should be either a type or an rparen
	get_token();

	unsigned int param_count = 0;

	while(token_.type() != token::RPAREN) {

		// while we are at it, get some basic data on params
		if(is_type(token_)) {
			++param_count;

			get_token();

			// if we didn't read an identifier (the name of the variable)
			test_token<identifier_expected>(token::IDENTIFIER);

			get_token();
			if(token_.type() == token::COMMA) {
				get_token();
			} else if(token_.type() != token::RPAREN) {
				throw paren_expected();
			}
		} else {
			throw type_expected();
		}
	}

	// create a new function object and add it to our map
	if(functions_.insert(std::make_pair(name, function(return_type, function_location, param_count))).second == false) {
		throw duplicate_function();
	}

	// the token following a function's right paren should ALWAYS be
	// a left brace
	get_token();
	test_token<brace_expected>(token::LBRACE);
	++brace;
}

//-----------------------------------------------------------------------------
// Name: prescan
// Desc: Find the location of all functions in the
//       program and store global variables
//-----------------------------------------------------------------------------
void script_engine::prescan() {
	const address_t program_start = program_counter_;
	int brace = 0;  // When 0, this var tells us that
					// current source position is outside
					// of any function.

	prescan_ = true;

	do {
		while(brace != 0) {
			// bypass code inside functions
			get_token();
			if(token_.type() == token::LBRACE)      ++brace;
			else if(token_.type() == token::RBRACE) --brace;
		}

		// save current position
		const address_t typename_location = program_counter_;
		get_token();

		// global var type or function return type
		if(is_type(token_)) {

			// save data type
			const token::Type data_type = token_.type();
			get_token();

			if(token_.type() == token::IDENTIFIER) {
				auto identifier_name = to_string(token_);
				get_token();

				if(token_.type() != token::LPAREN) {
					// must be global var
					// return to start of declaration
					program_counter_ = typename_location;
					declare_global();
					test_token<semicolon_expected>(token::SEMICOLON);
				} else {
					// must be a function
					declare_function(identifier_name, data_type, brace);
				}
			} else {
				throw identifier_expected();
			}
		} else if(token_.type() != token::FINISHED) {
			throw type_expected();
		}
	} while(token_.type() != token::FINISHED);

	program_counter_ = program_start;

	prescan_ = false;
}


//-----------------------------------------------------------------------------
// Name: load_preprocessed_file
//-----------------------------------------------------------------------------
std::vector<char> script_engine::load_preprocessed_file(const std::string &name) {

	// TODO: implement some level of a pre-processor
	// and have the interpreter understand it
	// (most noteably includes which account for line number
	// adjustments)

	// the idea here is that imports_.top() will represent the file currently being imported
	imports_.push(name);

	std::vector<char> ret;
	std::ifstream file(name.c_str());
	if(file) {
		ret = std::vector<char>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	}
	
	imports_.pop();
	return ret;
}

//-----------------------------------------------------------------------------
// Name: load_program
//-----------------------------------------------------------------------------
bool script_engine::load_program(const std::string &name) {
	source_ = load_preprocessed_file(name);
	tokenize();
	prescan();
	return false;
}

//-----------------------------------------------------------------------------
// Name: get_keyword
// Desc: resolves a string keyword to a token
//-----------------------------------------------------------------------------
token::Type script_engine::get_keyword(const std::string &s) const {
	auto it = keywords_.find(s);
	return (it != keywords_.end()) ? it->second : token::UNKNOWN;
}

//-----------------------------------------------------------------------------
// Name: get_function
//-----------------------------------------------------------------------------
const function &script_engine::get_function(const std::string &name) const {

	auto it = functions_.find(name);
	if(it == functions_.end()) {
		throw undefined_function();
	}

	return it->second;
}

//-----------------------------------------------------------------------------
// Name: get_variable
//-----------------------------------------------------------------------------
variable &script_engine::get_variable(const std::string &name) {

	// first, see if it's a local variable
	if(!function_variables_.empty()) {
		locals_t &locals = function_variables_.top();

		for(std::vector<local_variable> &vars : locals) {		
			for(local_variable &local : vars) {
				if(local.name == name) {
					return local.value;
				}
			}
		}
	}

	// otherwise, try global vars
	auto it = global_variables_.find(name);
	if(it != global_variables_.end()) {
		return it->second;
	}

	// variable not found
	throw variable_not_found();
}

//-----------------------------------------------------------------------------
// Name: reset
//-----------------------------------------------------------------------------
void script_engine::reset() {

	// clear out global variables and functions
	global_variables_.clear();
	functions_.clear();

	// initialize local variable stack
	function_variables_ = std::stack<locals_t>();

	// set program pointer to start of program buffer
	program_counter_ = 0;
}

//-----------------------------------------------------------------------------
// Name: start
// Desc: really all this does is simulate what it would look like to the
//       interpreter if this function had been called by an already running
//       script
//-----------------------------------------------------------------------------
int script_engine::start(const std::string &function_name) {

	// setup call to entry point
	const function &func = get_function(function_name);

	program_counter_ = func.offset();

	// intialize our token to point to main
	token_ = token(token::IDENTIFIER, function_name);

	// back up to opening (
	--program_counter_;

	// call entry point to start interpreting
	return to_integer(call());
}

//-----------------------------------------------------------------------------
// Name: exec_return
//-----------------------------------------------------------------------------
int script_engine::exec_return() {

	// get return value, if any
	variable value;
	eval_exp(value);

	// must be followed by a semicolon...
	test_token<semicolon_expected>(token::SEMICOLON);
	return_value_.reset(value);
	return 1;
}

//-----------------------------------------------------------------------------
// Name: declare_variable
// Desc: common variable declaration code
//-----------------------------------------------------------------------------
template <class F>
variable &script_engine::declare_variable(F func) {
	// get type
	get_token();

	// save var type
	// TODO: make it get this from a general type system
	const token::Type var_type = token_.type();

	if(!is_type(token_)) {
		throw type_expected();
	}
	
	std::string last_variable;

	// process comma-separated list
	do {
		// get var name
		get_token();

		auto var_name = to_string(token_);
		last_variable = var_name;
		
		if(is_keyword(var_name)) {
			throw variable_name_is_keyword();
		}

		// get next token ( , or ; or  =)
		get_token();


		variable expression;
		if(token_.type() == token::ASSIGN) {

			eval_exp(expression);

			// get next token (, or ;)
			get_token();
		}
		
		// so, we get the expression and create the variable 
		// seperately so that we can force the variable to be
		// the requested type, and then when the expression result
		// gets assigned to the variable, the type safety can kick
		// in and make sure it is the right type.
		variable var;

		// force the variable to be a specific type
		switch(var_type) {
		case token::STRING:
			var = variable::create_string();
			break;
		case token::INT:
			var = variable::create_integer();
			break;
		case token::CHAR:
			var = variable::create_character();
			break;
		case token::AUTO:
			// generic!
			break;
		default:
			throw type_expected();
		}
		
		var = expression;

		// push it on the stack of variables
		func(var, var_name);

	} while(token_.type() == token::COMMA);
	
	return get_variable(last_variable);
}

//-----------------------------------------------------------------------------
// Name: declare_local
//-----------------------------------------------------------------------------
variable &script_engine::declare_local() {
	return declare_variable([this](const variable &v, const std::string &name) {
		push_local(v, name);
	});	
}

//-----------------------------------------------------------------------------
// Name: declare_global
//-----------------------------------------------------------------------------
variable &script_engine::declare_global() {
	return declare_variable([this](const variable &v, const std::string &name) {
		push_global(v, name);
	});
}

//-----------------------------------------------------------------------------
// Name: get_arguments
//-----------------------------------------------------------------------------
std::vector<variable> script_engine::get_arguments() {
	std::vector<variable> arguments;

	get_token();
	test_token<paren_expected>(token::LPAREN);

	// process a comma-separated list of values
	if(peek_token().type() != token::RPAREN) {
		do {
			variable argument_value;
			eval_exp(argument_value);
			arguments.push_back(std::move(argument_value));
			get_token();
		} while(token_.type() == token::COMMA);
	} else {
		// ok we just saw a rparen, eat it up so the parser is happy
		get_token();
	}

	test_token<paren_expected>(token::RPAREN);

	return arguments;
}

//-----------------------------------------------------------------------------
// Name: interpret_block
// Desc: Interpret a single statement or block of code. When
//       interp_block() returns from its initial call, the final
//       brace (or a return) in main() has been encountered.
//-----------------------------------------------------------------------------
int script_engine::interpret_block() {

	const int block_start = block_depth_;

	if(peek_token().type() != token::LBRACE) {
		throw brace_expected();
	}

	do {
		get_token();

		// see what kind of token is up
		if(is_type(token_)) {
			// declare local variables
			put_back();
			declare_local();
			test_token<semicolon_expected>(token::SEMICOLON);
		} else {

			switch(token_.type()) {
			case token::IDENTIFIER:
			case token::LPAREN:
				{
					put_back(); // restore token to input stream for
								// further processing by eval_exp()

					// process the expression, since this is a stand-alone
					// expression, we just ditch the return value
					variable value;
					eval_exp(value);

					test_token<semicolon_expected>(token::SEMICOLON);
				}
				break;

			// keywords
			case token::RETURN: // return from function call
				exec_return();

				do {
					get_token();
					if(token_.type() == token::LBRACE)      { ++block_depth_; create_scope(); }
					else if(token_.type() == token::RBRACE) { --block_depth_; destroy_scope(); }
				} while(block_start != block_depth_);

				// this return instead of a break is important
				// it makes sure that we stop interpreting this block
				return 1;

			case token::IF: // process an if statement
				if(exec_if()) {
					do {
						get_token();
						if(token_.type() == token::LBRACE)      { ++block_depth_; create_scope(); }
						else if(token_.type() == token::RBRACE) { --block_depth_; destroy_scope(); }
					} while(block_start != block_depth_);
					return 1;
				}
				break;

			case token::WHILE: // process a while loop
				if(exec_while()) {
					do {
						get_token();
						if(token_.type() == token::LBRACE)      { ++block_depth_; create_scope(); }
						else if(token_.type() == token::RBRACE) { --block_depth_; destroy_scope(); }
					} while(block_start != block_depth_);
					return 1;
				}
				break;

			case token::DO: // process a do-while loop
				if(exec_do()) {
					do {
						get_token();
						if(token_.type() == token::LBRACE)      { ++block_depth_; create_scope(); }
						else if(token_.type() == token::RBRACE) { --block_depth_; destroy_scope(); }
					} while(block_start != block_depth_);
					return 1;

				}
				break;

			case token::FOR: // process a for loop
				if(exec_for()) {
					do {
						get_token();
						if(token_.type() == token::LBRACE)      { ++block_depth_; create_scope(); }
						else if(token_.type() == token::RBRACE) { --block_depth_; destroy_scope(); }
					} while(block_start != block_depth_);
					return 1;

				}
				break;

			case token::BREAK:
				throw unimplemented_keyword();
				break;

			case token::CONTINUE:
				throw unimplemented_keyword();
				break;

			case token::SWITCH:
				throw unimplemented_keyword();
				break;

			case token::CASE:
				throw unimplemented_keyword();
				break;

			// block delimiters
			case token::LBRACE:
				++block_depth_;
				create_scope();
				break;

			case token::RBRACE:
				destroy_scope();
				--block_depth_;
				break;

			case token::SEMICOLON:
				// do nothing :)
				break;

			default:
				throw syntax_error();
				break;
			}
		}
	} while (token_.type() != token::FINISHED && (block_depth_ != block_start));

	return 0;
}

//-----------------------------------------------------------------------------
// Name: push_function
//-----------------------------------------------------------------------------
void script_engine::push_function() {
	call_stack_.push(program_counter_);
}

//-----------------------------------------------------------------------------
// Name: pop_function
//-----------------------------------------------------------------------------
address_t script_engine::pop_function() {
	if(call_stack_.empty()) {
		throw return_outside_call();
	}

	const address_t return_address = call_stack_.top();
	call_stack_.pop();
	return return_address;
}

//-----------------------------------------------------------------------------
// Name: call
//-----------------------------------------------------------------------------
variable script_engine::call(const function &func) {

	// is it a builtin?
	if(!func.name().empty()) {
		auto it = builtin_functions_.find(func.name());
		if(it != builtin_functions_.end()) {
		
			get_token();
			test_token<paren_expected>(token::LPAREN);
		
			return_value_.reset(variable(it->second(this)));
		
			get_token();
			test_token<paren_expected>(token::RPAREN);
		
			return return_value_;
		}
	}


	const std::vector<variable> args = get_arguments();
	std::vector<std::string> argument_names;

	if(args.size() != func.param_count()) {
		throw incorrect_param_count();
	}


	push_function();                  // save return location
	program_counter_ = func.offset(); // set program_counter_ to start of function


	get_parameter_metadata(args, argument_names); // load the function's parameters with
                                                  // the values of the arguments
												  

	function_variables_.push(locals_t());
	create_scope();

	// finally, push the parameters onto the stack
	for(size_t i = 0; i < args.size(); ++i) {
		push_local(args[i], argument_names[i]);
	}
	
	const int return_seen = interpret_block(); // interpret the function
	program_counter_ = pop_function();   // reset the program pointer

	destroy_scope();
	function_variables_.pop();

	// if no return seen, return int(0)
	if(!return_seen) {
		return_value_.reset(variable(0));
	}

	// force the variable to be a specific type
	switch(func.type()) {
	case token::STRING:
		if(!is_string(return_value_)) throw invalid_type_conversion();
		break;
	case token::INT:
		if(!is_integer(return_value_)) throw invalid_type_conversion();
		break;
	case token::CHAR:
		if(!is_character(return_value_)) throw invalid_type_conversion();
		break;
	case token::AUTO:
		// generic!
		break;
	default:
		throw type_expected();
	}

	// should be setup by now
	return return_value_;
}

//-----------------------------------------------------------------------------
// Name: call
//-----------------------------------------------------------------------------
variable script_engine::call(const std::string &function_name) {
	return call(get_function(function_name));
}

//-----------------------------------------------------------------------------
// Name: call
//-----------------------------------------------------------------------------
variable script_engine::call() {
	return call(to_string(token_));
}

//-----------------------------------------------------------------------------
// Name: get_parameter_metadata
// Desc: binds types/name to arguments of a function when a function gets
//       called, until this, they are typeless and nameless
//-----------------------------------------------------------------------------
void script_engine::get_parameter_metadata(const std::vector<variable> &arguments, std::vector<std::string> &names) {

	int i = 0;
	do {
		// process comma-separated list of parameters
		get_token();
		if(token_.type() != token::RPAREN) {

			if(!is_type(token_)) {
				throw type_expected();
			}

			// link parameter type with argument already on
			// local var stack
			const token::Type type = token_.type();

			switch(type) {
			case token::STRING:
				if(!is_string(arguments[i])) throw invalid_type_conversion();
				break;
			case token::INT:
				if(!is_integer(arguments[i])) throw invalid_type_conversion();
				break;
			case token::CHAR:
				if(!is_character(arguments[i])) throw invalid_type_conversion();
				break;
			case token::AUTO:
				// generic!
				break;
			default:
				throw type_expected();
			}

			get_token();

			// link parameter name with argument already on
			// local var stack
			names.push_back(to_string(token_));

			get_token();
			++i;
		}
	} while(token_.type() == token::COMMA);

	// if it wasn't a comma (which is why we are here)
	// then it should have been an rparen
	test_token<paren_expected>(token::RPAREN);

	assert(arguments.size() == names.size());
}

//-----------------------------------------------------------------------------
// Name: push_local
//-----------------------------------------------------------------------------
void script_engine::push_local(const variable &v, const std::string &name) {

	locals_t &locals = function_variables_.top();
	std::vector<local_variable> &current_scope = locals.front();

	for(const local_variable &local : current_scope) {
		if(local.name == name) {
			throw duplicate_local();
		}
	}

	current_scope.push_back({name, v});
}

//-----------------------------------------------------------------------------
// Name: push_global
//-----------------------------------------------------------------------------
void script_engine::push_global(const variable &v, const std::string &name) {
	if(global_variables_.insert(std::make_pair(name, v)).second == false) {
		throw duplicate_global();
	}
}

//-----------------------------------------------------------------------------
// Name: is_variable
//-----------------------------------------------------------------------------
bool script_engine::is_variable(const token &tok) const {
	return is_variable(to_string(tok));
}

//-----------------------------------------------------------------------------
// Name: is_variable
//-----------------------------------------------------------------------------
bool script_engine::is_variable(const std::string &name) const {

	if(!function_variables_.empty()) {
		const locals_t &locals = function_variables_.top();

		// first, see if it's a local variable
		for(const std::vector<local_variable> &local : locals) {
			for(const local_variable& elem : local) {
				if(elem.name == name) {
					return true;
				}
			}
		}
	}

	// otherwise, try global vars
	auto it = global_variables_.find(name);
	return it != global_variables_.end();
}

//-----------------------------------------------------------------------------
// Name: is_function
//-----------------------------------------------------------------------------
bool script_engine::is_function(const std::string &name) const {
	auto it = functions_.find(name);
	return it != functions_.end();
}

//-----------------------------------------------------------------------------
// Name: is_function
//-----------------------------------------------------------------------------
bool script_engine::is_function(const token &tok) const {
	return is_function(to_string(tok));
}

//-----------------------------------------------------------------------------
// Name: exec_if
//-----------------------------------------------------------------------------
int script_engine::exec_if() {

	// make sure that the expression is in parens following the if
	if(peek_token().type() != token::LPAREN) {
		throw paren_expected();
	}

	// get if expression
	variable cond;
	eval_exp(cond);

	if(to_integer(cond)) {

		// is true so process target of IF
		if(interpret_block()) {
			return 1;
		}

		// now we need to eat up as many if's as neccessary
		// until we are no longer on an if chain

		while(peek_token().type() == token::ELSE) {

			// eat up the "else"
			get_token();

			if(peek_token().type() == token::IF) {
				// eat the if
				get_token();

				// eat the expression
				while(token_.type() != token::RPAREN) {
					get_token();
				}

				// ok, we've just eaten up the rparam
				test_token<paren_expected>(token::RPAREN);
			}

			// get to the next "}", note, find_eob will eat up the starting "{" too
			find_eob();
			test_token<brace_expected>(token::RBRACE);
		}
	} else {

		// otherwise skip around IF block and
		// process the ELSE, if present

		find_eob(); // find start of next line

		get_token();

		if(token_.type() != token::ELSE) {
			put_back();  // restore token if no ELSE is present
		} else {
			if(peek_token().type() == token::IF) {
				// we are in an else if block..
				get_token();
				return exec_if();
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Name: exec_while
//-----------------------------------------------------------------------------
int script_engine::exec_while() {

	// save location of top of while loop
	// minus one because we want to reprocess the while each time
	const address_t while_start = program_counter_ - 1;

	// make sure that the expression is in parens following the while
	if(peek_token().type() != token::LPAREN) {
		throw paren_expected();
	}

	// get if expression
	variable cond;
	eval_exp(cond);

	if(to_integer(cond)) {
		if(interpret_block()) { // if true, interpret
			return 1;
		}

		// loop back to top
		program_counter_ = while_start;
	} else {
		// otherwise, skip around loop
		find_eob();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Name: exec_do
//-----------------------------------------------------------------------------
int script_engine::exec_do() {
	// save location of top of do loop
	const address_t do_start = program_counter_ - 1;

	// interpret loop
	if(interpret_block()) {
		return 1;
	}

	get_token();
	test_token<while_expected>(token::WHILE);

	// make sure that the expression is in parens following the while
	if(peek_token().type() != token::LPAREN) {
		throw paren_expected();
	}

	variable cond;
	eval_exp(cond); // check the loop condition
	test_token<semicolon_expected>(token::SEMICOLON);

	if(to_integer(cond)) {
		program_counter_ = do_start; // if true loop; otherwise, continue on
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Name: find_eob
//-----------------------------------------------------------------------------
void script_engine::find_eob() {
	int brace = 1;

	// for this to work, we need to eat the first {
	get_token();
	test_token<brace_expected>(token::LBRACE);

	do {
		get_token();
		if(token_.type() == token::LBRACE)      ++brace;
		else if(token_.type() == token::RBRACE) --brace;
	} while(brace);
}


//-----------------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------
int script_engine::exec_foreach_body(variable &it) {
	
	get_token();
	test_token<colon_expected>(token::COLON);

	variable var;
	eval_exp(var);

	const int sz = size(var);

	get_token();
	test_token<paren_expected>(token::RPAREN);

	const address_t loop_start = program_counter_;
	for(int i = 0; i < sz; ++i) {
		it.reset(var[variable(i)]);
		if(interpret_block()) {
			destroy_scope();
			return 1;
		}
		program_counter_ = loop_start;

	}
	find_eob();

	destroy_scope();
	return 0;
}

//-----------------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------
int script_engine::exec_for_body() {
	// should be followed by a semicolon
	test_token<semicolon_expected>(token::SEMICOLON);

	// get past the ;
	get_token();

	// remember this spot, it's the loop invariant
	const address_t invariant_address = program_counter_;

	for(;;) {
		variable cond;
		eval_exp(cond); // check the condition

		test_token<semicolon_expected>(token::SEMICOLON);

		// get past the ;
		get_token();

		// this points to the loop increment part
		const address_t increment_address = program_counter_;

		// find the start of the for block
		int brace = 1;
		do {
			get_token();
			if(token_.type() == token::LPAREN)      ++brace;
			else if(token_.type() == token::RPAREN) --brace;
		} while(brace);

		if(to_integer(cond)) {
			if(interpret_block()) {  // if true, interpret
				destroy_scope();
				return 1;
			}
		} else {
			// otherwise, skip around loop
			find_eob();
			break;
		}

		program_counter_ = increment_address;
		eval_exp(cond);                       // do the increment
		program_counter_ = invariant_address; // loop back to top
	}

	destroy_scope();
	return 0;
}

//-----------------------------------------------------------------------------
// Name: exec_for
//-----------------------------------------------------------------------------
int script_engine::exec_for() {

	create_scope();

	// eat up the leading (
	get_token();
	test_token<paren_expected>(token::LPAREN);

	// initialization expression
	if(is_type(peek_token())) {
		variable &var = declare_local();
		if(current_token().type() == token::COLON) {
			put_back();
			return exec_foreach_body(var);
		}
		put_back();
	} else {
		variable init_expression;
		eval_exp(init_expression);
	}
	
	return exec_for_body();
}

//-----------------------------------------------------------------------------
// Name: put_back
//-----------------------------------------------------------------------------
void script_engine::put_back() {
	--program_counter_;
}

//-----------------------------------------------------------------------------
// Name: get_token
//-----------------------------------------------------------------------------
token &script_engine::get_token() {
	token_ = peek_token();
	++program_counter_;
	return token_;
}

//-----------------------------------------------------------------------------
// Name: peek_token
//-----------------------------------------------------------------------------
token &script_engine::peek_token() {
	if(program_counter_ >= program_.size()) {
		throw unexpected_eof();
	}

	return program_[program_counter_];
}

//-----------------------------------------------------------------------------
// Name: process_token
//-----------------------------------------------------------------------------
template <class In>
token script_engine::process_token(In &it, In end) const {

	using std::isdigit;
	using std::isalpha;

	std::string temp_string;

	// eat up all the comments and whitespace
	// loop cause there may be more than one in a row
	do {
		skip_whitespace(it, end);
	} while(skip_comments(it, end));

	if(it == end) {
		return token(token::FINISHED);
	}

	switch(*it) {
	case '{':
		++it;
		return token(token::LBRACE);
	case '}':
		++it;
		return token(token::RBRACE);
	case '[':
		++it;
		return token(token::LBRACKET);
	case ']':
		++it;
		return token(token::RBRACKET);
	case '.':
		++it;
		return token(token::DOT);
	case ';':
		++it;
		return token(token::SEMICOLON);
	case '(':
		++it;
		return token(token::LPAREN);
	case ')':
		++it;
		return token(token::RPAREN);
	case ',':
		++it;
		return token(token::COMMA);
	case '~':
		++it;
		return token(token::CMP);

	case ':':
		++it;
		if(it != end && *it == ':') {
			++it;
			return token(token::DOUBLECOLON);
		} else {
			return token(token::COLON);
		}
	case '/':
		++it;
		if(it != end && *it == '=') {
			++it;
			return token(token::DIV_EQ);
		} else {
			return token(token::DIV);
		}
	case '^':
		++it;
		if(it != end && *it == '=') {
			++it;
			return token(token::XOR_EQ);
		} else {
			return token(token::XOR);
		}
	case '=':
		++it;
		if(it != end && *it == '=') {
			++it;
			return token(token::EQ);
		} else {
			return token(token::ASSIGN);
		}
	case '+':
		++it;
		if(it != end && *it == '=') {
			++it;
			return token(token::PLUS_EQ);
		} else {
			return token(token::PLUS);
		}
	case '-':
		++it;
		if(it != end && *it == '=') {
			++it;
			return token(token::MINUS_EQ);
		} else {
			return token(token::MINUS);
		}
	case '!':
		++it;
		if(it != end && *it == '=') {
			++it;
			return token(token::NE);
		} else {
			return token(token::NOT);
		}
	case '*':
		++it;
		if(it != end && *it == '=') {
			++it;
			return token(token::MUL_EQ);
		} else {
			return token(token::MUL);
		}
	case '%':
		++it;
		if(it != end && *it == '=') {
			++it;
			return token(token::MOD_EQ);
		} else {
			return token(token::MOD);
		}
	case '&':
		++it;
		if(it != end && *it == '=') {
			++it;
			return token(token::AND_EQ);
		} else if(it != end && *it == '&') {
			++it;
			return token(token::LOGICAL_AND);
		} else {
			return token(token::AND);
		}
	case '|':
		++it;
		if(it != end && *it == '=') {
			++it;
			return token(token::OR_EQ);
		} else if(it != end && *it == '|') {
			++it;
			return token(token::LOGICAL_OR);
		} else {
			return token(token::OR);
		}
	case '>':
		++it;
		if(it != end && *it == '>') {
			++it;
			if(it != end && *it == '=') {
				++it;
				return token(token::RSHIFT_EQ);
			} else {
				return token(token::RSHIFT);
			}
		} else if(it != end && *it == '=') {
			++it;
			return token(token::GE);
		} else {
			return token(token::GT);
		}
	case '<':
		++it;
		if(it != end && *it == '<') {
			++it;
			if (it != end && *it == '=') {
				++it;
				return token(token::LSHIFT_EQ);
			} else {
				return token(token::LSHIFT);
			}
		} else if(it != end && *it == '=') {
			++it;
			return token(token::LE);
		} else {
			return token(token::LT);
		}

	// character constant
	case '\'':
		// skip past opening quote
		++it;

		temp_string = process_char(it, end);

		// the next character better be a single quote
		if(it == end || *it != '\'') {
			throw quote_expected();
		}

		// skip past the ending quote
		++it;

		return token(token::CHARACTER, temp_string);

	// quoted string
	case '"':

		// skip past opening quote
		++it;

		// start with empty string
		temp_string = "";

		while(it != end && *it != '"' && *it != '\n') {
			temp_string += process_char(it, end);
		}

		if(it == end) {
			throw unexpected_eof();
		}

		// make sure we stoped because of an ending quote, not a newline...
		if(*it != '"') {
			throw multiline_string_literal();
		}

		// skip past the ending quote
		++it;

		return token(token::STRING_LITERAL, temp_string);

	default:
		if(isdigit(*it)) {
			// number
			temp_string = "";
			char ch;

			while(it != end && !is_delim((ch = *it))) {
				temp_string += ch;
				++it;
			}

			if(it == end) {
				throw unexpected_eof();
			}

			return token(token::INTEGER, temp_string);

		} else if(isalpha(*it)) {
			// var or command
			temp_string = "";
			char ch;

			while(it != end && !is_delim((ch = *it))) {
				temp_string += ch;
				++it;
			}

			if(it == end) {
				throw unexpected_eof();
			}

			// is a keyword or an identifier
			if(is_keyword(temp_string)) {
				return token(get_keyword(temp_string), temp_string);
			} else {
				return token(token::IDENTIFIER, temp_string);
			}		
		}
	}
	
	throw syntax_error();
}

//-----------------------------------------------------------------------------
// Name: eval_exp
// Desc: entry point of expression parser
//-----------------------------------------------------------------------------
void script_engine::eval_exp(variable &value) {
	get_token();

	if(token_.type() == token::SEMICOLON) {
		// empty expression
	} else {
		eval_exp0(value);
		// return last token read to input stream
		put_back();
	}
}


//-----------------------------------------------------------------------------
// Name: eval_exp0
// Desc: Process an assignment expression
//-----------------------------------------------------------------------------
void script_engine::eval_exp0(variable &value) {

	if(token_.type() == token::IDENTIFIER) {
		if(is_variable(token_)) {

			// holds name of var receiving the assignment
			const token temp_token = token_;
			variable &var_ref      = get_variable(to_string(temp_token));
			get_token();

			// if a var, see if assignment
			switch(token_.type()) {
			case token::ASSIGN:
				get_token();
				eval_exp0(value);
				var_ref = value;
				return;
			case token::PLUS_EQ:
				get_token();
				eval_exp0(value);
				var_ref += value;
				return;
			case token::MINUS_EQ:
				get_token();
				eval_exp0(value);
				var_ref -= value;
				return;
			case token::AND_EQ:
				get_token();
				eval_exp0(value);
				var_ref &= value;
				return;
			case token::OR_EQ:
				get_token();
				eval_exp0(value);
				var_ref |= value;
				return;
			case token::XOR_EQ:
				get_token();
				eval_exp0(value);
				var_ref ^= value;
				return;
			case token::MUL_EQ:
				get_token();
				eval_exp0(value);
				var_ref *= value;
				return;
			case token::MOD_EQ:
				get_token();
				eval_exp0(value);
				var_ref %= value;
				return;
			case token::DIV_EQ:
				get_token();
				eval_exp0(value);
				var_ref /= value;
				return;
			case token::LSHIFT_EQ:
				get_token();
				eval_exp0(value);
				var_ref <<= value;
				return;
			case token::RSHIFT_EQ:
				get_token();
				eval_exp0(value);
				var_ref >>= value;
				return;
			default:
				// roll back this read
				put_back();
				token_ = temp_token;
			}
		}
	}

	eval_exp1(value);
}

//-----------------------------------------------------------------------------
// Name: eval_exp1
// Desc: Process logical operations
//-----------------------------------------------------------------------------
void script_engine::eval_exp1(variable &value) {
	int op;

	eval_exp2(value);

	while((op = token_.type()) == token::LOGICAL_AND || op == token::LOGICAL_OR) {
		variable partial_value;

		get_token();
		eval_exp2(partial_value);

		// perform the relational operation
		switch(op) {
		case token::LOGICAL_AND:
			value = value && partial_value;
			break;

		case token::LOGICAL_OR:
			value = value || partial_value;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: eval_exp2
// Desc: Process binary operations
//-----------------------------------------------------------------------------
void script_engine::eval_exp2(variable &value) {
	int op;

	eval_exp3(value);

	while((op = token_.type()) == token::AND || op == token::XOR || op == token::OR) {
		variable partial_value;

		get_token();
		eval_exp3(partial_value);

		// add or subtract
		switch(op) {
		case token::AND:
			value &= partial_value;
			break;

		case token::OR:
			value |= partial_value;
			break;

		case token::XOR:
			value ^= partial_value;
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// Name: eval_exp3
// Desc: Process relational operators
//-----------------------------------------------------------------------------
void script_engine::eval_exp3(variable &value) {
	int op;

	eval_exp4(value);

	while((op = token_.type()) == token::LT || op == token::LE || op == token::GT || op == token::GE || op == token::EQ || op == token::NE) {
		variable partial_value;

		get_token();
		eval_exp4(partial_value);

		// perform the relational operation
		switch(op) {
		case token::LT:
			value = value < partial_value;
			break;

		case token::LE:
			value = value <= partial_value;
			break;

		case token::GT:
			value = value > partial_value;
			break;

		case token::GE:
			value = value >= partial_value;
			break;

		case token::EQ:
			value = value == partial_value;
			break;

		case token::NE:
			value = value != partial_value;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: eval_exp4
// Desc: Process shift operations
//-----------------------------------------------------------------------------
void script_engine::eval_exp4(variable &value) {
	int op;

	eval_exp5(value);

	while((op = token_.type()) == token::RSHIFT || op == token::LSHIFT) {
		variable partial_value;

		get_token();
		eval_exp5(partial_value);

		// perform the shift operation
		switch(op) {
		case token::LSHIFT:
			value <<= partial_value;
			break;

		case token::RSHIFT:
			value >>= partial_value;
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// Name: eval_exp5
// Desc: Add or subtract two terms
//-----------------------------------------------------------------------------
void script_engine::eval_exp5(variable &value) {
	int op;

	eval_exp6(value);

	while((op = token_.type()) == token::PLUS || op == token::MINUS) {
		variable partial_value;

		get_token();
		eval_exp6(partial_value);

		// add or subtract
		switch(op) {
		case token::MINUS:
			value -= partial_value;
			break;

		case token::PLUS:
			value += partial_value;
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// Name: eval_exp6
// Desc: Multiply or divide two factors
//-----------------------------------------------------------------------------
void script_engine::eval_exp6(variable &value) {

	int op;

	eval_exp7(value);

	while((op = token_.type()) == token::MUL || op == token::DIV || op == token::MOD) {
		variable partial_value;

		get_token();
		eval_exp7(partial_value);

		// mul, div, or modulus
		switch(op) {
		case token::MUL:
			value *= partial_value;
			break;

		case token::DIV:
			value /= partial_value;
			break;

		case token::MOD:
			value %= partial_value;
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// Name: eval_exp7
// Desc: Is a unary +, - or ~
//-----------------------------------------------------------------------------
void script_engine::eval_exp7(variable &value) {

	int op;

	if((op = token_.type()) == token::PLUS || op == token::MINUS || op == token::CMP || op == token::NOT) {
		get_token();
	}

	eval_exp8(value);

	switch(op) {
	case token::MINUS:
		value = -value;
		break;
	case token::CMP:
		value = ~value;
		break;
	case token::PLUS:
		value = +value;
		break;
	case token::NOT:
		value = !value;
		break;
	}
}


//-----------------------------------------------------------------------------
// Name: eval_exp8
// Desc: Process parenthesized expression
//-----------------------------------------------------------------------------
void script_engine::eval_exp8(variable &value) {

	switch(token_.type()) {
	case token::LPAREN:
		get_token();
		// get subexpression
		eval_exp0(value);
		test_token<paren_expected>(token::RPAREN);
		get_token();
		break;
	default:
		atom(value);
		break;
	}
}


//-----------------------------------------------------------------------------
// Name: atom
// Desc: Find value of number, variable, or function.
//-----------------------------------------------------------------------------
void script_engine::atom(variable &value) {

	// so, we could just reuse value this whole time...
	// but, if we did that, then we would lose the original
	// type of the variable being set, and would lose the automatic
	// detection of incorrect types we have in place, so we work with the 
	// temporary until the whole thing is ready, then assign it. At that point
	// it better have the right type!
	
	variable partial_value;
	
	auto token_string = to_string(token_);
	
	switch(token_.type()) {
	case token::FUNCTION:
		partial_value = function_literal();
		break;
		
	case token::LBRACKET:
		partial_value = array_literal();
		break;
	
	case token::IDENTIFIER:
		if(is_variable(token_string)) {
			// get var's value
			partial_value = get_variable(token_string);
		} else if(is_function(token_string)) {
			// call user-defined function
			partial_value = variable(get_function(token_string));
		} else {
			// call "built-in" function
			if(builtin_functions_.find(token_string) == builtin_functions_.end()) {
				throw unknown_identifier();
			}
			partial_value = variable(function(token_string));
		}
		break;

	case token::INTEGER:
		// is a numeric constant
		partial_value = variable(static_cast<int>(stol(token_string, nullptr, 0)));
		break;

	case token::CHARACTER:
		// is a character constant
		partial_value = variable(token_string[0]);
		break;

	case token::STRING_LITERAL:
		// is a string constant
		partial_value = variable(token_string);
		break;
		
	default:
	
		// constructor syntax for types
		// like: // string("hello") or just string()
		if(is_type(token_) && peek_token().type() == token::LPAREN) {
			
			token::Type var_type = token_.type();
		
			get_token();
			
			if(peek_token().type() != token::RPAREN) {
				variable value;
				eval_exp(value);

				switch(var_type) {
				case token::STRING:
					partial_value = variable(to_string(value));
					break;
				case token::INT:
					partial_value = variable(to_integer(value));
					break;
				case token::CHAR:
					// special case integer -> character explicit conversion
					partial_value = variable(static_cast<char>(to_integer(value)));					
					break;
				default:
					throw type_expected();
				}
			} else {
				switch(var_type) {
				case token::STRING:
					partial_value = variable("");
					break;
				case token::INT:
					partial_value = variable(0);
					break;
				case token::CHAR:
					partial_value = variable('\0');
					break;
				default:
					throw type_expected();
				}
			}
			
			get_token();
			test_token<paren_expected>(token::RPAREN);
			
		} else {
			throw syntax_error(); // syntax error
		}
	}

	// handle R-VALUE indexes
	
	while(peek_token().type() == token::LBRACKET || peek_token().type() == token::LPAREN) {
		switch(peek_token().type()) {
		case token::LBRACKET:
			{
				get_token();
				variable index_expression;

				eval_exp(index_expression);

				get_token();
				test_token<bracket_expected>(token::RBRACKET);

				partial_value.reset(partial_value[index_expression]);
			}
			break;
		case token::LPAREN:
			{
				if(prescan_) {
					throw function_during_prescan();
				}
				partial_value.reset(call(partial_value()));
			}
			break;
		default:
			break;
		}
	}
	
	value = partial_value;
	get_token();
}


//-----------------------------------------------------------------------------
// Name: array_literal
//-----------------------------------------------------------------------------
variable script_engine::array_literal() {
	test_token<brace_expected>(token::LBRACKET);
		
	std::vector<variable_base::pointer> contents;
	
	if(peek_token().type() == token::RBRACKET) {
		get_token();
	} else {
	
		while(token_.type() != token::RBRACKET) {

			variable value;
			eval_exp(value);

			contents.push_back(value.get());

			get_token();
			if(token_.type() != token::COMMA) {
				break;
			}
		}
	}
	
	test_token<brace_expected>(token::RBRACKET);
	
	return variable(std::move(contents));
}

//-----------------------------------------------------------------------------
// Name: function_literal
//-----------------------------------------------------------------------------
variable script_engine::function_literal() {
	
	get_token();
	test_token<paren_expected>(token::LPAREN);

	// track the program counter's original location
	const address_t function_location = program_counter_;

	// we just saw an lparen, get next token...
	// it should be either a type or an rparen
	get_token();

	unsigned int param_count = 0;

	while(token_.type() != token::RPAREN) {

		// while we are at it, get some basic data on params
		if(is_type(token_)) {
			++param_count;

			get_token();

			// if we didn't read an identifier (the name of the variable)
			test_token<identifier_expected>(token::IDENTIFIER);

			get_token();
			if(token_.type() == token::COMMA) {
				get_token();
			} else if(token_.type() != token::RPAREN) {
				throw paren_expected();
			}
		} else {
			throw type_expected();
		}
	}

	const variable partial_value = variable(function(token::AUTO, function_location, param_count));

	// the token following a function's right paren should ALWAYS be
	// a left brace
	get_token();
	test_token<brace_expected>(token::LBRACE);
	int brace = 1;

	while(brace != 0) {
		// bypass code inside functions
		get_token();
		if(token_.type() == token::LBRACE)      ++brace;
		else if(token_.type() == token::RBRACE) --brace;
	}
	
	return partial_value;
}
