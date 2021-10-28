#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "OperationTree.hpp"
#include "Number.hpp"
#include "RuntimeResult.hpp"
#include "Context.hpp"

class Interpreter {
    private:
        // Returns true if node contains number
        bool isNumberNode(Node* node) {
            return node->data.getTokenType() == TT_INT || node->data.getTokenType() == TT_FLOAT;
        }

        // Returns true if node contains + or - operator
        bool isSymbolNode(Node* node) {
            return node->data.getTokenType() == TT_PLUS || node->data.getTokenType() == TT_MINUS;
        }
    public:
        string getMethodName(Node* node) {
            // Programming methods:
            if (node->data.getTokenType() == TT_IDENTIFIER && node->left == NULL && node->right == NULL) return "VarAccessNode";
            if (node->data.getTokenType() == TT_IDENTIFIER && node->right == NULL) return "VarAssignNode";
            // Arithmetic methods:
            if (isNumberNode(node)) return "NumberNode";
            if (isSymbolNode(node) && node->right == NULL) return "UnaryOpNode";
            return "BinOpNode";
        }

        RuntimeResult visit(Node* node, Context &context) {
            string methodName = getMethodName(node);
            if (methodName == "VarAssignNode") {
                return visit_VarAssignNode(node, context);
            }
            if (methodName == "VarAccessNode") {
                return visit_VarAccessNode(node, context);
            }
            if (methodName == "NumberNode") {
                return visit_NumberNode(node, context);
            }
            if (methodName == "UnaryOpNode") {
                return visit_UnaryOpNode(node, context);
            }
            // "BinaryOpNode"
            return visit_BinaryOpNode(node, context);
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

            case TT_POW:
                result = left.toPowerOf(right);
                break;

            case TT_EQEQ: // ==
                result = left.getCompEqual(right);
                break;

            case TT_NOTEQ: // !=
                result = left.getCompNotEqual(right);
                break;

            case TT_LESST: // <
                result = left.getCompLesst(right);
                break;

            case TT_GREATT: // >
                result = left.getCompGreatt(right);
                break;

            case TT_LESSTEQ: // <=
                result = left.getCompLessEqualt(right);
                break;

            case TT_GREATTEQ: // >=
                result = left.getCompGreatEqualt(right);
                break;

            case TT_KEYWORD:
                if (node->data.getValue() == "AND") {
                    result = left.getCompAND(right);
                    break;
                } else if (node->data.getValue() == "OR") {
                    result = left.getCompOR(right);
                    break;
                }

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

            if (node->data.getTokenType() == TT_MINUS) {
                number = number.multedBy(Number(-1));
            }

            number.setNumberPosition(node->data.getPosStart(), node->data.getPosEnd());
            return res.success(number);
        }

        RuntimeResult visit_VarAccessNode(Node* node, Context &context) {
            RuntimeResult res;
            string varName = node->data.getValue();
            SymbolValue varValue = context.getSymbolTable().get(varName);
            if (varValue.type == "") {
                return res.failure(RuntimeError(node->data.getPosStart(), node->data.getPosStart(), "\'" + varName + "\' is not defined", &context));
            }
            return res.success(Number(stof(varValue.value)));
        }

        RuntimeResult visit_VarAssignNode(Node* node, Context &context) {
            RuntimeResult res;

            Number num = res.mRegister(visit(node->left, context));
            if (res.getError().isError()) {
                return res;
            }

            string varName = node->data.getValue();
            SymbolValue varValue;
            varValue.type = "number";
            varValue.value = num.as_string();
            context.getSymbolTable().append(varName, varValue);
            return res.success(num);
        }
};

#endif // INTERPRETER_HPP
