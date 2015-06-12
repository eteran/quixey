
#ifndef LIBRARY_ADAPTOR_H_
#define LIBRARY_ADAPTOR_H_

#include "error.h"
#include "quixey.h"
#include <functional>
#include <type_traits>
#include <cassert>

namespace {
	// helpers to fetch arguments more genrically
	template <class T>
	struct argument_handler {
	};

	template <>
	struct argument_handler<int> {
		argument_handler(quixey *engine) : engine_(engine) {
			assert(engine);
		}

		int operator()() const {
			variable ret;
			engine_->eval_exp(ret);
			return to_integer(ret);
		}
	private:
		quixey *const engine_;
	};

	template <>
	struct argument_handler<char> {
		argument_handler(quixey *engine) : engine_(engine) {
			assert(engine);
		}

		char operator()() const {
			variable ret;
			engine_->eval_exp(ret);
			return static_cast<char>(to_integer(ret));
		}
	private:
		quixey *const engine_;
	};

	template <>
	struct argument_handler<std::string> {
		argument_handler(quixey *engine) : engine_(engine) {
			assert(engine);
		}

		std::string operator()() const {
			variable ret;
			engine_->eval_exp(ret);
			return to_string(ret);
		}
	private:
		quixey *const engine_;
	};

	template <>
	struct argument_handler<variable> {
		argument_handler(quixey *engine) : engine_(engine) {
			assert(engine);
		}

		variable operator()() const {
			variable ret;
			engine_->eval_exp(ret);
			return ret;
		}
	private:
		quixey *const engine_;
	};

	template <>
	struct argument_handler<quixey *> {
		argument_handler(quixey *engine) : engine_(engine) {
			assert(engine);
		}

		quixey *operator()() const {
			return engine_;
		}
	private:
		quixey *const engine_;
	};


	template <class T>
	void fetch_and_test_token_x(quixey *engine, token::Type expected) {
		assert(engine);
		const token &tok = engine->get_token();
		if(tok.type() != expected) {
			throw T();
		}
	}

	// processing function arguments

	template <class F, class... Args>
	struct function_helper;

	template <class F>
	struct function_helper<F> {
		explicit function_helper(F f) : f_(f) {
		}

		int operator()(quixey *engine) const {
			(void)engine;
			assert(engine);
			return f_();
		}
	private:
		F f_;
	};

	template <class F, class A1>
	struct function_helper<F, A1> {
	public:
		explicit function_helper(F f) : f_(f) {
		}

		int operator()(quixey *engine) const {
			assert(engine);
			const A1 arg1 = argument_handler<A1>(engine)();
			return f_(arg1);
		}

	private:
		F f_;
	};

	template <class F, class A1, class A2>
	struct function_helper<F, A1, A2> {
	public:
		explicit function_helper(F f) : f_(f) {
		}

		int operator()(quixey *engine) const {

			assert(engine);
			const A1 arg1 = argument_handler<A1>(engine)();
			fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
			const A2 arg2 = argument_handler<A2>(engine)();
			return f_(arg1, arg2);
		}
	private:
		F f_;
	};

	template <class F, class A1, class A2, class A3>
	struct function_helper<F, A1, A2, A3> {
	public:
		explicit function_helper(F f) : f_(f) {
		}

		int operator()(quixey *engine) const {

			assert(engine);
			const A1 arg1 = argument_handler<A1>(engine)();
			fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
			const A2 arg2 = argument_handler<A2>(engine)();
			fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
			const A3 arg3 = argument_handler<A3>(engine)();
			return f_(arg1, arg2, arg3);
		}
	private:
		F f_;
	};

	template <class F, class A1, class A2, class A3, class A4>
	struct function_helper<F, A1, A2, A3, A4> {
	public:
		explicit function_helper(F f) : f_(f) {
		}

		int operator()(quixey *engine) const {

			assert(engine);
			const A1 arg1 = argument_handler<A1>(engine)();
			fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
			const A2 arg2 = argument_handler<A2>(engine)();
			fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
			const A3 arg3 = argument_handler<A3>(engine)();
			fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
			const A4 arg4 = argument_handler<A4>(engine)();
			return f_(arg1, arg2, arg3, arg4);
		}
	private:
		F f_;
	};

	template <class F, class A1, class A2, class A3, class A4, class A5>
	struct function_helper<F, A1, A2, A3, A4, A5> {
	public:
		explicit function_helper(F f) : f_(f) {
		}

		int operator()(quixey *engine) const {

			assert(engine);
			const A1 arg1 = argument_handler<A1>(engine)();
			fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
			const A2 arg2 = argument_handler<A2>(engine)();
			fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
			const A3 arg3 = argument_handler<A3>(engine)();
			fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
			const A4 arg4 = argument_handler<A4>(engine)();
			fetch_and_test_token_x<comma_expected>(engine, token::COMMA);
			const A5 arg5 = argument_handler<A5>(engine)();
			return f_(arg1, arg2, arg3, arg4, arg5);
		}
	private:
		F f_;
	};

}

template <class R, class...Args>
std::function<int(quixey *engine)> wrap_function(R(*f)(Args...)) {
	static_assert(sizeof...(Args) <= 5, "Function Has Too Many Arguments");
	return function_helper<decltype(f), Args...>(f);
}

#endif
