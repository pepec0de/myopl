#ifndef OPERATIONSTRUCTS_HPP
#define OPERATIONSTRUCTS_HPP

#include "Token.hpp"

struct BinOpNode {
    struct BinOpNode* left_node;
    Token tok;
    struct BinOpNode* right_node;
};

struct BinOpNode* newNode(Token tok) {
    struct BinOpNode* node = new struct BinOpNode;
    node->tok = tok;
    node->left_node = NULL;
    node->right_node = NULL;
    return node;
};


#endif // OPERATIONSTRUCTS_HPP
