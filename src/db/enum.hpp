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
        OUTPUT,
        FIXED
    };

    enum class ActionType
    {
        NONE = -1,
        START_PUBLISH = 0,
        STOP_PUBLISH,
        INPUT,
        OUTPUT,
        OVERWRITE_INPUT,
        OVERWRITE_OUTPUT,
        OVERWRITE_DEACTIVATE,
        ACTIVATE_LINK,
        DEACTIVATE_LINK
    };

} // namespace db


#endif // ENUM_HPP_