#ifndef MINI_VECTOR_H
#define MINI_VECTOR_H

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <utility>

namespace mini {

/**
 * A simplified vector implementation demonstrating RAII, templates,
 * and the Rule of Five in modern C++.
 */
template <typename T>
class Vector {
public:
    // Type aliases for STL compatibility
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;

    // =========================================================================
    // Constructors and Destructor
    // =========================================================================

    /**
     * Default constructor - creates an empty vector.
     */
    Vector() noexcept
        : data_{nullptr}
        , size_{0}
        , capacity_{0}
    {}

    /**
     * Construct with initial size, default-initialized elements.
     */
    explicit Vector(size_type count)
        : data_{count > 0 ? new T[count]() : nullptr}
        , size_{count}
        , capacity_{count}
    {}

    /**
     * Construct with initial size and value.
     */
    Vector(size_type count, const T& value)
        : data_{count > 0 ? new T[count] : nullptr}
        , size_{count}
        , capacity_{count}
    {
        std::fill_n(data_, count, value);
    }

    /**
     * Initializer list constructor.
     */
    Vector(std::initializer_list<T> init)
        : data_{init.size() > 0 ? new T[init.size()] : nullptr}
        , size_{init.size()}
        , capacity_{init.size()}
    {
        std::copy(init.begin(), init.end(), data_);
    }

    /**
     * Destructor - RAII cleanup.
     */
    ~Vector() {
        delete[] data_;
    }

    // =========================================================================
    // Copy Operations (Rule of Five - Part 1)
    // =========================================================================

    /**
     * Copy constructor - deep copy.
     */
    Vector(const Vector& other)
        : data_{other.size_ > 0 ? new T[other.size_] : nullptr}
        , size_{other.size_}
        , capacity_{other.size_}
    {
        std::copy(other.data_, other.data_ + other.size_, data_);
    }

    /**
     * Copy assignment operator - copy-and-swap idiom.
     */
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            swap(temp);
        }
        return *this;
    }

    // =========================================================================
    // Move Operations (Rule of Five - Part 2)
    // =========================================================================

    /**
     * Move constructor - transfer ownership.
     */
    Vector(Vector&& other) noexcept
        : data_{other.data_}
        , size_{other.size_}
        , capacity_{other.capacity_}
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    /**
     * Move assignment operator - transfer ownership.
     */
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // =========================================================================
    // Element Access
    // =========================================================================

    /**
     * Access element by index (unchecked).
     */
    reference operator[](size_type pos) {
        return data_[pos];
    }

    const_reference operator[](size_type pos) const {
        return data_[pos];
    }

    /**
     * Access element by index with bounds checking.
     * @throws std::out_of_range if pos >= size()
     */
    reference at(size_type pos) {
        if (pos >= size_) {
            throw std::out_of_range("Vector::at - index out of range");
        }
        return data_[pos];
    }

    const_reference at(size_type pos) const {
        if (pos >= size_) {
            throw std::out_of_range("Vector::at - index out of range");
        }
        return data_[pos];
    }

    /**
     * Access first element.
     */
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }

    /**
     * Access last element.
     */
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }

    /**
     * Direct access to underlying array.
     */
    pointer data() noexcept { return data_; }
    const_pointer data() const noexcept { return data_; }

    // =========================================================================
    // Iterators (for range-based for loops)
    // =========================================================================

    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }

    iterator end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cend() const noexcept { return data_ + size_; }

    // =========================================================================
    // Capacity
    // =========================================================================

    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]] size_type size() const noexcept { return size_; }
    [[nodiscard]] size_type capacity() const noexcept { return capacity_; }

    /**
     * Reserve capacity for at least new_cap elements.
     */
    void reserve(size_type new_cap) {
        if (new_cap > capacity_) {
            reallocate(new_cap);
        }
    }

    /**
     * Reduce capacity to fit size.
     */
    void shrink_to_fit() {
        if (capacity_ > size_) {
            reallocate(size_);
        }
    }

    // =========================================================================
    // Modifiers
    // =========================================================================

    /**
     * Remove all elements.
     */
    void clear() noexcept {
        size_ = 0;
    }

    /**
     * Add element to the end (copy).
     */
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            grow();
        }
        data_[size_++] = value;
    }

    /**
     * Add element to the end (move).
     */
    void push_back(T&& value) {
        if (size_ >= capacity_) {
            grow();
        }
        data_[size_++] = std::move(value);
    }

    /**
     * Construct element in place at the end.
     */
    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (size_ >= capacity_) {
            grow();
        }
        data_[size_] = T(std::forward<Args>(args)...);
        return data_[size_++];
    }

    /**
     * Remove last element.
     */
    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }

    /**
     * Resize the vector.
     */
    void resize(size_type count) {
        if (count > capacity_) {
            reallocate(count);
        }
        if (count > size_) {
            std::fill(data_ + size_, data_ + count, T{});
        }
        size_ = count;
    }

    void resize(size_type count, const T& value) {
        if (count > capacity_) {
            reallocate(count);
        }
        if (count > size_) {
            std::fill(data_ + size_, data_ + count, value);
        }
        size_ = count;
    }

    /**
     * Swap contents with another vector.
     */
    void swap(Vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

private:
    T* data_;
    size_type size_;
    size_type capacity_;

    /**
     * Grow capacity (typically doubles).
     */
    void grow() {
        size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
        reallocate(new_cap);
    }

    /**
     * Reallocate to new capacity.
     */
    void reallocate(size_type new_cap) {
        T* new_data = new_cap > 0 ? new T[new_cap] : nullptr;
        size_type elements_to_copy = std::min(size_, new_cap);

        if (data_ && new_data) {
            std::move(data_, data_ + elements_to_copy, new_data);
        }

        delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
        if (size_ > new_cap) {
            size_ = new_cap;
        }
    }
};

/**
 * Non-member swap.
 */
template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept {
    lhs.swap(rhs);
}

/**
 * Comparison operators.
 */
template <typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}

} // namespace mini

#endif // MINI_VECTOR_H
