#ifndef POSITION_HPP
#define POSITION_HPP

#include <iostream>

using namespace std;

/*
    Clase para almacenar los datos de la posicion del archivo
    que vamos a interpretar
*/

// NOTA: Es mejor para el compilador tener las implementaciones de los metodos en el respectivo fichero cpp

class Position {
private:
    int idx;
    int ln;
    int col;
    string filename;
    string fileContent;

public:
    Position() {}
    Position(int pIdx, int pLn, int pCol, string pFilename, string pFileContent) {
        idx = pIdx;
        ln = pLn;
        col = pCol;
        filename = pFilename;
        fileContent = pFileContent;
    }

    void advance() {
        idx++;
        col++;
    }

    void advance(char pCurrChar) {
        idx++;
        col++;

        if (pCurrChar == '\n') {
            ln++;
            col = 0;
        }
    }

//    Position get() {
//        return Position(idx, ln, col, filename, fileContent);
//    }

    int getIdx() { return idx; }
    int getLn() { return ln; }
    int getCol() { return col; }
    string getFilename() { return filename; }
    string getFileContent() { return fileContent; }

};

#endif // POSITION_HPP
