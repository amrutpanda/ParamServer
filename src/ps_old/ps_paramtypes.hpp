#ifndef _PS_PARAMTYPES_HPP
#define _PS_PARAMTYPES_HPP

namespace ps
{
    enum class ParameterType
    {
        UNDEFINED = 0,
        INPUT,
        OUTPUT,
        PARAMETER
    };

    enum class ParameterFlag
    {
        OVERWRITE_INPUT_ACTIVE = 0,
        OVERWRITE_OUTPUT_ACTIVE
    };

    enum class DataType
    {
        undefined = 0,
        INT8,
        UINT8,
        INT16,
        UINT16,
        INT32,
        UINT32,
        INT64,
        UINT64,
        BOOL,
        INT = INT32,
        UINT = UINT32,
        FLOAT,
        DOUBLE,
        BYTES
    };

    enum class UserGroup
    {
        undefined = 0,
        GUEST,
        OPERATOR,
        ADMINISTRATOR
    };
    constexpr UserGroup DEFAULT_GROUP = UserGroup::undefined;
    
    enum class Permission
    {
        USER_READ = 0,
        USER_WRITE,
        GROUP_READ,
        GROUP_WRITE
    };
} // namespace ps




#endif // _PS_PARAMTYPES_HPP