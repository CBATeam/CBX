/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Snippers <snippers@idi-systems.com>
 *
 * Parse Arma WRP as well as CBX/ACRE fake wrp files. Originally developed for the ACRE2 project.
 */

#pragma once

#include "compressed_base.hpp"
#include "types.hpp"
#include "vector.hpp"
#include "logging.hpp"

#include <fstream>
#include <vector>
#include <memory>
#include <cassert>

namespace cbx {
    namespace wrp {
        template<typename T>
        class compressed : public compressed_base<T> {
        public:
            compressed() { }
            compressed(std::istream &stream_, const uint32_t size_, const bool compressed_ = false, const bool fill_ = false, const uint32_t version_ = 25u) {
                (void)fill_;
                //assert(size_ < 4095 * 10);
                if (size_ > 0) {
                    if (this->fill) {
                        T val;
                        stream_.read((char *)&val, sizeof(T));
                        for (uint32_t x = 0u; x < size_; x++) {
                            this->data.push_back(val);
                        }
                    } else {
                        if ((size_ * sizeof(T) >= 1024u && compressed_)) {
                            if (version_ >= 23) {
                                const int32_t result = this->_decompress_safe(stream_, size_ * sizeof(T));
                                assert(result > 0);
                                T * ptr = (T *)(this->_data.get());
                                this->data.assign(ptr, ptr + size_);
                            } else {
                                const bool ok = this->_lzss_decompress(stream_, (size_) * sizeof(T));
                                if (ok) {
                                    T * ptr = (T *)(this->_data.get());
                                    this->data.assign(ptr, ptr + size_);
                                }
                            }
                        } else {
                            for (uint32_t x = 0u; x < size_; x++) {
                                T val;
                                stream_.read((char *)&val, sizeof(T));
                                this->data.push_back(val);
                            }
                        }
                    }
                }
            }
        };

        template<>
        class compressed<vec3_fp32_t> : public compressed_base<vec3_fp32_t>{
        public:
            compressed() {}
            compressed(std::istream &stream_, const uint32_t size_, const bool compressed_ = false, const bool fill_ = false, const bool xyzCompressed = false, const uint32_t version_ = 25u) {
                (void)version_;
                if (fill_) {
                    const vec3_fp32_t val(stream_);
                    for (uint32_t x = 0u; x < size_; x++) {
                        data.push_back(val);
                    }
                } else {
                    if ((size_ * sizeof(float32_t) * 3u >= 1024u && compressed_)) {
                        if (xyzCompressed) {
                            const int32_t result = _decompress_safe(stream_, size_ * sizeof(float32_t));
                            if (result < 0) {
                                spd_logging::logger->error("Decompression error: {0}", result);
                            }
                            uint32_t * ptr = (uint32_t *)(_data.get());
                            for (uint32_t x = 0; x < size_; x++) {
                                const uint32_t value = ptr[x];
                                data.push_back(decode_xyz(value));
                            }
                        } else {
                            const int32_t result = _decompress_safe(stream_, size_ * sizeof(float32_t) * 3u);
                            if (result < 0) {
                                spd_logging::logger->error("Decompression error: {0}", result);
                            }
                            float32_t * ptr = (float32_t *)(_data.get());
                            for (uint32_t x = 0; x < size_ * 3; x += 3) {
                                data.push_back(vec3_fp32_t(ptr + x));
                            }
                        }
                    } else {
                        for (uint32_t x = 0u; x < size_; x++) {
                            data.push_back(vec3_fp32_t(stream_));
                        }
                    }
                }
            }

            vec3_fp32_t decode_xyz(uint32_t CompressedXYZ) {
                float32_t scale_factor = -1.0f / 511.0f;

                int32_t x = CompressedXYZ & 0x3FF;
                int32_t y = (CompressedXYZ >> 10) & 0x3FF;
                int32_t z = (CompressedXYZ >> 20) & 0x3FF;
                if (x > 511) {
                    x -= 1024;
                }
                if (y > 511) {
                    y -= 1024;
                }
                if (z > 511) {
                    z -= 1024;
                }

                return vec3_fp32_t(static_cast<float32_t>(x) * scale_factor, static_cast<float32_t>(y) * scale_factor, static_cast<float32_t>(z) * scale_factor);
            }
        };

        enum class LandscapeResult : int8_t {
            Failure = -2,
                    Recovered = -1,
                    Success = 0
        };

        class Landscape {
        public:
            Landscape() {};
            Landscape(const std::string &path_);
            Landscape(std::istream &stream_);
            ~Landscape();

            compressed<float32_t> elevations;
            std::vector<vec3_fp32_t> peaks;

            char filetype[5];
            uint32_t map_size_x, map_size_y;
            uint32_t layer_size_x, layer_size_y;
            uint32_t some_bit;
            uint32_t version;
            float32_t cell_size;
            float32_t map_grid_size;
            LandscapeResult failure;

            LandscapeResult parse(const std::string &path_);
            LandscapeResult parse(std::istream &stream_);
            Result generate_cbx_wrp(std::ofstream &out_file_);
        protected:
            bool read_binary_tree_block(std::istream &stream_, const uint32_t bit_length_, const uint32_t block_size_, const uint32_t cell_size_, const uint32_t block_offset_x_, const uint32_t block_offset_y_);
            bool process(std::istream &stream_);
        };
        typedef std::shared_ptr<Landscape> landscape_p;
    }
}
