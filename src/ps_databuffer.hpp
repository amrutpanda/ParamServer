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
    template <typename T>
    struct Slot
    {
        using D = std::remove_cv_t<std::remove_reference_t<T>>;
        // using D = std::remove_cv_t<std::remove_reference_t<std::remove_pointer_t<std::decay_t<T>>>>;
        // using DataType = std::conditional_t<std::is_array_v<D>, 
        //                             std::remove_extent_t<D>[std::extent_v<D>], D>;
        static_assert(std::is_trivially_copyable_v<D>,"Slot requires trivially copyable type");

        std::type_index BaseType;
        alignas(64) std::atomic<bool> is_writing{};
        alignas(64) std::atomic<uint64_t> seq_s{};
        alignas(64) std::atomic<uint64_t> seq_e{};
        static constexpr unsigned int s = std::is_array_v<D> ? std::extent_v<D> : 1;
        unsigned int size_ = 1;
        alignas(64) D data{};

        constexpr unsigned int getElemSize() const {return s;};
        constexpr unsigned int getSize() const {return sizeof(data);};

        Slot() : BaseType(typeid(std::remove_extent_t<D>)) {};
    
        void read(D& out) noexcept
        {
            memcpy( &out, &data, sizeof(D));
        };

        void read(D* out) noexcept
        {
            memcpy( out, &data, sizeof(D));
        };

        void read(void* out_buffer) noexcept
        {
            memcpy(out_buffer, &data ,sizeof(D));
        };

        void write(const D& in) noexcept
        {
            memcpy(&data, &in ,sizeof(D));
        };

        void write(const D* in) noexcept
        {
            memcpy(&data, in, sizeof(D));
        };

        void write(const void* in_buffer) noexcept
        {
            memcpy(&data, in_buffer, sizeof(D) );
        };

        template <typename U>
        void read(U& out, int offset)
        {
            if constexpr (!std::is_array_v<D>) return;
            if (offset >= s) return;
            memcpy(&out , data+offset, sizeof(U));
        };

        void read(void* out, int offset)
        {
            if constexpr (!std::is_array_v<D>) return;
            if (offset >= s) return;
            memcpy(out, data+offset, sizeof(std::remove_extent_t<D>));
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


    template <typename T, unsigned int NUM_ELEMENTS = 1 ,unsigned int BUFFER_SIZE = 16>
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
        alignas(64) Slot<T> data_[BUFFER_SIZE];
    protected:
        
    public:
        SpmcBuffer() {};
        ~SpmcBuffer() {};

        constexpr unsigned int size() const {return BUFFER_SIZE;};

        void write(const T& data)
        {
            unsigned int p = pos.fetch_add(1,std::memory_order_relaxed) + 1;
            Slot<T>& buf = data_[p & BUFFER_MASK];
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
            Slot<T>& buf = data_[p & BUFFER_MASK];
            buf.is_writing.store(true, std::memory_order_release);
            buf.seq_s.fetch_add(1, std::memory_order_release);
            buf.write(data);
            buf.seq_e.fetch_add(1, std::memory_order_release);
            buf.is_writing.store(false, std::memory_order_release);
            latest_pos.store(p, std::memory_order_release);
        }

        void write(const void* data, unsigned int& offset)
        {
            if constexpr(std::is_array_v<T>)
                throw std::runtime_error("While writing spmc buffer, offset is only valid for array types.");
            unsigned int p = pos.fetch_add(1,std::memory_order_relaxed) + 1;
            Slot<T>& buf = data_[p & BUFFER_MASK];
            buf.is_writing.store(true, std::memory_order_release);
            buf.seq_s.fetch_add(1, std::memory_order_release);
            buf.write(data, offset);
            buf.seq_e.fetch_add(1, std::memory_order_release);
            buf.is_writing.store(false, std::memory_order_release);
            latest_pos.store(p, std::memory_order_release);
        }

        void read(T& data)
        {
            T tmp = data;
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
                    buf.read(data);  
                    uint64_t s2 = buf.seq_e.load(std::memory_order_acquire); 
                    if (s1 == s2) return;
                    // ++offset;
                }
                count++;
            } 
            data = tmp; 
        }

        void read(void* data)
        {
            T tmp;
            memcpy(&tmp,data,sizeof(T));
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
                    buf.read(data);  
                    uint64_t s2 = buf.seq_e.load(std::memory_order_acquire); 
                    if (s1 == s2) return;
                    // ++offset;
                }
                count++;
            } 
            // data = tmp; 
            memcpy(data,&tmp, sizeof(T));
        }

        void read(void* data, unsigned int offset)
        {
            if constexpr(std::is_array_v<T>)
                throw std::runtime_error("While reading spmc buffer, offset is only valid for array types.");
            T tmp;
            memcpy(&tmp,data+offset,sizeof(T));
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
                    buf.read(data,offset);  
                    uint64_t s2 = buf.seq_e.load(std::memory_order_acquire); 
                    if (s1 == s2) return;
                    // ++offset;
                }
                count++;
            } 
            data = tmp; 
            memcpy(data,&tmp+offset,sizeof(T));
        }        
    };
    
    
} // namespace paramserver



#endif // PS_DATABUFFER_HPP