#ifndef RUNTIMERESULT_HPP
#define RUNTIMERESULT_HPP

#include "Number.hpp"
#include "Error.hpp"

class RuntimeResult
{
    Number value;
    Error error;

    public:
        RuntimeResult() {}

        Number mRegister(RuntimeResult res) {
            if (res.getError().isError()) {
                error = res.getError();
            }
            return res.getValue();
        }

        RuntimeResult success(Number pValue) {
            value = pValue;
            return *this;
        }

        RuntimeResult failure(Error pError) {
            error = pError;
            return *this;
        }

        Number getValue() { return value; }
        Error getError() { return error; }
};

#endif // RUNTIMERESULT_HPP
