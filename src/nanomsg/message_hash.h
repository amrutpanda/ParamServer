/* Auto-generated hash file */
#pragma once
#include <stdint.h>

namespace proto_hash {

template <class T>
constexpr uint32_t getHash(void)
{
    return 0;
}

template <>
constexpr uint32_t getHash<Header>()
{
    return 0x69deadf7;
}

template <>
constexpr uint32_t getHash<GetParameterFromPathMsg>()
{
    return 0xb382809f;
}

template <>
constexpr uint32_t getHash<GetParameterMsg>()
{
    return 0xdd2e9ae8;
}

template <>
constexpr uint32_t getHash<GetParameterInfoMsg>()
{
    return 0x8bca1746;
}

template <>
constexpr uint32_t getHash<ParameterInfoMsg>()
{
    return 0xaec2b214;
}

template <>
constexpr uint32_t getHash<SetParameterMsg>()
{
    return 0x133060c4;
}

template <>
constexpr uint32_t getHash<SaveMsg>()
{
    return 0x991522a8;
}

template <>
constexpr uint32_t getHash<GetLinkInfoMsg>()
{
    return 0x10034e1c;
}

} // namespace proto_hash