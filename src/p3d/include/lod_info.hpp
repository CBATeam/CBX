/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Read LOD information. Originally developed for the ACE3 and ACRE2 projects.
 */
#pragma once

#include "types.hpp"
#include "vector.hpp"
#include "transform_matrix.hpp"
#include "compressed.hpp"

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace cbx {
    namespace p3d {

        class Proxy {
        public:
            Proxy() {};
            Proxy(std::istream &stream_, uint32_t version_ = 73);
            ~Proxy() {};

            std::string                 name;        //"\ca\a10\agm65" (.p3d is implied) <<note the leading filename backslash
            transform_matrix_fp32_t            transform;           //see Generic FileFormat Data Types
            uint32_t                    sequence_id;     //
            uint32_t                    named_selection_id; //see P3D Named Selections
                                                    //////// ARMA ONLY (ODOLV4x) ///////
            int32_t                     bone_id;
            uint32_t                    section_id;        //see P3D_Lod_Sections
        };
        using proxy_p = std::shared_ptr<Proxy>;

        class StageTexture {
        public:
            StageTexture() : filter(0), file(""), transform_id(0), use_world_env_map(false) { };
            StageTexture(std::istream &stream_, uint32_t type_);
            ~StageTexture() {}

            uint32_t    filter;
            std::string file;
            uint32_t    transform_id; 
            bool        use_world_env_map;
        };
        using stage_texture_p = std::shared_ptr<StageTexture>;

        class Material {
        public:
            Material();
            Material(std::istream &stream_, const uint32_t version_ = 73);
            ~Material() {};

            std::string name;
            std::string surface;

            uint32_t    render_flags;
            uint32_t    type;

            float32_t   emissive[4];
            float32_t   ambient[4];
            float32_t   diffuse[4];
            float32_t   forced_diffuse[4];
            float32_t   specular[4];
            float32_t   specular_2[4];
            float32_t   specular_power;

            uint32_t    pixel_shader;
            uint32_t    vertex_shader;

            uint32_t    main_light; 
            uint32_t    fog_mode;
            uint32_t    render_flags_size;
            
            std::vector<stage_texture_p> texture_stages;
            std::vector<std::pair<uint32_t, transform_matrix_fp32_t>> transform_stages;
        };
        using material_p = std::shared_ptr<Material>;

        class EdgeSet {
        public:
            std::vector<uint32_t> mlod;
            std::vector<uint32_t> vertex;
        };

        class Face {
        public:
            Face() {};
            Face(std::istream & stream_, uint32_t version_ = 73);
            ~Face() {};

            uint32_t              flags;           //ODOL7 ONLY see P3D Point and Face Flags
            uint16_t              texture;         //ODOL7 ONLY
            uint8_t               type;            // 3==Triangle or 4==Box
            std::vector<uint32_t> vertex_table;
        };
        using face_p = std::shared_ptr<Face>;

        class Section {
        public:
            Section(){};
            Section(std::istream &stream_, uint32_t version_ = 73);
            ~Section() {};

            uint32_t  face_offsets[2];     // from / to region of LodFaces used
            uint32_t  min_bone_index;
            uint32_t  bones_count;
            uint32_t  mat_dummy;           // Should be always 0?
            uint16_t  common_texture_index;
            uint32_t  common_face_flags;
            int32_t   material_index;
            float32_t *area_over_tex;
            uint32_t  num_stages;  // Should be 2?
            uint32_t  u_long_1;
            uint8_t   extra;      // Surface material?
        };
        using section_p = std::shared_ptr<Section>;

        class NamedSelection {
        public:
            NamedSelection() {};
            NamedSelection(std::istream &stream_, uint32_t version_ = 73);
            ~NamedSelection() {};

            std::string          name;              // "rightleg" or "neck" eg
            compressed<uint32_t> faces;             // indexing into the LodFaces Table
            uint32_t             always_0_count;
            bool                 is_sectional;      //Appears in the sections[]= list of a model.cfg
            compressed<uint32_t> sections;          //IsSectional must be true. Indexes into the LodSections Table
            compressed<uint32_t> vertex_table;
            compressed<uint8_t>  texture_weights;   // if present they correspond to (are exentsions of) the VertexTableIndexes
        };
        using named_selection_p = std::shared_ptr<NamedSelection>;

        class Frame {
        public:
            Frame() {};
            Frame(std::istream &stream_, uint32_t version_ = 73);
            ~Frame() {}

            float32_t                time;
            std::vector<vec3_fp32_t> bone_positions;
        };
        using frame_p = std::shared_ptr<Frame>;

        class UV {
        public:
            UV() {};
            UV(std::istream &stream_, uint32_t version_ = 73);
            ~UV() {};

            float32_t             uv_scale[4];
            compressed<float32_t> data;
        };
        using uv_p = std::shared_ptr<UV>;

        class VertexTable {
        public:
            VertexTable() {};
            VertexTable(std::istream &stream_, uint32_t size_, uint32_t version_ = 73);
            ~VertexTable() {};
            
            uint32_t                 size;

            compressed<uint32_t>     point_flags;  // Potentially compressed
            std::vector<uv_p>        uvsets;

            compressed<vec3_fp32_t>  points;
            compressed<vec3_fp32_t>  normals;
            compressed<cbx::pair<float32_t>> minmax;
            compressed<vec3_fp32_t>  vert_properties;
            compressed<vec3_fp32_t>  unknown_vtx;
        };
        using vertex_table_p = std::shared_ptr<VertexTable>;

        class Lod {
        public:
            Lod() {};
            Lod(std::istream & stream_, uint32_t id_, uint32_t version_ = 73);
            ~Lod() {};

            uint32_t                            id;

            std::vector<proxy_p>                proxies;             // see P3D Lod Proxies
            std::vector<uint32_t>               items;               // potentially compressed
            std::vector<std::vector<uint32_t>>  bone_links;
            uint32_t                            point_count;
            uint32_t                            face_area;
            uint32_t                            orHints;
            uint32_t                            andHints;
            
            vec3_fp32_t                 min_pos;
            vec3_fp32_t                 max_pos;
            vec3_fp32_t                 autocenter_pos;
            float32_t                           sphere;    // same as geo or mem values in modelinfo, if this lod is geo or memlod of course
            std::vector<std::string>            textures;  //"ca\characters\hhl\hhl_01_co.paa"
            std::vector<material_p>             materials;

            EdgeSet                            edges;
            
            uint32_t                            u_count;
            uint32_t                            offset_sections;            // see below
            uint16_t                            u_short_1;

            uint32_t                            faces_allocation_size;
            std::vector<face_p>                 faces;
            std::vector<section_p>              sections;

            std::vector<named_selection_p>      selections;

            std::map<std::string, std::string>  named_properties;

            std::vector<frame_p>                frames;
            
            uint32_t                      icon_color;
            uint32_t                      selected_color;
            uint32_t                      u_residue;
            uint8_t                       u_byte_1;
            bool                          vertex_bone_is_simple;
            uint32_t                      vertex_table_size;


            vertex_table_p                vertices;
        };
        using lod_p = std::shared_ptr<Lod>;
    }
}
