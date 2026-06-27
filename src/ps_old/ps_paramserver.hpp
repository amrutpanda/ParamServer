// #ifndef PS_PARAM_SERVER_HPP
// #define PS_PARAM_SERVER_HPP

// #include <ps_table.hpp>
// #include <pugixml.hpp>
// #include <concepts>

// namespace ps
// {
    
//     class ParamServer final : public ParameterTable 
//     {
//     private:
//         std::vector<std::byte> general_msg_queue;
//         std::vector<std::byte> emergency_msg_queue;
//         std::vector<std::byte> log_msg_queue;
//     public:
//         ParamServer(const unsigned int& table_size = 1000, 
//                     const unsigned int& user_buffer_size = 100): 
//                     ParameterTable(table_size,user_buffer_size) {};
//         ~ParamServer() {};

//         void UpdateParameters(const unsigned int& group_id);
//         void FetchParameters(const unsigned int& group_id);

//         void ReadParameterByIndex(const unsigned int& idx, void* data);
//         void WriteParameterByIndex(const unsigned int& idx, const void* data);

//         void ReadParameterFromKey(const std::string& idx, void* data);
//         void WriteParameterToKey(const std::string& idx, const void* data);

//         bool load(const std::string& filepath);
//         bool save(const std::string& filepath);

//         void link(const std::string& filepath) {};
//         void link_params(const std::string& src_key, const std::string& dst_key);

//         template <typename T>
//         unsigned int AddParameter(const std::string& key, T& variable, const unsigned int& mode = 0,
//                                const unsigned int& param_type = 0, const int& group_id = -1);

//         /* *
//         * This function add basic data type parameters to parameter server.
//         * */                         
//         // template<typename T>
//         // requires std::derived_from<T, Eigen::EigenBase<T>> || 
//         //         std::is_same_v<T, std::array<typename T::value_type,std::tuple_size_v<T>>>
//         // unsigned int AddParameterVec(const std::string& key, T& variable, const unsigned int& mode = 0,
//         //                             const unsigned int& numElements = 1, const unsigned int& param_type = 0,
//         //                             const int& group_id = -1);

//         // for std::array
//         template <typename T, std::size_t N>
//         unsigned int AddParameterVec(const std::string& key, std::array<T,N>& variable, const unsigned int& mode = 0,
//                                     const unsigned int& param_type = 0,
//                                     const int& group_id = -1);
//         // for Eigen vectors.
//         template <typename T>
//         requires (T::SizeAtCompileTime != Eigen::Dynamic)
//         unsigned int AddParameterVec(const std::string& key, T& variable, const unsigned int& mode = 0,
//                                     const unsigned int& param_type = 0,
//                                     const int& group_id = -1);

//         // for Eigen vector dynamic.
//         template <typename T, unsigned int N, typename Derived>
//         unsigned int AddParameterVec(const std::string& key, Eigen::MatrixBase<Derived>& Variable,
//                                      const unsigned int& mode = 0,
//                                      const unsigned int& param_type = 0,
//                                      const int& group_id = -1  );
        
 
//     };

//     template <typename T>
//     unsigned int ParamServer::AddParameter(const std::string& key, T& variable, const unsigned int& mode,
//                                const unsigned int& param_type, const int& group_id)
//     {
//         using D = std::remove_cv_t<std::remove_reference_t<std::remove_pointer_t<std::decay_t<T>>>>;
//         /// this is just for checking will implement with group id later.
//         // get group name from ID.
//         std::string group_name, modified_key;
//         unsigned int ret;
//         if (group_id != -1)
//         {
//             group_name = getGroupNameFromID(group_id);
//             modified_key = group_name + "/"+ key;
//             // register key and variable to the parameter table. 
//             ret = addTableEntry<D,1>(modified_key,variable,mode,param_type,group_id);
//         }
//         else
//         {
//             group_name = "/origin";
//             modified_key = group_name + "/" + key;
//             // register key and variable to the parameter table. 
//             ret = addTableEntry<D,1>(modified_key,variable,mode);
//         }      
//        // register key and variable to the parameter table. 
//         // auto ret = addTableEntry<D,1>(modified_key,variable,mode,param_type,group_id);
//         return ret;
//     }

//     // template <typename T>
//     // requires std::derived_from<T, Eigen::EigenBase<T>> || 
//     //             std::is_same_v<T, std::array<typename T::value_type,std::tuple_size_v<T>>>
//     // unsigned int ParamServer::AddParameterVec(const std::string& key, T& variable, const unsigned int& mode,
//     //                              const unsigned int& numElements, const unsigned int& param_type,
//     //                              const int& group_id)
//     // {
//     //     // check whether the object has .size() and .data() method.
//     //     // if constexpr(!(requires{variable.size();}))
//     //     //     static_assert(false,"Type must provide size() method.");
//     //     // if constexpr(!(requires{variable.data();}))
//     //     //     static_assert(false,"Type must provide data() method.");

//     //     using D = std::remove_cv_t<std::remove_reference_t<std::remove_pointer_t<std::decay_t<T>>>>;
//     //     constexpr unsigned int N = numElements;
//     //     // check for size
//     //     if (variable.size() == 0 && N == 1) // for eigen dynamic vector or matrix.
//     //         throw std::runtime_error("Looks like Eigen matrix or vector has not been resized");
//     //     // get group name from ID.
//     //     std::string group_name, modified_key;
//     //     unsigned int ret;
//     //     if (group_id != -1)
//     //     {
//     //         group_name = getGroupNameFromID(group_id);
//     //         modified_key = group_name + "/"+ key;
//     //         // register key and variable to the parameter table. 
//     //         ret = addTableEntry<D,N>(modified_key,variable.data(),mode,param_type,group_id);
//     //     }
//     //     else
//     //     {
//     //         group_name = "/origin";
//     //         modified_key = group_name + "/" + key;
//     //         // register key and variable to the parameter table. 
//     //         // ret = addTableEntry<D,N>(modified_key,variable.data(),mode,param_type);
//     //     } 
//     //    // register key and variable to the parameter table.
//     //     // ret = addTableEntry<D,N>(modified_key,variable.data(),mode,param_type,group_id);
//     //     return ret;
//     // }

//     template <typename T, std::size_t N>
//     unsigned int ParamServer::AddParameterVec(const std::string& key, std::array<T,N>& variable, 
//                                                 const unsigned int& mode ,const unsigned int& param_type,
//                                                 const int& group_id)
//     {
//         // using D = std::remove_cv_t<std::remove_reference_t<std::remove_pointer_t<std::decay_t<T>>>>;
//         using D = T;
//         // get group name from ID.
//         std::string group_name, modified_key;
//         unsigned int ret;
//         if (group_id != -1)
//         {
//             group_name = this->getGroupNameFromID(group_id);
//             modified_key = group_name + "/"+ key;
//             // register key and variable to the parameter table. 
//             ret = addTableEntry<D,N>(modified_key,variable.data(),mode,param_type,group_id);
//         }
//         else
//         {
//             group_name = "/origin";
//             modified_key = group_name + "/" + key;
//             // register key and variable to the parameter table. 
//             ret = addTableEntry<D,N>(modified_key,variable.data(),mode,param_type);
//         } 
//         return ret;
//     }

//     template <typename T>
//     requires (T::SizeAtCompileTime != Eigen::Dynamic)
//     unsigned int ParamServer::AddParameterVec(const std::string& key, T& variable, const unsigned int& mode,
//                                     const unsigned int& param_type, const int& group_id )
//     {
//         using D = typename T::Scalar;
//         constexpr unsigned int N = T::SizeAtCompileTime;
//         // get group name from ID.
//         std::string group_name, modified_key;
//         unsigned int ret;
//         if (group_id != -1)
//         {
//             group_name = this->getGroupNameFromID(group_id);
//             modified_key = group_name + "/"+ key;
//             // register key and variable to the parameter table. 
//             ret = addTableEntry<D,N>(modified_key,variable.data(),mode,param_type,group_id);
//         }
//         else
//         {
//             group_name = "/origin";
//             modified_key = group_name + "/" + key;
//             // register key and variable to the parameter table. 
//             ret = addTableEntry<D,N>(modified_key,variable.data(),mode,param_type);
//         } 
//         return ret;
//     }

//     template <typename T, unsigned int N, typename Derived>
//     unsigned int ParamServer::AddParameterVec(const std::string& key, Eigen::MatrixBase<Derived>& variable,
//                                      const unsigned int& mode,
//                                      const unsigned int& param_type,
//                                      const int& group_id  )
//     {
//         using D = typename Derived::Scalar;
//         // static_assert(T == D, "Type mismatch");
//          // get group name from ID.
//         std::string group_name, modified_key;
//         unsigned int ret;
//         if (group_id != -1)
//         {
//             group_name = this->getGroupNameFromID(group_id);
//             modified_key = group_name + "/"+ key;
//             // register key and variable to the parameter table. 
//             ret = addTableEntry<D,N>(modified_key,variable.derived().data(),mode,param_type,group_id);
//         }
//         else
//         {
//             group_name = "/origin";
//             modified_key = group_name + "/" + key;
//             // register key and variable to the parameter table. 
//             ret = addTableEntry<D,N>(modified_key,variable.derived().data(),mode,param_type);
//         } 
//         return ret;
//     }
    
// } // namespace ps


// #endif