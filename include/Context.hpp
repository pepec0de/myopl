#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <iostream>
#include "Position.hpp"
#include "SymbolTable.hpp"

using namespace std;

class Context {
    private:
        string displayName;
        Context* parent = NULL;
        Position parentEntryPos;
        SymbolTable symbolTable;

    public:
        Context() {
            displayName = "";
        }

        Context(string pDisplayName, Context* pParent = NULL, Position pParentEntryPos = Position()) {
            displayName = pDisplayName;
            parent = pParent;
            parentEntryPos = pParentEntryPos;
        }

        string getDisplayName() { return displayName; }

        Context* getParent() { return parent; }

        Position getParentEntryPos() { return parentEntryPos; }
        SymbolTable & getSymbolTable() { return symbolTable; }
        void setSymbolTable(SymbolTable pSymbolTable) { symbolTable = pSymbolTable; }
};

#endif // CONTEXT_HPP
