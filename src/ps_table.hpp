#ifndef _PS_TABLE_HPP
#define _PS_TABLE_HPP

#include <iostream>
#include <cstring>
#include <cassert>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <array>
#include <tuple>
#include <variant>

#include <Eigen/Dense>
#include <Eigen/Core>

#include <ps_databuffer.hpp>
#include <ps_group.hpp>

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
            std::type_index ftype;
            unsigned int index;
            unsigned int data_type;
            unsigned int nele;
            unsigned int is_linked;
            int link_offset = -1;  // applicable for array like storage.
            unsigned int param_type; // it means output or input or both or one time publish
            unsigned int user_permission;
            unsigned int registered_writers{};
            unsigned int registered_readers{};
            bool recently_updated = false;
            TableEntryBase() = default;
            virtual ~TableEntryBase() = default;

            TableEntryBase(const std::type_info& t): dtype(t), ftype{t} {}; 

            void print_type() {std::cout << dtype.name() << std::endl;}

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

            // const std::type_index normalized_type()
            // {
            //     using U = std::remove_cv_t<std::remove_reference_t<T>>;
            //     if constexpr (std::is_integral_v<U>)
            //         return typeid(int);
            //     else if constexpr (std::is_same_v<U,float>)
            //         return typeid(float);
            //     else if constexpr(std::is_floating_point_v<U>)
            //         return typeid(double);
            //     else
            //         return typeid(std::string);
            // }

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
                // std::cout <<  "writing to param table. " << std::endl;
            }

            // void write_to_param_table(const void* data, unsigned int& offset) override
            // {
            //     data_buffer.write(data,offset);
            // }

            void read_from_param_table(void* data)
            {
                data_buffer.read(data);
                // std::cout << "reading from param table. " << std::endl;
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
    protected:
        // std::vector<std::unique_ptr<TableEntryBase>> _table;
        std::vector<TableEntryBase*> _table;
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
        virtual ~ParameterTable() {};

        std::string getGroupNameFromID(const unsigned int& id_);
        [[nodiscard]]unsigned int createGroup(const std::string& name);
        void readByIndex(const unsigned int& idx, void* data);
        void readFromKey(const std::string& key, void* data);
        void writeByIndex(const unsigned int& idx, const void* data);
        void writeToKey(const std::string& key, const void* data);
        void GroupReads(const unsigned int& group_idx);
        void GroupWrites(const unsigned int& group_idx);
        unsigned int getParamBufferSize(const unsigned int& idx);
        unsigned int getParamDataType(const unsigned int& idx);
        std::vector<std::byte>& UserRead(const unsigned int& idx);
        void UserWrite(const unsigned int& idx, std::vector<std::byte>& buf);
        std::unordered_map<std::string,unsigned int>& getKeyIndexMap();
        void destroy();

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
                if (te == nullptr)
                    throw std::runtime_error("No parameter with given key: " + key + " index: " + std::to_string(n));
                std::cout << "key: " << key << " NumElements: " << te->nele << " index: " << n << std::endl; 
                // check whether the types are matching.
                if (te->dtype != typeid(T)) throw std::runtime_error("type mismatch: AddTableEntry(), key: " + key);
                if (te->nele != N) throw std::runtime_error("size mismatch of key: " + key);
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
                // std::unique_ptr<TableEntry<T,N>> te = std::make_unique<TableEntry<T,N>>();
                TableEntry<T,N>* te = new TableEntry<T,N>();
                te->data_buffer.write(value);
                te->nele = N;
                std::cout << "New key: " << key << " NumElements: " << te->nele << " index: " << n << std::endl; 
                te->index = n;
                // determine whether it read or write.
                if (mode == 1)
                    te->registered_writers++;
                else if (mode == 0)
                    te->registered_readers++;
                // push the unique pointer into table container.
                // _table.push_back(std::move(te));
                _table.push_back(te);
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
                if (te == nullptr)
                    throw std::runtime_error("No parameter with given key: " + key + " index: " + std::to_string(n));
                std::cout << "key: P " << key << " NumElements: " << te->nele << " index: " << n << std::endl; 

                // check whether the types are matching.
                if (te->dtype != typeid(T)) throw std::runtime_error("type mismatch: AddTableEntry(), key: " + key);
                if (te->nele != N) throw std::runtime_error("size mismatch of key: " + key);
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
                // std::unique_ptr<TableEntry<T,N>> te = std::make_unique<TableEntry<T,N>>();
                TableEntry<T,N>* te = new TableEntry<T,N>();
                // write the current value to param server.
                te->data_buffer.write(*value);
                te->nele = N;
                std::cout << "New key: P " << key << " NumElements: " << te->nele << " index: " << n << std::endl; 
                te->index = n;
                // determine whether it read or write.
                if (mode == 1)
                    te->registered_writers++;
                else if (mode == 0)
                    te->registered_readers++;
                // push the unique pointer into table container.
                // _table.push_back(std::move(te));
                _table.push_back(te);
                // add the key to unordered_map along with the index.
                key_table_index_map[key] = n;
            }
            return n;
        }



        template <typename T, unsigned int N = 1>
        unsigned int addTableEntry(const std::string& key, T& value, const unsigned int& mode,
                                    const unsigned int& param_type, const int& group_id = -1)
        {
            int n;
            if (key_table_index_map.contains(key))
            {
                n = key_table_index_map.at(key);
                auto& te = _table[n];
                if (te == nullptr)
                    throw std::runtime_error("No parameter with given key: " + key + " index: " + std::to_string(n));
                // check whether the types are matching.
                if (te->dtype != typeid(T)) throw std::runtime_error("type mismatch: AddTableEntry(), key: " + key);
                if (te->nele != N) throw std::runtime_error("size mismatch of key: " + key);
                if(mode == 1)
                {
                    if (te->registered_writers > 0) 
                        throw std::runtime_error("cannot have two writers to a parameter, key: " + key);
                } 
                
                 // determine whether it read or write.
                if (group_id >= group_list.size()) throw std::runtime_error("group id out of range.");
                if (mode == 1)
                {
                    group_list[group_id].insert_for_write(n,static_cast<void*>(&value));
                }
                else if (mode == 0)
                {
                   group_list[group_id].insert_for_read(n,static_cast<void*>(&value));
                }
            } 
            else
            {    
                // get the current size of table.
                n = _table.size();
                // create a unique pointer for a new table row.
                // std::unique_ptr<TableEntry<T,N>> te = std::make_unique<TableEntry<T,N>>();
                TableEntry<T,N>* te = new TableEntry<T,N>();
                te->data_buffer.write(value);
                te->nele = N;
                te->param_type = param_type;
                // determine whether it read or write.
                if (group_id >= group_list.size()) throw std::runtime_error("group id out of range");
                if (mode == 1)
                {
                    te->registered_writers++;
                    group_list[group_id].insert_for_write(n,static_cast<void*>(&value));
                }
                else if(mode == 0)
                {
                   group_list[group_id].insert_for_read(n,static_cast<void*>(&value));
                }
                
                // push the unique pointer into table container.
                // _table.push_back(std::move(te));
                _table.push_back(te);
                key_table_index_map[key] = n;
            }
            
            return n;
        }

        template <typename T, unsigned int N = 1>
        unsigned int addTableEntry(const std::string& key, T* value, const unsigned int& mode,
                                    const unsigned int& param_type, const int& group_id = -1)
        {
            int n;
            if (key_table_index_map.contains(key))
            {
                n = key_table_index_map.at(key);
                auto& te = _table[n];
                if (te == nullptr)
                    throw std::runtime_error("No parameter with given key: " + key + " index: " + std::to_string(n));
                // check whether the types are matching.
                if (te->dtype != typeid(T)) throw std::runtime_error("type mismatch: AddTableEntry(), key: " + key);
                if (te->nele != N) throw std::runtime_error("size mismatch of key: " + key);
                if(mode == 1)
                {
                    if (te->registered_writers > 0) 
                        throw std::runtime_error("cannot have two writers to a parameter, key: " + key);
                } 
                
                 // determine whether it read or write.
                if (group_id >= group_list.size()) throw std::runtime_error("group id out of range.");
                if (mode == 1)
                {
                    group_list[group_id].insert_for_write(n,static_cast<void*>(value));
                }
                else if (mode == 0)
                {
                   group_list[group_id].insert_for_read(n,static_cast<void*>(value));
                }
            } 
            else
            {    
                // get the current size of table.
                n = _table.size();
                // create a unique pointer for a new table row.
                // std::unique_ptr<TableEntry<T,N>> te = std::make_unique<TableEntry<T,N>>();
                TableEntry<T,N>* te = new TableEntry<T,N>();
                te->data_buffer.write(value);
                te->nele = N;
                te->param_type = param_type;
                // determine whether it read or write.
                if (group_id >= group_list.size()) throw std::runtime_error("group id out of range");
                if (mode == 1)
                {
                    te->registered_writers++;
                    group_list[group_id].insert_for_write(n,value);
                }
                else if(mode == 0)
                {
                   group_list[group_id].insert_for_read(n,value);
                }
                
                // push the unique pointer into table container.
                // _table.push_back(std::move(te));
                _table.push_back(te);
                key_table_index_map[key] = n;
            }
            
            return n;
        }

    };
       
} // namespace ps



#endif // _PS_TABLE_HPP