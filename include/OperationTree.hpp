#ifndef OPERATIONTREE_HPP
#define OPERATIONTREE_HPP

#include "StringUtils.hpp"
#include "Token.hpp"

using namespace std;

struct Node {
    Token data;
    Node *left;
    Node *right;
};

class OperationTree {
    private:
        Node *root;
        StringUtils strUtils;

    public:
        OperationTree();
        OperationTree(Token rootToken);
        ~OperationTree();

        bool addNode(Node *parentNode, Token data);
        bool addNode(Node *parentNode, Token data, bool left);
        bool hasChildren(Node *node);
        int hasChild(Node *node);

        void setRootNode(Node *node);
        void setToken(Node *node, Token token) { node->data = token; }
        Node* getRootNode() { return root; }
        Token getToken(Node *node) { return node->data; }
        Node* getLeftChild(Node *node) { return node->left; }
        Node* getRightChild(Node *node) { return node->right; }

        void deleteNode(Node *node); // RECURSIVE func

        string as_string();
        string getNodeAsString(Node *node);

};

#endif // OPERATIONTREE_HPP
