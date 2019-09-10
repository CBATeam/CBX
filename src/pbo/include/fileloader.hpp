/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <magnetar5891@gmail.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Loads a pbo file. Originally developed for the ACRE2 project.
 */
#pragma once

#include "archive.hpp"
#include "search.hpp"
#include "membuf.hpp"

namespace cbx {
    namespace pbo {
        class FileEntry {
        public:
            FileEntry() {};
            FileEntry(file_p entry_) : file(entry_) {
                this->memory_buffer = Membuf(( char*) file->data.get(), entry_->entry->storage_size);
                this->istream = new std::istream(&this->memory_buffer);
            };

            ~FileEntry() { delete this->istream; };
            std::istream& stream() { return *(this->istream); };
        protected:
            file_p file;
            Membuf memory_buffer;
            std::istream* istream;
        };
        using file_entry_p = std::shared_ptr<FileEntry>;

        class Fileloader { // TODO: Read singleton?
        public:
            Fileloader() {
                pbo_searcher = std::make_shared<Search>();
            };

            ~Fileloader() {};

            //@TODO: yea this is not the best way to do this...
            //Initialize the fileloader before threads start eating it.
            void poke() {
                return;
            }

            bool get_file(const std::string& path_, std::string& _extension, file_entry_p& entry_);
        protected:
            search_p pbo_searcher;

            bool load_file(const std::string& file_path_, const std::string& pbo_path_, file_entry_p& entry_);
        };
    }
}
