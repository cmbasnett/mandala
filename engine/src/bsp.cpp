//glm
#include "glm\ext.hpp"

//mandala
#include "bsp.hpp"
#include "camera.hpp"
#include "image.hpp"
#include "gpu.hpp"
#include "collision.hpp"
#include "bsp_gpu_program.hpp"
#include "resource_mgr.hpp"
#include "gpu_program_mgr.hpp"
#include "gpu_buffer_mgr.hpp"
#include "io.hpp"

//boost
#include <boost\algorithm\string.hpp>

namespace mandala
{
    struct bsp_chunk_t
	{
		enum class type_e : uint8_t
		{
            ENTITIES,
            PLANES,
            TEXTURES,
            VERTICES,
            VISIBLIITY,
            NODES,
            TEXTURE_INFO,
            FACES,
            LIGHTING,
            CLIP_NODES,
            LEAVES,
            MARK_SURFACES,
            EDGES,
            SURFACE_EDGES,
            MODELS,
            COUNT
        };

        uint32_t offset = 0;
        uint32_t length = 0;
    };

    bsp_t::bsp_t(std::istream& istream)
    {
        if (!istream.good())
        {
            throw std::exception();
        }

        //version
        int32_t version;

        read(istream, version);

        static const auto BSP_VERSION = 30;

        if (version != BSP_VERSION)
        {
            throw std::exception();
        }

        //chunks
        std::vector<bsp_chunk_t> chunks;

		chunks.resize(static_cast<size_t>(bsp_chunk_t::type_e::COUNT));

        for (auto& chunk : chunks)
        {
            read(istream, chunk.offset);
            read(istream, chunk.length);
        }

        //planes
		const auto& plane_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::PLANES)];
        istream.seekg(plane_chunk.offset, std::ios_base::beg);

        auto plane_count = plane_chunk.length / sizeof(plane_t);
        planes.resize(plane_count);

        for (auto& plane : planes)
        {
            read(istream, plane.plane.normal.x);
            read(istream, plane.plane.normal.z);
            read(istream, plane.plane.normal.y);
            read(istream, plane.plane.distance);
            read(istream, plane.type);

            plane.plane.normal.z = -plane.plane.normal.z;
        }

        //vertex_locations
		const auto& vertices_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::VERTICES)];
        istream.seekg(vertices_chunk.offset, std::ios_base::beg);

        std::vector<vec3_t> vertex_locations;

        auto vertex_location_count = vertices_chunk.length / sizeof(vec3_t);
        vertex_locations.resize(vertex_location_count);

        for (auto& vertex_location : vertex_locations)
        {
            read(istream, vertex_location.x);
            read(istream, vertex_location.z);
            read(istream, vertex_location.y);

            vertex_location.z = -vertex_location.z;
        }

        //edges
		const auto& edges_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::EDGES)];
        istream.seekg(edges_chunk.offset, std::ios_base::beg);

        auto edge_count = edges_chunk.length / sizeof(edge_t);
        edges.resize(edge_count);

        for (auto& edge : edges)
        {
            read(istream, edge.vertex_indices[0]);
            read(istream, edge.vertex_indices[1]);
        }

        //surface_edges
		const auto& surface_edges_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::SURFACE_EDGES)];
        istream.seekg(surface_edges_chunk.offset, std::ios_base::beg);

        auto surface_edge_count = surface_edges_chunk.length / sizeof(int32_t);
        surface_edges.resize(surface_edge_count);

        for (auto& surface_edge : surface_edges)
        {
            read(istream, surface_edge);
        }

        //faces
		const auto& faces_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::FACES)];
        istream.seekg(faces_chunk.offset, std::ios_base::beg);

        auto face_count = faces_chunk.length / sizeof(face_t);
        faces.resize(face_count);

        for (auto& face : faces)
        {
            read(istream, face.plane_index);
            read(istream, face.plane_side);
            read(istream, face.surface_edge_start_index);
            read(istream, face.surface_edge_count);
            read(istream, face.texture_info_index);
            read(istream, face.lighting_styles[0]);
            read(istream, face.lighting_styles[1]);
            read(istream, face.lighting_styles[2]);
            read(istream, face.lighting_styles[3]);
            read(istream, face.lightmap_offset);
        }

        //nodes
		const auto& nodes_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::NODES)];
        istream.seekg(nodes_chunk.offset, std::ios_base::beg);

        auto node_count = nodes_chunk.length / sizeof(node_t);
        nodes.resize(node_count);

        for (auto& node : nodes)
        {
            read(istream, node.plane_index);
            read(istream, node.child_indices[0]);
            read(istream, node.child_indices[1]);
            read(istream, node.aabb.min.x);
            read(istream, node.aabb.min.z);
            read(istream, node.aabb.min.y);
            read(istream, node.aabb.max.x);
            read(istream, node.aabb.max.z);
            read(istream, node.aabb.max.y);
            read(istream, node.face_start_index);
            read(istream, node.face_count);

            node.aabb.min.z = -node.aabb.min.z;
            node.aabb.max.z = -node.aabb.max.z;
        }

        //leaves
		const auto& leaves_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::LEAVES)];
        istream.seekg(leaves_chunk.offset, std::ios_base::beg);

        auto leaf_count = leaves_chunk.length / sizeof(leaf_t);
        leafs.resize(leaf_count);

        for (auto& leaf : leafs)
        {
            read(istream, leaf.content_type);
            read(istream, leaf.visibility_offset);
            read(istream, leaf.aabb.min.x);
            read(istream, leaf.aabb.min.z);
            read(istream, leaf.aabb.min.y);
            read(istream, leaf.aabb.max.x);
            read(istream, leaf.aabb.max.z);
            read(istream, leaf.aabb.max.y);
            read(istream, leaf.mark_surface_start_index);
            read(istream, leaf.mark_surface_count);
            read(istream, leaf.ambient_sound_levels[0]);
            read(istream, leaf.ambient_sound_levels[1]);
            read(istream, leaf.ambient_sound_levels[2]);
            read(istream, leaf.ambient_sound_levels[3]);

            leaf.aabb.min.z = -leaf.aabb.min.z;
            leaf.aabb.max.z = -leaf.aabb.max.z;
        }

        //mark_surfaces
		const auto& mark_surfaces_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::MARK_SURFACES)];
        istream.seekg(mark_surfaces_chunk.offset, std::ios_base::beg);

        auto mark_surface_count = mark_surfaces_chunk.length / sizeof(uint16_t);
        mark_surfaces.resize(mark_surface_count);

        for (auto& mark_surface : mark_surfaces)
        {
            read(istream, mark_surface);
        }

        //clip_nodes
		const auto& clip_nodes_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::CLIP_NODES)];
        istream.seekg(clip_nodes_chunk.offset, std::ios_base::beg);

        auto clip_node_count = clip_nodes_chunk.length / sizeof(clip_node_t);
        clip_nodes.resize(clip_node_count);

        for (auto& clip_node : clip_nodes)
        {
            read(istream, clip_node.plane_index);
            read(istream, clip_node.child_indices[0]);
            read(istream, clip_node.child_indices[1]);
        }

        //models
		const auto& models_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::MODELS)];
        istream.seekg(models_chunk.offset, std::ios_base::beg);

        auto model_count = models_chunk.length / sizeof(model_t);
        models.resize(model_count);

        for (auto& model : models)
        {
            read(istream, model.aabb.min.x);
            read(istream, model.aabb.min.z);
            read(istream, model.aabb.min.y);
            read(istream, model.aabb.max.x);
            read(istream, model.aabb.max.z);
            read(istream, model.aabb.max.y);
            read(istream, model.origin.x);
            read(istream, model.origin.z);
            read(istream, model.origin.y);
            read(istream, model.head_node_indices[0]);
            read(istream, model.head_node_indices[1]);
            read(istream, model.head_node_indices[2]);
            read(istream, model.head_node_indices[3]);
            read(istream, model.vis_leafs);
            read(istream, model.face_start_index);
            read(istream, model.face_count);

            model.aabb.min.z = -model.aabb.min.z;
            model.aabb.max.z = -model.aabb.max.z;
            model.origin.z = -model.origin.z;
        }

        //visibility
		const auto& visibility_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::VISIBLIITY)];
        istream.seekg(visibility_chunk.offset, std::ios_base::beg);

        if (visibility_chunk.length > 0)
        {
            std::function<void(int32_t)> count_vis_leaves = [&](int32_t node_index)
            {
                if (node_index < 0)
                {
                    if (node_index == -1 || leafs[~node_index].content_type == content_type_e::SOLID)
                    {
                        return;
                    }

                    ++vis_leaf_count;

                    return;
                }

                count_vis_leaves(nodes[node_index].child_indices[0]);
                count_vis_leaves(nodes[node_index].child_indices[1]);
            };

            count_vis_leaves(0);

            std::vector<uint8_t> visibility_data;
            read(istream, visibility_data, visibility_chunk.length);

            for (size_t i = 0; i < vis_leaf_count; ++i)
            {
                const auto& leaf = leafs[i + 1];

                if (leaf.visibility_offset < 0)
                {
                    continue;
                }

                auto leaf_pvs = boost::dynamic_bitset<>(leaf_count - 1);
                leaf_pvs.reset();

                size_t leaf_pvs_index = 0;

                auto visibility_data_itr = visibility_data.begin() + leaf.visibility_offset;

                while (leaf_pvs_index < vis_leaf_count)
                {
                    if (*visibility_data_itr == 0)
                    {
                        ++visibility_data_itr;

                        leaf_pvs_index += 8 * (*visibility_data_itr);
                    }
                    else
                    {
                        for (uint8_t mask = 1; mask != 0; ++leaf_pvs_index, mask <<= 1)
                        {
                            if ((*visibility_data_itr & mask) && (leaf_pvs_index < vis_leaf_count))
                            {
                                leaf_pvs[leaf_pvs_index] = true;
                            }
                        }
                    }

                    ++visibility_data_itr;
                }

                leaf_pvs_map.insert(std::make_pair(i, std::move(leaf_pvs)));
            }
        }

        //textures
		const auto& textures_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::TEXTURES)];
        istream.seekg(textures_chunk.offset, std::ios_base::beg);

        uint32_t texture_count;

        read(istream, texture_count);

        std::vector<uint32_t> texture_offsets;
        texture_offsets.resize(texture_count);

        for (auto& texture_offset : texture_offsets)
        {
            read(istream, texture_offset);
        }

        std::vector<bsp_texture_t> bsp_textures;

        for (uint32_t i = 0; i < texture_count; ++i)
        {
            istream.seekg(textures_chunk.offset + texture_offsets[i], std::ios_base::beg);

            char texture_name_bytes[16];
            read(istream, texture_name_bytes);

            std::string texture_name = texture_name_bytes;

            bsp_texture_t bsp_texture;

            read(istream, bsp_texture.width);
            read(istream, bsp_texture.height);
            read(istream, bsp_texture.mipmap_offsets);

            bsp_textures.push_back(bsp_texture);

            texture_name.append(".png");

            boost::shared_ptr<texture_t> texture;

            try
            {
                texture = resources.get<texture_t>(hash_t(texture_name));
            }
            catch (...)
            {
                std::cerr << "couldn't load " << texture_name << std::endl;
            }

            textures.push_back(texture);
        }

        //texture_info
		const auto& texture_info_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::TEXTURE_INFO)];
        istream.seekg(texture_info_chunk.offset, std::ios_base::beg);

        auto texture_info_count = texture_info_chunk.length / sizeof(texture_info_t);

        texture_infos.resize(texture_info_count);

        for (auto& texture_info : texture_infos)
        {
            read(istream, texture_info.s.axis.x);
            read(istream, texture_info.s.axis.z);
            read(istream, texture_info.s.axis.y);
            read(istream, texture_info.s.offset);
            read(istream, texture_info.t.axis.x);
            read(istream, texture_info.t.axis.z);
            read(istream, texture_info.t.axis.y);
            read(istream, texture_info.t.offset);
            read(istream, texture_info.texture_index);
            read(istream, texture_info.flags);

            texture_info.s.axis.z = -texture_info.s.axis.z;
            texture_info.t.axis.z = -texture_info.t.axis.z;
        }

        std::vector<index_type> indices;
        std::vector<vertex_type> vertices;

        for (size_t face_index = 0; face_index < faces.size(); ++face_index)
        {
            const auto& face = faces[face_index];

            auto normal = planes[face.plane_index].plane.normal;

            if (face.plane_side != 0)
            {
                normal = -normal;
            }

            face_start_indices.push_back(indices.size());

            auto& texture_info = texture_infos[face.texture_info_index];
            auto& bsp_texture = bsp_textures[texture_info.texture_index];

            for (auto i = 0; i < face.surface_edge_count; ++i)
            {
                vertex_t vertex;

                vertex.normal = normal;

                auto edge_index = surface_edges[face.surface_edge_start_index + i];

                if (edge_index > 0)
                {
                    vertex.location = vertex_locations[edges[edge_index].vertex_indices[0]];
                }
                else
                {
                    edge_index = -edge_index;

                    vertex.location = vertex_locations[edges[edge_index].vertex_indices[1]];
                }

                vertex.diffuse_texcoord.x = (glm::dot(vertex.location, texture_info.s.axis) + texture_info.s.offset) / bsp_texture.width;
                vertex.diffuse_texcoord.y = -(glm::dot(vertex.location, texture_info.t.axis) + texture_info.t.offset) / bsp_texture.height;

                vertices.push_back(vertex);
                indices.push_back(static_cast<uint32_t>(indices.size()));
            }
        }

        //lighting
		const auto& lighting_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::LIGHTING)];
        istream.seekg(lighting_chunk.offset, std::ios_base::beg);

        std::vector<uint8_t> lighting_data;
        read(istream, lighting_data, lighting_chunk.length);

        face_lightmap_textures.resize(faces.size());

        for (size_t face_index = 0; face_index < faces.size(); ++face_index)
        {
            auto& face = faces[face_index];

            if (face.lighting_styles[0] == 0 && static_cast<int32_t>(face.lightmap_offset) >= -1)
            {
                auto min_u = std::numeric_limits<float32_t>::max();
                auto min_v = std::numeric_limits<float32_t>::max();
                auto max_u = -std::numeric_limits<float32_t>::max();
                auto max_v = -std::numeric_limits<float32_t>::max();

                auto& texture_info = texture_infos[face.texture_info_index];

                for (auto surface_edge_index = 0; surface_edge_index < face.surface_edge_count; ++surface_edge_index)
                {
                    auto edge_index = surface_edges[face.surface_edge_start_index + surface_edge_index];
                    vec3_t vertex_location;

                    if (edge_index >= 0)
                    {
                        vertex_location = vertex_locations[edges[edge_index].vertex_indices[0]];
                    }
                    else
                    {
                        vertex_location = vertex_locations[edges[-edge_index].vertex_indices[1]];
                    }

                    auto u = glm::dot(texture_info.s.axis, vertex_location) + texture_info.s.offset;

                    min_u = glm::min(u, min_u);
                    max_u = glm::max(u, max_u);

                    auto v = glm::dot(texture_info.t.axis, vertex_location) + texture_info.t.offset;

                    min_v = glm::min(v, min_v);
                    max_v = glm::max(v, max_v);
                }

                auto texture_min_u = glm::floor(min_u / 16);
                auto texture_min_v = glm::floor(min_v / 16);
                auto texture_max_u = glm::ceil(max_u / 16);
                auto texture_max_v = glm::ceil(max_v / 16);

                vec2_t texture_size;
                texture_size.x = texture_max_u - texture_min_u + 1;
                texture_size.y = texture_max_v - texture_min_v + 1;

                for (auto surface_edge_index = 0; surface_edge_index < face.surface_edge_count; ++surface_edge_index)
                {
                    auto edge_index = surface_edges[face.surface_edge_start_index + surface_edge_index];

                    vec3_t vertex_location;

                    if (edge_index >= 0)
                    {
                        vertex_location = vertex_locations[edges[edge_index].vertex_indices[0]];
                    }
                    else
                    {
                        vertex_location = vertex_locations[edges[-edge_index].vertex_indices[1]];
                    }

                    auto u = glm::dot(texture_info.s.axis, vertex_location) + texture_info.s.offset;
                    auto v = glm::dot(texture_info.t.axis, vertex_location) + texture_info.t.offset;

                    auto lightmap_u = (texture_size.x / 2) + (u - ((min_u + max_u) / 2)) / 16;
                    auto lightmap_v = (texture_size.y / 2) + (v - ((min_v + max_v) / 2)) / 16;

                    auto& lightmap_texcoord = vertices[face_start_indices[face_index] + surface_edge_index].lightmap_texcoord;
                    lightmap_texcoord.x = lightmap_u / texture_size.x;
                    lightmap_texcoord.y = lightmap_v / texture_size.y;
                }

                auto lighting_data_size = 3 * static_cast<int32_t>(texture_size.x) * static_cast<int32_t>(texture_size.y);

                auto image = boost::make_shared<image_t>(
                    static_cast<image_t::size_type>(texture_size),
                    8,
                    color_type_e::rgb,
                    lighting_data.data() + face.lightmap_offset,
                    lighting_data_size);

                auto lightmap_texture = boost::make_shared<texture_t>(image);

                face_lightmap_textures[face_index] = lightmap_texture;
            }
        }

        //entities
		const auto& entities_chunk = chunks[static_cast<size_t>(bsp_chunk_t::type_e::ENTITIES)];
        istream.seekg(entities_chunk.offset, std::ios_base::beg);

        std::vector<char> entities_buffer;
        read(istream, entities_buffer, entities_chunk.length);

        std::string entities_string = entities_buffer.data();
        size_t end = -1;

        for (;;)
        {
            auto begin = entities_string.find_first_of('{', end + 1);

            if (begin == -1)
            {
                break;
            }

            end = entities_string.find_first_of('}', begin + 1);

            auto entity_string = entities_string.substr(begin + 1, end - begin - 1);

            bsp_entity_t entity(entity_string);

            auto model_optional = entity.get_optional<std::string>("model");

            if (model_optional)
            {
                auto model = model_optional.get();

                if (boost::algorithm::starts_with(model, "*"))
                {
                    brush_entity_indices.push_back(entities.size());
                }
            }

            entities.emplace_back(std::move(entity));
        }

        vertex_buffer = gpu_buffers.make<vertex_buffer_type>().lock();
        vertex_buffer->data(vertices, gpu_t::buffer_usage_e::STATIC_DRAW);

        index_buffer = gpu_buffers.make<index_buffer_type>().lock();
        index_buffer->data(indices, gpu_t::buffer_usage_e::STATIC_DRAW);
    }

    void bsp_t::render(const camera_t& camera)
    {
        static boost::dynamic_bitset<> faces_rendered = boost::dynamic_bitset<>(faces.size());

        faces_rendered.reset();

        auto camera_leaf_index = get_leaf_index_from_location(camera.pose.location);

        //culling
        auto culling_state = gpu.culling.get_state();
        culling_state.is_enabled = true;
        culling_state.mode = gpu_t::culling_mode_e::FRONT;

        gpu.culling.push_state(culling_state);

		//blend
        auto blend_state = gpu.blend.get_state();
		blend_state.is_enabled = false;

        gpu.blend.push_state(blend_state);

		static const auto DIFFUSE_TEXTURE_INDEX = 0;
		static const auto LIGHTMAP_TEXTURE_INDEX = 1;

        //bind buffers
        gpu.buffers.push(gpu_t::buffer_target_e::ARRAY, vertex_buffer);
		gpu.buffers.push(gpu_t::buffer_target_e::ELEMENT_ARRAY, index_buffer);

		//bind program
        const auto gpu_program = gpu_programs.get<bsp_gpu_program_t>();

        gpu.programs.push(gpu_program);

        gpu.set_uniform("world_matrix", mat4_t());
        gpu.set_uniform("view_projection_matrix", camera.get_projection_matrix() * camera.get_view_matrix());
        gpu.set_uniform("diffuse_texture", DIFFUSE_TEXTURE_INDEX);
        gpu.set_uniform("lightmap_texture", LIGHTMAP_TEXTURE_INDEX);
        gpu.set_uniform("lightmap_gamma", render_settings.lightmap_gamma);

        auto render_face = [&](int32_t face_index)
        {
            if (faces_rendered[face_index])
            {
                return;
            }

            const auto& face = faces[face_index];

            if (face.lighting_styles[0] == face_t::LIGHTING_STYLE_NONE)
            {
                return;
            }

            const auto& diffuse_texture = textures[texture_infos[face.texture_info_index].texture_index];
            const auto& lightmap_texture = face_lightmap_textures[face_index];

            gpu.textures.bind(DIFFUSE_TEXTURE_INDEX, diffuse_texture);
            gpu.textures.bind(LIGHTMAP_TEXTURE_INDEX, lightmap_texture);

            gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLE_FAN,
                face.surface_edge_count,
                index_buffer_type::DATA_TYPE,
                face_start_indices[face_index] * sizeof(index_type));

            faces_rendered[face_index] = true;

            ++render_stats.face_count;
        };

        auto render_leaf = [&](node_index_type leaf_index)
        {
            const auto& leaf = leafs[leaf_index];

            for (auto i = 0; i < leaf.mark_surface_count; ++i)
            {
                render_face(mark_surfaces[leaf.mark_surface_start_index + i]);
            }

            ++render_stats.leaf_count;
        };

		std::function<void(node_index_type, node_index_type)> render_node = [&](node_index_type node_index, node_index_type camera_leaf_index)
        {
            if (node_index < 0)
            {
                if (node_index == -1)
                {
                    return;
                }

                auto leaf_pvs_map_itr = leaf_pvs_map.find(camera_leaf_index - 1);

                if (camera_leaf_index > 0 &&
                    leaf_pvs_map_itr != leaf_pvs_map.end() &&
                    !leaf_pvs_map[camera_leaf_index - 1][~node_index - 1])
                {
                    return;
                }

                render_leaf(~node_index);

                return;
            }

            const auto& node = nodes[node_index];
			const auto& plane = planes[node.plane_index];
			float32_t distance = 0;

            switch (plane.type)
            {
            case plane_t::type_e::x:
                distance = camera.pose.location.x - plane.plane.distance;
                break;
            case plane_t::type_e::y:
                distance = camera.pose.location.y - plane.plane.distance;
                break;
            case plane_t::type_e::z:
                distance = camera.pose.location.z - plane.plane.distance;
                break;
            default:
                distance = glm::dot(plane.plane.normal, camera.pose.location) - plane.plane.distance;
            }

            if (distance > 0)
            {
				render_node(node.child_indices[1], camera_leaf_index);
				render_node(node.child_indices[0], camera_leaf_index);
            }
            else
            {
				render_node(node.child_indices[0], camera_leaf_index);
				render_node(node.child_indices[1], camera_leaf_index);
            }
        };

        auto render_brush_entity = [&](int32_t entity_index)
        {
			return;

            const auto& entity = entities[entity_index];
            const auto model_index = boost::lexical_cast<int32_t>(entity.get("model").substr(1));
            const auto& model = models[model_index];

            //render mode
            auto render_mode_optional = entity.get_optional<int32_t>("rendermode");

            render_mode_e render_mode = render_mode_e::NORMAL;

            if (render_mode_optional)
            {
                render_mode = static_cast<render_mode_e>(render_mode_optional.get());
            }

            //alpha
            float32_t alpha = 1.0f;

            auto alpha_optional = entity.get_optional<uint8_t>("renderamt");

            if (alpha_optional)
            {
                alpha = static_cast<float32_t>(alpha_optional.get()) / 255.0f;
            }

            //origin
            vec3_t origin;

            auto origin_optional = entity.get_optional("origin");

            if (origin_optional)
            {
                std::vector<std::string> tokens;
                boost::algorithm::split(tokens, origin_optional.get(), boost::is_any_of(" "), boost::algorithm::token_compress_on);

                origin.x = boost::lexical_cast<float32_t>(tokens[0]);
                origin.y = boost::lexical_cast<float32_t>(tokens[2]);
                origin.z = -boost::lexical_cast<float32_t>(tokens[1]);
			}

			//color
			vec4_t color(1.0f);

			auto color_optional = entity.get_optional("rendercolor");

			if (color_optional)
			{
			    std::vector<std::string> tokens;
			    boost::algorithm::split(tokens, color_optional.get(), boost::is_any_of(" "), boost::algorithm::token_compress_on);

			    color.r = boost::lexical_cast<float32_t>(tokens[0]) / 255.0f;
			    color.g = boost::lexical_cast<float32_t>(tokens[1]) / 255.0f;
			    color.b = boost::lexical_cast<float32_t>(tokens[2]) / 255.0f;
			}

			auto blend_state = gpu.blend.get_state();
			auto depth_state = gpu.depth.get_state();

			depth_state.should_test = true;

            const auto gpu_program = gpu_programs.get<bsp_gpu_program_t>();

            switch (render_mode)
            {
            case render_mode_e::TEXTURE:
                gpu.set_uniform("alpha", 0.0f);
				blend_state.is_enabled = true;
				blend_state.src_factor = gpu_t::blend_factor_e::SRC_ALPHA;
				blend_state.dst_factor = gpu_t::blend_factor_e::ONE;
                break;
            case render_mode_e::SOLID:
                gpu.set_uniform("should_test_alpha", 1);
                break;
            case render_mode_e::ADDITIVE:
                gpu.set_uniform("alpha", alpha);
				blend_state.is_enabled = true;
				blend_state.src_factor = gpu_t::blend_factor_e::ONE;
				blend_state.dst_factor = gpu_t::blend_factor_e::ONE;
				depth_state.should_write_mask = false;
                break;
            default:
				blend_state.is_enabled = false;
				depth_state.should_write_mask = true;
                break;
            }

			gpu.blend.push_state(blend_state);
            gpu.depth.push_state(depth_state);

            auto world_matrix = glm::translate(model.origin);
            world_matrix *= glm::translate(origin);

            gpu.set_uniform("world_matrix", world_matrix);

            render_node(model.head_node_indices[0], -1);

            switch (render_mode)
            {
            case render_mode_e::TEXTURE:
            case render_mode_e::ADDITIVE:
                gpu.set_uniform("alpha", 1.0f);
                break;
            case render_mode_e::SOLID:
                gpu.set_uniform("should_test_alpha", 0);
                break;
            default:
                break;
            }

			gpu.depth.pop_state();
            gpu.blend.pop_state();
        };

        render_stats.reset();

		//depth
        auto depth_state = gpu.depth.get_state();
		depth_state.should_test = true;

        gpu.depth.push_state(depth_state);

        render_node(0, camera_leaf_index);

        gpu.depth.pop_state();

        for (auto brush_entity_index : brush_entity_indices)
        {
            render_brush_entity(brush_entity_index);
		}

		gpu.programs.pop();

        gpu.buffers.pop(gpu_t::buffer_target_e::ELEMENT_ARRAY);
        gpu.buffers.pop(gpu_t::buffer_target_e::ARRAY);

        gpu.culling.pop_state();

        gpu.blend.pop_state();
    }

    int32_t bsp_t::get_leaf_index_from_location(const vec3_t& location) const
    {
        node_index_type node_index = 0;

        while (node_index >= 0)
        {
            const auto& node = nodes[node_index];
            const auto& plane = planes[node.plane_index].plane;

            if (glm::dot(plane.normal, (location - plane.origin())) >= 0)
            {
                node_index = node.child_indices[0];
            }
            else
            {
                node_index = node.child_indices[1];
            }
        }

        return ~node_index;
    }

    //bsp_t::trace_result_t bsp_t::trace(const trace_args_t& trace_args) const
    //{
    //	trace_result_t trace_result;

    //	auto hull_point_contents = [&](size_t node_index, vec3_t point)
    //	{
    //		float32_t d = 0;

    //		while (node_index >= 0)
    //		{
    //			const auto& node = clip_nodes[node_index];
    //			const auto& plane = planes[node.plane_index];

    //			d = glm::dot(plane.plane.normal, point) - plane.plane.distance;
    //			
    //			if (d < 0)
    //			{
    //				node_index = node.children[1];
    //			}
    //			else
    //			{
    //				node_index = node.children[0];
    //			}
    //		}
    //	};

    //	std::function<bool(size_t, float, float, vec3_t, vec3_t)> recursive_hull_check = [&](size_t node_index, float p1, float p2, vec3_t start, vec3_t end)
    //	{
    //		if (node_index < 0)
    //		{
    //			auto leaf_index = ~node_index;

    //			const auto& node = leafs[leaf_index];

    //			if (node.content_type != content_type_e::solid)
    //			{
    //				trace_result.is_all_solid = false;
    //			}

    //			return true;
    //		}

    //		const auto& node = nodes[node_index];
    //		const auto& plane = planes[node.plane_index];

    //		auto t1 = glm::dot(plane.plane.normal, start) - plane.plane.distance;
    //		auto t2 = glm::dot(plane.plane.normal, end) - plane.plane.distance;

    //		if (t1 >= 0 && t2 >= 0)
    //		{
    //			return recursive_hull_check(node.children[0], p1, p2, start, end);
    //		}

    //		if (t1 < 0 && t2 < 0)
    //		{
    //			return recursive_hull_check(node.children[1], p1, p2, start, end);
    //		}

    //		float32_t frac;

    //		if (t1 < 0)
    //		{
    //			frac = (t1 + glm::epsilon<float32_t>()) / (t1 - t2);
    //		}
    //		else
    //		{
    //			frac = (t1 - glm::epsilon<float32_t>()) / (t1 - t2);
    //		}

    //		frac = glm::clamp(frac, 0.0f, 1.0f);

    //		auto midf = p1 + (p2 - p1) * frac;
    //		auto mid = start + (end - start) * frac;

    //		auto child_index = t1 < 0 ? 1 : 0;

    //		if (!recursive_hull_check(node.children[child_index], p1, midf, mid, end))
    //		{
    //			return false;
    //		}

    //		if (hull_point_contents(node.children[child_index ^ 1], mid) != content_type_e::solid)
    //		{
    //			return recursive_hull_check(node.children[child_index ^ 1], midf, p2, mid, end);
    //		}

    //		if (trace_result.is_all_solid)
    //		{
    //			return false;
    //		}

    //		if (child_index == 0)
    //		{
    //			trace_result.plane = plane.plane;
    //		}
    //		else
    //		{
    //			trace_result.plane = -plane.plane;
    //		}

    //		while (hull_point_contents(models.front().head_nodes[0], mid) == content_type_e::solid)
    //		{
    //			frac -= 0.1f;

    //			if (frac < 0)
    //			{
    //				trace_result.ratio = midf;

    //				return false;
    //			}

    //			midf = p1 + (p2 - p1) * frac;
    //			mid = p1 + (end - start) * frac;
    //		}

    //		trace_result.ratio = midf;

    //		return false;
    //	};

    //	recursive_hull_check(models.front().head_nodes[0], 0, 1, trace_args.line.start, trace_args.line.end);

    //	if (trace_result.ratio == 1.0f)
    //	{
    //		trace_result.did_hit = false;
    //	}
    //	else
    //	{
    //		trace_result.hit_position = trace_args.line.start + (trace_args.line.end - trace_args.line.start) * trace_result.ratio;
    //		trace_result.hit_normal = trace_result.
    //	}

    //	return result;
    //}
}
