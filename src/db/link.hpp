#ifndef _LINK_HPP
#define _LINK_HPP

#include <cstdint>
#include <iostream>

namespace ps
{
    struct Linkinfo
    {
        size_t src_offset;
        size_t src_length;
        size_t src_ele_size;
        size_t src_nele;
        size_t src_total_size;

    };
    
} // namespace ps


#endif // _LINK_HPP