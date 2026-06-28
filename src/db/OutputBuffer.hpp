#ifndef _OUTPUTBUFFER_HPP
#define _OUTPUTBUFFER_HPP

#include <SpmcBuffer.hpp>
#include <DataContainer.hpp>
namespace ps
{
    class OutputBuffer : SpmcBuffer<DataContainer>
    {
    private:
        
    public:
        OutputBuffer() {data.setType(DataContainerType::OUTPUT);};
        ~OutputBuffer() {};

        bool allocate(int size);
        bool deallocate();
    };
    
    
} // namespace ps


#endif // _OUTPUTBUFFER_HPP