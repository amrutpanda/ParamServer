#ifndef ENUM_HPP_
#define ENUM_HPP_

#include <message.pb.h>

namespace ps
{
    enum class PSDataType
    {
        UNKNOWN = 0,
        UINT8 = 1,
        INT8 = 2,
        UINT16 = 3,
        INT16 = 4,
        UINT32 = 5,
        INT32 = 6,
        UINT64 = 7,
        INT64 = 8,
        FLOAT = 9,
        DOUBLE = 10,
        BOOL = 11,
        STRING = 12
    };

    enum class DataContainerType
    {
        INPUT = 0,
        OUTPUT
    };

} // namespace db


#endif // ENUM_HPP_