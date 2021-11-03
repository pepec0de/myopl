#include "OperationTree.hpp"

OperationTree::OperationTree() {
    root = new Node;
    if (root != NULL) {
        root->left = NULL;
        root->right = NULL;
        root->data = Token();
    }
}

OperationTree::OperationTree(Token rootData) {
    root = new Node;
    if (root != NULL) {
        root->left = NULL;
        root->right = NULL;
        root->data = rootData;
    }
}

OperationTree::~OperationTree() {
    deleteNode(root);
}

void OperationTree::deleteNode(Node *node) {
    if (hasChildren(node->left)) {
        deleteNode(node->left);
    } else if (node->left != NULL) {
        delete node->left;
        node->left = NULL;
    }
    if (hasChildren(node->right)) {
        deleteNode(node->right);
    } else if (node->right != NULL) {
        delete node->right;
        node->right = NULL;
    }

    delete node;
    node = NULL;
}

void OperationTree::setRootNode(Node *node) {
    deleteNode(root);
    root = node;
}

bool OperationTree::hasChildren(Node *node) {
    if (node != NULL)
        return (node->left != NULL || node->right != NULL);
    else
        return false;
}

string OperationTree::as_string() {
    return getNodeAsString(root);
}

string OperationTree::getNodeAsString(Node *node) {
    return node->data.as_string() + "{" + getNodeTypeAsString(node->type) + "}(" + (node->left == NULL ? " " : getNodeAsString(node->left))
        + "," + (node->right == NULL ? " " : getNodeAsString(node->right)) + ")";
}

string OperationTree::getNodeTypeAsString(NodeType nt) {
    switch (nt) {
        case NumberNode:
            return "NumberNode";
        case BinOpNode:
            return "BinOpNode";
        case UnaryOpNode:
            return "UnaryOpNode";
        case VarAccessNode:
            return "VarAccessNode";
        case VarAssignNode:
            return "VarAssignNode";
        case IfNode:
            return "IfNode";
        default:
            return "Unhandled node type";
    }
}
