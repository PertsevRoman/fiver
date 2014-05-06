#ifndef CODES_H
#define CODES_H

#include <QMetaType>

namespace Errors {
    enum ImgErrorCode {
        NoError = 1,
        Error   = 2
    };
}

Q_DECLARE_METATYPE(Errors::ImgErrorCode)

#endif // CODES_H
