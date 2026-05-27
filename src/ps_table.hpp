#ifndef _PS_TABLE_HPP
#define _PS_TABLE_HPP

#include <iostream>
#include <cstring>
#include <cassert>
#include "ps_databuffer.hpp"
#include <ps_group.hpp>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <Eigen/Dense>
#include <Eigen/Core>
#include <cstddef>

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
            std::type_index dtype;
            unsigned int index;
            unsigned int data_type;
            unsigned int nele;
            unsigned int is_linked;
            int link_offset = -1;  // applicable for array like storage.
            unsigned int param_type; // it means output or input or both or one time publish
            unsigned int user_permission;
            unsigned int registered_writers{};
            unsigned int registered_readers{};
            TableEntryBase() = default;
            virtual ~TableEntryBase() = default;

            TableEntryBase(const std::type_info& t): dtype(t) {}; 

            virtual void write_to_param_table(const void* data) {};
            // virtual void write_to_param_table(const void* data, unsigned int& offset);
            virtual void read_from_param_table(void* data) {};
            // virtual void read_from_param_table(void* data, unsigned int& offset);
    };
    

    template <typename T, unsigned int NUM_ELEMENTS = 1 ,unsigned int BUFFER_SIZE = 16>
    struct TableEntry : public TableEntryBase
    {
        public:
            T* data_pointer;
            ps::SpmcBuffer<T,NUM_ELEMENTS,BUFFER_SIZE> data_buffer; 

            TableEntry() : TableEntryBase(typeid(T)) {};

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

            void write_to_param_table(const void* data) override
            {
                data_buffer.write(data);
                std::cout <<  "writing to param table. " << std::endl;
            }

            // void write_to_param_table(const void* data, unsigned int& offset) override
            // {
            //     data_buffer.write(data,offset);
            // }

            void read_from_param_table(void* data)
            {
                data_buffer.read(data);
                std::cout << "reading from param table. " << std::endl;
            }

            // void read_from_param_table(void* data, unsigned int& offset)
            // {
            //     data_buffer.read(data, offset);
            // }
    };

    class ParameterTable
    {
    private:
        unsigned int tbsize;
        std::vector<std::unique_ptr<TableEntryBase>> _table;
        std::vector<Group> group_list; 
        std::unordered_map<std::string, unsigned int> key_table_index_map;
        std::vector<std::byte> user_buffer;
    public:
        ParameterTable(const unsigned int table_size = 1000, const unsigned int user_buffer_size = 100) 
        {   
            tbsize = table_size; 
            _table.reserve(tbsize);
            user_buffer.reserve(user_buffer_size);
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
                std::cout << "key: " << key << " NumElements: " << te->nele << std::endl; 

                // check whether the types are matching.
                if (te->dtype != typeid(T)) throw std::runtime_error("type mismatch: AddTableEntry(), key: " + key);
                if(mode == 1)
                {
                    if (te->registered_writers > 0) 
                        throw std::runtime_error("cannot have two writers to a parameter, key: " + key);
                }
                else if (mode == 0)
                {
                    te->registered_readers++;
                }
            }
            else
            {
                // get the current size of table.
                n = _table.size();
                // create a unique pointer for a new table row.
                std::unique_ptr<TableEntry<T,N>> te = std::make_unique<TableEntry<T,N>>();
                te->data_buffer.write(value);
                te->nele = std::is_array_v<T> ? std::extent_v<T> : 1 ;
                std::cout << "key: " << key << " NumElements: " << te->nele << std::endl; 
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
        unsigned int addTableEntry(const std::string& key, T* value, const unsigned int& mode = 0)
        {
            // define a variable to store indexes.
            int n;
            // first check whether the key already exists.
            if (key_table_index_map.contains(key))
            {
                n = key_table_index_map.at(key);
                auto& te = _table[n];
                std::cout << "key: P " << key << " NumElements: " << te->nele << std::endl; 

                // check whether the types are matching.
                if (te->dtype != typeid(T)) throw std::runtime_error("type mismatch: AddTableEntry(), key: " + key);
                if(mode == 1)
                {
                    if (te->registered_writers > 0) 
                        throw std::runtime_error("cannot have two writers to a parameter, key: " + key);
                }
            }
            else
            {
                // get the current size of table.
                n = _table.size();
                // create a unique pointer for a new table row.
                std::unique_ptr<TableEntry<T,N>> te = std::make_unique<TableEntry<T,N>>();
                te->data_buffer.write(*value);

                te->nele = std::is_array_v<T> ? std::extent_v<T> : 1 ;
                std::cout << "key: P " << key << " NumElements: " << te->nele << std::endl; 

                te->index = n;
                // determine whether it read or write.
                if (mode == 1)
                    te->registered_writers++;
                else if (mode == 0)
                    
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
                                    const unsigned int& mode = 0,
                                    const unsigned int& data_type = 0,
                                    const unsigned int& user_perm = 0)
        {
            int n;
            if (key_table_index_map.contains(key))
            {
                n = key_table_index_map.at(key);
                auto& te = _table[n];
                
                // check whether the types are matching.
                if (te->dtype != typeid(T)) throw std::runtime_error("type mismatch: AddTableEntry(), key: " + key);
                if(mode == 1)
                {
                    if (te->registered_writers > 0) 
                        throw std::runtime_error("cannot have two writers to a parameter, key: " + key);
                } 
                
                 // determine whether it read or write.
                if (group_id >= group_list.size()) throw std::runtime_error("group id out of range.");
                if (mode == 1)
                {
                    group_list[group_id].insert_for_write(n,value);
                }
                else
                {
                   group_list[group_id].insert_for_read(n,value);
                }
            } 
            else
            {    
                // get the current size of table.
                n = _table.size();
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
            }
            
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

        void readByIndex(const unsigned int& idx, void* data)
        {
            assert(( "readByIndex::invalid index",idx < _table.size()));
            _table[idx]->read_from_param_table(data);
        }



        void readFromKey(const std::string& key, void* data)
        {
            if (key_table_index_map.contains(key))
            {
                int idx = key_table_index_map.at(key);
                _table[idx]->read_from_param_table(data);
            }
        }
        
        void writeByIndex(const unsigned int& idx, const void* data)
        {
            assert(( "writeByIndex::invalid index",idx < _table.size()));
            _table[idx]->write_to_param_table(data);
        }

        void writeToKey(const std::string& key, const void* data)
        {
            if (key_table_index_map.contains(key))
            {
                int idx = key_table_index_map.at(key);
                _table[idx]->write_to_param_table(data);
            }
        }

        void GroupReads(const unsigned int& group_idx)
        {
            assert(( "groupRead::invalid index",group_idx < group_list.size()));
            auto& cGroup = group_list[group_idx];
            for (size_t i = 0; i < cGroup.read_indices.size(); i++)
            {
                unsigned int idx = cGroup.read_indices[i];
                void* ptr = cGroup.read_data_ptrs[i];
                readByIndex(idx, ptr);
            }
        }

        void GroupWrites(const unsigned int& group_idx)
        {
            assert(( "GroupWrite::invalid index",group_idx < group_list.size()));
            auto& cGroup = group_list[group_idx];
            for (size_t i = 0; i < cGroup.write_indices.size(); i++)
            {
                unsigned int idx = cGroup.write_indices[i];
                void* ptr = cGroup.write_data_ptrs[i];
                writeByIndex(idx, ptr);
            }
        }

        unsigned int getParamBufferSize(const unsigned int& idx)
        {
            assert(( "getParamBufferSize::invalid index",idx < _table.size()));
            _table[idx]->nele;   
        }

        unsigned int getParamDataTyep(const unsigned int& idx)
        {
            assert(( "getParamDataType::invalid index",idx < _table.size()));
            auto& te = _table[idx];
            return 1;
        }

        std::vector<std::byte>& UserRead(const unsigned int& idx)
        {
            assert(( "UserRead::invalid index",idx < _table.size()));
            if (_table[idx]->user_permission == 0)
                _table[idx]->read_from_param_table(user_buffer.data());
        }

        void UserWrite(const unsigned int& idx, std::vector<std::byte>& buf)
        {
            assert(( "UserWrite::invalid index",idx < _table.size()));
            if (_table[idx]->user_permission == 0)
                _table[idx]->write_to_param_table(buf.data());
        }

        std::unordered_map<std::string,unsigned int>& getKeyIndexMap()
        {
            return key_table_index_map;
        }

    };
       
} // namespace ps



#endif // _PS_TABLE_HPP