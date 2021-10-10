#ifndef PARSER_HPP
#define PARSER_HPP

#include "ParseResult.hpp"

#define DEBUG false

class Parser {

    private:
        vector<Token> tokens;
        int tok_idx;
        Token currTok;
        Node* aux;

        void advance() {
            tok_idx++;
            if (tok_idx < (int) tokens.size()) {
                currTok = tokens[tok_idx];
            }
        }

        ParseResult factor() {
            ParseResult res;
            Token tok = currTok;

            Node* result = new Node;
            if (result == NULL) return res.failure(MemoryError());

            result->left = NULL;
            result->right = NULL;

            if (tok.getTokenType() == TT_PLUS ||tok.getTokenType() == TT_MINUS) {
                // res.register(advance());
                advance();
                Node* factorNode = res.mRegister(factor());
                if (res.getError().isError()) {
                    return res;
                }
                result->data = tok;
                result->left = factorNode;
                return res.success(result);
            } else if (tok.getTokenType() == TT_INT || tok.getTokenType() == TT_FLOAT) {
                // res.register(advance());
                advance();
                result->data = tok;
                return res.success(result);
            } else if (tok.getTokenType() == TT_LPAREN) {
                // res.register(advance());
                advance();
                Node* exprNode = res.mRegister(expr());
                if (res.getError().isError()) {
                    return res;
                }
                if (currTok.getTokenType() == TT_RPAREN) {
                    // res.register(advance());
                    advance();
                    return res.success(exprNode);
                } else {
                    return res.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expected \')\'"));
                }
            }
            return res.failure(InvalidSyntaxError(tok.getPosStart(), tok.getPosEnd(), "Expected int or float"));
        }

        ParseResult expr() {
            return bin_op("term");
        }

        ParseResult bin_op(string func) { // func = "func" | func = "term"
            ParseResult res;
            if (DEBUG) cout << "START bin_op(\"" << func << "\")\n";
            TokenType tt1, tt2;
            Node* leftNode = NULL;
            if (func == "factor") {
                tt1 = TT_MUL;
                tt2 = TT_DIV;
                leftNode = res.mRegister(factor());
            } else {
                tt1 = TT_PLUS;
                tt2 = TT_MINUS;
                leftNode = res.mRegister(bin_op("factor"));
            }
            // Comprobamos si ParseResult ha devuelto error
            if (res.getError().isError()) {
                // Si devuelve error lo devolvemos a bin_op()
                return res;
            }

            if (leftNode != NULL) {
                if (DEBUG) cout << "AFTER \"" << func << "\": leftNode = ";
                printNode(leftNode);
                Node* binOpNode = NULL;
                while(currTok.getTokenType() == tt1 || currTok.getTokenType() == tt2) {
                    if (aux != NULL) {
                        leftNode = aux;
                    }
                    Token op_tok = currTok;
                    // res.register(advance());
                    advance();
                    Node* rightNode;
                    if (func == "factor") rightNode = res.mRegister(factor()); else rightNode = res.mRegister(bin_op("factor"));
                    if (res.getError().isError()) {
                        return res;
                    }
                    if (DEBUG) cout << "IN LOOP rightNode = ";
                    printNode(rightNode);
                    binOpNode = new Node;
                    if (binOpNode != NULL) {
                        binOpNode->data = op_tok;
                        binOpNode->left = leftNode;
                        binOpNode->right = rightNode;
                        aux = binOpNode;
                        if (DEBUG) cout << "IN LOOP binOpNode = ";
                        printNode(binOpNode);
                    } else return res.failure(MemoryError());
                }
                aux = NULL;
                if (binOpNode == NULL) {
                    if (DEBUG) cout << "END bin_op(\"" << func << "\") => leftNode = ";
                    printNode(leftNode);
                    return res.success(leftNode);
                } else {
                    if (DEBUG) cout << "END bin_op(\"" << func << "\") => binOpNode = ";
                    printNode(binOpNode);
                    return res.success(binOpNode);
                }
            }
            return res.failure(MemoryError());
        }

    public:
        Parser() {}

        Parser(vector<Token> pTokens) {
            tokens = pTokens;
            tok_idx = -1;
            advance();
        }

        ParseResult parse() {
            aux = NULL;
            ParseResult resultAST = expr();
            if (!resultAST.getError().isError() && currTok.getTokenType() != TT_EOF) {
                return resultAST.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expected an operator"));
            }
            return resultAST;
        }

        void printNode(Node *node) {
            if (DEBUG) cout << node << "(" << node->data.as_string() << ", " << node->left << ", " << node->right << ")\n";
        }
};

#endif // PARSER_HPP
