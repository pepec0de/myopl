#ifndef NUMBER_HPP
#define NUMBER_HPP

#include "Position.hpp"
#include "StringUtils.hpp"

typedef long long TNumber;

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

        Number divedBy(Number other) {
            Number result(value / other.getValue());
            return result;
        }

        string as_string() {
            return strUtils.tostring(value);
        }
};

#endif // NUMBER_HPP
