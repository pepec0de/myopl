#ifndef NODE_HPP
#define NODE_HPP

#include "Token.hpp"
#include <vector>

class Node {
    virtual void nulo() {}
};

class NumberNode : public Node {
public:
    Token numberTok;
};

class BinOpNode : public Node {
public:
    Token opTok;
    Node *leftNode;
    Node *rightNode;
    // Position pStart = leftNode.pStart
    // Position pEnd = rightNode.pEnd
};

class UnaryOpNode : public Node {
public:
    Token opTok;
    Node *node;
    // Position pStart = opTok.pStart
    // Position pEnd = node.pEnd
};

class VarAccessNode : public Node {
public:
    Token varNameTok;
};

class VarAssignNode : public Node {
public:
    Token varNameTok;
    Node *valueNode;
};

class IfNode : public Node {
public:
    vector<vector<Node*>> cases;
    Node *elseCase;
    // Position pStart = cases[0][0].pStart
    // Position pEnd = (elseNode || cases[cases.size() - 1][0]).pEnd
};

class ForNode : public Node {
public:
    Token varNameTok;
    Node *startValueNode;
    Node *endValueNode;
    Node *stepValueNode;
    Node *bodyNode;
    // Position pStart = varNameTok.pStart
    // Position pEnd = bodyNode.pEnd
};

class WhileNode : public Node {
public:
    Node *conditionNode;
    Node *bodyNode;
    // Position pStart = conditionNode.pStart
    // Position pEnd = bodyNode.pEnd
};
#endif // NODE_HPP
