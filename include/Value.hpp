#ifndef VALUE_HPP
#define VALUE_HPP

#include "Position.hpp"
#include "StringUtils.hpp"
#include "Errors.hpp"
#include "Context.hpp"

class Value {
    protected:
    StringUtils strUtils;
    Position posStart, posEnd;
    Context *context;

    public:
        Value() {}

        void setPosition(Position posStart, Position posEnd) {
            this->posStart = posStart;
            this->posEnd = posEnd;
        }

        Value* setContext(Context *pContext) {
            context = pContext;
            return this;
        }

        Position getPosStart() { return posStart; }
        Position getPosEnd() { return posEnd; }
        virtual string as_string() { return ""; }

        RuntimeError illegal_operation(Value *other) {
            if (other == NULL) {
                other = this;
            }
            return RuntimeError(posStart, posEnd, "Illegal operation", context);
        }
};

#endif // VALUE_HPP
