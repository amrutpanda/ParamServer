#ifndef _MPSCBUFFER_HPP
#define _MPSCBUFFER_HPP

#include <vector>

namespace ps
{
    template <typename T, unsigned int N = 16>
    class MpscBuffer
    {
    private:
        std::vector<T> data;
    public:
        MpscBuffer(/* args */);
        ~MpscBuffer();

        bool allocate(int size);
        bool deallocate();

        void writeto()
    };
    
} // namespace ps



#endif // _MPSCBUFFER_HPP