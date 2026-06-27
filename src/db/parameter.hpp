#ifndef PARAMETER_HPP_
#define PARAMETER_HPP_

#include <paraminfo.hpp>
#include <variant>

namespace ps
{
    using ParamType = std::variant<uint8_t,int8_t, uint16_t, int16_t, uint32_t,int32_t,
                                        uint64_t, int64_t, bool, int, float, double, std::string >;
    class Parameter
    {
    private:
        uint8_t index;
        uint8_t size;
    public:
        Parameter() {};
        ~Parameter() {};

        // public variables.
        ParamInfo param_info;
        ParamType ptype;

        
    };
    
    
    
} // namespace ps


#endif // PARAMETER_HPP_