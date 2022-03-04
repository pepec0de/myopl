#ifndef LANGTOOLS_HPP
#define LANGTOOLS_HPP

#include "Node.hpp"

class LangTools {

public:

    NodeType getNodeType(Node *node) {
        if (typeid(*node) == typeid(VarAssignNode)) {
            return VarAssign;
        } else if (typeid(*node) == typeid(VarAccessNode)) {
            return VarAccess;
        } else if (typeid(*node) == typeid(NumberNode)) {
            return NumberN;
        } else if (typeid(*node) == typeid(UnaryOpNode)) {
            return UnaryOp;
        } else if (typeid(*node) == typeid(BinOpNode)) {
            return BinaryOp;
        } else if (typeid(*node) == typeid(ForNode)) {
            return For;
        } else if (typeid(*node) == typeid(WhileNode)) {
            return While;
        } else if (typeid(*node) == typeid(IfNode)) {
            return If;
        } else if (typeid(*node) == typeid(FuncDefNode)) {
            return FuncDef;
        }

        return NullNode;
    }
    void deleteASTNode(Node* node) {
        if (node != NULL)
        switch (getNodeType(node)) {
            case VarAssign:
                deleteASTNode(((VarAssignNode*)node)->valueNode);
                break;

            case UnaryOp:
                deleteASTNode(((UnaryOpNode*)node)->node);
                break;

            case BinaryOp: {
                BinOpNode* cast = (BinOpNode*) node;
                deleteASTNode(cast->leftNode);
                deleteASTNode(cast->rightNode);
            }break;

            case For: {
                ForNode* cast = (ForNode*) node;
                deleteASTNode(cast->startValueNode);
                deleteASTNode(cast->endValueNode);
                deleteASTNode(cast->stepValueNode);
                deleteASTNode(cast->bodyNode);
            }break;

            case While: {
                WhileNode* cast = (WhileNode*) node;
                deleteASTNode(cast->conditionNode);
                deleteASTNode(cast->bodyNode);
            }break;

            case If: {
                IfNode* cast = (IfNode*) node;
                for (int i = 0; i < (int)cast->cases.size(); i++) {
                    for (int j = 0; j < (int)cast->cases[i].size(); j++) {
                        deleteASTNode(cast->cases[i][j]);
                    }
                }
                deleteASTNode(cast->elseCase);
            }break;
            default:;
        }
        delete node;
        node = NULL;
    }
};

#endif // LANGTOOLS_HPP
