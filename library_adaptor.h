
#ifndef LIBRARY_ADAPTOR_H_
#define LIBRARY_ADAPTOR_H_

#include "error.h"
#include "script_engine.h"
#include <boost/type_traits.hpp>
#include <functional>
#include <type_traits>
#include <cassert>

// helpers to fetch arguments more genrically
namespace {
	template <class T>
	struct argument_handler {};

	template <>
	struct argument_handler<int> {
		argument_handler(script_engine *engine) : engine_(engine) {
			assert(engine);
		}
	
		int operator()() const {
			variable ret;
			engine_->eval_exp(ret);
			return to_integer(ret);
		}
	private:
		script_engine *const engine_;
	};

	template <>
	struct argument_handler<char> {
		argument_handler(script_engine *engine) : engine_(engine) {
			assert(engine);
		}
	
		char operator()() const {
			variable ret;
			engine_->eval_exp(ret);
			return static_cast<char>(to_integer(ret));
		}
	private:
		script_engine *const engine_;
	};

	template <>
	struct argument_handler<std::string> {
		argument_handler(script_engine *engine) : engine_(engine) {
			assert(engine);
		}
	
		std::string operator()() const {
			variable ret;
			engine_->eval_exp(ret);
			return to_string(ret);
		}
	private:
		script_engine *const engine_;
	};
	
	template <>
	struct argument_handler<variable> {
		argument_handler(script_engine *engine) : engine_(engine) {
			assert(engine);
		}
	
		variable operator()() const {
			variable ret;
			engine_->eval_exp(ret);
			return ret;
		}
	private:
		script_engine *const engine_;
	};
	
	template <>
	struct argument_handler<script_engine *> {
		argument_handler(script_engine *engine) : engine_(engine) {
			assert(engine);
		}
	
		script_engine *operator()() const {
			return engine_;
		}
	private:
		script_engine *const engine_;
	};
	
}

template <class T>
void fetch_and_test_token_x(script_engine *engine, token::Type expected) {
	assert(engine);
	const token &tok = engine->get_token();
	if(tok.type() != expected) {
		throw T();
	}
}

template <class F>
struct function_helper0 {
public:
	explicit function_helper0(F f) : f_(f) {
	}

	int operator()(script_engine *engine) {
		(void)engine;
		assert(engine);
		return f_();
	}
private:
	F f_;
};

template <class F>
struct function_helper1 {
public:
	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg1_type T1;

	explicit function_helper1(F f) : f_(f) {
	}

	int operator()(script_engine *engine) {
		assert(engine);
		const T1 arg1 = argument_handler<T1>(engine)();
		return f_(arg1);
	}
	
private:
	F f_;
};

template <class F>
struct function_helper2 {
public:

	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg1_type T1;
	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg2_type T2;

	explicit function_helper2(F f) : f_(f) {
	}

	int operator()(script_engine *engine) {

		assert(engine);
		const T1 arg1 = argument_handler<T1>(engine)();
		fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
		const T2 arg2 = argument_handler<T2>(engine)();
		return f_(arg1, arg2);
	}
private:
	F f_;
};

template <class F>
struct function_helper3 {
public:

	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg1_type T1;
	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg2_type T2;
	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg3_type T3;
		
	explicit function_helper3(F f) : f_(f) {
	}

	int operator()(script_engine *engine) {
	
		assert(engine);
		const T1 arg1 = argument_handler<T1>(engine)();
		fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
		const T2 arg2 = argument_handler<T2>(engine)();
		fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
		const T3 arg3 = argument_handler<T3>(engine)();
		return f_(arg1, arg2, arg3);
	}
private:
	F f_;
};

template <class F>
struct function_helper4 {
public:

	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg1_type T1;
	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg2_type T2;
	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg3_type T3;
	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg4_type T4;

	explicit function_helper4(F f) : f_(f) {
	}

	int operator()(script_engine *engine) {
	
		assert(engine);
		const T1 arg1 = argument_handler<T1>(engine)();
		fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
		const T2 arg2 = argument_handler<T2>(engine)();
		fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
		const T3 arg3 = argument_handler<T3>(engine)();
		fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
		const T4 arg4 = argument_handler<T4>(engine)();
		return f_(arg1, arg2, arg3, arg4);
	}
private:
	F f_;
};

template <class F>
struct function_helper5 {
public:

	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg1_type T1;
	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg2_type T2;
	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg3_type T3;
	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg4_type T4;
	typedef typename boost::function_traits<typename std::remove_pointer<F>::type>::arg5_type T5;

	explicit function_helper5(F f) : f_(f) {
	}

	int operator()(script_engine *engine) {
	
		assert(engine);
		const T1 arg1 = argument_handler<T1>(engine)();
		fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
		const T2 arg2 = argument_handler<T2>(engine)();
		fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
		const T3 arg3 = argument_handler<T3>(engine)();
		fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
		const T4 arg4 = argument_handler<T4>(engine)();
		fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
		const T5 arg5 = argument_handler<T5>(engine)();
		return f_(arg1, arg2, arg3, arg4, arg5);
	}
private:
	F f_;
};

template <class F, int N>
struct function_wrapper;

template <class F>
struct function_wrapper<F, 0> {
	std::function<int(script_engine *engine)> get_function(F f) {
		return function_helper0<F>(f);
	}
};

template <class F>
struct function_wrapper<F, 1> {
	std::function<int(script_engine *engine)> get_function(F f) {
		return function_helper1<F>(f);
	}
};

template <class F>
struct function_wrapper<F, 2> {
	std::function<int(script_engine *engine)> get_function(F f) {
		return function_helper2<F>(f);
	}
};

template <class F>
struct function_wrapper<F, 3> {
	std::function<int(script_engine *engine)> get_function(F f) {
		return function_helper3<F>(f);
	}
};

template <class F>
struct function_wrapper<F, 4> {
	std::function<int(script_engine *engine)> get_function(F f) {
		return function_helper4<F>(f);
	}
};

template <class F>
struct function_wrapper<F, 5> {
	std::function<int(script_engine *engine)> get_function(F f) {
		return function_helper5<F>(f);
	}
};


template <class F>
std::function<int(script_engine *engine)> wrap_function(F f) {
	return function_wrapper<F, boost::function_traits<typename std::remove_pointer<F>::type>::arity>().get_function(f);
}

#endif
