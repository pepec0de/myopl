#ifndef OPERATIONTREE_HPP
#define OPERATIONTREE_HPP

#include "StringUtils.hpp"
#include "Token.hpp"
#include <map>

using namespace std;

enum NodeType {
    NumberNode,
    BinOpNode,
    UnaryOpNode,
    VarAccessNode,
    VarAssignNode,
    IfNode
};

struct Node {
    Token data;
    NodeType type;
    Node *left;
    Node *right;
    map<Node*,Node*> cases;
};

class OperationTree {
    private:
        Node *root;
        StringUtils strUtils;

    public:
        OperationTree();
        OperationTree(Token rootToken);
        ~OperationTree();

        void setRootNode(Node *node);
        Node* getRootNode() { return root; }

        void deleteNode(Node *node);
        bool hasChildren(Node *node);
        int hasChild(Node *node);

        // DEBUG
        string as_string();
        string getNodeAsString(Node *node);
};

#endif // OPERATIONTREE_HPP
