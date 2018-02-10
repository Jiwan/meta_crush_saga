#ifndef META_CRUSH_SAGA_RANDOM_HPP
#define META_CRUSH_SAGA_RANDOM_HPP

#include <cstdint>

class random_generator
{
public:
    constexpr random_generator(std::uint16_t seed) : prev_(seed) {}

    constexpr std::uint16_t next()
    {
        // LSFR
        /* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
        std::uint16_t bit  = ((prev_ >> 0) ^ (prev_ >> 2) ^ (prev_ >> 3) ^ (prev_ >> 5) ) & 1;
        prev_ =  (prev_ >> 1) | (bit << 15);

        return prev_;
    }

    std::uint16_t prev_;
};

#endif //META_CRUSH_SAGA_RANDOM_HPP
