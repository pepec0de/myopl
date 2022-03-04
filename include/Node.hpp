#ifndef NODE_HPP
#define NODE_HPP

#include "Token.hpp"
#include <vector>

enum NodeType {
    NullNode,
    VarAssign,
    VarAccess,
    NumberN,
    UnaryOp,
    BinaryOp,
    For,
    While,
    If,
    FuncDef,
};

class Node {
    virtual void thisMakesAPolymorphicClass() {}

public:
    virtual ~Node() {}
    Position pStart;
    Position pEnd;
};

class NumberNode : public Node {
public:
    Token numberTok;

    NumberNode(Token numberTok) {
        this->numberTok = numberTok;
        pStart = numberTok.getPosStart();
        pEnd = numberTok.getPosEnd();
    }
};

class BinOpNode : public Node {
public:
    Token opTok;
    Node *leftNode;
    Node *rightNode;

    BinOpNode(Token opTok, Node *leftNode, Node *rightNode) {
        this->opTok = opTok;
        this->leftNode = leftNode;
        this->rightNode = rightNode;
        pStart = leftNode->pStart;
        pEnd = rightNode->pEnd;
    }
};

class UnaryOpNode : public Node {
public:
    Token opTok;
    Node *node;

    UnaryOpNode(Token opTok, Node *node) {
        this->opTok = opTok;
        this->node = node;
        pStart = opTok.getPosStart();
        pEnd = node->pEnd;
    }
};

class VarAccessNode : public Node {
public:
    Token varNameTok;

    VarAccessNode(Token varNameTok) {
        this->varNameTok = varNameTok;
        pStart = varNameTok.getPosStart();
        pEnd = varNameTok.getPosEnd();
    }
};

class VarAssignNode : public Node {
public:
    Token varNameTok;
    Node *valueNode;

    VarAssignNode(Token varNameTok, Node *valueNode) {
        this->varNameTok = varNameTok;
        this->valueNode = valueNode;
        pStart = varNameTok.getPosStart();
        pEnd = valueNode->pEnd;
    }
};

class IfNode : public Node {
public:
    vector<vector<Node*>> cases;
    Node *elseCase;

    IfNode(vector<vector<Node*>> cases, Node *elseCase) {
        this->cases = cases;
        this->elseCase = elseCase;
        pStart = cases[0][0]->pStart;
        pEnd = (elseCase ? elseCase->pEnd : cases[cases.size()-1][0]->pEnd);
    }
};

class ForNode : public Node {
public:
    Token varNameTok;
    Node *startValueNode;
    Node *endValueNode;
    Node *stepValueNode;
    Node *bodyNode;

    ForNode(Token varNameTok, Node *startValueNode, Node *endValueNode, Node *stepValueNode, Node *bodyNode) {
        this->varNameTok = varNameTok;
        this->startValueNode = startValueNode;
        this->endValueNode = endValueNode;
        this->stepValueNode = stepValueNode;
        this->bodyNode = bodyNode;
        pStart = varNameTok.getPosStart();
        pEnd = bodyNode->pEnd;
    }
};

class WhileNode : public Node {
public:
    Node *conditionNode;
    Node *bodyNode;

    WhileNode(Node *conditionNode, Node *bodyNode) {
        this->conditionNode = conditionNode;
        this->bodyNode = bodyNode;
        pStart = conditionNode->pStart;
        pEnd = bodyNode->pEnd;
    }
};

class FuncDefNode : public Node {
public:
    Token varNameTok;
    vector<Token> argNameToks;
    Node *bodyNode;
    Position pStart;
    Position pEnd;

    FuncDefNode(Token varNameTok, vector<Token> argNameToks, Node *bodyNode) {
        this->varNameTok = varNameTok;
        this->argNameToks = argNameToks;
        this->bodyNode = bodyNode;

        if (varNameTok.getTokenType() != TT_NULL) {
            pStart = varNameTok.getPosStart();
        } else if (argNameToks.size() > 0) {
            pStart = argNameToks[0].getPosStart();
        } else {
            pStart = bodyNode->pStart;
        }
        pEnd = bodyNode->pEnd;
    }
};

class CallNode : public Node {
public:
    Node *nodeToCall;
    vector<Node*> argNodes;

    CallNode(Node *nodeToCall, vector<Node*> argNodes) {
        this->nodeToCall = nodeToCall;
        this->argNodes = argNodes;

        pStart = nodeToCall->pStart;
        if (argNodes.size() > 0) {
            pEnd = argNodes[argNodes.size() - 1]->pEnd;
        } else {
            pEnd = nodeToCall->pEnd;
        }
    }
};

#endif // NODE_HPP
