#ifndef _PS_TABLE_HPP
#define _PS_TABLE_HPP

#include <iostream>
#include <cstring>
#include "ps_databuffer.hpp"

namespace ps
{
    template <typename T, unsigned int NUM_ELEMENTS = 1 ,unsigned int BUFFER_SIZE = 16>
    struct TableEntry
    {
        std::string key;
        unsigned int group_name;
        unsigned int data_type;
        unsigned int nele;
        unsigned int is_linked;
        int link_offset = -1;  // applicable for array like storage.
        unsigned int param_type;
        T* data_ptr;
        ps::SpmcBuffer<T,NUM_ELEMENTS,BUFFER_SIZE>* _buffer = nullptr; 

        /**
         * @brief : is_linked parameter 
        */
        void process_table_entry()
        {
            _buffer = new ps::SpmcBuffer<T,NUM_ELEMENTS,BUFFER_SIZE>;
        }

        void link_to_key(const TableEntry& entry, const unsigned int arr_index)
        {
            delete _buffer;
        
        }
    };
    
} // namespace ps



#endif // _PS_TABLE_HPP