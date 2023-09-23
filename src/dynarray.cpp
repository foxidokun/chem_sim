#include "dynarray.h"

#include <cassert>

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

    T* new_data = malloc(sizeof(T));

}