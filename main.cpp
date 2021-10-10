#include <iostream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

using namespace std;

void input(string &cmdStr) {
    cout << ">> ";
    getline(cin, cmdStr);
}

void run(string text) {
    Lexer lexer(text);
    Error error;

    vector<Token> tokens = lexer.getTokens(error);
    if (!error.isError()) {
        cout << "[";
        for (Token token : tokens) {
            cout << token.as_string() << ", ";
        }
        cout << "]\n";

        Parser parser(tokens);
        ParseResult resultAST = parser.parse();
        if (resultAST.getError().isError()) {
            cout << resultAST.getError().as_string();
        } else {
            OperationTree ast;
            ast.setRootNode(resultAST.getNode());
            cout << ast.as_string() << endl;
            Interpreter interpreter;
            Number result = interpreter.visit(ast.getRootNode());
            cout << result.getValue() << endl;
        }
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
