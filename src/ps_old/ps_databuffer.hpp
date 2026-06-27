#ifndef PS_DATABUFFER_HPP
#define PS_DATABUFFER_HPP

#include <iostream>
#include <cstring>
#include <atomic>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace ps
{   
    template <typename T, int NUM_ELEMENTS = 1>
    struct Slot
    {
        // using D = std::remove_cv_t<std::remove_reference_t<T>>;
        using D = std::remove_cv_t<std::remove_reference_t<std::remove_pointer_t<std::decay_t<T>>>>;
        static_assert(std::is_trivially_copyable_v<D>,"Slot requires trivially copyable type");
        static_assert(!std::is_pointer_v<D>, "Double pointers not allowed");

        std::type_index BaseType;
        alignas(64) std::atomic<bool> is_writing{};
        alignas(64) std::atomic<uint64_t> seq_s{};
        alignas(64) std::atomic<uint64_t> seq_e{};
        static constexpr unsigned int s = std::is_array_v<D> ? std::extent_v<D> : 1;
        static constexpr unsigned int N = NUM_ELEMENTS;
        alignas(64) D data[N]{};
        // std::vector<D> data_vec{N};

        if constexpr (NUM_ELEMENTS == -1)
        {
            alignas(64) D data_[N] {};
        }
        else
        {
            D* data_;
        }
        
        constexpr unsigned int getElemSize() const {return N;};
        constexpr unsigned int getSize() const {return sizeof(data);};

        Slot() : BaseType(typeid(std::remove_extent_t<D>)) {};

        void resize(unsigned int size)
        {
            if constexpr ( NUM_ELEMENTS == -1)
                data_ = new D[size];
        }
    
        void read(D& out) noexcept
        {
            memcpy( &out, data, N*sizeof(D));
        };

        void read(D* out) noexcept
        {
            memcpy( out, data, N*sizeof(D));
        };

        void read(void* out_buffer) noexcept
        {
            memcpy(out_buffer, data ,N*sizeof(D));
        };

        void write(const D& in) noexcept
        {
            memcpy(data, &in ,N*sizeof(D));
        };

        void write(const D* in) noexcept
        {
            memcpy(data, in, N*sizeof(D));
        };

        void write(const void* in_buffer) noexcept
        {
            memcpy(data, in_buffer, N*sizeof(D) );
        };

        void read(D& out, int offset)
        {
            if (offset >= N) return;
            memcpy(&out , data+offset, sizeof(D));
        };

        void read(void* out, int offset)
        {
            if (offset >= N) return;
            memcpy(out, data+offset, sizeof(D));
        }

        // template<typename U>
        // void write(const U& in, int offset)
        // {
        //     if (offset >= s) return;
        //     memcpy(data+offset, &in, sizeof(U));
        // }

        // template<typename U>
        // void write(const U& in, int offset, int size)
        // {
        //     if (offset >= s) return;
        //     if (size > s - offset) return;
        //     memcpy(data+offset, U& in , size * sizeof(U));
        // }

    };


    template <typename T, int NUM_ELEMENTS = 1 ,unsigned int BUFFER_SIZE = 16>
    class SpmcBuffer
    {
    private:
        static_assert(BUFFER_SIZE >= 2, "BUFFER_SIZE must be >= 2");
        static_assert((BUFFER_SIZE & (BUFFER_SIZE - 1)) == 0, "BUFFER_SIZE must be a power of 2");
        constexpr static unsigned int BUFFER_MASK = BUFFER_SIZE - 1;
        constexpr static unsigned int N = NUM_ELEMENTS;
        unsigned int MAX_RETRIES = 1024;
        alignas(64) std::atomic<unsigned int> latest_pos{};
        alignas(64) std::atomic<unsigned int> pos{};
        alignas(64) Slot<T,N> data_[BUFFER_SIZE];
    protected:
        
    public:
        SpmcBuffer() {};
        ~SpmcBuffer() {};

        constexpr unsigned int size() const {return BUFFER_SIZE;};

        void write(const T& data)
        {
            unsigned int p = pos.fetch_add(1,std::memory_order_relaxed) + 1;
            Slot<T,N>& buf = data_[p & BUFFER_MASK];
            buf.is_writing.store(true, std::memory_order_release);
            buf.seq_s.fetch_add(1, std::memory_order_release);
            buf.write(data);
            buf.seq_e.fetch_add(1, std::memory_order_release);
            buf.is_writing.store(false, std::memory_order_release);
            latest_pos.store(p, std::memory_order_release);
        }

        void write(const void* data)
        {
            unsigned int p = pos.fetch_add(1,std::memory_order_relaxed) + 1;
            Slot<T,N>& buf = data_[p & BUFFER_MASK];
            buf.is_writing.store(true, std::memory_order_release);
            buf.seq_s.fetch_add(1, std::memory_order_release);
            buf.write(data);
            buf.seq_e.fetch_add(1, std::memory_order_release);
            buf.is_writing.store(false, std::memory_order_release);
            latest_pos.store(p, std::memory_order_release);
        }

        void write(const void* data_buf, unsigned int& offset)
        {
            if constexpr(std::is_array_v<T>)
                throw std::runtime_error("While writing spmc buffer, offset is only valid for array types.");
            unsigned int p = pos.fetch_add(1,std::memory_order_relaxed) + 1;
            Slot<T,N>& buf = data_[p & BUFFER_MASK];
            buf.is_writing.store(true, std::memory_order_release);
            buf.seq_s.fetch_add(1, std::memory_order_release);
            buf.write(data_buf, offset);
            buf.seq_e.fetch_add(1, std::memory_order_release);
            buf.is_writing.store(false, std::memory_order_release);
            latest_pos.store(p, std::memory_order_release);
        }

        void read(T& data_buf)
        {
            T tmp = data_buf;
            int count = 0;
            // unsigned int offset = 0;
            while (count < MAX_RETRIES)
            {   
                unsigned int lp = latest_pos.load(std::memory_order_acquire);
                // if (offset > static_cast<int>(lp)) break;
                // unsigned int lp = pos.load(std::memory_order_acquire);
                Slot<T,N>& buf = data_[lp & BUFFER_MASK];
                uint64_t s1 = buf.seq_s.load(std::memory_order_acquire);
                if (!(buf.is_writing.load(std::memory_order_acquire)))
                {
                    // uint64_t s1 = buf.seq_s.load(std::memory_order_acquire);
                    buf.read(data_buf);  
                    uint64_t s2 = buf.seq_e.load(std::memory_order_acquire); 
                    if (s1 == s2) return;
                    // ++offset;
                }
                count++;
            } 
            data_buf = tmp; 
        }

        void read(void* data_buf)
        {
            T tmp;
            memcpy(&tmp,data_buf,sizeof(T));
            int count = 0;
            // unsigned int offset = 0;
            while (count < MAX_RETRIES)
            {   
                unsigned int lp = latest_pos.load(std::memory_order_acquire);
                // if (offset > static_cast<int>(lp)) break;
                // unsigned int lp = pos.load(std::memory_order_acquire);
                Slot<T,N>& buf = data_[lp & BUFFER_MASK];
                uint64_t s1 = buf.seq_s.load(std::memory_order_acquire);
                if (!(buf.is_writing.load(std::memory_order_acquire)))
                {
                    // uint64_t s1 = buf.seq_s.load(std::memory_order_acquire);
                    buf.read(data_buf);  
                    uint64_t s2 = buf.seq_e.load(std::memory_order_acquire); 
                    if (s1 == s2) return;
                    // ++offset;
                }
                count++;
            } 
            // data = tmp; 
            memcpy(data_buf,&tmp, sizeof(T));
        }

        void read(void* data_buf, unsigned int offset)
        {
            if constexpr(std::is_array_v<T>)
                throw std::runtime_error("While reading spmc buffer, offset is only valid for array types.");
            T tmp;
            memcpy(&tmp,data_buf+offset,sizeof(T));
            int count = 0;
            // unsigned int offset = 0;
            while (count < MAX_RETRIES)
            {   
                unsigned int lp = latest_pos.load(std::memory_order_acquire);
                // if (offset > static_cast<int>(lp)) break;
                // unsigned int lp = pos.load(std::memory_order_acquire);
                Slot<T>& buf = data_[lp & BUFFER_MASK];
                uint64_t s1 = buf.seq_s.load(std::memory_order_acquire);
                if (!(buf.is_writing.load(std::memory_order_acquire)))
                {
                    // uint64_t s1 = buf.seq_s.load(std::memory_order_acquire);
                    buf.read(data_buf,offset);  
                    uint64_t s2 = buf.seq_e.load(std::memory_order_acquire); 
                    if (s1 == s2) return;
                    // ++offset;
                }
                count++;
            } 
            data_buf = tmp; 
            memcpy(data_buf,&tmp+offset,sizeof(T));
        }        
    };
    


    
} // namespace paramserver



#endif // PS_DATABUFFER_HPP