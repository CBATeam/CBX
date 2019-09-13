/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <magnetar5891@gmail.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Read from a pbo file. Originally developed for the ACRE2 project.
 */

#include "archive.hpp"
#include "read_helpers.hpp"

#include <algorithm>

using namespace cbx;

bool pbo::Archive::get_file(std::istream& stream_, const std::string& file_, file_p output_) {
    (void) stream_;
    (void) file_;
    (void) output_;
    return false;
}

bool pbo::Archive::get_file(std::istream& stream_, const entry_p entry_, file_p output_) {
    uint32_t use_size = 0u;

    std::streampos _save = stream_.tellg();
    std::streamoff file_offset = begin_data_offset + entry_->offset;

    stream_.seekg(file_offset, stream_.beg);
    if (entry_->compression == 'Cprs') {
        output_->data = std::unique_ptr<uint8_t[]>(new uint8_t[entry_->size]);
        _lzss_decompress(stream_, entry_->size);
        memcpy(output_->data.get(), this->_data.get(), entry_->size);
        use_size = entry_->size;
    } else {
        std::streamoff bytes_read = 0;
        use_size = entry_->storage_size;
        output_->data = std::unique_ptr<uint8_t[]>(new uint8_t[use_size]);
        while (bytes_read < use_size) {
            if (!stream_.read(( char*) output_->data.get() + bytes_read, use_size - bytes_read)) {
                return false;
            }
            bytes_read += stream_.gcount();
        }
    }

    output_->size = use_size;
    output_->entry = entry_;

    stream_.seekg(_save, stream_.beg);

    return true;
}

pbo::Archive::Archive(std::istream& stream_) {
    // Read the first entry, then info, then next entry
    uint32_t offset = 0;
    entry_p root_entry = std::make_shared<Entry>(stream_);
    info = std::make_shared<ExtEntry>(stream_);
    root_entry->offset = 0;
    entries.push_back(root_entry);

    entry_p next_entry = std::make_shared<Entry>(stream_);
    while (next_entry->filename != "") { // off by 1006 ?!
        next_entry->offset = offset;
        entries.push_back(next_entry);
        offset += next_entry->storage_size;

        next_entry = std::make_shared<Entry>(stream_);
    }
    begin_data_offset = stream_.tellg();;
}

pbo::Entry::Entry(std::istream& stream_) {
    filename = p3d::read_string(stream_);
    std::transform(std::cbegin(filename), std::cend(filename), std::begin(filename), [](const unsigned char i) { return static_cast<char>(tolower(i)); });

    stream_.read(( char*)& compression, sizeof(uint32_t));
    stream_.read(( char*)& size, sizeof(uint32_t));
    stream_.read(( char*)& reserved, sizeof(uint32_t));
    stream_.read(( char*)& timestamp, sizeof(uint32_t));
    stream_.read(( char*)& storage_size, sizeof(uint32_t));
}

pbo::ExtEntry::ExtEntry(std::istream& stream_) {
    std::string key = p3d::read_string(stream_);

    while (key != "") {
        std::string val = p3d::read_string(stream_);
        header[key] = val;
        key = p3d::read_string(stream_);
    }
}
