#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <vector>

#include "Constants.hpp"
#include "Token.hpp"
#include "Error.hpp"
#include "Errors.hpp"
#include "Position.hpp"

using namespace std;

class Lexer {
private:
    string filename;
    // Instruccion(es)
	string text;
	// posicion del caracter actual
	Position pos;
	// caracter actual
	char currChar;

	/*
        Metodo para el avance del caracter actual
	*/
	void advance() {
		pos.advance(currChar);
		currChar = (pos.getIdx() < (int)text.size()) ? text[pos.getIdx()] : '\0';
	}

public:
    Lexer(string pText) {
        filename = "<stdin>";
        pos = Position(-1, 0, -1, filename, pText);
        text = pText;
        currChar = '\0';
		advance();
    }

	Lexer(string pFileName, string pFileContent) {
	    filename = pFileName;
	    pos = Position(-1, 0, -1, pFileName, pFileContent);
		text = pFileContent;
		currChar = '\0';
		advance();
	}

	/*
        Metodo para analizar cada caracter de la instruccion/texto que queremos
        interpretar, cada caracter, si esta dentro de nuestras reglas lo vamos a
        almacenar como un token en un vector de estos.
        Este metodo nos sirve para la identificacion de errores y la de
        instrucciones o operadores.
	*/
	vector<Token> getTokens(Error &error) {
		vector<Token> tokens;
		string errorDetails = "";

		while (currChar != '\0') {
			switch (currChar) {
            case ' ':
            case '\t':
                advance();
                break;

            case '+':
                tokens.push_back(Token(TT_PLUS, "", pos));
                advance();
                break;

            case '-':
                tokens.push_back(Token(TT_MINUS, "", pos));
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

            case '(':
                tokens.push_back(Token(TT_LPAREN, "", pos));
                advance();
                break;

            case ')':
                tokens.push_back(Token(TT_RPAREN, "", pos));
                advance();
                break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                tokens.push_back(getNumber());
            break;

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

};

#endif // LEXER_HPP
