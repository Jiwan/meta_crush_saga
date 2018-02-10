#ifndef META_CRUSH_SAGA_CONSTEXPR_STRING_VIEW_HPP
#define META_CRUSH_SAGA_CONSTEXPR_STRING_VIEW_HPP

#include <algorithm>
#include <cstddef>
#include <stdexcept>

#include "constexpr_string.hpp"

class constexpr_string_view
{
public:
    using const_iterator = const char*;

public:
    template<std::size_t N>
    constexpr constexpr_string_view(const char(&a)[N]): ptr_(a), size_(N - 1)
    {
    }

    template <std::size_t N>
    constexpr constexpr_string_view(const constexpr_string<N>& s) : ptr_(s.data()), size_(s.size())
    {

    }

    constexpr constexpr_string_view(const char* a, int size): ptr_(a), size_(size)
    {
    }

    constexpr char operator[](std::size_t n) const
    {
        return n < size_? ptr_[n] : throw std::out_of_range("");
    }

    constexpr std::size_t size() const
    {
        return size_;
    }

    constexpr bool startswith(const constexpr_string_view& other)
    {
        int i = 0;

        for (; i < other.size() && i < size(); ++i) {
            if (ptr_[i] != other[i]) {
                return false;
            }
        }

        return i == other.size();

    }

    constexpr constexpr_string_view substr(std::size_t pos, std::size_t len = -1) const
    {
        if (pos > size_) {
            throw std::runtime_error("out of range");
        }

        const std::size_t new_size = (len == -1)?  (size_ - pos): std::min(len, (size_ - pos));
        return constexpr_string_view(ptr_ + pos,  new_size);
    }

    constexpr const_iterator begin() const
    {
        return ptr_;
    }

    constexpr const_iterator end() const
    {
        return ptr_ + size_;
    }

    constexpr const_iterator cbegin() const
    {
        return ptr_;
    }

    constexpr const_iterator cend() const
    {
        return ptr_ + size_;
    }

private:
    const char* ptr_;
    std::size_t size_;
};

#endif // META_CRUSH_SAGA_CONSTEXPR_STRING_VIEW_HPP
