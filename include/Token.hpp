#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include "Position.hpp"

using namespace std;

enum TokenType {
    TT_INT,
    TT_FLOAT,
    TT_PLUS,
    TT_MINUS,
    TT_MUL,
    TT_DIV,
    TT_LPAREN,
    TT_RPAREN,
    TT_EOF
};

/*
    Clase que determina el tipo de dato y si tenemos algun valor
*/
class Token {
private:
	TokenType type = TT_EOF;
	string value;
	Position posStart, posEnd;

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

	Token(TokenType pType, string pValue, Position pPosStart, Position pPosEnd) {
        type = pType;
		value = pValue;
		posStart = pPosStart;
		posEnd = pPosEnd;
	}

	Token(TokenType pType, string pValue, Position pPosStart) {
        type = pType;
		value = pValue;
		posStart = pPosStart;
		posEnd = pPosStart;
		posEnd.advance();
	}

    string getValue() { return value; }
    Position getPosStart() { return posStart; }
    Position getPosEnd() { return posEnd; }
    string as_string() {
        switch(type) {
        case TT_INT:
            return "TT_INT:" + value;

        case TT_FLOAT:
            return "TT_FLOAT:" + value;

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
        case TT_EOF:
            return "TT_EOF";
        }
        return "";
	}

	TokenType getTokenType() {
        return type;
	}
};

#endif // TOKEN_HPP
