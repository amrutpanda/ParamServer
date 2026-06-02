#include <ps_table.hpp>

namespace ps
{
        unsigned int ParameterTable::createGroup(const std::string& name)
        {
            // get the group vector size.
            int n = group_list.size();
            // check if the group name already exists.
            for (auto& it : group_list)
            {
                if (it.IfNameMatches(name))
                    throw std::runtime_error("Group name already exists! name: " + name + " id: " + std::to_string(it.id));
            }
            // if everything is alright create a new group and save it.
            Group group(name,n);
            group_list.push_back(group);
            return n;
        }

        std::string ParameterTable::getGroupNameFromID(const unsigned int& id_)
        {
            for (auto& _gr : group_list)
            {
                if (_gr.id == id_) return _gr.name;
            }
            throw std::runtime_error("Could not find any group name from given id: " + std::to_string(id_));
        }

        void ParameterTable::readByIndex(const unsigned int& idx, void* data)
        {
            assert(( "readByIndex::invalid index",idx < _table.size()));
            _table[idx]->read_from_param_table(data);
        }


        void ParameterTable::readFromKey(const std::string& key, void* data)
        {
            if (key_table_index_map.contains(key))
            {
                int idx = key_table_index_map.at(key);
                _table[idx]->read_from_param_table(data);
            }
        }
        
        void ParameterTable::writeByIndex(const unsigned int& idx, const void* data)
        {
            assert(( "writeByIndex::invalid index",idx < _table.size()));
            _table[idx]->write_to_param_table(data);
        }

        void ParameterTable::writeToKey(const std::string& key, const void* data)
        {
            if (key_table_index_map.contains(key))
            {
                int idx = key_table_index_map.at(key);
                _table[idx]->write_to_param_table(data);
            }
        }

        void ParameterTable::GroupReads(const unsigned int& group_idx)
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

        void ParameterTable::GroupWrites(const unsigned int& group_idx)
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

        unsigned int ParameterTable::getParamBufferSize(const unsigned int& idx)
        {
            assert(( "getParamBufferSize::invalid index",idx < _table.size()));
            return _table[idx]->nele;   
        
        }

        unsigned int ParameterTable::getParamDataType(const unsigned int& idx)
        {
            assert(( "getParamDataType::invalid index",idx < _table.size()));
            auto& te = _table[idx];
            return 1;
        }

        std::vector<std::byte>& ParameterTable::UserRead(const unsigned int& idx)
        {
            assert(( "UserRead::invalid index",idx < _table.size()));
            if (_table[idx]->user_permission == 0)
                _table[idx]->read_from_param_table(user_buffer.data());
            return user_buffer;
        }

        void ParameterTable::UserWrite(const unsigned int& idx, std::vector<std::byte>& buf)
        {
            assert(( "UserWrite::invalid index",idx < _table.size()));
            if (_table[idx]->user_permission == 0)
                _table[idx]->write_to_param_table(buf.data());
        }

        std::unordered_map<std::string,unsigned int>& ParameterTable::getKeyIndexMap()
        {
            return key_table_index_map;
        }

        void ParameterTable::destroy()
        {
            _table.clear();
            group_list.clear();
            key_table_index_map.clear();
            
        }

} // namespace ps
