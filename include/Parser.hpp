#ifndef PARSER_HPP
#define PARSER_HPP

#include "OperationTree.hpp"

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

        Node* factor() {
            Node* result = new Node;
            if (result != NULL) {
                result->left = NULL;
                result->right = NULL;
                if (currTok.getTokenType() == TT_INT || currTok.getTokenType() == TT_FLOAT) {
                    result->data = currTok;
                    advance();
                    return result;
                }
            }
            return NULL;
        }

        Node* expr() {
            return bin_op("term");
        }

        Node* bin_op(string func) {
            cout << "START bin_op(\"" << func << "\")\n";
            TokenType tt1, tt2;
            Node* leftNode = NULL;
            if (func == "factor") {
                tt1 = TT_MUL;
                tt2 = TT_DIV;
                leftNode = factor();
            } else if (func == "term") {
                tt1 = TT_PLUS;
                tt2 = TT_MINUS;
                leftNode = bin_op("factor");
            }

            if (leftNode != NULL) {
                cout << "AFTER \"" << func << "\": leftNode = ";
                printNode(leftNode);
                Node* binOpNode = NULL;
                while(currTok.getTokenType() == tt1 || currTok.getTokenType() == tt2) {
                    if (aux != NULL) {
                        leftNode = aux;
                    }
                    Token op_tok = currTok;
                    advance();
                    Node* rightNode;
                    if (func == "factor") rightNode = factor(); else rightNode = bin_op("factor");
                    cout << "IN LOOP rightNode = ";
                    printNode(rightNode);
                    binOpNode = new Node;
                    if (binOpNode != NULL) {
                        binOpNode->data = op_tok;
                        binOpNode->left = leftNode;
                        binOpNode->right = rightNode;
                        aux = binOpNode;
                        cout << "IN LOOP binOpNode = ";
                        printNode(binOpNode);
                    } else return NULL;
                }
                aux = NULL;
                if (binOpNode == NULL) {
                    cout << "END bin_op(\"" << func << "\") => leftNode = ";
                    printNode(leftNode);
                    return leftNode;
                } else {
                    cout << "END bin_op(\"" << func << "\") => binOpNode = ";
                    printNode(binOpNode);
                    return binOpNode;
                }
            }
            return NULL;
        }

    public:
        Parser() {}

        Parser(vector<Token> pTokens) {
            tokens = pTokens;
            tok_idx = -1;
            advance();
        }

        OperationTree parse() {
            OperationTree tree;
            aux = NULL;
            Node* root = expr();
            tree.setRootNode(root);
            return tree;
        }

        void printNode(Node *node) {
            cout << node << "(" << node->data.as_string() << ", " << node->left << ", " << node->right << ")\n";
        }
};

#endif // PARSER_HPP
