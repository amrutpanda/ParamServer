#ifndef PARAMETER_HPP_
#define PARAMETER_HPP_

#include <paraminfo.hpp>
#include <variant>
#include <OutputBuffer.hpp>

namespace ps
{
    class Parameter;

    struct Action
    {
        ActionType type;
        int offset;
        int length;
    };
    
    struct Linkinfo
    {
        size_t src_offset;
        size_t src_length;
        size_t src_ele_size;
        size_t src_nele;
        size_t src_total_size;
        Parameter* src_ptr;

        size_t dst_offset;
        size_t dst_length;
        size_t dst_ele_size;
        size_t dst_nele;
        size_t dst_total_size;
        Parameter* dst_ptr;

    };

    using ParamType = std::variant<uint8_t,int8_t, uint16_t, int16_t, uint32_t,int32_t,
                                        uint64_t, int64_t, bool, int, float, double, std::string >;
    class Parameter
    {
    private:
        uint8_t index; // index in parameter storage table.
        uint8_t size;  // size of the parameter owner pointer.

        Action _action;

        std::vector<Linkinfo> links;

        bool publish = false;
        bool link_active = false;
        bool overwrite_active = false;
        bool overwrite_input_active = false;
        bool overwrite_output_active = false;
    public:
        Parameter() {};
        ~Parameter() {};

        // public variables.
        ParamInfo param_info;
        ParamType ptype;        // parameter type whether input or output.

        
    };
    
    
    
} // namespace ps


#endif // PARAMETER_HPP_