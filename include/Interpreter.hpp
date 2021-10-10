#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "OperationTree.hpp"
#include "Number.hpp"

class Interpreter {
    public:
        string getMethodName(Node* node) {
            if (isNumberNode(node)) return "NumberNode";
            if (isSymbolNode(node) && node->right == NULL) return "UnaryOpNode";
            return "BinOpNode";
        }

        Number visit(Node* node) {
            string methodName = getMethodName(node);
            if (methodName == "NumberNode") {
                return visit_NumberNode(node);
            } else if (methodName == "UnaryOpNode") {
                return visit_UnaryOpNode(node);
             } else { // "BinaryOpNode"
                return visit_BinaryOpNode(node);
            }
        }

        Number visit_NumberNode(Node* node) {
            Number result(stof(node->data.getValue()));
            result.setNumberPosition(node->data.getPosStart(), node->data.getPosEnd());
            return result;
        }

        Number visit_BinaryOpNode(Node* node) {
            Number left = visit(node->left);
            Number right = visit(node->right);

            Number result;
            switch (node->data.getTokenType()) {
            case TT_PLUS:
                result = left.addedTo(right);
                break;
            case TT_MINUS:
                result = left.subbedBy(right);
                break;
            case TT_MUL:
                result = left.multedBy(right);
                break;
            case TT_DIV:
                result = left.divedBy(right);
                break;
            default:
                cout << "visit_BinaryOpNode() cant handle operation token: " << node->data.as_string() << endl;
            }

            result.setNumberPosition(node->data.getPosStart(), node->data.getPosStart());
            return result;
        }

        Number visit_UnaryOpNode(Node* node) {
            Number number = visit(node->left);
            Number result;
            if (node->data.getTokenType() == TT_MINUS) {
                result = number.multedBy(Number(-1));
            }
            return result;
        }

        // Returns true if node contains number
        bool isNumberNode(Node* node) {
            return node->data.getTokenType() == TT_INT || node->data.getTokenType() == TT_FLOAT;
        }

        // Returns true if node contains + or - operator
        bool isSymbolNode(Node* node) {
            return node->data.getTokenType() == TT_PLUS || node->data.getTokenType() == TT_MINUS;
        }
};

#endif // INTERPRETER_HPP
