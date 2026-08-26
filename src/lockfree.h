// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_LOCKFREE_H_8C707AEB7C7235A2FBC5D4EDDF03B008
#define FS_LOCKFREE_H_8C707AEB7C7235A2FBC5D4EDDF03B008

#include <mutex>
#include <vector>
#include <new>

template <size_t TSize, size_t Capacity>
struct LockfreeFreeList
{
    class FreeList
    {
    public:
        bool pop(void*& p)
        {
            std::lock_guard lock(mutex_);

            if (storage_.empty()) {
                return false;
            }

            p = storage_.back();
            storage_.pop_back();
            return true;
        }

        bool bounded_push(void* p)
        {
            std::lock_guard lock(mutex_);

            if (storage_.size() >= Capacity) {
                return false;
            }

            storage_.push_back(p);
            return true;
        }

    private:
        std::vector<void*> storage_;
        std::mutex mutex_;
    };

    static FreeList& get()
    {
        static FreeList freeList;
        return freeList;
    }
};

template <typename T, size_t Capacity>
class LockfreePoolingAllocator
{
public:
    template <class U>
    struct rebind
    {
        using other = LockfreePoolingAllocator<U, Capacity>;
    };

    LockfreePoolingAllocator() = default;

    template <typename U>
    explicit constexpr LockfreePoolingAllocator(
        const LockfreePoolingAllocator<U, Capacity>&)
    {
    }

    using value_type = T;

    T* allocate(size_t) const
    {
        auto& inst = LockfreeFreeList<sizeof(T), Capacity>::get();

        void* p;

        if (!inst.pop(p)) {
            p = ::operator new(sizeof(T));
        }

        return static_cast<T*>(p);
    }

    void deallocate(T* p, size_t) const
    {
        auto& inst = LockfreeFreeList<sizeof(T), Capacity>::get();

        if (!inst.bounded_push(p)) {
            ::operator delete(p);
        }
    }
};

#endif
