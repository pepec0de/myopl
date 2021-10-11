#ifndef ERRORS_HPP
#define ERRORS_HPP

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

class RuntimeError : public Error {
    public:
        RuntimeError(Position posStart, Position posEnd, string details) : Error("Runtime Error", posStart, posEnd, details) {}
};

#endif // ERRORS_HPP
