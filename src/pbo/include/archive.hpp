/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <magnetar5891@gmail.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Read from a pbo file. Originally developed for the ACRE2 project.
 */
#pragma once

#include "compressed_base.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace cbx {
    namespace pbo {
        class ExtEntry {
        public:
            ExtEntry() {};
            ExtEntry(std::istream& stream_);
            ~ExtEntry() {};
            std::unordered_map<std::string, std::string> header;
        };
        typedef std::shared_ptr<ExtEntry> ext_entry_p;

        class Entry {
        public:
            Entry() {};
            Entry(std::istream& stream_);
            ~Entry() {};

            std::string filename;

            uint32_t    compression;
            uint32_t    size;
            uint32_t    storage_size;
            uint32_t    offset;         // pre-computed during search, offset of file

            uint32_t    reserved;
            uint32_t    timestamp;
        };
        using entry_p = std::shared_ptr<Entry>;

        class File {
        public:
            File() {};
            ~File() {};

            entry_p     entry;
            std::unique_ptr<uint8_t[]> data;
            std::streamsize    size;
        };
        using file_p = std::shared_ptr<File>;

        class Archive : public _compressed_base {
        public:
            Archive() {};
            Archive(std::istream& stream_);
            ~Archive() {};

            bool      get_file(std::istream& stream_, const std::string& file_, file_p output_);
            bool      get_file(std::istream& stream_, const entry_p entry_, file_p output_);

            std::streamoff          begin_data_offset;

            ext_entry_p             info;
            std::vector<entry_p>    entries;
        };
        using archive_p = std::shared_ptr<Archive>;
    }
}
