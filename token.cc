
#include "token.h"
#include "error.h"

token::token(Type type, const std::string &string) : string_(string), token_type_(type) {
}

token::token(Type type) : token(type, "") {
}

token::token() : token_type_(UNKNOWN) {
}

token &token::operator=(const token &rhs) {
	string_     = rhs.string_;
	token_type_ = rhs.token_type_;
	return *this;
}

token::token(const token &other) : string_(other.string_), token_type_(other.token_type_) {
}

std::string to_string(const token &t) {
	switch(t.type()) {
	case token::AND:         return "&";
	case token::AND_EQ:      return "&=";
	case token::ASSIGN:      return "=";
	case token::AUTO:        return "auto";
	case token::BREAK:       return "break";
	case token::CASE:        return "case";
	case token::CHAR:        return "char";
	case token::CMP:         return "~";
	case token::COLON:       return ":";
	case token::COMMA:       return ",";
	case token::CONTINUE:    return "continue";
	case token::DIV:         return "/";
	case token::DIV_EQ:      return "/=";
	case token::DO:          return "do";
	case token::DOT:         return ".";
	case token::DOUBLECOLON: return "::";
	case token::ELSE:        return "else";
	case token::EQ:          return "==";
	case token::FOR:         return "for";
	case token::FUNCTION:    return "function";
	case token::GE:          return ">=";
	case token::GT:          return ">";
	case token::IF:          return "if";
	case token::INT:         return "int";
	case token::LBRACE:      return "{";
	case token::LBRACKET:    return "[";
	case token::LE:          return "<=";
	case token::LOGICAL_AND: return "&&";
	case token::LOGICAL_OR:  return "||";
	case token::LPAREN:      return "(";
	case token::LSHIFT:      return "<<";
	case token::LSHIFT_EQ:   return "<<=";
	case token::LT:          return "<";
	case token::MINUS:       return "-";
	case token::MINUS_EQ:    return "-=";
	case token::MOD:         return "%";
	case token::MOD_EQ:      return "%=";
	case token::MUL:         return "*";
	case token::MUL_EQ:      return "*=";
	case token::NE:          return "!=";
	case token::NOT:         return "!";
	case token::OR:          return "|";
	case token::OR_EQ:       return "|=";
	case token::PLUS:        return "+";
	case token::PLUS_EQ:     return "+=";
	case token::RBRACE:      return "}";
	case token::RBRACKET:    return "]";
	case token::RETURN:      return "return ";
	case token::RPAREN:      return ")";
	case token::RSHIFT:      return ">>";
	case token::RSHIFT_EQ:   return ">>=";
	case token::SEMICOLON:   return ";";
	case token::SINGLEQUOTE: return "'";
	case token::STRING:      return "string";
	case token::SWITCH:      return "switch";
	case token::WHILE:       return "while";
	case token::XOR:         return "^";
	case token::XOR_EQ:      return "^=";
	case token::FINISHED:    return "<EOF>";

	case token::IDENTIFIER:
	case token::STRING_LITERAL:
	case token::CHARACTER:
	case token::INTEGER:
		return t.string_;

	default:
		return "";
	}
}

std::ostream &operator<<(std::ostream &o, const token &t) {
	return o << to_string(t);
}

