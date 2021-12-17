#ifndef PARSER_HPP
#define PARSER_HPP

#include "ParseResult.hpp"

#define DEBUG false
/*
    Clase dedicada a parsear los tokens que hemos recolectado, de manera que
    podemos construir nuestro AST para reconocer las instrucciones de nuestro lenguaje.

    AQUI es donde se invocan todos los demas errores cuando se incumplan
    las reglas de sintaxis y aritmeticas.
*/
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


///REPROGRAMAR ENTERO ------------------------------------------------------------------------------------------------------------------------------
        ParseResult atom() {
            ParseResult res;
            Token tok = currTok;

            if (tok.getTokenType() == TT_INT || tok.getTokenType() == TT_FLOAT) {
                NumberNode* result = new NumberNode();
                if (result != NULL) {
                    // res.register(advance());
                    advance();
                    result->numberTok = tok;
                    return res.success(result);
                } else {
                    return res.failure(MemoryError());
                }
            } else if (tok.getTokenType() == TT_IDENTIFIER) {
                // res.register(advance());
                advance();
                VarAccessNode* result = new VarAccessNode();
                if (result != NULL) {
                    result->varNameTok = tok;
                    return res.success(result);
                } else {
                    return res.failure(MemoryError());
                }
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
            } else if (tok.matches(TT_KEYWORD, "IF")) {
                Node* node = res.mRegister(if_expr());
                if (res.getError().isError()) return res;
                return res.success(node);
            } else if (tok.matches(TT_KEYWORD, "FOR")) {
                Node* node = res.mRegister(for_expr());
                if (res.getError().isError()) return res;
                return res.success(node);
            } else if (tok.matches(TT_KEYWORD, "WHILE")) {
                Node* node = res.mRegister(while_expr());
                if (res.getError().isError()) return res;
                return res.success(node);
            }

            return res.failure(InvalidSyntaxError(tok.getPosStart(), tok.getPosEnd(), "Expected int, float, identifier or term"));
        }

        ParseResult if_expr() {
            ParseResult res;
            vector<vector<Node*>> cases; // <Condition, Expression>
            Node* elseCase = NULL;

            advance();
            Node* condition = res.mRegister(expr());
            if (res.getError().isError()) return res;

            if (!currTok.matches(TT_KEYWORD, "THEN")) {
                return res.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expected THEN"));
            }

            advance();

            Node* nodeExpr = res.mRegister(expr());
            if (res.getError().isError()) return res;
            cases.push_back({condition, nodeExpr});

            while (currTok.matches(TT_KEYWORD, "ELIF")) {
                advance();

                condition = res.mRegister(expr());
                if (res.getError().isError()) return res;

                if (!currTok.matches(TT_KEYWORD, "THEN")) {
                    return res.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expected THEN"));
                }

                advance();

                nodeExpr = res.mRegister(expr());
                if (res.getError().isError()) return res;
                cases.push_back({condition, nodeExpr});
            }

            if (currTok.matches(TT_KEYWORD, "ELSE")) {
                advance();

                elseCase = res.mRegister(expr());
                if (res.getError().isError()) return res;
            }

            // Build cases tree
            IfNode* result = new IfNode();
            if (result != NULL) {
                result->cases = cases;
                result->elseCase = elseCase;
            } else return res.failure(MemoryError());
            return res.success(result);
        }

        ParseResult for_expr() {
            ParseResult res;

            advance();

            if (currTok.getTokenType() != TT_IDENTIFIER) {
                return res.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expected identifier"));
            }

            Token varTok = currTok;
            advance();

            if (currTok.getTokenType() != TT_EQUALS) {
                return res.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expeceted \'=\'"));
            }

            advance();

            Node* startValue = res.mRegister(expr());
            if (res.getError().isError()) return res;

            if (!currTok.matches(TT_KEYWORD, "TO")) {
                return res.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expeceted TO"));
            }

            advance();

            Node* endValue = res.mRegister(expr());
            if (res.getError().isError()) return res;

            Node* stepValue = NULL;
            if (currTok.matches(TT_KEYWORD, "STEP")) {
                advance();

                stepValue = res.mRegister(expr());
                if (res.getError().isError()) return res;
            }

            if (!currTok.matches(TT_KEYWORD, "THEN")) {
                return res.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expeceted THEN"));
            }

            advance();
            Node* body = res.mRegister(expr());
            if (res.getError().isError()) return res;

            ForNode* result = new ForNode;
            if (result != NULL) {
                result->varNameTok = varTok;
                result->startValueNode = startValue;
                result->endValueNode = endValue;
                result->stepValueNode = stepValue;
                result->bodyNode = body;
            } else return res.failure(MemoryError());

            return res.success(result);
        }

        ParseResult while_expr() {
            ParseResult res;

            advance();
            Node* condition = res.mRegister(expr());
            if (res.getError().isError()) return res;

            if (!currTok.matches(TT_KEYWORD, "THEN")) {
                return res.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expeceted THEN"));
            }

            advance();
            Node* body = res.mRegister(expr());
            if (res.getError().isError()) return res;

            WhileNode* result = new WhileNode;
            if (result != NULL) {
                result->conditionNode = condition;
                result->bodyNode = body;
            } else res.failure(MemoryError());

            return res.success(result);
        }

        ParseResult power() {
            TokenType tts[] = {TT_POW};
            return bin_op("atom", tts, 1);
        }

        ParseResult factor() {
            ParseResult res;
            Token tok = currTok;

            if (tok.getTokenType() == TT_PLUS ||tok.getTokenType() == TT_MINUS) {
                UnaryOpNode* result = new UnaryOpNode;
                if (result != NULL) {
                    // res.register(advance());
                    advance();
                    Node* factorNode = res.mRegister(factor());
                    if (res.getError().isError()) return res;

                    result->opTok = tok;
                    result->node = factorNode;
                    return res.success(result);
                } else {
                    return res.failure(MemoryError());
                }
            }
            return power();
        }

        ParseResult expr() {
            ParseResult res;
            // Comprobamos si el usuario quiere declarar una variable
            if (currTok.matches(TT_KEYWORD, "VAR")) {
                /// SINTAXIS VAR <identificador> = <valor>
                // res.register(advance());
                advance();

                if (currTok.getTokenType() != TT_IDENTIFIER) {
                    return res.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expected identifier"));
                }

                // Recogemos el nombre de la variable
                Token varToken = currTok;
                // res.register(advance());
                advance();

                if (currTok.getTokenType() != TT_EQUALS) {
                    return res.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expected \'=\'"));
                }

                // res.register(advance());
                advance();

                Node* varExpr = res.mRegister(expr());
                if (res.getError().isError()) {
                    return res;
                }
                VarAssignNode* result = new VarAssignNode;
                if (result != NULL) {
                    // VarAssignNode
                    result->varNameTok = varToken;
                    result->valueNode = varExpr;
                    return res.success(result);
                } else {
                    return res.failure(MemoryError());
                }
            }
            Node* node = res.mRegister(bin_op("comp_expr", {}, 0));
            if (res.getError().isError()) {
                return res.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expected \'VAR\', int, float, identifier, \'+\', \'-\' or \'(\'"));
            }
            return res.success(node);
        }

        ParseResult comp_expr() {
            ParseResult res;

            if (currTok.matches(TT_KEYWORD, "NOT")) {
                Token opTok = currTok;
                advance();

                Node* node = res.mRegister(comp_expr());
                if (res.getError().isError()) return res;

                UnaryOpNode* result = new UnaryOpNode;
                if (result != NULL) {
                    result->opTok = opTok;
                    result->node = node;
                    return res.success(result);
                } else {
                    res.failure(MemoryError());
                }
            }

            TokenType tts[] = {TT_EQEQ, TT_NOTEQ, TT_LESST, TT_GREATT, TT_LESSTEQ, TT_GREATTEQ};
            Node* node = res.mRegister(bin_op("arith_expr", tts, 6));

            if (res.getError().isError())
                return res.failure(InvalidSyntaxError(currTok.getPosStart(), currTok.getPosEnd(), "Expected int, float, identifier, \'+\', \'-\', \'(\', or \'NOT\'"));

            return res.success(node);
        }

        ParseResult arith_expr() {
            TokenType tts[] = {TT_PLUS, TT_MINUS};
            return bin_op("term", tts, 2);
        }

        ParseResult term() {
            TokenType tts[] = {TT_MUL, TT_DIV};
            return bin_op("factor", tts, 2);
        }

        bool in(TokenType tt, TokenType tts[], int size) {
            for (int i = 0; i < size; i++) {
                if (tts[i] == tt) {
                    return true;
                }
            }
            return false;
        }
        ParseResult bin_op(string func, TokenType tts[], int ttsSize) {
            ParseResult res;
            if (DEBUG) cout << "START bin_op(\"" << func << "\")\n";
            Node* leftNode = NULL;
            if (func == "factor") {
                leftNode = res.mRegister(factor());
            } else if (func == "term") {
                leftNode = res.mRegister(term());
            } else if (func == "atom") {
                leftNode = res.mRegister(atom());
            } else if (func == "comp_expr") {
                leftNode = res.mRegister(comp_expr());
            } else /*if (func == "arith_expr")*/{
                leftNode = res.mRegister(arith_expr());
            }
            // Comprobamos si ParseResult ha devuelto error
            if (res.getError().isError()) {
                // Si devuelve error lo devolvemos a bin_op()
                return res;
            }

            if (leftNode != NULL) {
                if (DEBUG) cout << "AFTER \"" << func << "\": leftNode = ";
                //printNode(leftNode);
                BinOpNode* binOpNode = NULL;

                while(in(currTok.getTokenType(), tts, ttsSize)
                      || (currTok.getTokenType() == TT_KEYWORD && (currTok.getValue() == "AND" || currTok.getValue() == "OR"))) {
                    if (aux != NULL) {
                        leftNode = aux;
                    }
                    Token op_tok = currTok;
                    // res.register(advance());
                    advance();
                    Node* rightNode;
                    if (func == "factor" || func == "atom") {
                        rightNode = res.mRegister(factor());
                    } else if (func == "comp_expr") {
                        rightNode = res.mRegister(comp_expr());
                    } else if (func == "arith_expr") {
                        rightNode = res.mRegister(arith_expr());
                    } else { // func == "term"
                        rightNode = res.mRegister(term());
                    }
                    if (res.getError().isError()) {
                        return res;
                    }
                    if (DEBUG) cout << "IN LOOP rightNode = ";
                    //printNode(rightNode);
                    binOpNode = new BinOpNode;
                    if (binOpNode != NULL) {
                        binOpNode->opTok = op_tok;
                        //binOpNode->type = BinOpNode;
                        binOpNode->leftNode = leftNode;
                        binOpNode->rightNode = rightNode;
                        aux = binOpNode;
                        if (DEBUG) cout << "IN LOOP binOpNode = ";
                        //printNode(binOpNode);
                    } else return res.failure(MemoryError());
                }
                aux = NULL;
                if (binOpNode == NULL) {
                    if (DEBUG) cout << "END bin_op(\"" << func << "\") => leftNode = ";
                    //printNode(leftNode);
                    return res.success(leftNode);
                } else {
                    if (DEBUG) cout << "END bin_op(\"" << func << "\") => binOpNode = ";
                    //printNode(binOpNode);
                    return res.success(binOpNode);
                }
            }
            return res.failure(MemoryError());
        }
///-----------------------------------------------------------------------------------------------------------------------------------------------

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

//        void printNode(Node *node) {
//            if (DEBUG) cout << node << "(" << node->data.as_string() << ", " << node->left << ", " << node->right << ")\n";
//        }
};

#endif // PARSER_HPP
