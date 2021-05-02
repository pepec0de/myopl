#include <iostream>
#include "Lexer.hpp"

using namespace std;

void input(string &cmdStr) {
    cout << ">> ";
    getline(cin, cmdStr);
    // TODO : cmdStr = strUtils.replaceAll(cmdStr, " ", "");
}

void run(string text) {
    Lexer lexer(text);
    Error error;

    vector<Token> tokens = lexer.getTokens(error);
    if (!error.isError()) {
        cout << "[";
        for (Token token : tokens) {
            cout << token.getTokenType() << ", ";
        }
        cout << "]\n";
    } else {
        cout << error.as_string();
    }
}

int main() {

    // String para almacenar la operacion
    string cmdStr;
    input(cmdStr);
    run(cmdStr);
    do {
        input(cmdStr);
        run(cmdStr);
    } while(cmdStr != "q");
    return 0;
}
