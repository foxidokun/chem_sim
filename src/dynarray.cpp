#include "dynarray.h"

#include <cassert>

#if !DISABLE_DYNARRAY_FOR_SAFETY

template<class T>
void dynarray<T>::resize_if_needed() {
    if (_size == _capacity) {
        resize(2 * _capacity);
    } else if (_size < _capacity / 2) {
        resize(_capacity / 2);
    }
}

template<class T>
void dynarray<T>::resize(size_t new_size) {
    assert (new_size >= _size);

    T* new_data = reinterpret_cast<T*>(malloc(new_size * sizeof(T)));

    for (size_t i = 0; i < _size; ++i) {
        new(_data + i) T(std::move(_data + i));
    }

    _capacity = new_size;
}

template<class T>
void dynarray<T>::push_back(T&& elem) {
    new(_data + _size) T(elem);
    _size++;
    resize_if_needed();
}

#endif