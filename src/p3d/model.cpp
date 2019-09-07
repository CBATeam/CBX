/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Read model data. Originally developed for the ACE3 and ACRE2 projects.
 */
#include "model.hpp"
#include "read_helpers.hpp"

#include <easylogging++.h>
#include <algorithm>

using namespace cbx;

p3d::Model::Model(std::istream &stream_, const std::string &filename_) {
    filename = filename_;

    // get the full file size
    stream_.seekg(0, std::ios::end);
    size = stream_.tellg();
    stream_.seekg(0, std::ios::beg);

    // Parse header here
    uint32_t appId;
    stream_.read((char *)&filetype, sizeof(uint32_t));
    stream_.read((char *)&version, sizeof(uint32_t));
    stream_.read((char *)&appId, sizeof(uint32_t));

    if (version >= 58) {
        prefix_name = read_string(stream_);
    }
    //stream_.seekg(4, std::ios_base::cur);

    stream_.read((char *)&lod_count, sizeof(uint32_t));

    // parse model info here
    info = std::make_shared<ModelInfo>(stream_, lod_count);
    skeleton = info->skeleton;

    has_animations = read_bool(stream_);
    if (has_animations) {
        uint32_t animation_count = 0;
        stream_.read((char *)&animation_count, sizeof(uint32_t));
        for (uint32_t x = 0; x < animation_count; x++) {
            animations.push_back(std::make_shared<Animation>(stream_, version));
        }

        // Now we re-walk, and association animations and bones
        uint32_t local_lod_count;
        stream_.read((char *)&local_lod_count, sizeof(uint32_t));
        for (uint32_t lod = 0; lod < local_lod_count; lod++) {
            uint32_t num_bones;
            stream_.read((char *)&num_bones, sizeof(uint32_t));

            std::vector<uint32_t> lod_bone2anim;
            for (uint32_t x = 0; x < num_bones; x++) {
                uint32_t anim_count;
                stream_.read((char *)&anim_count, sizeof(uint32_t));

                for (uint32_t anim = 0; anim < anim_count; anim++) {
                    uint32_t anim_index;
                    stream_.read((char *)&anim_index, sizeof(uint32_t));
                    if (std::find(skeleton->all_bones[x]->animations.begin(), skeleton->all_bones[x]->animations.end(), anim_index) == skeleton->all_bones[x]->animations.end()) {
                        skeleton->all_bones[x]->animations.push_back(anim_index);
                    }
                }
            }
        }

        // Anims2Bones
        for (uint32_t lod = 0; lod < lod_count; lod++) {
            for (animation_p & anim : animations) {
                animate_bone_p next = std::make_shared<AnimateBone>();
                next->lod = lod;
                stream_.read((char *)&next->index, sizeof(int32_t));
                if (next->index != -1 && (anim->type != 8) && (anim->type != 9)) {
                    next->axis_position = vec3_fp32_t(stream_);
                    next->axis_direction = vec3_fp32_t(stream_);
                }
                anim->bones.push_back(next);
            }
        }
    }
    // LOD indexes
    for (uint32_t lod = 0; lod < lod_count; lod++) {
        uint32_t offset;
        stream_.read((char *)&offset, sizeof(uint32_t));
        start_lod.push_back(offset);
        LOG(DEBUG) << "LOD Offset: #" << lod << " : " << offset;
    }
    for (uint32_t lod = 0; lod < lod_count; lod++) {
        uint32_t offset;
        stream_.read((char *)&offset, sizeof(uint32_t));
        end_lod.push_back(offset);
        LOG(DEBUG) << "LOD Offset: #" << lod << " : " << offset;
    }

    // Attempt to read the faces?
    use_face_defaults = new bool[lod_count];
    for (uint32_t lod = 0; lod < lod_count; lod++) {
        use_face_defaults[lod]  = read_bool(stream_);
    }
    for (uint32_t x = 0; x < lod_count; x++) {
        if (!use_face_defaults[x]) {
            FaceSettings tface(stream_);       // @TODO: we dont save these because WHY!?
        }
    }

    for (uint32_t lod = 0; lod < lod_count; lod++) {
#ifdef _DEBUG
        char buffer[64];
        sprintf_s(buffer, "\t\t%08X",  info->resolutions[lod]);
        LOG(DEBUG) << "LOD #" << lod << ", type: " << buffer;
#endif
        stream_.clear();
        stream_.seekg(start_lod[lod], stream_.beg);
        lods.push_back(std::make_shared<Lod>(stream_, lod, version));

    }
}

p3d::Model::~Model() {
    if (use_face_defaults) {
        delete[] use_face_defaults;
    }
}
