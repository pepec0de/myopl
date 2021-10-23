#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <iostream>
#include <map>

using namespace std;

struct SymbolValue {
    string type;
    string value;
};

class SymbolTable {
    map<string, SymbolValue> symbols;
    SymbolTable *parent;

public:
    SymbolTable() {
        parent = NULL;
        symbols = {};
    }

    SymbolValue get(string symbolName) {
        SymbolValue result;
        result.type = ""; // El tipo nunca puede ser nulo
        auto search = symbols.find(symbolName);
        if (search != symbols.end()) {
            result = search->second;
        } else {
            if (parent != NULL) return parent->get(symbolName);
        }
        return result;
    }

    void append(string keyName, SymbolValue value) {
        symbols[keyName] = value;
    }

    void remove(string keyName) {
        symbols.erase(keyName);
    }
};

#endif // SYMBOLTABLE_HPP
