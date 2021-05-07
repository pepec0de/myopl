#ifndef NUMBERNODE_HPP
#define NUMBERNODE_HPP

#include "Token.hpp"

class NumberNode
{
private:
    Token token;
    public:
        NumberNode(Token pToken) {
            token = pToken;
        }

        string as_string() {
            return token.as_string();
        }


};

#endif // NUMBERNODE_HPP
