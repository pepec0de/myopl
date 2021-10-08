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

bool OperationTree::addNode(Node *parentNode, Token data) {
    // Comprobamos que el nodo padre no tiene los dos nodos hijos ocupados
    if (hasChild(parentNode) != 3) {
        Node *newNode = new Node;
        // Comprobamos que tenemos memoria
        if (newNode != NULL) {
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->data = data;

            if (parentNode->left == NULL) {
                parentNode->left = newNode;
            } else {
                parentNode->right = newNode;
            }
            return true;
        }
    }
    return false;
}

bool OperationTree::addNode(Node *parentNode, Token data, bool left) {
    // Comprobamos que el nodo padre no tiene los dos nodos hijos ocupados
    if (hasChild(parentNode) != 3) {
        Node *newNode = new Node;
        // Comprobamos que tenemos memoria
        if (newNode != NULL) {
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->data = data;

            if (left) {
                if (parentNode->left == NULL) {
                    parentNode->left = newNode;
                } else {
                    delete newNode;
                    return false;
                }
            } else {
                if (parentNode->right == NULL) {
                    parentNode->right = newNode;
                } else {
                    delete newNode;
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

bool OperationTree::hasChildren(Node *node) {
    if (node != NULL)
        return (node->left != NULL || node->right != NULL);
    else
        return false;
}

int OperationTree::hasChild(Node *node) {
    // 0 no children
    // 1 left child
    // 2 right child
    // 3 both children
    if (!hasChildren(node)) {
        return 0;
    }
    if (node->left != NULL && node->right == NULL) {
        return 1;
    }
    if (node->left == NULL && node->right != NULL) {
        return 2;
    }
    return 3;
}

string OperationTree::as_string() {
    return root->data.as_string() + "(" + (root->left == NULL ? " " : getNodeAsString(root->left))
        + "," + (root->right == NULL ? " " : getNodeAsString(root->right)) + ")";
}

string OperationTree::getNodeAsString(Node *node) {
    return node->data.as_string() + "(" + (node->left == NULL ? " " : getNodeAsString(node->left))
        + "," + (node->right == NULL ? " " : getNodeAsString(node->right)) + ")";
}

