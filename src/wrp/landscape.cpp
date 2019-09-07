/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Snippers <snippers@idi-systems.com>
 *
 * Parse Arma WRP as well as CBX/ACRE fake wrp files. Originally developed for the ACRE2 project.
 */

#include "landscape.hpp"

#include <exception>
#include <cmath>
#include <cstring>

using namespace cbx;

wrp::Landscape::Landscape(const std::string &path_) {
    std::ifstream stream(path_, std::ifstream::binary);
    process(stream);
    stream.close();
}

wrp::Landscape::Landscape(std::istream &stream_) {
    process(stream_);
}

wrp::LandscapeResult wrp::Landscape::parse(const std::string &path_) {
    std::ifstream stream(path_, std::ifstream::binary);
    process(stream);
    stream.close();

    return failure;
}

wrp::LandscapeResult wrp::Landscape::parse(std::istream &stream_) {
    process(stream_);

    return failure;
}

wrp::Landscape::~Landscape() {}

bool wrp::Landscape::read_binary_tree_block(std::istream &stream_, const uint32_t bit_length_, const uint32_t block_size_, const uint32_t cell_size_, const uint32_t block_offset_x_, const uint32_t block_offset_y_)
{
    uint32_t blockSize = static_cast<uint32_t>(std::ceil((std::log10(static_cast<float32_t>(block_size_)) / std::log10(2.0f)) / 2.0f));
    blockSize = static_cast<uint32_t>(std::pow(2.0f, 2.0f * static_cast<float32_t>(block_size_)));

    const uint32_t current_block_size = blockSize / 4u;//256..64..16..1

    uint16_t packet_flag = 0;
    stream_.read((char *)&packet_flag, sizeof(uint16_t));

    for (int32_t bit_count = 0; bit_count < 16; bit_count++, packet_flag >>= 1) {
        const uint32_t current_block_offset_x = block_offset_x_ + (bit_count % 4) * blockSize*bit_length_;// 0,1,2,3
        const uint32_t current_block_offset_y = block_offset_y_ + ((bit_count / 4) * blockSize); // 0,1,2,3

        if (packet_flag & 1) {
            if (!read_binary_tree_block(stream_, bit_length_, current_block_size, cell_size_, current_block_offset_x, current_block_offset_y)) {
                return false;
            }
        } else {
            uint16_t A = 0;
            uint16_t B = 0;
            stream_.read((char *)&A, bit_length_);
            stream_.read((char *)&B, bit_length_);
            if (!A && !B) {
                continue; // filler
            }

            if ((current_block_offset_x >= cell_size_) || (current_block_offset_y >= cell_size_)) {
                return false;
            }
/*
            for (uint32_t iy = 0u; iy < current_block_offset_y; iy++) {
                for (uint32_t ix = 0u; ix < current_block_size; ix++)
                {
                    const int32_t OffsetAB = ((current_block_offset_y + iy)*cell_size) + current_block_offset_x + ix;
                    //_Grid[OffsetAB] = A; _Grid[OffsetAB + ThisBlockSize.x] = B;
                }
            }*/
        }
    }
    return true;
}

bool wrp::Landscape::process(std::istream &stream_) {
    stream_.read(filetype, 4);
    filetype[4] = 0x00;
    failure = LandscapeResult::Success;

    if (strcmp(filetype, "8WVR") == 0) {
        // Header
        //Texture Size
        stream_.read((char *)&layer_size_x, sizeof(uint32_t));
        stream_.read((char *)&layer_size_y, sizeof(uint32_t));
        //TerrainGrid Size
        stream_.read((char *)&map_size_x, sizeof(uint32_t));
        stream_.read((char *)&map_size_y, sizeof(uint32_t));
        //Cell size
        stream_.read((char *)&cell_size, sizeof(float32_t));
        map_grid_size = cell_size;

        // elevations (Elevations   [TerrainGridSize.y][TerrainGridSize.x];)
        elevations = compressed<float32_t>(stream_, map_size_x * map_size_x, false, false, version);

        //Generate peaks.
        return true;
    }

    // Proceed with OPRW/ACRE
    stream_.read((char *)&version, sizeof(uint32_t));
    if (version > 24) {
        stream_.read((char *)&some_bit, sizeof(uint32_t));
    }
    stream_.read((char *)&layer_size_x, sizeof(uint32_t));
    stream_.read((char *)&layer_size_y, sizeof(uint32_t));
    stream_.read((char *)&map_size_x, sizeof(uint32_t));
    stream_.read((char *)&map_size_y, sizeof(uint32_t));

    stream_.read((char *)&cell_size, sizeof(float32_t));
    map_grid_size = cell_size * layer_size_x / map_size_x;

    // Failure is only likely on recompression/reading gridblocks.
    try {
        uint8_t flag; // Determines if the blocks are present.

        /* GridBlock - CellEnv */
        stream_.read((char *)&flag, sizeof(uint8_t));
        if (flag == '\x1') {
            read_binary_tree_block(stream_, sizeof(uint16_t), 16, map_size_x, 0, 0);
        } else {
            // Grid is not present, read the fill bits.
            uint32_t fill_bits;
            stream_.read((char *)&fill_bits, sizeof(uint32_t));
        }

        /* GridBlock - CfgEnvSounds */
        stream_.read((char *)&flag, sizeof(uint8_t));
        if (flag == '\x1') {
            read_binary_tree_block(stream_, sizeof(uint16_t), 16, map_size_x, 0, 0);
        } else {
            // Grid is not present, read the fill bits.
            uint32_t fill_bits;
            stream_.read((char *)&fill_bits, sizeof(uint32_t));
        }

        uint32_t peak_count;
        stream_.read((char *)&peak_count, sizeof(uint32_t));

        float32_t *peak_floats = new float32_t[peak_count * 3u];
        stream_.read((char *)peak_floats, sizeof(float32_t)*peak_count * 3u);

        for (uint32_t i = 0u; i < peak_count * 3u; i = i + 3u) {
            peaks.push_back(vec3_fp32_t(peak_floats[i], peak_floats[i + 1u], peak_floats[i + 2u]));
        }
        delete[] peak_floats;

        /* GridBlock - RvmatLayerIndex*/
        stream_.read((char *)&flag, sizeof(uint8_t));
        if (flag == '\x1') {
            read_binary_tree_block(stream_, sizeof(int16_t), 16, layer_size_x, 0, 0);
        } else {
            // Grid is not present, read the fill bits.
            uint32_t fill_bits;
            stream_.read((char *)&fill_bits, sizeof(uint32_t));
        }

        /* CHEEKY HACK for our dummy Tanoa WRP */
        if ((strcmp(filetype, "ACRE") == 0) || (strcmp(filetype, "CBXW") == 0)) {
            elevations = compressed<float32_t>(stream_, map_size_x * map_size_x, false, false, version);
        } else {
            //Random clutter
            if (version <= 20) {
                compressed<int16_t> test_demcompress_old = compressed<int16_t>(stream_, layer_size_x*layer_size_y, true, false, version); // int16_t
            } else {
                compressed<uint8_t> test_demcompress1 = compressed<uint8_t>(stream_, map_size_x*map_size_y, true, false, version);
            }
            if (version >= 22) { //CompressedBytes 1
                compressed<uint8_t> test_demcompress2 = compressed<uint8_t>(stream_, map_size_x*map_size_y, true, false, version);
            }

            elevations = compressed<float32_t>(stream_, map_size_x * map_size_y, true, false, version);
        }
    } catch (std::exception& e) {
        LOG(ERROR) << "WRP Parsing exception: " << e.what();
        //Insert 1 peak in the middle.
        peaks.clear();
        peaks.push_back(vec3_fp32_t((map_size_x * map_grid_size)/2.0f, (map_size_y * map_grid_size)/2.0f, 0.0f));

        // Fill elevation grid with 0 data.
        for (uint32_t x = 0u; x < (map_size_x * map_size_y); x++) {
            elevations.data.push_back(0.0f);
        }
        //elevations.size = elevations
        failure = LandscapeResult::Recovered;
    }
    return true;
}


cbx::Result wrp::Landscape::generate_cbx_wrp(std::ofstream &out_file_) {

    /*
     * Following the file format https://community.bistudio.com/wiki/Wrp_File_Format_-_OPRWv17_to_24
     * in order to be compatible with our parser.
     */

    // ===================== Header =====================
    out_file_.write("CBXW", 4);                                // Filetype
    out_file_.write((char *)&version, sizeof(uint32_t));       // Version

    // Some dummy bits
    uint32_t fill_bits = 0u;
    out_file_.write((char *)&fill_bits, sizeof(uint32_t));     // As of version 25

    out_file_.write((char *)&layer_size_x, sizeof(uint32_t));  // Layer  Size
    out_file_.write((char *)&layer_size_y, sizeof(uint32_t));

    out_file_.write((char *)&map_size_x, sizeof(uint32_t));    // Map Size
    out_file_.write((char *)&map_size_y, sizeof(uint32_t));

    out_file_.write((char *)&cell_size, sizeof(uint32_t));     // Layer Cell Size

    // ===================== Grid Block Cell Environment =====================
    uint8_t not_present = 0;
    out_file_.write((char *)&not_present, sizeof(uint8_t));    // In order to spare size, we do not fill anything
    out_file_.write((char *)&fill_bits, sizeof(uint32_t));

    // ===================== Grid Block CfgEnvSounds =====================
    out_file_.write((char *)&not_present, sizeof(uint8_t));    // In order to spare size, we do not fill anything
    out_file_.write((char *)&fill_bits, sizeof(uint32_t));

    // ===================== Peaks =====================
    fill_bits = static_cast<uint32_t>(peaks.size());
    out_file_.write((char *)&fill_bits, sizeof(uint32_t));

    for (auto peak : peaks) {
        float32_t value = peak.x();
        out_file_.write((char *)&value, sizeof(float32_t));
        value = peak.y();
        out_file_.write((char *)&value, sizeof(float32_t));
        value = peak.z();
        out_file_.write((char *)&value, sizeof(float32_t));
    }

    // ===================== Grid Block Rvmat Layer Index =====================
    out_file_.write((char *)&not_present, sizeof(uint8_t));    // In order to spare size, we do not fill anything
    out_file_.write((char *)&fill_bits, sizeof(uint32_t));

    // ===================== And now the ACRE hack: elevations =====================
    for (uint32_t elevIdx = 0u; elevIdx < (map_size_x * map_size_y); elevIdx++) {
        out_file_.write((char *) &elevations[elevIdx], sizeof(float32_t));
    }

    return Result::ok;
}
