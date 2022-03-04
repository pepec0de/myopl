#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "Node.hpp"
#include "Number.hpp"
#include "RuntimeResult.hpp"
#include "Context.hpp"
#include "LangTools.hpp"

class Interpreter {
    LangTools langTools;
    public:
        RuntimeResult visit(Node* node, Context *context) {
            switch (langTools.getNodeType(node)) {
                case VarAssign:
                    return visit_VarAssignNode((VarAssignNode*)node, context);

                case VarAccess:
                    return visit_VarAccessNode((VarAccessNode*)node, context);

                case NumberN:
                    return visit_NumberNode((NumberNode*)node, context);

                case UnaryOp:
                    return visit_UnaryOpNode((UnaryOpNode*)node, context);

                case BinaryOp:
                    return visit_BinaryOpNode((BinOpNode*)node, context);

                case For:
                    return visit_ForNode((ForNode*)node, context);

                case While:
                    return visit_WhileNode((WhileNode*)node, context);

                case If:
                    return visit_IfNode((IfNode*)node, context);

                default:
                    cout << "FATAL ERROR! Couldnt handle node type\n";
                    return RuntimeResult();
            }
        }

        RuntimeResult visit_NumberNode(NumberNode* node, Context *context) {
            Number number(stof(node->numberTok.getValue()));
            number.setPosition(node->numberTok.getPosStart(), node->numberTok.getPosEnd());
            number.setContext(context);

            RuntimeResult result;
            return result.success(number);
        }

        RuntimeResult visit_BinaryOpNode(BinOpNode* node, Context *context) {
            RuntimeResult res;
            Number left = res.mRegister(visit(node->leftNode, context));
            if (res.getError().isError()) return res;
            Number right = res.mRegister(visit(node->rightNode, context));
            if (res.getError().isError()) return res;

            Number result;
            Error divError;
            switch (node->opTok.getTokenType()) {
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
                if (node->opTok.getValue() == "AND") {
                    result = left.getCompAND(right);
                } else if (node->opTok.getValue() == "OR") {
                    result = left.getCompOR(right);
                }
                break;

            default:
                cout << "visit_BinaryOpNode() cant handle operation token: " << node->opTok.as_string() << endl;
            }

            if (divError.isError()) return res.failure(divError);
            // else
            result.setPosition(node->opTok.getPosStart(), node->opTok.getPosStart()); // la posicion esta mal
            return res.success(result);
        }

        RuntimeResult visit_UnaryOpNode(UnaryOpNode* node, Context *context) {
            RuntimeResult res;
            Number number = res.mRegister(visit(node->node, context));
            if (res.getError().isError()) return res;

            if (node->opTok.getTokenType() == TT_MINUS) {
                number = number.multedBy(Number(-1));
            } else if (node->opTok.matches(TT_KEYWORD, "NOT")) {
                number = number.getCompNOT();
            }

            number.setPosition(node->opTok.getPosStart(), node->opTok.getPosEnd());
            return res.success(number);
        }

        RuntimeResult visit_VarAccessNode(VarAccessNode* node, Context *context) {
            RuntimeResult res;
            string varName = node->varNameTok.getValue();
            SymbolValue varValue = context->getSymbolTable().get(varName);
            if (varValue.type == "") {
                return res.failure(RuntimeError(node->varNameTok.getPosStart(), node->varNameTok.getPosStart(), "\'" + varName + "\' is not defined", context));
            }
            return res.success(Number(stof(varValue.value)));
        }

        RuntimeResult visit_VarAssignNode(VarAssignNode* node, Context *context) {
            RuntimeResult res;

            Number num = res.mRegister(visit(node->valueNode, context));
            if (res.getError().isError()) {
                return res;
            }

            string varName = node->varNameTok.getValue();
            SymbolValue varValue;
            varValue.type = "number";
            varValue.value = num.as_string();
            context->getSymbolTable().append(varName, varValue);
            return res.success(num);
        }

        RuntimeResult visit_IfNode(IfNode* node, Context *context) {
            RuntimeResult res;
            // Iterate through cases
            for (unsigned int i = 0; i < node->cases.size(); i++) {
                Number conditionValue = res.mRegister(visit(node->cases[i][0], context));
                if (res.getError().isError()) return res;

                if (conditionValue.getValue() != 0) {
                    Number exprValue = res.mRegister(visit(node->cases[i][1], context));
                    if (res.getError().isError()) return res;
                    return res.success(exprValue);
                }
            }

            // Check elseCase
            if (node->elseCase != NULL) {
                Number elseValue = res.mRegister(visit(node->elseCase, context));
                if (res.getError().isError()) return res;
                return res.success(elseValue);
            }

            Number none(0);
            return res.success(none);
        }

        RuntimeResult visit_ForNode(ForNode* node, Context *context) {
            RuntimeResult res;

            Number startValue = res.mRegister(visit(node->startValueNode, context));
            if (res.getError().isError()) return res;
            Number endValue = res.mRegister(visit(node->endValueNode, context));
            if (res.getError().isError()) return res;

            Number stepValue;
            if (node->stepValueNode != NULL) {
                stepValue = res.mRegister(visit(node->stepValueNode, context));
                if (res.getError().isError()) return res;
            } else {
                stepValue = Number(1);
            }

            TNumber i = startValue.getValue();
            bool bContinue = false;
            if (stepValue.getValue() >= 0) {
                bContinue = i < endValue.getValue();
            } else {
                bContinue = i > endValue.getValue();
            }

            while(bContinue) {
                SymbolValue value;
                value.type = "number";
                value.value = Number(i).as_string();
                context->getSymbolTable().append(node->varNameTok.getValue(), value);
                cout << "SymbolTable:\n";
                i += stepValue.getValue();

                res.mRegister(visit(node->bodyNode, context));
                if (res.getError().isError()) return res;

                if (stepValue.getValue() >= 0) {
                    bContinue = i < endValue.getValue();
                } else {
                    bContinue = i > endValue.getValue();
                }
            }

            return res.success(Number(0));
        }

        RuntimeResult visit_WhileNode(WhileNode* node, Context *context) {
            RuntimeResult res;
            Number condition;
            while(true) {
                condition = res.mRegister(visit(node->conditionNode, context));
                if (res.getError().isError()) return res;

                if (condition.getValue() == 0) break;

                res.mRegister(visit(node->bodyNode, context));
                if (res.getError().isError()) return res;
            }
            return res.success(Number(0));
        }
};

#endif // INTERPRETER_HPP
