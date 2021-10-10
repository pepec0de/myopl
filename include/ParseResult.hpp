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

            ParseResult copia;
            copia.setNode(getNode());
            copia.setError(getError());
            return getCopia();
        }

        ParseResult failure(Error pError) {
            error = pError;
            return getCopia();
        }

        ParseResult getCopia() {
            ParseResult copia;
            copia.setNode(getNode());
            copia.setError(getError());
            return copia;
        }

        void setError(Error pError) { error = pError; }
        Error getError() { return error; }
        void setNode(Node* pNode) { node = pNode; }
        Node* getNode() { return node; }

};

#endif // PARSERESULT_HPP
