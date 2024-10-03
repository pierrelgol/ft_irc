/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoundedArray.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:07:31 by pollivie          #+#    #+#             */
/*   Updated: 2024/10/01 12:07:31 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "Optionnal.hpp"
#include "Types.hpp"

#include <cstring>
#include <stdexcept>

template <typename T, std::size_t N>
struct BoundedArray {
        T     _items[N];
        usize _len;
        usize _cap;

        typedef T                                     item;
        typedef const T                               const_item;
        typedef std::size_t                           size_type;
        typedef std::ptrdiff_t                        diff_type;
        typedef item&                                 reference_item;
        typedef const item&                           const_reference_item;
        typedef item*                                 pointer_item;
        typedef const item*                           const_pointer_item;
        typedef item*                                 iterator;
        typedef const item*                           const_iterator;
        typedef std::reverse_iterator<iterator>       reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        BoundedArray() : _len(0), _cap(N) {
                for (size_type i = 0; i < N; i++) {
                        _items[0] = 0;
                }
        }

        // returns a const T&;
        const_reference_item peek_front() const {
                if (_len == 0) throw std::out_of_range("panic: index 0 is out of bound when len is 0");
                return (_items[0]);
        }

        // returns a const T&;
        const_reference_item peek_back() const {
                if (_len == 0) throw std::out_of_range("panic: index 0 is out of bound when len is 0");
                return (_items[_len - 1]);
        }

        // returns a const T& or throw if index >= to index;
        const_reference_item peek_at(size_type index) const {
                if (index >= _len) throw std::out_of_range("panic: index out of bound");
                return (_items[index]);
        }

        // returns a T& or throw if index >= to index;
        reference_item operator[](size_type index) {
                if (index >= _len) throw std::out_of_range("panic: index out of bound");
                return (_items[index]);
        }

        // returns a const T& or throw if index >= to index;
        const_reference_item operator[](size_type index) const {
                if (index >= _len) throw std::out_of_range("panic: index out of bound");
                return (_items[index]);
        }

        // returns the total capacity
        size_type capacity() const {
                return (_cap);
        }

        // returns the current number of items
        size_type len() const {
                return (_len);
        }

        bool is_empty() const {
                return (_len == 0);
        }

        bool is_full() const {
                return (_len == _cap);
        }

        // insertion in place, will overwrite current value
        bool insert_front(reference_item item) {
                if (is_empty()) return (false);
                _items[0] = item;
        }

        // insertion in place, will overwrite current value
        bool insert_back(reference_item item) {
                if (is_empty()) return (false);
                _items[_len] = item;
        }

        // insertion in place, will overwrite current value
        bool insert_at(reference_item item, size_type index) {
                if (is_empty() or index >= _cap) return (false);
                _items[index] = item;
                return (true);
        }

        // remove value
        bool erase_front() {
                if (is_empty()) return (false);
                _items[0] = 0;
        }

        // remove value
        bool erase_back() {
                if (is_empty()) return (false);
                _items[_len] = 0;
        }

        // remove value
        bool erase_at(size_type index) {
                if (is_empty() or index >= _cap) return (false);
                _items[index] = 0;
        }

        // tries to push an item to the front, returns false if len == capacity
        bool push_front(reference_item item) {
                if (is_full()) return (false);
                std::memmove(_items + 1, _items, _len * sizeof(T));
                _items[0] = item;
                _len++;
                return (true);
        }

        // tries to push an item to the back, returns false if len == capacity
        bool push_back(reference_item item) {
                if (is_full()) return (false);
                _items[_len] = item;
                _len++;
                return (true);
        }

        // tries to push an item at index, returns false if len == capacity
        bool push_at(reference_item item, size_type index) {
                if (index > _len) throw std::out_of_range("Index out of bounds");
                if (is_full()) return (false);
                std::memmove(_items + index + 1, _items + index, (_len - index) * sizeof(T));
                _items[index] = item;
                _len++;
                return (true);
        }

        // returns an Optional<T> unsafe to unwrap()
        Optional<T> pop_front() {
                if (is_empty()) return (Optional<T>());
                Optional<T> result(_items[0]);
                std::memmove(_items, _items + 1, (_len - 1) * sizeof(T));
                _len -= 1;
                return (result);
        }

        // returns an Optional<T> unsafe to unwrap()
        Optional<T> pop_back() {
                if (is_empty()) return (Optional<T>());
                Optional<T> result(_items[_len - 1]);
                _len -= 1;
                return (result);
        }

        // returns an Optional<T> unsafe to unwrap()
        Optional<T> pop_at(size_type index) {
                if (is_empty() or index >= _len) return (Optional<T>());
                Optional<T> result(_items[index]);
                std::memmove(_items + index, _items + index + 1, (_len - index - 1) * sizeof(T));
                _len -= 1;
                return (result);
        }

        void clear() {
                for (size_type i = 0; i < len(); i++) {
                        _items[0] = 0;
                }
                _len = 0;
        }

        void fill(const_reference_item value) {
                std::fill(this->begin(), this->end(), value);
        }

        void swap(BoundedArray& other) {
                std::swap_ranges(this->begin(), this->end(), other.begin());
        }

        iterator begin() {
                return iterator(_items);
        }

        const_iterator begin() const {
                return iterator(_items);
        }

        iterator end() {
                return iterator(_items + len());
        }

        const_iterator end() const {
                return iterator(_items + len());
        }

        const_iterator cbegin() const {
                return iterator(_items);
        }

        const_iterator cend() const {
                return iterator(_items + len());
        }

        reverse_iterator rbegin() {
                return reverse_iterator(end());
        }

        reverse_iterator rend() {
                return reverse_iterator(begin());
        }

        const_reverse_iterator rbegin() const {
                return reverse_iterator(end());
        }

        const_reverse_iterator rend() const {
                return reverse_iterator(begin());
        }

        const_reverse_iterator crbegin() const {
                return reverse_iterator(end());
        }

        const_reverse_iterator crend() const {
                return reverse_iterator(begin());
        }

        pointer_item data() {
                return (_items);
        }

        const_pointer_item data() const {
                return (_items);
        }
};

template <typename T, std::size_t N>
bool operator==(const BoundedArray<T, N>& lhs, const BoundedArray<T, N>& rhs) {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, std::size_t N>
bool operator!=(const BoundedArray<T, N>& lhs, const BoundedArray<T, N>& rhs) {
        return !(lhs == rhs);
}

template <typename T, std::size_t N>
bool operator<(const BoundedArray<T, N>& lhs, const BoundedArray<T, N>& rhs) {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, std::size_t N>
bool operator<=(const BoundedArray<T, N>& lhs, const BoundedArray<T, N>& rhs) {
        return (lhs == rhs) || (lhs < rhs);
}

template <typename T, std::size_t N>
bool operator>(const BoundedArray<T, N>& lhs, const BoundedArray<T, N>& rhs) {
        return !(lhs <= rhs);
}

template <typename T, std::size_t N>
bool operator>=(const BoundedArray<T, N>& lhs, const BoundedArray<T, N>& rhs) {
        return !(lhs < rhs);
}

template <typename T, std::size_t N>
void swap(BoundedArray<T, N>& lhs, BoundedArray<T, N>& rhs) {
        lhs.swap(rhs);
}

// Overloads for the range functions
template <typename T, std::size_t N>
typename BoundedArray<T, N>::iterator begin(BoundedArray<T, N>& in) {
        return in.begin();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::const_iterator begin(const BoundedArray<T, N>& in) {
        return in.cbegin();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::const_iterator cbegin(const BoundedArray<T, N>& in) {
        return in.cbegin();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::iterator end(BoundedArray<T, N>& in) {
        return in.end();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::const_iterator end(const BoundedArray<T, N>& in) {
        return in.cend();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::const_iterator cend(const BoundedArray<T, N>& in) {
        return in.cend();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::reverse_iterator rbegin(BoundedArray<T, N>& in) {
        return in.rbegin();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::const_reverse_iterator rbegin(const BoundedArray<T, N>& in) {
        return in.crbegin();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::const_reverse_iterator crbegin(const BoundedArray<T, N>& in) {
        return in.crbegin();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::reverse_iterator rend(BoundedArray<T, N>& in) {
        return in.rend();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::const_reverse_iterator rend(const BoundedArray<T, N>& in) {
        return in.crend();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::const_reverse_iterator crend(const BoundedArray<T, N>& in) {
        return in.crend();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::pointer data(BoundedArray<T, N>& in) {
        return in.data();
}
template <typename T, std::size_t N>
typename BoundedArray<T, N>::const_pointer data(const BoundedArray<T, N>& in) {
        return in.data();
}

#endif
