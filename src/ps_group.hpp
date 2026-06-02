#ifndef PS_GROUP_HPP
#define PS_GROUP_HPP

#include <iostream>
#include <vector>

namespace ps
{
    struct Group
    {
        std::string name;
        unsigned int id;
        std::vector<unsigned int> read_indices;
        std::vector<unsigned int> write_indices;
        std::vector<void*> read_data_ptrs;
        std::vector<void*> write_data_ptrs;

        Group(const std::string& _name, const unsigned int& _id)
        {
            name = _name;
            id = _id;
        }

        bool IfNameMatches(const std::string& group_name)
        {
            if (name == group_name) return true;
            return false;
        }

        void insert_for_read(const unsigned int& idx, void* ptr)
        {
            read_indices.push_back(idx);
            read_data_ptrs.push_back(ptr);
        }
        void insert_for_write(const unsigned int& idx, void* ptr)
        {
            write_indices.push_back(idx);
            write_data_ptrs.push_back(ptr);
        }


    };
    
} // namespace ps


#endif // PS_GROUP_HPP