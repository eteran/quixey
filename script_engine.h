
#ifndef SCRIPTENGINE_20060121_H_
#define SCRIPTENGINE_20060121_H_

#include "function.h"
#include "script_types.h"
#include "token.h"
#include "variable.h"

#include <functional>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct local_variable {
	std::string name;
	variable    value;
};

class script_engine {
public:
	script_engine();

public:
	void eval_exp(variable &value);

private:
	void eval_exp0(variable &value); // Process an assignment expression
	void eval_exp1(variable &value); // Process comparitive operators
	void eval_exp2(variable &value); // Process binary operators
	void eval_exp3(variable &value); // Process relational operators
	void eval_exp4(variable &value); // Process shift operators
	void eval_exp5(variable &value); // Add or subtract two terms
	void eval_exp6(variable &value); // Multiply or divide two factors
	void eval_exp7(variable &value); // Is a unary +, - or !
	void eval_exp8(variable &value); // Process parenthesized expression
	void atom(variable &value);

private:
	token    token_;
	variable return_value_;

private:
	typedef std::function<int(script_engine *)>          builtin_t;
	typedef std::unordered_map<std::string, variable>    globals_t;
	typedef std::deque<std::vector<local_variable>>      locals_t;
	typedef std::unordered_map<std::string, builtin_t>   builtin_functions_t;
	typedef std::unordered_map<std::string, function>    functions_t;
	typedef std::unordered_map<std::string, token::Type> keywords_t;

private:
	std::unordered_set<std::string> types_;
	keywords_t                      keywords_;
	builtin_functions_t             builtin_functions_;
	std::vector<char>               source_;
	std::vector<token>              program_;
	std::stack<address_t>           call_stack_;

private:
	// cleared on reset
	globals_t               global_variables_;
	functions_t             functions_;
	address_t               program_counter_;
	std::stack<locals_t>    function_variables_;
	int                     block_depth_;
	bool                    prescan_;
	std::stack<std::string> imports_;

public:
	token &get_token();
	void reset();
	void prescan();
	void tokenize();
	bool load_program(const std::string &name);
	int start(const std::string &function);

private:
	address_t pop_function();
	bool is_keyword(const std::string &s) const;
	bool is_type(const std::string &s) const;
	bool is_type(const token &tok) const;
	bool is_valid(address_t address) const;
	bool is_variable(const std::string &name) const;
	bool is_variable(const token &tok) const;
	bool is_function(const std::string &name) const;
	bool is_function(const token &tok) const;
	bool skip_comments();
	char process_char();
	const function &get_function(const std::string &name) const;
	int exec_do();
	int exec_for();
	int exec_for_body();
	int exec_foreach_body(variable &it);
	int exec_if();
	int exec_return();
	int exec_while();
	int interpret_block();
	std::vector<variable> get_arguments();
	token &peek_token();
	token::Type get_keyword(const std::string &s) const;
	variable &get_variable(const std::string &name);
	variable call();
	variable call(const function &func);
	variable call(const std::string &name);
	variable function_literal();
	variable array_literal();
	void create_scope();
	void declare_function(const std::string &name, int return_type, int &brace);
	variable &declare_global();
	variable &declare_local();
	void destroy_scope();
	void find_eob();
	void get_parameter_metadata(const std::vector<variable> &arguments, std::vector<std::string> &names);
	void process_token();
	void push_function();
	void push_global(const variable &v, const std::string &name);
	void push_local(const variable &v, const std::string &name);
	void put_back();
	void skip_whitespace();
	std::vector<char> load_preprocessed_file(const std::string &name);

	template <class F>
	variable &declare_variable(F func);

public:
	unsigned int line_number(address_t offset) const;
	unsigned int line_number() const { return line_number(program_counter_); }

	const token &current_token() const { return token_; }

public:
	template<class T>
	void test_token(token::Type expected) const {
		if (current_token().type() != expected) {
			throw T();
		}
	}
	
	template <class F>
	void register_function(const std::string &name, F func);
};

#endif
