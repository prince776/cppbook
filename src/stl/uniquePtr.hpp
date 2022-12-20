#pragma once
#include <concepts>
#include <type_traits>
#include <utility>

template <std::regular T>
class UniquePtr {
   public:
    UniquePtr() = default;
    explicit UniquePtr(T* ptr) : ptr(ptr) {}

    ~UniquePtr() { reset(); }

    // Move operations
    UniquePtr(UniquePtr&& rhs) : ptr(rhs.release()) {}
    UniquePtr& operator=(UniquePtr&& rhs) {
        reset(rhs.release());
        return *this;
    }

    T* release() noexcept {
        auto oldPtr = ptr;
        ptr = nullptr;
        return oldPtr;
    }

    void reset(T* newPtr = nullptr) noexcept {
        auto oldPtr = ptr;
        ptr = newPtr;
        if (oldPtr) {
            delete oldPtr;
        }
    }

    void swap(UniquePtr& other) { std::swap(ptr, other.ptr); }

    // Operator overloads
    explicit operator bool() const noexcept { return ptr != nullptr; }
    T* operator->() const noexcept { return get(); }
    T& operator*() const noexcept {
        assert(ptr != nullptr);
        return *get();
    }

    T* get() const noexcept { return ptr; }

   private:
    T* ptr = nullptr;
};

template <std::regular T, typename... Args>
UniquePtr<T> makeUnique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}