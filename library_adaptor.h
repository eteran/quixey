
#ifndef LIBRARY_ADAPTOR_H_
#define LIBRARY_ADAPTOR_H_

#include "error.h"
#include "quixey.h"
#include <functional>
#include <type_traits>
#include <tuple>
#include <cassert>

namespace detail {

template <class F, class Tuple, bool Done, size_t Total, size_t... N>
struct call_impl {
    static int call(F f, Tuple && t) {
        return call_impl<F, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(f, std::forward<Tuple>(t));
    }
};

template <class F, class Tuple, size_t Total, size_t... N>
struct call_impl<F, Tuple, true, Total, N...> {
    static int call(F f, Tuple && t) {
        return f(std::get<N>(std::forward<Tuple>(t))...);
    }
};

//-----------------------------------------------------------------------------
// Name: call
// Desc: calls f, with each element of the tuple t as arguments
//-----------------------------------------------------------------------------
template <class F, class Tuple>
int call(F f, Tuple &&t) {
    using tuple_type = typename std::decay<Tuple>::type;
    return call_impl<F, Tuple, 0 == std::tuple_size<tuple_type>::value, std::tuple_size<tuple_type>::value>::call(f, std::forward<Tuple>(t));
}

//-----------------------------------------------------------------------------
// Name: for_each
// Desc: for_each, but for tuples (terminating case)
//-----------------------------------------------------------------------------
template <std::size_t I = 0, class Func, class... Tp>
typename std::enable_if<I == sizeof...(Tp), void>::type for_each(std::tuple<Tp...> &, Func) {
}

//-----------------------------------------------------------------------------
// Name: for_each
// Desc: for_each, but for tuples (general case)
//-----------------------------------------------------------------------------
template<std::size_t I = 0, class Func, class... Tp>
typename std::enable_if<I < sizeof...(Tp), void>::type for_each(std::tuple<Tp...>& t, Func f) {
	f(std::get<I>(t));
	for_each<I + 1, Func, Tp...>(t, f);
}


// helpers to fetch arguments more genrically
template <class T>
struct fetch_argument {
};

//-----------------------------------------------------------------------------
// Name: fetch_argument
// Desc:
//-----------------------------------------------------------------------------
template <>
struct fetch_argument<int> {
	fetch_argument(quixey *engine) : engine_(engine) {
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

//-----------------------------------------------------------------------------
// Name: fetch_argument
// Desc:
//-----------------------------------------------------------------------------
template <>
struct fetch_argument<char> {
	fetch_argument(quixey *engine) : engine_(engine) {
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

//-----------------------------------------------------------------------------
// Name: fetch_argument
// Desc:
//-----------------------------------------------------------------------------
template <>
struct fetch_argument<std::string> {
	fetch_argument(quixey *engine) : engine_(engine) {
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

//-----------------------------------------------------------------------------
// Name: fetch_argument
// Desc:
//-----------------------------------------------------------------------------
template <>
struct fetch_argument<variable> {
	fetch_argument(quixey *engine) : engine_(engine) {
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

//-----------------------------------------------------------------------------
// Name: fetch_argument
// Desc:
//-----------------------------------------------------------------------------
template <>
struct fetch_argument<quixey *> {
	fetch_argument(quixey *engine) : engine_(engine) {
		assert(engine);
	}

	quixey *operator()() const {
		return engine_;
	}
private:
	quixey *const engine_;
};


//-----------------------------------------------------------------------------
// Name: fetch_and_test_token_x
// Desc: read a token and make sure it's correct
//-----------------------------------------------------------------------------
template <class T>
void fetch_and_test_token_x(quixey *engine, token::Type expected) {
	assert(engine);
	const token &tok = engine->get_token();
	if(tok.type() != expected) {
		throw T();
	}
}

//-----------------------------------------------------------------------------
// Name: assignment_helper
// Desc: reads a comma, then an argument of type T and assigns it to the argument
//       wish we had generic lambdas like C++14 :-P
//-----------------------------------------------------------------------------
struct assignment_helper {

	assignment_helper(quixey *engine) : engine_(engine) {
	}

	template <class T>
	void operator()(T &arg) const {
		// we want a comma, then an arg.
		// this is used to fetch arguments 2-N
		fetch_and_test_token_x<comma_expected>(engine_, token::COMMA);
		arg = fetch_argument<T>(engine_)();
	}
private:
	quixey *const engine_;
};

//-----------------------------------------------------------------------------
// Name: function_helper
// Desc: handles 0
//-----------------------------------------------------------------------------
template <class F, class... Args>
struct function_helper;

// no arguments
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

//-----------------------------------------------------------------------------
// Name: function_helper
// Desc: handles 1 or more arguments
//-----------------------------------------------------------------------------
template <class F, class A1, class... A2>
struct function_helper<F, A1, A2...> {
public:
	explicit function_helper(F f) : f_(f) {
	}

	int operator()(quixey *engine) const {

		assert(engine);

		// get the first argument
		auto arg1 = std::make_tuple(fetch_argument<A1>(engine)());

		// read the rest of the arguments
		std::tuple<A2...> args;
		for_each(args, assignment_helper(engine));

		// merge the two tuples into 1 and then call f_
		// with the full tuple as arguments
		auto all_args = std::tuple_cat(arg1, args);
		return call(f_, all_args);
	}
private:
	F f_;
};

}

//-----------------------------------------------------------------------------
// Name: wrap_function
// Desc: works for plain old function pointers
//-----------------------------------------------------------------------------
template <class R, class...Args>
std::function<int(quixey *engine)> wrap_function(R(*f)(Args...)) {
	return detail::function_helper<decltype(f), Args...>(f);
}

//-----------------------------------------------------------------------------
// Name: wrap_function
// Desc: works with std::function<>
//-----------------------------------------------------------------------------
template <class R, class...Args>
std::function<int(quixey *engine)> wrap_function(std::function<R(Args...)> f) {
	return detail::function_helper<decltype(f), Args...>(f);
}

#endif
