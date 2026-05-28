#ifndef PS_PARAM_SERVER_HPP
#define PS_PARAM_SERVER_HPP

#include <ps_table.hpp>

namespace ps
{
    class ParamServer final : public ParameterTable 
    {
    private:
        /* data */
    public:
        ParamServer(const unsigned int& table_size = 1000, 
                    const unsigned int& user_buffer_size = 100): 
                    ParameterTable(table_size,user_buffer_size) {};
        ~ParamServer();

        template <typename T>
        unsigned int AddParameter(const std::string& key, T& variable, const unsigned int& mode = 0,
                               const int& group_id = -1, const unsigned int& user_perm = 0);

        /* *
        * This function add basic data type parameters to parameter server.
        * */                         
        template<typename T>
        requires std::derived_from<T, Eigen::EigenBase<T>> || 
                std::is_same_v<T, std::array<typename T::value_type,std::tuple_size_v<T>>>
        unsigned int AddParameterVec(const std::string& key, T& variable, const unsigned int& mode = 0,
                                    const unsigned int& numElements = 1, const int& group_id = -1, 
                                    const unsigned int& user_perm = 0);
 
    };

    template <typename T>
    unsigned int ParamServer::AddParameter(const std::string& key, T& variable, const unsigned int& mode,
                               const int& group_id, const unsigned int& user_perm)
    {
        using D = std::remove_cv_t<std::remove_reference_t<std::remove_pointer_t<std::decay_t<T>>>>;
        /// this is just for checking will implement with group id later.
        auto ret = addTableEntry<D,1>(key,variable,mode);
        return ret;
    }

    template <typename T>
    requires std::derived_from<T, Eigen::EigenBase<T>> || 
                std::is_same_v<T, std::array<typename T::value_type,std::tuple_size_v<T>>>
    unsigned int ParamServer::AddParameterVec(const std::string& key, T& variable, const unsigned int& mode,
                                 const unsigned int& numElements, const int& group_id, 
                                 const unsigned int& user_perm)
    {
        using D = std::remove_cv_t<std::remove_reference_t<std::remove_pointer_t<std::decay_t<T>>>>;
        unsigned int N = numElements;
        auto ret = addTableEntry<D,N>(key,variable,mode);
        return ret;
    }
    
} // namespace ps


#endif