#ifndef NUMBER_HPP
#define NUMBER_HPP

#include "Position.hpp"
#include "StringUtils.hpp"
#include "Errors.hpp"

typedef long double TNumber;

class Number
{
    StringUtils strUtils;
    TNumber value;
    Position posStart, posEnd;

    public:
        Number() {}
        Number(TNumber pValue) {
            value = pValue;
        }

        void setNumberPosition(Position posStart, Position posEnd) {
            this->posStart = posStart;
            this->posEnd = posEnd;
        }

        TNumber getValue() { return value; }

        Number addedTo(Number other) {
            Number result(value + other.getValue());
            return result;
        }

        Number subbedBy(Number other) {
            Number result(value - other.getValue());
            return result;
        }

        Number multedBy(Number other) {
            Number result(value * other.getValue());
            return result;
        }

        Number divedBy(Number other, Error &error) {
            if (other.getValue() == 0) {
                error = RuntimeError(other.getPosStart(), other.getPosEnd(), "Division by zero");
                return *this;
            } else {
                Number result(value / other.getValue());
                return result;
            }
        }

        Position getPosStart() { return posStart; }
        Position getPosEnd() { return posEnd; }
        string as_string() {
            return strUtils.tostring(value);
        }
};

#endif // NUMBER_HPP
