/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Skeleton data. Originally developed for the ACE3 and ACRE2 projects.
 */
#pragma once

#include "types.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cbx {
    namespace p3d {
        class Bone {
        public:
            Bone() : name(""), parent("") {}
            Bone(const std::string & name_, const std::string & parent_)
                : name(name_), parent(parent_) {}
            std::string name;
            std::string parent;
            std::vector<std::string> children;
            std::vector<uint32_t> animations;
        };
        using bone_p = std::shared_ptr<Bone>;

        class Skeleton {
        public:
            Skeleton() : inherited(false), size(0) {};
            Skeleton(std::istream &, const uint32_t lod_count_);
            ~Skeleton() {};

            std::string name;
            bool inherited;
            uint32_t size;
            std::map<std::string, bone_p> root_bones;
            std::vector<bone_p> all_bones;
        };
        using skeleton_p = std::shared_ptr<Skeleton>;
    }
}
