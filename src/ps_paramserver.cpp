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
        // create a buffer to save values.
        int n;
        std::vector<std::byte> value_buffer;
        value_buffer.reserve(1000);
        // create the pugixml empty doc container.
        pugi::xml_document doc;
        // add declaration to it.
        pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
        decl.append_attribute("version") = "1.0";
        decl.append_attribute("encoding") = "UTF-8";
        // Create the root element node
        pugi::xml_node root = doc.append_child("Parameters");
        // loop over the complete parameter map and append values to xml doc.
        for(const auto& [key,idx] : key_table_index_map)
        {
            pugi::xml_node Param = root.append_child("Parameter");
            Param.append_attribute("Name") = key.c_str();
            // copy the data from param server to value buffer.
            // extract the table entry.
            auto& it = _table[idx];
            
            


            // if (it->dtype == typeid(int))

        }
        return true;
    }


    void ParamServer::link_params(const std::string& src_key, const std::string& dst_key)
    {
        unsigned int sidx;
        unsigned int didx;
        // if (key_table_index_map.contains(src_key))  sidx = _table[]
        // first check type match and size match
        
    }
} // namespace ps
