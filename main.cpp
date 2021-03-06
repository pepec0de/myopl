#include <iostream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

using namespace std;

void input(string &cmdStr) {
    cout << ">> ";
    getline(cin, cmdStr);
}

void run(string text, Context *context) {
    Lexer lexer(text);
    Error error;

    vector<Token> tokens = lexer.getTokens(error);
    for (Token tok : tokens) {
        cout << tok.as_string() + ", ";
    }
    cout << endl;
    if (!error.isError()) {
        Parser parser(tokens);
        ParseResult resultAST = parser.parse();
        if (resultAST.getError().isError()) {
            cout << resultAST.getError().as_string();
        } else {
            Node* rootNode = resultAST.getNode();
            if (rootNode != NULL) {
                Interpreter interpreter;
                RuntimeResult result = interpreter.visit(rootNode, context);
                if (result.getError().isError()) {
                    cout << result.getError().as_string() << endl;
                } else {
                    cout << result.getValue().getValue() << endl;
                }

            } else cout << "FATAL ERROR!!! Root pointer of AST is 0\n";
        }
    } else {
        cout << error.as_string();
    }
}

int main() {
    SymbolTable globalST;
    SymbolValue value;
    value.type = "number"; value.value = "0";
    globalST.append("null", value);
    value.type = "number"; value.value = "1";
    globalST.append("true", value);
    value.type = "number"; value.value = "0";
    globalST.append("false", value);

    Context *context = new Context("<program>");
    context->setSymbolTable(globalST);

    // String para almacenar la operacion
    string cmdStr;
    input(cmdStr);
    while(cmdStr != "q") {
        if (cmdStr != "") run(cmdStr, context);
        input(cmdStr);
    }
    return 0;
}
