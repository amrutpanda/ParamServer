#include <ps_paramserver.hpp>

namespace ps
{
    void ParamServer::ReadParameterByIndex(const unsigned int& idx, void* data)
    {
        readByIndex(idx, data);
    }  

    void ParamServer::WriteParameterByIndex(const unsigned int& idx, const void* data)
    {
        writeByIndex(idx,data);
    }

    void ParamServer::ReadParameterFromKey(const std::string& key, void* data)
    {
        readFromKey(key, data);
    }

    void ParamServer::WriteParameterToKey(const std::string& key, const void* data)
    {
        writeToKey(key, data);
    }

    void ParamServer::UpdateParameters(const unsigned int& group_id)
    {
        GroupWrites(group_id);
    }

    void ParamServer::FetchParameters(const unsigned int& group_id)
    {
        GroupReads(group_id);
    }

    bool ParamServer::load(const std::string& filepath)
    {

        return true;
    }

    bool ParamServer::save(const std::string& filepath)
    {
        // for(const auto& [key,idx] : key_table_index_map)
        // {

        // }
        return true;
    }

} // namespace ps
