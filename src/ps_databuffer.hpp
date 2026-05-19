#ifndef _PS_DATABUFFER_HPP
#define _PS_DATABUFFER_HPP

#include <iostream>
#include <cstring>
#include <atomic>
#include <type_traits>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace ps
{   
    template <typename T>
    struct Slot
    {
        using D = std::remove_cv_t<std::remove_reference_t<T>>;
        // using DataType = std::conditional_t<std::is_array_v<D>, 
        //                             std::remove_extent_t<D>[std::extent_v<D>], D>;
        
        static_assert(std::is_trivially_copyable_v<D>,"Slot requires trivially copyable type");

        std::atomic<bool> is_writing{};
        std::atomic<uint64_t> seq_s{};
        std::atomic<uint64_t> seq_e{};
        unsigned int s = std::extent_v<D>;
        alignas(64) D data;

        constexpr unsigned int getElemSize() const {return s;};
        constexpr unsigned int getSize() const {return sizeof(data);};

        void read(D& out) noexcept
        {
            memcpy( &out, &data, sizeof(D));
        };

        void write(const D& in) noexcept
        {
            memcpy(&data, &in ,sizeof(D));
        };

        template <typename U>
        void read(U& out, int offset)
        {
            if (offset >= s) return;
            memcpy(&out , data+offset, sizeof(U));
        }

        template<typename U>
        void write(const U& in, int offset)
        {
            if (offset >= s) return;
            memcpy(data+offset, &in, sizeof(U));
        }

        template<typename U>
        void write(const U& in, int offset, int size)
        {
            if (offset >= s) return;
            if (size > N - offset) return;
            memcpy(data+offset, U& in , size * sizeof(U));
        }

    };


    template <typename T, unsigned int NUM_ELEMENTS = 1 ,unsigned int BUFFER_SIZE = 16>
    class SpmcBuffer
    {
    private:
        constexpr static unsigned int BUFFER_MASK = BUFFER_SIZE - 1;
        constexpr static unsigned int N = NUM_ELEMENTS;
        unsigned int MAX_RETRIES = 1024;
        std::atomic<unsigned int> latest_pos{};
        std::atomic<unsigned int> pos{};
        Slot<T> data_[BUFFER_SIZE];
    protected:
        
    public:
        SpmcBuffer() {};
        ~SpmcBuffer() {};

        constexpr unsigned int size() const {return BUFFER_SIZE;};

        void write(const T& data)
        {
            ++pos;
            Slot<T> buf = data_[pos & BUFFER_MASK];
            buf.is_writing = true;
            ++buf.seq_s;
            buf.write(data);
            ++buf.seq_e;
            latest_pos = pos;
        }

        void read(T& data)
        {
            T tmp = data;
            int count = 0;
            int offset = 0;
            while (count < MAX_RETRIES)
            {   
                Slot<T> buf = data_[(latest_pos - offset) & BUFFER_MASK];
                if (!buf.is_writing)
                {
                    read(data);   
                    if (buf.seq_s == buf.seq_e) break;
                    ++offset;
                }
                count++;
            } 
            data = tmp; 
        }

        
    };
    
    
} // namespace paramserver



#endif // _PS_DATABUFFER_HPP