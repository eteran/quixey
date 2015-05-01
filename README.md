
Quixey
======

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

**Note** : unlike C/C++ keywords like if, else, for, do, while **require** the 
           curly braces, they are not optional

`if`, `else`:

	if(x) { }
	if(x) { } else { }
	if(x) { } else if(y) { }
	if(x) { } else if(y) { } else { }

`for`:

** Note**: you may declare a variable in the initialization part of the
         for loop, this variable only exists inside the loop. Just like in C++.

	for(i = 0; i < 10; i += 1) { }
	for(int i = 0; i < 10; i += 1) { }
	
`do`:

    do { } while(x);
	
`while`:

    while(x) { }
	
`return`:

	return 0;
	
`int`, `char`, `string`:
	int x = 5;
	char y;
	string s = "hello";
	char ch = s[3];
	
lamdas are also supported, and can only be assigned to a variable 
of type `auto`:
	
	auto f = function() {
		puts("hello world");
	};

lambdas also, always have the return type of `auto`.
