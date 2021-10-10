#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>
#include <string>

#include "Position.hpp"
#include "StringUtils.hpp"

using namespace std;

class Error {

private:
    string errorName;
    string details;
    Position posStart;
    Position posEnd;

    StringUtils strUtils;

public:
    Error() {
        errorName = "";
        details = "";
    }

    Error(string pErrorName) {
        errorName = pErrorName;
    }

    Error(string pErrorName, Position pPosStart, Position pPosEnd, string pDetails) {
        errorName = pErrorName;
        posStart = pPosStart;
        posEnd = pPosEnd;
        details = pDetails;
    }

    bool isError() {
        return errorName != "";
    }

    string as_string() {
        string result = errorName + " : " + details + "\n File " + posStart.getFilename()
                        + ", line " + strUtils.tostring(posStart.getLn() + 1) + "\n";
        return result;
    }
};

#endif // ERROR_HPP
