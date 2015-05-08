
#ifndef TOKEN_20060121_H_
#define TOKEN_20060121_H_

#include "script_types.h"
#include <iostream>
#include <ostream>
#include <string>

class token {
public:
	enum Type  {

		UNKNOWN        = 0xffff,

		// delimiters
		DELIMETER      = 0x1000,
		SEMICOLON      = DELIMETER | 0x00,
		LPAREN         = DELIMETER | 0x01,
		RPAREN         = DELIMETER | 0x02,
		COMMA          = DELIMETER | 0x03,
		SINGLEQUOTE    = DELIMETER | 0x04,
		LBRACE         = DELIMETER | 0x05,
		RBRACE         = DELIMETER | 0x06,
		LBRACKET       = DELIMETER | 0x07,
		RBRACKET       = DELIMETER | 0x08,
		DOT            = DELIMETER | 0x09,
		COLON          = DELIMETER | 0x0a,
		DOUBLECOLON    = DELIMETER | 0x0b,

		// operators
		OPERATOR       = 0x2000,
		LT             = OPERATOR | 0x00,
		GT             = OPERATOR | 0x01,
		PLUS           = OPERATOR | 0x02,
		MUL            = OPERATOR | 0x03,
		MOD            = OPERATOR | 0x04,
		MINUS          = OPERATOR | 0x05,
		ASSIGN         = OPERATOR | 0x06,
		DIV            = OPERATOR | 0x07,
		AND            = OPERATOR | 0x08,
		OR             = OPERATOR | 0x09,
		XOR            = OPERATOR | 0x0a,
		CMP            = OPERATOR | 0x0b,
		NOT            = OPERATOR | 0x0c,
		LE             = OPERATOR | 0x0d,
		GE             = OPERATOR | 0x0e,
		EQ             = OPERATOR | 0x0f,
		NE             = OPERATOR | 0x10,
		LOGICAL_AND    = OPERATOR | 0x11,
		LOGICAL_OR     = OPERATOR | 0x12,
		LSHIFT         = OPERATOR | 0x13,
		RSHIFT         = OPERATOR | 0x14,

		PLUS_EQ        = OPERATOR | 0x15,
		MINUS_EQ       = OPERATOR | 0x16,
		MUL_EQ         = OPERATOR | 0x17,
		MOD_EQ         = OPERATOR | 0x18,
		DIV_EQ         = OPERATOR | 0x19,
		AND_EQ         = OPERATOR | 0x1a,
		XOR_EQ         = OPERATOR | 0x1b,
		OR_EQ          = OPERATOR | 0x1c,
		RSHIFT_EQ      = OPERATOR | 0x1d,
		LSHIFT_EQ      = OPERATOR | 0x1e,

		// types that a variable can have..
		TYPENAME       = 0x3000,
		STRING_LITERAL = TYPENAME | 0x00,
		INTEGER        = TYPENAME | 0x01,
		CHARACTER      = TYPENAME | 0x02,

		// keywords
		KEYWORD        = 0x4000,
		IF             = KEYWORD | 0x00,
		ELSE           = KEYWORD | 0x01,
		FOR            = KEYWORD | 0x02,
		DO             = KEYWORD | 0x03,
		WHILE          = KEYWORD | 0x04,
		CHAR           = KEYWORD | 0x05,
		INT            = KEYWORD | 0x06,
		STRING         = KEYWORD | 0x07,
		RETURN         = KEYWORD | 0x08,
		BREAK          = KEYWORD | 0x09,
		CONTINUE       = KEYWORD | 0x0a,
		SWITCH         = KEYWORD | 0x0b,
		CASE           = KEYWORD | 0x0c,
		AUTO           = KEYWORD | 0x0d,
		FOREACH        = KEYWORD | 0x0e,
		FUNCTION       = KEYWORD | 0x0f,

		IDENTIFIER     = 0x5000,
		FINISHED       = 0x6000
	};
public:

	token(Type token, const std::string &string);
	token(Type token);
	token();
	token(const token &other);
	token &operator=(const token &rhs);

public:
	Type type() const        { return token_type_; }
	Type type_class() const  { return static_cast<Type>(token_type_ & 0xf000); }


private:
	std::string string_;
	Type        token_type_;

private:
	friend std::string to_string(const token &t);
};


std::string to_string(const token &t);

std::ostream &operator<<(std::ostream &o, const token &t);

#endif
