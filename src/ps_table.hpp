#ifndef _PS_TABLE_HPP
#define _PS_TABLE_HPP

#include <iostream>
#include <cstring>
#include "ps_databuffer.hpp"
#include <ps_group.hpp>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>

template <typename T>
struct TypeInfo
{
    static const std::type_index id;
};

template <typename T>
const std::type_index TypeInfo<T>::id = typeid(T);

namespace ps
{
   

    struct TableEntryBase
    {
        public:
            unsigned int index;
            unsigned int data_type;
            unsigned int nele;
            unsigned int is_linked;
            int link_offset = -1;  // applicable for array like storage.
            unsigned int param_type; // it means output or input or both or one time publish
            unsigned int user_permission;
            unsigned int registered_writers{};
            TableEntryBase() = default;
            virtual ~TableEntryBase() = default;
    };
    

    template <typename T, unsigned int NUM_ELEMENTS = 1 ,unsigned int BUFFER_SIZE = 16>
    struct TableEntry : public TableEntryBase
    {
        public:
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
        std::vector<Group> group_list; 
        std::unordered_map<std::string, unsigned int> key_table_index_map;
    public:
        ParameterTable(const unsigned int table_size = 1000) 
        {   
            tbsize = table_size; 
            _table.reserve(tbsize);
        };
        ~ParameterTable() {};

        template <typename T, unsigned int N = 1>
        unsigned int addTableEntry(const std::string& key, T& value, const unsigned int& mode = 0)
        {
            // define a variable to store indexes.
            int n;
            // first check whether the key already exists.
            if (key_table_index_map.contains(key))
            {
                n = key_table_index_map.at(key);
                auto& te = _table[n];
                
                // if (mode == 1 )
            }
            else
            {
                // get the current size of table.
                n = _table.size();
                // create a unique pointer for a new table row.
                std::unique_ptr<TableEntry<T,N>> te = std::make_unique<TableEntry<T,N>>();
                te->data_buffer.write(value);
                te->nele = N;
                te->index = n;
                // determine whether it read or write.
                if (mode == 1)
                    te->registered_writers++;
                // push the unique pointer into table container.
                _table.push_back(std::move(te));
                // add the key to unordered_map along with the index.
                key_table_index_map[key] = n;
            }
            
            return n;
        }

        template <typename T, unsigned int N = 1>
        unsigned int addTableEntry(const std::string& key, T& value, 
                                    const unsigned int& group_id, const unsigned int& param_type,
                                    const unsigned int& data_type = 0,
                                    const unsigned int& user_perm = 0,
                                    const unsigned int& mode = 0)
        {
            
            // get the current size of table.
            int n = _table.size();
            // create a unique pointer for a new table row.
            std::unique_ptr<TableEntry<T,N>> te = std::make_unique<TableEntry<T,N>>();
            te->data_buffer.write(value);
            te->nele = N;
            te->user_permission = user_perm;
            te->param_type = param_type;
            te->data_type = data_type;
            // determine whether it read or write.
            if (group_id >= group_list.size()) throw std::runtime_error("group id out of range");
            if (mode == 1)
            {
                te->registered_writers++;
                group_list[group_id].insert_for_write(n,value);
            }
            else
            {
               group_list[group_id].insert_for_read(n,value);
            }
            
            // push the unique pointer into table container.
            _table.push_back(std::move(te));
            key_table_index_map[key] = n;
            return n;
        }

        unsigned int createGroup(const std::string& name)
        {
            // get the group vector size.
            int n = group_list.size();
            Group group(name,n);
            group_list.push_back(group);
            return n;
        }



    };
    
    
    
} // namespace ps



#endif // _PS_TABLE_HPP