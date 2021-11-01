#ifndef OPTREENODE_HPP
#define OPTREENODE_HPP

#include "Token.hpp"

enum NodeType {
    NumberNode,
    BinOpNode,
    UnaryOpNode,
    VarAccessNode,
    VarAssignNode,
    IfNode
};

class OPTreeNode {
    Token data;
    NodeType type;

public:
    OPTreeNode* left;
    OPTreeNode* right;

    void setData(Token pData) { data = pData; }
    Token getData() { return data; }
    void setType(NodeType pType) { type = pType; }
    NodeType getType() {return type; }


};

#endif // OPTREENODE_HPP
