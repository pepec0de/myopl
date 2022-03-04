#ifndef NUMBER_HPP
#define NUMBER_HPP

#include "Value.hpp"
#include <math.h>

typedef long double TNumber;

class Number : public Value {
    TNumber value;

    public:
        Number() {}

        Number(TNumber pValue) {
            value = pValue;
        }

        TNumber getValue() { return value; }

        Number addedTo(Number other) {
            Number result(value + other.getValue());
            result.setContext(context);
            return result;
        }

        Number subbedBy(Number other) {
            Number result(value - other.getValue());
            result.setContext(context);
            return result;
        }

        Number multedBy(Number other) {
            Number result(value * other.getValue());
            result.setContext(context);
            return result;
        }

        Number divedBy(Number other, Error &error) {
            if (other.getValue() == 0) {
                error = RuntimeError(other.getPosStart(), other.getPosEnd(), "Division by zero", context);
                return *this;
            } else {
                Number result(value / other.getValue());
                result.setContext(context);
                return result;
            }
        }

        Number toPowerOf(Number other) {
            Number result(pow(value, other.getValue()));
            result.setContext(context);
            return result;
        }

        Number getCompEqual(Number other) {
            Number result(value == other.getValue() ? 1 : 0);
            result.setContext(context);
            return result;
        }

        Number getCompNotEqual(Number other) {
            Number result(value != other.getValue() ? 1 : 0);
            result.setContext(context);
            return result;
        }

        Number getCompLesst(Number other) {
            Number result(value < other.getValue() ? 1 : 0);
            result.setContext(context);
            return result;
        }

        Number getCompGreatt(Number other) {
            Number result(value > other.getValue() ? 1 : 0);
            result.setContext(context);
            return result;
        }

        Number getCompLessEqualt(Number other) {
            Number result(value <= other.getValue() ? 1 : 0);
            result.setContext(context);
            return result;
        }

        Number getCompGreatEqualt(Number other) {
            Number result(value >= other.getValue() ? 1 : 0);
            result.setContext(context);
            return result;
        }

        Number getCompAND(Number other) {
            Number result(value && other.getValue() ? 1 : 0);
            result.setContext(context);
            return result;
        }

        Number getCompOR(Number other) {
            Number result(value || other.getValue() ? 1 : 0);
            result.setContext(context);
            return result;
        }

        Number getCompNOT() {
            Number result(!value);
            result.setContext(context);
            return result;
        }

        string as_string() {
            return strUtils.tostring(value);
        }
};
#endif // NUMBER_HPP
