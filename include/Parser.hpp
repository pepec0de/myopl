#ifndef PARSER_HPP
#define PARSER_HPP

#include "Token.hpp"
#include "OperationStructs.hpp"

class Parser {

    private:
        vector<Token> tokens;
        int tok_idx;
        Token currTok;

        void advance() {
            tok_idx++;
            if (tok_idx < (int) tokens.size()) {
                currTok = tokens[tok_idx];
            }
            //return currTok;
        }

        BinOpNode factor() {
            BinOpNode result;
            if (currTok.getTokenType() == TT_INT || currTok.getTokenType() == TT_FLOAT) {
                result.tok = currTok;
                advance(); // no se si va antes que el result
            }
            return result;
        }

        BinOpNode term() {
            return bin_op("factor");
        }

        BinOpNode expr() {
            return bin_op("term");
        }

        BinOpNode bin_op(string func) {
            TokenType tt1, tt2;
            BinOpNode left;
            if (func == "factor") {
                tt1 = TT_MUL;
                tt2 = TT_DIV;
                left = factor();
            } else if (func == "term") {
                tt1 = TT_PLUS;
                tt2 = TT_MINUS;
                left = term();
            }

            while(currTok.getTokenType() == tt1 || currTok.getTokenType() == tt2) {
                left.left_node = &left;
                left.tok = currTok;
                BinOpNode right_node = factor();
                BinOpNode left_node = term();
                if (func == "factor") left.right_node = &right_node; else if (func == "term") left.right_node = &left_node;
                advance();
            }
            return left;
        }

    public:
        Parser() {}

        Parser(vector<Token> pTokens) {
            tokens = pTokens;
            tok_idx = -1;
            advance();
        }

        BinOpNode parse() {
            BinOpNode result;
            result = expr();
            return result;
        }
};

#endif // PARSER_HPP
