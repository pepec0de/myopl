#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "OperationTree.hpp"
#include "Number.hpp"
#include "RuntimeResult.hpp"
#include "Context.hpp"

class Interpreter {
    public:
        string getMethodName(Node* node) {
            if (isNumberNode(node)) return "NumberNode";
            if (isSymbolNode(node) && node->right == NULL) return "UnaryOpNode";
            return "BinOpNode";
        }

        RuntimeResult visit(Node* node, Context context) {
            string methodName = getMethodName(node);
            if (methodName == "NumberNode") {
                return visit_NumberNode(node, context);
            } else if (methodName == "UnaryOpNode") {
                return visit_UnaryOpNode(node, context);
             } else { // "BinaryOpNode"
                return visit_BinaryOpNode(node, context);
            }
        }

        RuntimeResult visit_NumberNode(Node* node, Context context) {
            Number number(stof(node->data.getValue()));
            number.setNumberPosition(node->data.getPosStart(), node->data.getPosEnd());
            number.setContext(context);

            RuntimeResult result;
            return result.success(number);
        }

        RuntimeResult visit_BinaryOpNode(Node* node, Context context) {
            RuntimeResult res;
            Number left = res.mRegister(visit(node->left, context));
            if (res.getError().isError()) return res;
            Number right = res.mRegister(visit(node->right, context));
            if (res.getError().isError()) return res;

            Number result;
            Error divError;
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
                result = left.divedBy(right, divError);
                break;
            default:
                cout << "visit_BinaryOpNode() cant handle operation token: " << node->data.as_string() << endl;
            }

            if (divError.isError()) return res.failure(divError);
            // else
            result.setNumberPosition(node->data.getPosStart(), node->data.getPosStart());
            return res.success(result);
        }

        RuntimeResult visit_UnaryOpNode(Node* node, Context context) {
            RuntimeResult res;
            Number number = res.mRegister(visit(node->left, context));
            if (res.getError().isError()) return res;

            Number result;
            if (node->data.getTokenType() == TT_MINUS) {
                result = number.multedBy(Number(-1));
            }
            result.setNumberPosition(node->data.getPosStart(), node->data.getPosEnd());
            return res.success(result);
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
