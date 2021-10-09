#ifndef ILLEGALCHARERROR_HPP
#define ILLEGALCHARERROR_HPP

#include "Error.hpp"
#include "Position.hpp"

class IllegalCharError : public Error {
    public:
        IllegalCharError(Position posStart, Position posEnd, string details) : Error("Illegal Character", posStart, posEnd, details) {}
};

#endif // ILLEGALCHARERROR_HPP


#ifndef INVALIDSYNTAXERROR_HPP
#define ILLEGALCHARERROR_HPP

#include "Error.hpp"
#include "Position.hpp"

class InvalidSyntaxError : public Error {
    public:
        InvalidSyntaxError(Position posStart, Position posEnd, string details) : Error("Invalid Synax", posStart, posEnd, details) {}
};

#endif // ILLEGALCHARERROR_HPP
