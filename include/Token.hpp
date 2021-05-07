#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include "Constants.hpp"

using namespace std;

class Token {
private:
	TokenType type;
	string value;

public:
    Token() {}
    Token(TokenType pType) {
        type = pType;
        value = "";
    }

	Token(TokenType pType, string pValue) {
		type = pType;
		value = pValue;
	}

	string as_string() {
        switch(type) {
        case TT_INT:
            return "TT_INT";

        case TT_FLOAT:
            return "TT_FLOAT";

        case TT_PLUS:
            return "TT_PLUS";

        case TT_MINUS:
            return "TT_MINUS";

        case TT_MUL:
            return "TT_MUL";

        case TT_DIV:
            return "TT_DIV";

        case TT_LPAREN:
            return "TT_LPAREN";

        case TT_RPAREN:
            return "TT_RPAREN";
        }
        return "";
	}

	TokenType getTokenType() {
        return type;
	}
};

#endif // TOKEN_HPP
