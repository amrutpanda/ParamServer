#ifndef DATACONTAINER_HPP_
#define DATACONTAINER_HPP_

#include <cstdint>
#include <enum.hpp>
#include <vector>

namespace ps
{

    struct CopyInfo
    {
        size_t src_offset;
        size_t src_length;
        size_t src_ele_size;
        size_t src_nele;

        size_t dst_offset;
        size_t dst_length;
        size_t dst_ele_size;
        size_t dst_nele;
    };

    class DataContainer
    {
    private:
        size_t offset;
        size_t length;
        size_t nele;
        size_t elesize;

        size_t Size;
        int8_t dataPointer;

        std::vector<uint8_t> buffer;

        DataContainerType type;
    public:
        DataContainer() {};

        DataContainer(const DataContainerType& _Type) {};

        // DataContainer(const DataContainerType _type, const unsigned int& elesize_, 
        //                                     const unsigned int& nele_ = 1) {};

        ~DataContainer() {};

        void allocate(const unsigned int& elesize_, const unsigned int& nele_ = 1);
        void deallocate();

        void setOffset(const unsigned int& _offset);
        void setLength(const unsigned int& _length);

        unsigned int getType() const;
        void setType(const DataContainerType& _type);

        void readfrom(const DataContainer& dnct, int len, int off = 0);
        void readfrom(const void* dptr);

        void writeto(DataContainer& dnct, int len, int off = 0);
        void writeto(void* dptr);
        
        void getUserData(ActionType action, void* data, int len, int off = 0);
        void setUserData(void* data, int len, int off = 0);

        void processUserInput();

    };
    
    
} // namespace ps


#endif // DATACONTAINER_HPP_