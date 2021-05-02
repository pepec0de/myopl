#ifndef POSITION_HPP
#define POSITION_HPP

#include <iostream>

using namespace std;

/*
    Clase para almacenar los datos de la posicion del archivo
    que vamos a interpretar
*/
class Position {
private:
    int idx;
    int ln;
    int col;
    string filename;
    string fileContent;

public:
    Position() {}
//    Position() {
//        idx = -1;
//        ln = 0;
//        col = -1;
//    }

    Position(int pIdx, int pLn, int pCol, string pFilename, string pFileContent) {
        idx = pIdx;
        ln = pLn;
        col = pCol;
        filename = pFilename;
        fileContent = pFileContent;
    }

    void advance(char pCurrChar) {
        idx++;
        col++;

        if (pCurrChar == '\n') {
            ln++;
            col = 0;
        }
    }

    Position getPosition() {
        return Position(idx, ln, col, filename, fileContent);
    }

    int getIdx() { return idx; }
    int getLn() { return ln; }
    int getCol() { return col; }
    string getFilename() { return filename; }
    string getFileContent() { return fileContent; }

};

#endif // POSITION_HPP
