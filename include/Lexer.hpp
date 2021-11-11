#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include "Token.hpp"
#include "Error.hpp"
#include "Errors.hpp"
#include "Position.hpp"

using namespace std;

/*
    Clase dedicada para hacer las operaciones de identificacion de tokens
    y reconocimiento del error de CARACTERES ILEGALES.
    Funcion principal -> getTokens()
*/
class Lexer {

private:
    string filename;
    // Instruccion(es)
	string text;
	// posicion del caracter actual
	Position pos;
	// caracter actual
	char currChar;
	StringUtils strUtils;
	vector<string> keywords;

	/*
        Metodo para el avance del caracter actual
	*/
	void advance() {
		pos.advance(currChar);
		currChar = (pos.getIdx() < (int)text.size()) ? text[pos.getIdx()] : '\0';
	}

public:

    Lexer(string pContent, string pFileName = "<stdin>") {
        initKeywords();
        filename = pFileName;
	    pos = Position(-1, 0, -1, pFileName, pContent);
		text = pContent;
		currChar = '\0';
		advance();
    }

    void initKeywords() {
        keywords.push_back("VAR");
        keywords.push_back("AND");
        keywords.push_back("OR");
        keywords.push_back("NOT");
        keywords.push_back("IF");
        keywords.push_back("THEN");
        keywords.push_back("ELIF");
        keywords.push_back("ELSE");
        keywords.push_back("FOR");
        keywords.push_back("STEP");
        keywords.push_back("TO");
        keywords.push_back("WHILE");
        keywords.push_back("FUN");
    }

	/*
        Metodo para analizar cada caracter de la instruccion/texto que queremos
        interpretar, cada caracter o conjunto de caracteres, si esta dentro de
        nuestras reglas (no incumple ningun error) lo vamos a almacenar como
        un token en un vector de estos.
	*/
	vector<Token> getTokens(Error &error) {
		vector<Token> tokens;
		string errorDetails = "";

		while (currChar != '\0') {
            if (strUtils.isDigit(currChar)) {
                tokens.push_back(getNumber());
            } else if (strUtils.isLetter(currChar)) {
                tokens.push_back(getWord());
            } else {
                switch (currChar) {
                // Ignoramos espacios y sangrias (NO COMO PYTHON >:( )
                case ' ':
                case '\t':
                    advance();
                    break;

                /// MATH
                case '+':
                    tokens.push_back(Token(TT_PLUS, "", pos));
                    advance();
                    break;

                case '-':
                    tokens.push_back(getMinusOrArrow());
                    advance();
                    break;

                case '*':
                    tokens.push_back(Token(TT_MUL, "", pos));
                    advance();
                    break;

                case '/':
                    tokens.push_back(Token(TT_DIV, "", pos));
                    advance();
                    break;

                case '^':
                    tokens.push_back(Token(TT_POW, "", pos));
                    advance();
                    break;

                case '(':
                    tokens.push_back(Token(TT_LPAREN, "", pos));
                    advance();
                    break;

                case ')':
                    tokens.push_back(Token(TT_RPAREN, "", pos));
                    advance();
                    break;

                /// LOGIC
                case '!': {
                    Token tok = getNotEquals(error);
                    if (error.isError()) {
                        return tokens;
                    }
                    tokens.push_back(tok);
                    }break;

                case '=':
                    tokens.push_back(getEquals());
                    break;

                case '<':
                    tokens.push_back(getLesst());
                    break;

                case '>':
                    tokens.push_back(getGreatt());
                    break;

                case ',':
                    tokens.push_back(Token(TT_COMMA, "", pos));

                default: {
                    // ERROR
                    Position posStart;
                    posStart = pos;
                    errorDetails.push_back('\'');
                    errorDetails.push_back(currChar);
                    errorDetails.push_back('\'');
                    advance();
                    error = IllegalCharError(posStart, pos, errorDetails);
                }
                }
            }
		}

		tokens.push_back(Token(TT_EOF, "", pos));
		return tokens;
	}

	/*
        Metodo para recolectar el numero completo cuando se reconoce un
        valor digito por primera vez, comprobamos ademas si el numero es
        flotante o entero.
	*/
	Token getNumber() {
        string num_str = "";
        int dotCont = 0;
        Position posStart = pos;

        while (currChar != '\0' && (isdigit(currChar) || currChar == '.')) {
            if (currChar == '.') {
                if (dotCont == 1) break;
                dotCont++;
                num_str.push_back('.');
            } else {
                num_str.push_back(currChar);
            }
            advance();
        }

        if (dotCont == 0) {
            return Token(TT_INT, num_str, posStart, pos);
        } else {
            return Token(TT_FLOAT, num_str, posStart, pos);
        }
	}

	/*
        Funcion que se encarga de recoger una palabra entera la primera vez
        que se encuentra un caracter alfabetico. Con esto recogemos las
        keywords de nuestro lenguaje de programacion y nombres declarados por
        el usuario.
	*/
	Token getWord() {
        string idStr = "";
        Position posStart = pos;

        while (currChar != '\0' && (strUtils.isDigit(currChar) || strUtils.isLetter(currChar) || currChar == '_')) {
            idStr.push_back(currChar);
            advance();
        }

        TokenType tt;
        if (find(keywords.begin(), keywords.end(), idStr) != keywords.end()) {
            tt = TT_KEYWORD;
        } else {
            tt = TT_IDENTIFIER;
        }
        return Token(tt, idStr, posStart, pos);
	}

	Token getNotEquals(Error &error) {
        Position posStart = pos;
        advance();
        if (currChar == '=') {
            advance();
            return Token(TT_NOTEQ, "", posStart, pos);
        }
        advance();
        error = ExpectedCharError(posStart, pos, "\'=\' (after \'!\')");
        return Token(TT_NOTEQ, "", posStart, pos);
	}

	Token getEquals() {
	    TokenType tt = TT_EQUALS;
        Position posStart = pos;
        advance();
        if (currChar == '=') {
            advance();
            tt = TT_EQEQ;
        }
        return Token(tt, "", posStart, pos);
	}

	Token getLesst() {
        TokenType tt = TT_LESST;
        Position posStart = pos;
        advance();
        if (currChar == '=') {
            advance();
            tt = TT_LESSTEQ;
        }
        return Token(tt, "", posStart, pos);
	}

	Token getGreatt() {
        TokenType tt = TT_GREATT;
        Position posStart = pos;
        advance();
        if (currChar == '=') {
            advance();
            tt = TT_GREATTEQ;
        }
        return Token(tt, "", posStart, pos);
	}

	Token getMinusOrArrow() {
        TokenType tt = TT_MINUS;
        Position posStart = pos;
        advance();

        if (currChar == '>') {
            advance();
            tt = TT_ARROW;
        }
        return Token(tt, "", pos);
	}

};

#endif // LEXER_HPP
