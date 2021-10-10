#include "Error.hpp"
#include "Position.hpp"

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
