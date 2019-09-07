#pragma once

#include "vector.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace cbx {
    class _compressed_base {
    protected:
        int32_t _mikero_lzo1x_decompress_safe(const uint8_t*, uint8_t*, uint32_t);
        int32_t _decompress_safe(std::istream &, uint32_t);
        bool _lzss_decompress(std::istream &, long);
#if _MSC_VER == 1800
        std::shared_ptr<uint8_t[]> _data;
#else
        std::unique_ptr<uint8_t[]> _data;
#endif
    };
    template<typename T>
    class compressed_base : public _compressed_base {
    public:
        compressed_base() : size(0), fill(false), flag(0) {}

        T & operator[] (const int32_t index) { return data[index]; }

        uint32_t          size;
        bool              fill;
        std::vector<T>    data;
        bool              flag;
    };
}
