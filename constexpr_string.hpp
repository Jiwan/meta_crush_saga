//
// Created by jguegant on 2017-06-08.
//

#ifndef TEMPLATE_CRUSH_SAGA_CONSTEXPR_STRING_HPP
#define TEMPLATE_CRUSH_SAGA_CONSTEXPR_STRING_HPP

class constexpr_string
{
public:
    template<std::size_t N>
    constexpr constexpr_string(const char(&a)[N]): ptr_(a), size_(N - 1)
    {
    }

    constexpr constexpr_string(const char* a, int size): ptr_(a), size_(size)
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

    constexpr bool startswith(const constexpr_string& other)
    {
        int i = 0;

        for (; i < other.size() && i < size(); ++i) {
            if (ptr_[i] != other[i]) {
                return false;
            }
        }

        return i == other.size();

    }

    constexpr constexpr_string substr(std::size_t pos, std::size_t len = -1) const
    {
        if (pos > size_) {
            throw std::runtime_error("out of range");
        }

        const std::size_t new_size = (len == -1)?  (size_ - pos): std::min(len, (size_ - pos));
        return constexpr_string(ptr_ + pos,  new_size);
    }

private:
    const char* ptr_;
    std::size_t size_;
};

#endif //TEMPLATE_CRUSH_SAGA_CONSTEXPR_STRING_HPP
