#ifndef _PS_TABLE_HPP
#define _PS_TABLE_HPP

#include <iostream>
#include <cstring>
#include "ps_databuffer.hpp"
#include <memory>

namespace ps
{

    struct TableEntryBase
    {
        public:
            TableEntryBase() = default;
            virtual ~TableEntryBase() = default;
    };
    

    template <typename T, unsigned int NUM_ELEMENTS = 1 ,unsigned int BUFFER_SIZE = 16>
    struct TableEntry : public TableEntryBase
    {
        public:
            std::string key;
            unsigned int group_name;
            unsigned int data_type;
            unsigned int nele;
            unsigned int is_linked;
            int link_offset = -1;  // applicable for array like storage.
            unsigned int param_type; // it means output or input or both or one time publish
            T* data_pointer;
            ps::SpmcBuffer<T,NUM_ELEMENTS,BUFFER_SIZE> data_buffer; 

            /**
             * @brief : is_linked parameter 
            */
            void process_table_entry()
            {
                // _buffer = new ps::SpmcBuffer<T,NUM_ELEMENTS,BUFFER_SIZE>;
            }

            void link_to_key(const TableEntry& entry, const unsigned int arr_index)
            {
                // delete _buffer;
            
            }
    };

    class ParameterTable
    {
    private:
        unsigned int tbsize;
        std::vector<std::unique_ptr<TableEntryBase>> _table;
    public:
        ParameterTable(const unsigned int table_size = 1000) 
        {   
            tbsize = table_size; 
            _table.resize(tbsize);
        };
        ~ParameterTable() {};

        template <typename T, unsigned int N = 1>
        unsigned int addTableEntry(const std::string key, T* value)
        {
            auto te = std::make_unique<TableEntry<int>>();
            te->data_buffer = value;
            te->key = key;
            te->nele = N;
            _table.push_back(te);
            return _table.size()-1;
        }
    };
    
    
    
} // namespace ps



#endif // _PS_TABLE_HPP