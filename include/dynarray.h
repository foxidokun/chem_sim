#pragma once

#include <cstdlib>
#include <new>

template<typename T>
class dynarray {
private:
    T* _data;
    size_t _capacity;
    size_t _size;

    void resize(size_t new_size);
    void resize_if_needed();

public:
    dynarray() = default;
    
    explicit dynarray(size_t capacity):
        _data(reinterpret_cast<T*>(malloc(capacity * sizeof(T)))),
        _capacity(capacity),
        _size(0)
        {
            if (!_data) {
                throw std::bad_alloc();
            }
        }
    
    size_t capacity() { return _capacity; }
    size_t size()     { return _size; }

    void push_pack(T elem);
    void push_pack(T&& elem);

    T& operator[](size_t index){
        return &_data[index];
    }

    const T& operator[](size_t index) const {
        return &_data[index];
    }
};