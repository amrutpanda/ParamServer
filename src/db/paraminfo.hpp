#ifndef PARAMINFO_HPP_
#define PARAMINFO_HPP_

#include <iostream>
#include <enum.hpp>

namespace ps
{
    template <typename T>

    // this function will return datatype enum and size 
    PSDataType getType(T& var)
    {
        using D = std::remove_cv_t< std::remove_reference_t<std::remove_extent_t<T>>>;
       
        if constexpr(std::is_same_v<D,uint8_t>)
            return PSDataType::UINT8;
        else if constexpr(std::is_same_v<D, int8_t>)
            return PSDataType::INT8;
        else if constexpr(std::is_same_v<D, uint16_t>)
            return PSDataType::UINT16;
        else if constexpr(std::is_same_v<D, int16_t>)
            return PSDataType::INT16;
        else if constexpr(std::is_same_v<D, uint32_t>)
            return PSDataType::UINT32;
        else if constexpr(std::is_same_v<D, int32_t>)
            return PSDataType::INT32;
        else if constexpr(std::is_same_v<D, uint64_t>)
            return PSDataType::UINT64;
        else if constexpr(std::is_same_v<D, int64_t>)
            return PSDataType::INT64;
        else if constexpr(std::is_same_v<D, float>)
            return PSDataType::FLOAT;
        else if constexpr(std::is_same_v<D, double>)
            return PSDataType::DOUBLE;
        else if constexpr(std::is_same_v<D, std::string>)
            return PSDataType::STRING;
        else if constexpr(std::is_same_v<D, bool>)
            return PSDataType::BOOL;
        else
            return PSDataType::UNKNOWN;
    }

    template <typename T>
    size_t getSize(T& var)
    {
        using D = std::remove_cv_t< std::remove_reference_t<std::remove_extent_t<T>>>;
        try
        {
            return sizeof(D);
        }
        catch(const std::exception& e)
        {
            throw std::runtime_error("Invalid data type. Cannot get size.");
        }
        
    }

    // Define paraminfo struct.
    struct ParamInfo
    {
        std::string key;
        PSDataType dtype;
        uint8_t size;
        uint8_t nele;
        std::string group_name;
        int8_t group_index = -1;

    };
    
} // namespace ps


#endif // PARAMINFO_HPP_