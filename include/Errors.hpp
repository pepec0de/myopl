#ifndef ERRORS_HPP
#define ERRORS_HPP

#include "Position.hpp"
#include "Context.hpp"
#include "Error.hpp"

class IllegalCharError : public Error {
    public:
        IllegalCharError(Position posStart, Position posEnd, string details) : Error("Illegal Character", posStart, posEnd, details) {}
};

class InvalidSyntaxError : public Error {
    public:
        InvalidSyntaxError(Position posStart, Position posEnd, string details) : Error("Invalid Syntax", posStart, posEnd, details) {}
};

class MemoryError : public Error {
    public:
        MemoryError() : Error("CRITICAL ERROR: Out of memory!!") {}
};

class RuntimeError : public Error {
    Context* context;
    public:
        RuntimeError(Position posStart, Position posEnd, string details, Context* pContext) : Error("Runtime Error", posStart, posEnd, details) {
            context = pContext;
            cout << generateTraceback() << endl;
        }

        string as_string() {
            string error = Error::as_string() + "\n" + generateTraceback();
            return error;
        }

        string generateTraceback() {
            string result;
            Position pos = posStart;

            Context* ctx = context;
            while (ctx->getDisplayName() != "") {
                result = "\tFile " + pos.getFilename() + ", line " + strUtils.tostring(pos.getLn() + 1) + ", in " + ctx->getDisplayName() + "\n" + result;
                pos = ctx->getParentEntryPos();
                ctx = ctx->getParent();
                if (ctx == NULL) break;
            }
            return "Traceback (most recent call last):\n" + result;
        }
};

class ExpectedCharError : public Error {
    public:
        ExpectedCharError(Position posStart, Position posEnd, string details) : Error("Expected Char", posStart, posEnd, details) {}
};

#endif // ERRORS_HPP
