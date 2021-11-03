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
        bool isUnarySymbolNode(Node* node) {
            return node->data.getTokenType() == TT_PLUS || node->data.getTokenType() == TT_MINUS || node->data.matches(TT_KEYWORD, "NOT");
        }
    public:
        RuntimeResult visit(Node* node, Context &context) {
            switch (node->type) {
                case VarAssignNode:
                    return visit_VarAssignNode(node, context);

                case VarAccessNode:
                    return visit_VarAccessNode(node, context);

                case NumberNode:
                    return visit_NumberNode(node, context);

                case UnaryOpNode:
                    return visit_UnaryOpNode(node, context);

                case BinOpNode:
                    return visit_BinaryOpNode(node, context);

                default:
                    return visit_IfNode(node, context);
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
            } else if (node->data.matches(TT_KEYWORD, "NOT")) {
                number = number.getCompNOT();
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

        RuntimeResult visit_IfNode(Node* node, Context context) {
            RuntimeResult res;
            // Iterate through cases map
            for (map<Node*, Node*>::iterator it = node->cases.begin(); it != node->cases.end(); it++) {
                Number conditionValue = res.mRegister(visit(it->first, context));
                if (res.getError().isError()) return res;

                if (conditionValue.getValue() != 0) {
                    Number exprValue = res.mRegister(visit(it->second, context));
                    if (res.getError().isError()) return res;
                    return res.success(exprValue);
                }
            }

            // Check elseCase
            if (node->right != NULL) {
                Number elseValue = res.mRegister(visit(node->right, context));
                if (res.getError().isError()) return res;
                return res.success(elseValue);
            }

            Number none(0);
            return res.success(none);
        }
};

#endif // INTERPRETER_HPP
