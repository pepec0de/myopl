#ifndef PARSERESULT_HPP
#define PARSERESULT_HPP

#include "Error.hpp"
#include "OperationTree.hpp"

class ParseResult
{
    Node *node;
    Error error;

    public:
        ParseResult() {
            node = NULL;
        }
        Node* mRegister(ParseResult res) {
            if (res.getError().isError()) {
                error = res.getError();
            }
            return res.getNode();
        }

        Node* mRegister(Node* res) {
            return res;
        }

        ParseResult success(Node* pNode) {
            node = pNode;
            return *this;
        }

        ParseResult failure(Error pError) {
            error = pError;
            return *this;
        }

        Error getError() { return error; }
        Node* getNode() { return node; }

};

#endif // PARSERESULT_HPP
