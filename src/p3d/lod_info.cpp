/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Read LOD information. Originally developed for the ACE3 and ACRE2 projects.
 */
#include "lod_info.hpp"
#include "read_helpers.hpp"

#include <easylogging++.h>
#include <cassert>
#include <algorithm>

using namespace cbx;

p3d::Lod::Lod(std::istream & stream_, uint32_t id_, uint32_t version_) : id(id_) {
    uint32_t temp_count;

    // proxies
    stream_.read((char *)&temp_count, sizeof(uint32_t)); assert(temp_count < 4096 * 10);
    for (uint32_t x = 0; x < temp_count; x++) {
        proxies.push_back(std::make_shared<Proxy>(stream_, version_));
    }
    LOG(DEBUG) << "Found Proxies:";
    for (auto & proxy : proxies) {
        LOG(DEBUG) << "\t" << proxy->name;
    }

    compressed<uint32_t> item(stream_, false, false);
    items.resize(item.data.size());
    std::copy(item.data.begin(), item.data.end(), items.begin());

    // bone links
    stream_.read((char *)&temp_count, sizeof(uint32_t)); assert(temp_count < 4096 * 10);
    for (uint32_t x = 0; x < temp_count; x++) {
        // @ TODO: BoneLInks
        uint32_t link_count;
        std::vector<uint32_t> links;
        stream_.read((char *)&link_count, sizeof(uint32_t));
        for (uint32_t jidx = 0; jidx < link_count; jidx++) {
            uint32_t val = 0u;
            stream_.read((char *)&val, sizeof(uint32_t));
            links.push_back(val);
        }
        bone_links.push_back(links);
    }

    stream_.read((char *)&point_count, sizeof(uint32_t));
    stream_.read((char *)&face_area, sizeof(float));
    stream_.read((char *)&orHints, sizeof(uint32_t));
    stream_.read((char *)&andHints, sizeof(uint32_t));

    min_pos = vec3_fp32_t(stream_);
    max_pos = vec3_fp32_t(stream_);
    autocenter_pos = vec3_fp32_t(stream_);

    stream_.read((char *)&sphere, sizeof(float));

    // Texture strings
    stream_.read((char *)&temp_count, sizeof(uint32_t));
    for (uint32_t x = 0; x < temp_count; x++) {
        std::string temp;

        temp = read_string(stream_);;
        textures.push_back(temp);
        LOG(DEBUG) << "Found texture: " << temp << " pos: " << stream_.tellg();
    }

    //Materials
    stream_.read((char *)&temp_count, sizeof(uint32_t));
    for (uint32_t x = 0; x < temp_count; x++) {
        materials.push_back(std::make_shared<Material>(stream_, version_));
    }

    edges.mlod = compressed<uint32_t>(stream_, true, false, version_).data;
    edges.vertex = compressed<uint32_t>(stream_, true, false, version_).data;

    LOG(DEBUG) << "Finished reading material: " << stream_.tellg();
    // @TODO: THIS IS OFF WTF?!
    // The first face is coming up null, so we missed something
    // Faces magic
    stream_.read((char *)&temp_count, sizeof(uint32_t));
    stream_.read((char *)&faces_allocation_size, sizeof(uint32_t));

    uint16_t dummyU16;
    stream_.read((char *)&dummyU16, sizeof(uint16_t)); // It  should be always 0

    for (uint32_t x = 0; x < temp_count; x++) {
        faces.push_back(std::make_shared<Face>(stream_, version_));
    }

    // Sections
    stream_.read((char *)&temp_count, sizeof(uint32_t));
    for (uint32_t x = 0; x < temp_count; x++) {
        sections.push_back(std::make_shared<Section>(stream_, version_));
    }

    stream_.read((char *)&temp_count, sizeof(uint32_t));
    for (uint32_t x = 0; x < temp_count; x++) {
        selections.push_back(std::make_shared<NamedSelection>(stream_, version_));
    }

    // named properties
    LOG(DEBUG) << "Loaded properties";
    stream_.read((char *)&temp_count, sizeof(uint32_t));
    for (uint32_t x = 0; x < temp_count; x++) {
        const std::string key = read_string(stream_);
        const std::string value = read_string(stream_);

        named_properties[key] = value;
        LOG(DEBUG) << "\t\t" << key << "->" << value;
    }

    stream_.read((char *)&temp_count, sizeof(uint32_t));
    for (uint32_t x = 0; x < temp_count; x++) {
        frames.push_back(std::make_shared<Frame>(stream_, version_));
    }

    stream_.read((char *)&icon_color, sizeof(uint32_t));
    stream_.read((char *)&selected_color, sizeof(uint32_t));
    stream_.read((char *)&u_residue, sizeof(uint32_t));

    vertex_bone_is_simple = read_bool(stream_);
    stream_.read((char *)&vertex_table_size, sizeof(uint32_t));

    // Vertex Table starts here
    vertices = std::make_shared<VertexTable>(stream_, vertex_table_size, version_);
}

p3d::UV::UV(std::istream &stream_, uint32_t version_) {
    stream_.read((char *)&uv_scale, sizeof(float32_t) * 4);
    data = compressed<float32_t>(stream_, true, true, version_);
}

p3d::VertexTable::VertexTable(std::istream &stream_, uint32_t size_, uint32_t version_) : size(size_) {
    point_flags = compressed<uint32_t>(stream_, true, true, version_);

    uvsets.push_back(std::make_shared<UV>(stream_, version_));

    // UV optionala additional sets
    uint32_t temp_count;
    stream_.read((char *)&temp_count, sizeof(uint32_t));
    for (uint32_t x = 0; x < temp_count-1; x++) {
        uvsets.push_back(std::make_shared<UV>(stream_, version_));
    }

    points = compressed<vec3_fp32_t>(stream_, true, false, false, version_);

    size = points.size;
}

p3d::NamedSelection::NamedSelection(std::istream &stream_, uint32_t version_) {
    name = read_string(stream_);

    faces = compressed<uint32_t>(stream_, true, false, version_);

    stream_.read((char *)&always_0_count, sizeof(uint32_t));
    is_sectional = read_bool(stream_);

    sections = compressed<uint32_t>(stream_, true, false, version_);
    vertex_table = compressed<uint32_t>(stream_, true, false, version_);
    texture_weights = compressed<uint8_t>(stream_, true, false, version_);
}

p3d::Section::Section(std::istream &stream_, uint32_t version_) : area_over_tex(nullptr) {
    (void)version_;

    stream_.read((char *)&face_offsets, sizeof(uint32_t) * 2);
    stream_.read((char *)&min_bone_index, sizeof(uint32_t));
    stream_.read((char *)&bones_count, sizeof(uint32_t));
    stream_.read((char *)&mat_dummy, sizeof(uint32_t));  // Should be always 0
    stream_.read((char *)&common_texture_index, sizeof(uint16_t));
    stream_.read((char *)&common_face_flags, sizeof(uint32_t));
    stream_.read((char *)&material_index, sizeof(int32_t));

    if (material_index == -1) {
        stream_.read((char *)&extra, sizeof(uint8_t));
    }

    stream_.read((char *)&num_stages, sizeof(uint32_t));  // Should be 2?
    area_over_tex = new float[num_stages];
    for (uint32_t count = 0u; count < num_stages; count++) {
        stream_.read((char *)&area_over_tex[count], sizeof(float));
    }
    stream_.read((char *)&u_long_1, sizeof(uint32_t));
}

p3d::Face::Face(std::istream & stream_, uint32_t version_) {
    (void)version_;

    stream_.read((char *)&type, sizeof(uint8_t));
    //assert(type == 3 || type == 4);
    for (uint8_t x = 0; x < type; x++) {
        uint32_t val;
        stream_.read((char *)&val, sizeof(uint32_t));
        vertex_table.push_back(val);
    }
}

p3d::StageTexture::StageTexture(std::istream &stream_, uint32_t type_) {
    stream_.read((char *)&filter, sizeof(uint32_t));
    file = read_string(stream_);
    stream_.read((char *)&transform_id, sizeof(uint32_t));
    if (type_ == 11) {
        use_world_env_map = read_bool(stream_);
    }
}

p3d::Material::Material(std::istream &stream_, const uint32_t version_) {
    (void)version_;
    name = read_string(stream_);

    stream_.read((char *)&type, sizeof(uint32_t));

    stream_.read((char *)&emissive, sizeof(float) * 4);
    stream_.read((char *)&ambient, sizeof(float) * 4);
    stream_.read((char *)&diffuse, sizeof(float) * 4);
    stream_.read((char *)&forced_diffuse, sizeof(float) * 4);
    stream_.read((char *)&specular, sizeof(float) * 4);
    stream_.read((char *)&specular_2, sizeof(float) * 4);

    stream_.read((char *)&specular_power, sizeof(float));
    stream_.read((char *)&pixel_shader, sizeof(uint32_t));
    stream_.read((char *)&vertex_shader, sizeof(uint32_t));
    stream_.read((char *)&main_light, sizeof(uint32_t));
    stream_.read((char *)&fog_mode, sizeof(uint32_t));

    surface = read_string(stream_);

    stream_.read((char *)&render_flags_size, sizeof(uint32_t));
    stream_.read((char *)&render_flags, sizeof(uint32_t));

    uint32_t textures_count, transforms_count;
    stream_.read((char *)&textures_count, sizeof(uint32_t));
    stream_.read((char *)&transforms_count, sizeof(uint32_t));

    for (uint32_t x = 0; x < textures_count; x++) {
        texture_stages.push_back(std::make_shared<StageTexture>(stream_, type));
    }

    for (uint32_t x = 0; x < transforms_count; x++) {
        uint32_t uv_source;
        stream_.read((char *)&uv_source, sizeof(uint32_t));
        transform_stages.push_back(std::pair<uint32_t, transform_matrix_fp32_t>(uv_source, transform_matrix_fp32_t(stream_)));
    }
    if (type >= 10) {
        texture_stages.push_back(std::make_shared<StageTexture>(stream_, type));
    }
}

p3d::Frame::Frame(std::istream &stream_, uint32_t version_) {
    (void)version_;
    stream_.read((char *)&time, sizeof(float));

    uint32_t count = 0;
    stream_.read((char *)&count, sizeof(uint32_t));
    for (uint32_t x = 0; x < count; x++) {
        bone_positions.push_back(vec3_fp32_t(stream_));
    }
}

p3d::Proxy::Proxy(std::istream &stream_, uint32_t version_) {
    (void) version_;
    name = read_string(stream_);
    transform = transform_matrix_fp32_t(stream_);
    stream_.read((char *)&sequence_id, sizeof(uint32_t));
    stream_.read((char *)&named_selection_id, sizeof(uint32_t));
    stream_.read((char *)&bone_id, sizeof(uint32_t));
    stream_.read((char *)&section_id, sizeof(uint32_t));

    LOG(DEBUG) << "Proxy: [" << name << "]";
}
