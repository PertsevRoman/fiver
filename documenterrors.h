#ifndef DOCUMENTERRORS_H
#define DOCUMENTERRORS_H

namespace DocumentState {
    enum ProcessingState {
        NoError = 0,
        ReadError = 1,
        OpenError = 2,
        ParseError = 3
    };
}

#endif // DOCUMENTERRORS_H
