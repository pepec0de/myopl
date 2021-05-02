#ifndef ILLEGALCHARERROR_HPP
#define ILLEGALCHARERROR_HPP

#include "Error.hpp"
#include "Position.hpp"

class IllegalCharError : public Error {
    public:
        IllegalCharError(Position posStart, Position posEnd, string details) : Error("Illegal Character Error", posStart, posEnd, details) {}
};

#endif // ILLEGALCHARERROR_HPP
