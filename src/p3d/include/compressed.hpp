/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Read compressed data. Originally developed for the ACE3 and ACRE2 projects.
 */
#pragma once

#include "types.hpp"
#include "vector.hpp"
#include "read_helpers.hpp"
#include "compressed_base.hpp"
#include "logging.hpp"

#include <cassert>

namespace cbx {
    namespace p3d {
        template<typename T>
        class compressed : public compressed_base<T> {
        public:
            compressed() { }
            compressed(std::istream &stream_, bool compressed_ = false, bool fill_ = false, uint32_t version_ = 73) {
                stream_.read((char *)&this->size, sizeof(uint32_t));

                // if (version <)
                if (fill_) {
                    this->fill = read_bool(stream_);
                }

                //assert(this->size < 4095 * 10);
                if (this->size > 0) {
                    if (this->fill) {
                        T val;
                        stream_.read((char *)&val, sizeof(T));
                        for (uint32_t x = 0; x < this->size; x++) {
                            this->data.push_back(val);
                        }
                    } else {
                        if (version_ >= 64 && compressed_) {
                            this->flag = read_bool(stream_);
                        }
                        if ( (this->size * sizeof(T) >= 1024 && compressed_  && version_ < 64) || (this->flag && compressed_)) {
                            int32_t result = this->_decompress_safe(stream_, this->size * sizeof(T));
                            assert(result > 0);
                            T * ptr = (T *)(this->_data.get());
                            this->data.assign(ptr, ptr + this->size );
                        } else {
                            for (uint32_t x = 0; x < this->size; x++) {
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
            compressed()  {}
            compressed(std::istream &stream_, bool compressed_ = false, bool fill_ = false, bool xyzCompressed = false, uint32_t version = 73) {
                stream_.read((char *)&size, sizeof(uint32_t));
                
                if (fill_) {
                    fill = read_bool(stream_);
                }
                
                if (fill) {
                    vec3_fp32_t val(stream_);
                    for (uint32_t x = 0; x < size; x++) {
                        data.push_back(val);
                    }
                } else {
                    if (version >= 64) {
                        flag = read_bool(stream_);
                    }
                    if ((size * sizeof(float32_t)*3 >= 1024 && compressed_  && version < 64) || (flag && compressed_)) {
                        if (xyzCompressed) {
                            int32_t result = _decompress_safe(stream_, size * sizeof(float32_t));
                            if (result < 0) {
                                spd_logging::logger->error("Decompression failed");
                                assert(false);
                            }
                            uint32_t * ptr = (uint32_t *)(_data.get());
                            for (uint32_t x = 0; x < size; x++) {
                                uint32_t value = ptr[x];
                                data.push_back(decode_xyz(value));
                            }
                        } else {
                            int32_t result = _decompress_safe(stream_, size * sizeof(float32_t) * 3);
                            if (result < 0) {
                                spd_logging::logger->error("Decompression failed");
                                assert(false);
                            }
                            float32_t * ptr = (float32_t *)(_data.get());
                            for (uint32_t x = 0; x < size*3; x+=3) {
                                data.push_back(vec3_fp32_t(ptr+x));
                            }
                        }
                    } else {
                        for (uint32_t x = 0; x < size; x++) {
                            data.push_back(vec3_fp32_t(stream_));
                        }
                    }
                }
            }

            vec3_fp32_t decode_xyz(uint32_t CompressedXYZ)
            {
                const float32_t scaleFactor = -1.0f / 511.0f;

                int32_t x = CompressedXYZ & 0x3FF;
                int32_t y = (CompressedXYZ >> 10) & 0x3FF;
                int32_t z = (CompressedXYZ >> 20) & 0x3FF;
                if (x > 511) x -= 1024;
                if (y > 511) y -= 1024;
                if (z > 511) z -= 1024;

                return vec3_fp32_t(x * scaleFactor, y * scaleFactor, z * scaleFactor);
            }
        };

        template<>
        class compressed<pair<float32_t>> : public compressed_base<pair<float32_t>>{
        public:
            compressed() {}
            compressed(std::istream &stream_, bool compressed_ = false, bool fill_ = false, uint32_t version = 73) {
                stream_.read((char *)&size, sizeof(uint32_t));

                if (fill_) {
                    fill = read_bool(stream_);
                }

                if (fill) {
                    cbx::pair<float32_t> val(stream_);
                    for (uint32_t x = 0; x < size; x++) {
                        data.push_back(val);
                    }
                } else {
                    if (version >= 64) {
                        flag = read_bool(stream_);
                    }
                    if ((size * sizeof(float32_t) * 2 >= 1024 && compressed_  && version < 64) || (flag && compressed_)) {
                        int32_t result = _decompress_safe(stream_, size * sizeof(float32_t) * 2);
                        if (result < 0) {
                            spd_logging::logger->error("Decompression failed");
                            assert(false);
                        }
                        float32_t * ptr = (float32_t *)(_data.get());
                        for (uint32_t x = 0; x < size * 2; x += 2) {
                            data.push_back(cbx::pair<float32_t>(ptr + x));
                        }
                    } else {
                        for (uint32_t x = 0; x < size; x++) {
                            data.push_back(cbx::pair<float32_t>(stream_));
                        }
                    }
                }
            }
        };
    }
}
