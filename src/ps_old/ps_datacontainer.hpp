#ifndef PS_DATACONTAINER_HPP_
#define PS_DATACONTAINER_HPP_

#include <cstddef>
#include <cstdint>
#include <ctime>

class DataContainer
{
public:
    enum class Type
    {
        NONE,
        INPUT_BUFFER,
        OUTPUT_BUFFER,
        PARAMETER_FIXED
    };

    DataContainer();

    DataContainer(Type& type, const void* data_ptr, size_t offset_bytes, size_t length_bytes);

    void setDataPointer(Type& type, const void* data_ptr, size_t offset_bytes, size_t length_bytes);

    virtual ~DataContainer();

    bool allocate(Type type, size_t size);

    void deallocate();

    // delete the copy constructor.
    DataContainer(const DataContainer& orig) = delete;

    size_t getMaxsize() const {return offset_bytes_.max_size;};
    
    size_t getSize() const {return offset_bytes_.size;};

    size_t getOffset() const {return offset_bytes_.offset;};

private:
    Type type_{Type::NONE};
    bool allocated_{};
    
    struct 
    {
        size_t offset;
        size_t size;
        size_t max_size;
    } offset_bytes_;

    uint8_t* data_ptr{};
    timespec timestamp_{};
};


#endif // PS_DATACONTAINER_HPP_