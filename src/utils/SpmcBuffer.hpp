#ifndef _SPMCBUFFER_HPP
#define _SPMCBUFFER_HPP

namespace ps
{
    template < typename T, unsigned int N = 16>
    class SpmcBuffer
    {
    protected:
        T data;
    public:
        SpmcBuffer(/* args */);
        ~SpmcBuffer();
    };
    
    
} // namespace ps


#endif // _SPMCBUFFER_HPP