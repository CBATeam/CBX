/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <magnetar5891@gmail.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Loads a pbo file. Originally developed for the ACRE2 project.
 */
#include "fileloader.hpp"

#include <algorithm>

using namespace cbx;

bool pbo::Fileloader::get_file(const std::string& path_, std::string& _extension, file_entry_p& entry_) {
    std::string working_path = path_;
    // remove leading slash
    if (working_path.c_str()[0] == '\\')
        working_path.erase(working_path.begin());

    std::transform(std::cbegin(working_path), std::cend(working_path), std::begin(working_path), [](const unsigned char i) { return static_cast<char>(tolower(i)); });

    auto find_ext = working_path.find(_extension);
    if (find_ext == std::string::npos) {
        working_path = working_path + _extension;
    }

    auto iter = this->pbo_searcher->file_index().find(working_path);
    if (iter != this->pbo_searcher->file_index().end()) {
        return this->load_file(iter->first, iter->second, entry_);
    }
    return false;
}

bool pbo::Fileloader::load_file(const std::string& file_path_, const std::string& pbo_path_, file_entry_p& entry_) {

    std::ifstream input;
    input.open(pbo_path_, std::ios::binary | std::ios::in);

    Archive pbo_archive(input);
    file_p pbo_file = std::make_shared<File>();

    std::string search_filename = file_path_;

    // Remove leading slash
    if (search_filename[0] == '\\') {
        search_filename.erase(0, 1);
    }

    std::transform(std::cbegin(search_filename), std::cend(search_filename), std::begin(search_filename), [](const unsigned char i) { return static_cast<char>(tolower(i)); });

    std::string prefix = std::string(pbo_archive.info->header["prefix"]);
    std::transform(std::cbegin(prefix), std::cend(prefix), std::begin(prefix), [](const unsigned char i) { return static_cast<char>(tolower(i)); });

    search_filename.erase(search_filename.find(prefix), prefix.size() + 1);

    for (auto& entry : pbo_archive.entries) {
        if (entry->filename == search_filename) {
            // Do shit here
            if (pbo_archive.get_file(input, entry, pbo_file)) {
                entry_ = std::make_shared<FileEntry>(pbo_file);
                input.close();
                return true;
            }
            break;
        }
    }

    input.close();
    return false;
};
