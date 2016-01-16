
Quixey
======

Quixey is a C-ish scripting language where I toy around with a few ideas that I
have found interesting over the years. It inherits a many things from C, such 
as the scoping rules, most operators, and general curly brace syntax. However, 
unlike C, it has a native `string` type, `foreach`, lambdas, an `auto` type, 
and much more.

I don't intend for this language to be taken particularly seriously, but it 
was fun to develop.

An interesting property of the implementation of the `auto` keyword is that
since it is defined as assuming the type of the first assignment to it, is 
that it can have a different type in different scopes... But it can never 
change types in a given scope. For example:

	auto foo(int x) {
		if(x) {
			return "Hello";
		} else {
			return 42;
		}
	}
	
	// since this function returns the auto type, we must capture the result
	// in an auto type as well. 
	auto n1 = foo(1); // n == "Hello" and is of type string
	auto n2 = foo(0); // n == 42 and is of type int
	
	// Now we can use is_integer and is_string to decide what to do with the 
	// values as needed.
	
Aditionally, for similar reasons, the following is perfectly legal:

    auto n;
	if(func()) {
		n = "A String!";
	} else {
		n = 123;
	}
	
	// at this point n is one of the two possible types depending on the result
	// of func()
	
Of course for "poor mans templates", you can use `auto` for function parameters 
as well.

    auto func(auto n) {
		// do something with n depending on the type!
	}
	
However, you cannot change the type once it is set. So unlike weakly typed 
languages. **The following is not allowed in quixey**.

    auto x = 1;
	x = 'A'; // ERROR: cannot change type!
	

## Supported Escape Sequences:
* `\'`    : Single quote
* `\"`    : Double quote
* `\\`    : Backslash
* `\a`    : Audible bell
* `\b`    : Backspace
* `\f`    : Formfeed
* `\n`    : Newline
* `\r`    : Carriage return
* `\t`    : Horizontal tab
* `\v`    : Vertical tab
* `\xnnn` : Hexadecimal number (nnn)
* `\nnn`  :  number (nnn)

**Note**: in octal and hex decimal escape sequences, there is no limit on the number
          of digits but the least significant digits will be used (as happens in gcc).
          So for example `'\x12345678'` is functionally the same as `'\x78'`

## Supported Operators:

* `/=`  : divide left by right and assign to left
* `/`   : divide left by right
* `&=`  : binary AND left and right and assign to left
* `&&`  : logical AND left and right
* `&`   : binary AND left and right
* `|=`  : binary OR left and right and assign to left
* `||`  : logical OR left and right
* `|`   : binary OR left and right
* `^=`  : XOR left and right and assign to left
* `^`   : XOR left and right
* `~`   : compliment unary operand
* `==`  : test if left equals right
* `=`   : assign right to left
* `!=`  : test if left does not equal right
* `!`   : logical NOT of unary operand
* `+=`  : add left and right and assign to left
* `+`   : add left and right
* `-=`  : subtract right from left and assign to left
* `-`   : subtract right from left
* `*=`  : multiply left and right and assign to left
* `*`   : multiply left and right
* `%=`  : modulo divide left by right and assign to left
* `%`   : modulo divide left by right
* `>>=` : right shift left by right and assign to left
* `>>`  : right shift left by right
* `>=`  : test if left is greater than or equal to right
* `>`   : test if left is greater than right
* `<<=` : left shift left by right and assign to left
* `<<`  : left shift left by right
* `<=`  : test if left is less than or equal to right
* `<`   : test if left is less than right

## Supported Types:

* `char`
* `int`
* `string`
* `auto` (assumes the type of the first thing assigned to it)

**Note**: modifiers such as unsigned are not supported

## Supported Keywords:

**Note** : unlike C/C++ keywords like `if`, `else`, `for`, `do`, `while` **require** the 
           curly braces, they are not optional

`if`, `else`:

	if(x) { }
	if(x) { } else { }
	if(x) { } else if(y) { }
	if(x) { } else if(y) { } else { }

`for`:

**Note**: you may declare a variable in the initialization part of the
          for loop, this variable only exists inside the loop. Just like in C++.

**Note**: the for each style syntax requires that you declare a variable as the
          initializer.

	for(i = 0; i < 10; i += 1) { }
	for(int i = 0; i < 10; i += 1) { }
	for(auto e : a) {}

`do`:

    do { } while(x);
	
`while`:

    while(x) { }
	
`return`:

**NOTE:** Every function has a return type. If the end of a function is reached with no return, then an implicit
          `return 0;` is executed at function exit.
	
`int`, `char`, `string`. `auto`

	int x = 5;
	char y;
	string s = "hello";
	char ch = s[3];
	auto a1 = "hello";
	auto a2 = 5;
	auto a3 = 'C';
	auto a4 = a2;

## Arrays

Arrays are heterogeneous, and are created via array literals so far:

    auto x = ['a', 'b', 'c', 1234, "HELLO", [1, 2, 3, 4], function() { printf("W00t!\n"); }];
	
See `test4.qc` for some advanced usage of them such as type deduction.

## Lambdas

lamdas are supported, and can only be assigned to a variable of type `auto`:
	
	auto f = function() {
		puts("hello world");
	};
	
	f(); // prints "hello world"

lambdas also, always have the return type of `auto`.


## Built-in Functions
* `int size(auto x); // returns the length of an array or string`
* `int puts(string s); // just like C's puts`
* `int getch(); // gets 1 character from stdin`
* `int getnum(); // gets an integer from stdin`
* `int putchar(char ch); // puts a character to stdout`
* `int is_integer(auto x); // returns non-zero if the parameter is an integer`
* `int is_character(auto x); // returns non-zero if the parameter is a character`
* `int is_string(auto x); // returns non-zero if the parameter is a string`
* `int is_function(auto x); // returns non-zero if the parameter is an function or lambda`
* `int is_array(auto x); // returns non-zero if the parameter is an array`
* `int printf(string s, ...); // just like C's printf, an experiement in supporting variadic functions (may not stick around)`
