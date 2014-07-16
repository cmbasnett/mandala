//glm
#include "glm\ext.hpp"

//mandala
#include "bsp.hpp"
#include "camera.hpp"
#include "app.hpp"
#include "gpu_program.hpp"
#include "image.hpp"
#include "gpu_mgr.hpp"

#define BSP_VERSION (30)

namespace mandala
{
	bsp_t::bsp_t(std::istream& istream)
	{
		if (!istream.good())
		{
			throw std::exception();
		}

		//version
		int32_t version;

		istream.read(reinterpret_cast<char*>(&version), sizeof(version));

        if (version != BSP_VERSION)
		{
			throw std::exception();
		}

		//chunk_infos
		std::vector<chunk_info_t> chunk_infos;

		chunk_infos.resize((size_t)chunk_type_e::count);

		for (auto& chunk_info : chunk_infos)
		{
			istream.read(reinterpret_cast<char*>(&chunk_info.offset), sizeof(chunk_info.offset));
			istream.read(reinterpret_cast<char*>(&chunk_info.length), sizeof(chunk_info.length));
		}

		//planes
		const auto& plane_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::planes)];
		istream.seekg(plane_chunk_info.offset, std::ios_base::beg);

		auto plane_count = plane_chunk_info.length / sizeof(plane_t);
		planes.resize(plane_count);

		for (auto& plane : planes)
		{
			istream.read(reinterpret_cast<char*>(&plane.plane.normal.x), sizeof(plane.plane.normal.x));
			istream.read(reinterpret_cast<char*>(&plane.plane.normal.z), sizeof(plane.plane.normal.z));
			istream.read(reinterpret_cast<char*>(&plane.plane.normal.y), sizeof(plane.plane.normal.y));
			istream.read(reinterpret_cast<char*>(&plane.plane.distance), sizeof(plane.plane.distance));
			istream.read(reinterpret_cast<char*>(&plane.type), sizeof(plane.type));

			plane.plane.normal.z = -plane.plane.normal.z;
		}

		//vertex_positions
		const auto& vertices_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::vertices)];
		istream.seekg(vertices_chunk_info.offset, std::ios_base::beg);

		auto vertex_position_count = vertices_chunk_info.length / sizeof(vec3_t);
		vertex_positions.resize(vertex_position_count);

		for (auto& vertex_position : vertex_positions)
		{
			istream.read(reinterpret_cast<char*>(&vertex_position.x), sizeof(vertex_position.x));
			istream.read(reinterpret_cast<char*>(&vertex_position.z), sizeof(vertex_position.z));
			istream.read(reinterpret_cast<char*>(&vertex_position.y), sizeof(vertex_position.y));

			vertex_position.z = -vertex_position.z;
		}

		//edges
		const auto& edges_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::edges)];
		istream.seekg(edges_chunk_info.offset, std::ios_base::beg);

		auto edge_count = edges_chunk_info.length / sizeof(edge_t);
		edges.resize(edge_count);

		for (auto& edge : edges)
		{
			istream.read(reinterpret_cast<char*>(&edge.vertex_indices[0]), sizeof(edge.vertex_indices[0]));
			istream.read(reinterpret_cast<char*>(&edge.vertex_indices[1]), sizeof(edge.vertex_indices[1]));
		}

		//surface_edges
		const auto& surface_edges_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::surface_edges)];
		istream.seekg(surface_edges_chunk_info.offset, std::ios_base::beg);

		auto surface_edge_count = surface_edges_chunk_info.length / sizeof(int32_t);
		surface_edges.resize(surface_edge_count);

		for (auto& surface_edge : surface_edges)
		{
			istream.read(reinterpret_cast<char*>(&surface_edge), sizeof(surface_edge));
		}

		//faces
        const auto& faces_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::faces)];
		istream.seekg(faces_chunk_info.offset, std::ios_base::beg);

		auto face_count = faces_chunk_info.length / sizeof(face_t);
		faces.resize(face_count);

		for (auto& face : faces)
		{
			istream.read(reinterpret_cast<char*>(&face.plane_index), sizeof(face.plane_index));
			istream.read(reinterpret_cast<char*>(&face.plane_side), sizeof(face.plane_side));
			istream.read(reinterpret_cast<char*>(&face.surface_edge_start_index), sizeof(face.surface_edge_start_index));
            istream.read(reinterpret_cast<char*>(&face.surface_edge_count), sizeof(face.surface_edge_count));
            istream.read(reinterpret_cast<char*>(&face.texture_info_index), sizeof(face.texture_info_index));
            istream.read(reinterpret_cast<char*>(&face.lighting_styles[0]), sizeof(face.lighting_styles[0]));
            istream.read(reinterpret_cast<char*>(&face.lighting_styles[1]), sizeof(face.lighting_styles[1]));
            istream.read(reinterpret_cast<char*>(&face.lighting_styles[2]), sizeof(face.lighting_styles[2]));
            istream.read(reinterpret_cast<char*>(&face.lighting_styles[3]), sizeof(face.lighting_styles[3]));
            istream.read(reinterpret_cast<char*>(&face.lightmap_offset), sizeof(face.lightmap_offset));
		}

		//nodes
		const auto& nodes_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::nodes)];
		istream.seekg(nodes_chunk_info.offset, std::ios_base::beg);

		auto node_count = nodes_chunk_info.length / sizeof(node_t);
		nodes.resize(node_count);

		for (auto& node : nodes)
		{
			istream.read(reinterpret_cast<char*>(&node.plane_index), sizeof(node.plane_index));
			istream.read(reinterpret_cast<char*>(&node.children[0]), sizeof(node.children[0]));
			istream.read(reinterpret_cast<char*>(&node.children[1]), sizeof(node.children[1]));
			istream.read(reinterpret_cast<char*>(&node.aabb.min.x), sizeof(node.aabb.min.x));
			istream.read(reinterpret_cast<char*>(&node.aabb.min.z), sizeof(node.aabb.min.z));
			istream.read(reinterpret_cast<char*>(&node.aabb.min.y), sizeof(node.aabb.min.y));
			istream.read(reinterpret_cast<char*>(&node.aabb.max.x), sizeof(node.aabb.max.x));
			istream.read(reinterpret_cast<char*>(&node.aabb.max.z), sizeof(node.aabb.max.z));
			istream.read(reinterpret_cast<char*>(&node.aabb.max.y), sizeof(node.aabb.max.y));
			istream.read(reinterpret_cast<char*>(&node.face_start_index), sizeof(node.face_start_index));
			istream.read(reinterpret_cast<char*>(&node.face_count), sizeof(node.face_count));

			node.aabb.min.z = -node.aabb.min.z;
			node.aabb.max.z = -node.aabb.max.z;
		}

		//leaves
		const auto& leaves_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::leaves)];
		istream.seekg(leaves_chunk_info.offset, std::ios_base::beg);

		auto leaf_count = leaves_chunk_info.length / sizeof(leaf_t);
		leafs.resize(leaf_count);

		for (auto& leaf : leafs)
		{
			istream.read(reinterpret_cast<char*>(&leaf.content_type), sizeof(leaf.content_type));
			istream.read(reinterpret_cast<char*>(&leaf.visibility_offset), sizeof(leaf.visibility_offset));
			istream.read(reinterpret_cast<char*>(&leaf.aabb.min.x), sizeof(leaf.aabb.min.x));
			istream.read(reinterpret_cast<char*>(&leaf.aabb.min.z), sizeof(leaf.aabb.min.z));
			istream.read(reinterpret_cast<char*>(&leaf.aabb.min.y), sizeof(leaf.aabb.min.y));
			istream.read(reinterpret_cast<char*>(&leaf.aabb.max.x), sizeof(leaf.aabb.max.x));
			istream.read(reinterpret_cast<char*>(&leaf.aabb.max.z), sizeof(leaf.aabb.max.z));
			istream.read(reinterpret_cast<char*>(&leaf.aabb.max.y), sizeof(leaf.aabb.max.y));
			istream.read(reinterpret_cast<char*>(&leaf.mark_surface_start_index), sizeof(leaf.mark_surface_start_index));
			istream.read(reinterpret_cast<char*>(&leaf.mark_surface_count), sizeof(leaf.mark_surface_count));
			istream.read(reinterpret_cast<char*>(&leaf.ambient_sound_levels[0]), sizeof(leaf.ambient_sound_levels[0]));
			istream.read(reinterpret_cast<char*>(&leaf.ambient_sound_levels[1]), sizeof(leaf.ambient_sound_levels[1]));
			istream.read(reinterpret_cast<char*>(&leaf.ambient_sound_levels[2]), sizeof(leaf.ambient_sound_levels[2]));
			istream.read(reinterpret_cast<char*>(&leaf.ambient_sound_levels[3]), sizeof(leaf.ambient_sound_levels[3]));

			leaf.aabb.min.z = -leaf.aabb.min.z;
			leaf.aabb.max.z = -leaf.aabb.max.z;
		}

		//mark_surfaces
		const auto& mark_surfaces_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::mark_surfaces)];
		istream.seekg(mark_surfaces_chunk_info.offset, std::ios_base::beg);

		auto mark_surface_count = mark_surfaces_chunk_info.length / sizeof(uint16_t);
		mark_surfaces.resize(mark_surface_count);

		for (auto& mark_surface : mark_surfaces)
		{
			istream.read(reinterpret_cast<char*>(&mark_surface), sizeof(mark_surface));
		}

		//clip_nodes
		const auto& clip_nodes_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::clip_nodes)];
		istream.seekg(clip_nodes_chunk_info.offset, std::ios_base::beg);

		auto clip_node_count = clip_nodes_chunk_info.length / sizeof(clip_node_t);
		clip_nodes.resize(clip_node_count);

		for (auto& clip_node : clip_nodes)
		{
			istream.read(reinterpret_cast<char*>(&clip_node.plane_index), sizeof(clip_node.plane_index));
			istream.read(reinterpret_cast<char*>(&clip_node.children[0]), sizeof(clip_node.children[0]));
			istream.read(reinterpret_cast<char*>(&clip_node.children[1]), sizeof(clip_node.children[1]));
		}

		//models
		const auto& models_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::models)];
		istream.seekg(models_chunk_info.offset, std::ios_base::beg);

		auto model_count = models_chunk_info.length / sizeof(model_t);
		models.resize(model_count);

		for (auto& model : models)
		{
			istream.read(reinterpret_cast<char*>(&model.aabb.min.x), sizeof(model.aabb.min.x));
			istream.read(reinterpret_cast<char*>(&model.aabb.min.z), sizeof(model.aabb.min.z));
			istream.read(reinterpret_cast<char*>(&model.aabb.min.y), sizeof(model.aabb.min.y));
			istream.read(reinterpret_cast<char*>(&model.aabb.max.x), sizeof(model.aabb.max.x));
			istream.read(reinterpret_cast<char*>(&model.aabb.max.z), sizeof(model.aabb.max.z));
			istream.read(reinterpret_cast<char*>(&model.aabb.max.y), sizeof(model.aabb.max.y));
			istream.read(reinterpret_cast<char*>(&model.origin.x), sizeof(model.origin.x));
			istream.read(reinterpret_cast<char*>(&model.origin.z), sizeof(model.origin.z));
			istream.read(reinterpret_cast<char*>(&model.origin.y), sizeof(model.origin.y));
			istream.read(reinterpret_cast<char*>(&model.head_nodes[0]), sizeof(model.head_nodes[0]));
			istream.read(reinterpret_cast<char*>(&model.head_nodes[1]), sizeof(model.head_nodes[1]));
			istream.read(reinterpret_cast<char*>(&model.head_nodes[2]), sizeof(model.head_nodes[2]));
			istream.read(reinterpret_cast<char*>(&model.head_nodes[3]), sizeof(model.head_nodes[3]));
			istream.read(reinterpret_cast<char*>(&model.vis_leafs), sizeof(model.vis_leafs));
			istream.read(reinterpret_cast<char*>(&model.face_start_index), sizeof(model.face_start_index));
			istream.read(reinterpret_cast<char*>(&model.face_count), sizeof(model.face_count));

			model.aabb.min.z = -model.aabb.min.z;
			model.aabb.max.z = -model.aabb.max.z;
			model.origin.z = -model.origin.z;
		}

		//visibility
		std::function<void(int32_t)> count_vis_leaves = [&](int32_t node_index)
		{
			if (node_index < 0)
			{
				if (node_index == -1 || leafs[~node_index].content_type == content_type_e::solid)
				{
					return;
				}

				++vis_leaf_count;

				return;
			}

			count_vis_leaves(nodes[node_index].children[0]);
			count_vis_leaves(nodes[node_index].children[1]);
		};

		count_vis_leaves(0);

		const auto& visibility_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::visibliity)];
		istream.seekg(visibility_chunk_info.offset, std::ios_base::beg);

		std::vector<uint8_t> visibility_data;
		visibility_data.resize(visibility_chunk_info.length, '\0');
		istream.read(reinterpret_cast<char*>(visibility_data.data()), visibility_data.size());

		for (size_t i = 0; i < vis_leaf_count; ++i)
		{
			const auto& leaf = leafs[i + 1];

			if (leaf.visibility_offset == 0)
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
						if ((*visibility_data_itr & mask) && (leaf_pvs_index < leaf_count))
						{
							leaf_pvs[leaf_pvs_index] = true;
						}
					}
				}

				++visibility_data_itr;
			}

			leaf_pvs_map.insert(std::make_pair(i, std::move(leaf_pvs)));
		}

		//textures
		const auto& textures_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::textures)];
		istream.seekg(textures_chunk_info.offset, std::ios_base::beg);

		uint32_t texture_count;

		istream.read(reinterpret_cast<char*>(&texture_count), sizeof(texture_count));

		std::vector<uint32_t> texture_offsets;
		texture_offsets.resize(texture_count);

		for (auto& texture_offset : texture_offsets)
		{
			istream.read(reinterpret_cast<char*>(&texture_offset), sizeof(texture_offset));
		}

		std::vector<bsp_texture_t> bsp_textures;

		for (uint32_t i = 0; i < texture_count; ++i)
		{
			istream.seekg(textures_chunk_info.offset + texture_offsets[i], std::ios_base::beg);

			char texture_name_bytes[16] = { '\0' };
			istream.read(reinterpret_cast<char*>(texture_name_bytes), sizeof(texture_name_bytes));

			std::string texture_name = texture_name_bytes;

			bsp_texture_t bsp_texture;

			istream.read(reinterpret_cast<char*>(&bsp_texture.width), sizeof(bsp_texture.width));
			istream.read(reinterpret_cast<char*>(&bsp_texture.height), sizeof(bsp_texture.height));
			istream.read(reinterpret_cast<char*>(bsp_texture.mipmap_offsets), sizeof(bsp_texture.mipmap_offsets));

			bsp_textures.push_back(bsp_texture);

			texture_name.append(".png");

			std::shared_ptr<texture_t> texture;

			try
			{
				texture = app.resources.get<texture_t>(hash_t(texture_name));
			}
			catch (...)
			{
				std::cerr << "couldn't load " << texture_name << std::endl;
			}

			textures.push_back(texture);
		}

		//texture_info
		const auto& texture_info_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::texture_info)];
		istream.seekg(texture_info_chunk_info.offset, std::ios_base::beg);

		auto texture_info_count = texture_info_chunk_info.length / sizeof(texture_info_t);

		texture_infos.resize(texture_info_count);

		for (auto& texture_info : texture_infos)
		{
			istream.read(reinterpret_cast<char*>(&texture_info.s.axis.x), sizeof(texture_info.s.axis.x));
			istream.read(reinterpret_cast<char*>(&texture_info.s.axis.z), sizeof(texture_info.s.axis.z));
			istream.read(reinterpret_cast<char*>(&texture_info.s.axis.y), sizeof(texture_info.s.axis.y));
			istream.read(reinterpret_cast<char*>(&texture_info.s.offset), sizeof(texture_info.s.offset));
			istream.read(reinterpret_cast<char*>(&texture_info.t.axis.x), sizeof(texture_info.t.axis.x));
			istream.read(reinterpret_cast<char*>(&texture_info.t.axis.z), sizeof(texture_info.t.axis.z));
			istream.read(reinterpret_cast<char*>(&texture_info.t.axis.y), sizeof(texture_info.t.axis.y));
			istream.read(reinterpret_cast<char*>(&texture_info.t.offset), sizeof(texture_info.t.offset));
			istream.read(reinterpret_cast<char*>(&texture_info.texture_index), sizeof(texture_info.texture_index));
			istream.read(reinterpret_cast<char*>(&texture_info.flags), sizeof(texture_info.flags));

			texture_info.s.axis.z = -texture_info.s.axis.z;
			texture_info.t.axis.z = -texture_info.t.axis.z;
		}

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
					vertex.position = vertex_positions[edges[edge_index].vertex_indices[0]];
				}
				else
				{
					edge_index = -edge_index;

					vertex.position = vertex_positions[edges[edge_index].vertex_indices[1]];
				}

				vertex.diffuse_texcoord.x = (glm::dot(vertex.position, texture_info.s.axis) + texture_info.s.offset) / bsp_texture.width;
				vertex.diffuse_texcoord.y = -(glm::dot(vertex.position, texture_info.t.axis) + texture_info.t.offset) / bsp_texture.height;

				vertices.push_back(vertex);
                indices.push_back(static_cast<uint32_t>(indices.size()));
			}
		}

		//lighting
		const auto& lighting_chunk_info = chunk_infos[static_cast<size_t>(chunk_type_e::lighting)];
		istream.seekg(lighting_chunk_info.offset, std::ios_base::beg);

		std::vector<uint8_t> lighting_data;
		lighting_data.resize(lighting_chunk_info.length);

		istream.read(reinterpret_cast<char*>(lighting_data.data()), lighting_data.size());

		face_lightmap_textures.resize(faces.size());

		for (size_t face_index = 0; face_index < faces.size(); ++face_index)
		{
			auto& face = faces[face_index];

			if (face.lighting_styles[0] == 0 && (int32_t)face.lightmap_offset >= -1)
			{
				auto min_u = std::numeric_limits<float32_t>::max();
				auto min_v = std::numeric_limits<float32_t>::max();
				auto max_u = -std::numeric_limits<float32_t>::max();
				auto max_v = -std::numeric_limits<float32_t>::max();

				auto& texture_info = texture_infos[face.texture_info_index];

				for (auto surface_edge_index = 0; surface_edge_index < face.surface_edge_count; ++surface_edge_index)
				{
					auto edge_index = surface_edges[face.surface_edge_start_index + surface_edge_index];
					vec3_t vertex_position;

					if (edge_index >= 0)
					{
						vertex_position = vertex_positions[edges[edge_index].vertex_indices[0]];
					}
					else
					{
						vertex_position = vertex_positions[edges[-edge_index].vertex_indices[1]];
					}

					auto u = glm::dot(texture_info.s.axis, vertex_position) + texture_info.s.offset;

					min_u = glm::min(u, min_u);
					max_u = glm::max(u, max_u);

					auto v = glm::dot(texture_info.t.axis, vertex_position) + texture_info.t.offset;

					min_v = glm::min(v, min_v);
					max_v = glm::max(v, max_v);
				}

				auto texture_min_u = glm::floor(min_u / 16);
				auto texture_min_v = glm::floor(min_v / 16);
				auto texture_max_u = glm::ceil(max_u / 16);
				auto texture_max_v = glm::ceil(max_v / 16);

				auto texture_width = texture_max_u - texture_min_u + 1;
				auto texture_height = texture_max_v - texture_min_v + 1;

				for (auto surface_edge_index = 0; surface_edge_index < face.surface_edge_count; ++surface_edge_index)
				{
					auto edge_index = surface_edges[face.surface_edge_start_index + surface_edge_index];

					vec3_t vertex_position;

					if (edge_index >= 0)
					{
						vertex_position = vertex_positions[edges[edge_index].vertex_indices[0]];
					}
					else
					{
						vertex_position = vertex_positions[edges[-edge_index].vertex_indices[1]];
					}

					auto u = glm::dot(texture_info.s.axis, vertex_position) + texture_info.s.offset;
					auto v = glm::dot(texture_info.t.axis, vertex_position) + texture_info.t.offset;

					auto lightmap_u = (texture_width / 2) + (u - ((min_u + max_u) / 2)) / 16;
					auto lightmap_v = (texture_height / 2) + (v - ((min_v + max_v) / 2)) / 16;

					auto& lightmap_texcoord = vertices[face_start_indices[face_index] + surface_edge_index].lightmap_texcoord;
					lightmap_texcoord.x = lightmap_u / texture_width;
					lightmap_texcoord.y = lightmap_v / texture_height;
				}

				//TODO: this is a bit of a monstrosity, clean this up
				auto image = std::make_shared<image_t>();
				image->width = (uint32_t)texture_width;
				image->height = (uint32_t)texture_height;
				image->bit_depth = 8;
				image->color_type = color_type_e::rgb;

				auto lighting_data_length = (3 * (int32_t)texture_width * (int32_t)texture_height);
				auto lighting_data_begin = lighting_data.begin() + face.lightmap_offset;
				auto lighting_data_end = lighting_data_begin + lighting_data_length;

				image->data.reserve(lighting_data_length);

				std::copy(lighting_data_begin, lighting_data_end, std::back_inserter(image->data));

				auto lightmap_texture = std::make_shared<texture_t>(image);

				face_lightmap_textures[face_index] = lightmap_texture;
			}
		}

		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex_t), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void bsp_t::render(const camera_t& camera) const
    {
        auto leaf_index = get_leaf_index_from_position(camera.position);

        if (leaf_index <= 0)
        {
            return;
        }

		auto gpu_program = app.resources.get<gpu_program_t>(hash_t("bsp.gpu"));
		auto program_id = gpu_program->id;

		//cull face
		auto is_cull_face_enabled = glIsEnabled(GL_CULL_FACE);
		glEnable(GL_CULL_FACE);

		//cull face mode
		GLint cull_face_mode;
		glGetIntegerv(GL_CULL_FACE_MODE, &cull_face_mode);
		glCullFace(GL_FRONT);

		//blend
		auto is_blend_enabled = glIsEnabled(GL_BLEND);
		glEnable(GL_BLEND);

		//TODO: push/pop blend func thing
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		gpu.programs.push(gpu_program);

        static const auto position_location = glGetAttribLocation(program_id, "position"); glCheckError();
        static const auto diffuse_texcoord_location = glGetAttribLocation(program_id, "diffuse_texcoord"); glCheckError();
        static const auto lightmap_texcoord_location = glGetAttribLocation(program_id, "lightmap_texcoord"); glCheckError();
        static const auto world_location = glGetUniformLocation(program_id, "world"); glCheckError();
        static const auto view_projection_location = glGetUniformLocation(program_id, "view_projection"); glCheckError();
        static const auto diffuse_texture_location = glGetUniformLocation(program_id, "diffuse_texture"); glCheckError();
        static const auto lightmap_texture_location = glGetUniformLocation(program_id, "lightmap_texture"); glCheckError();
        static const auto lightmap_gamma_location = glGetUniformLocation(program_id, "lightmap_gamma"); glCheckError();
		static const auto vertex_size = sizeof(vertex_t);
        static const auto position_offset = reinterpret_cast<void*>(offsetof(vertex_t, position));
        static const auto diffuse_texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_t, diffuse_texcoord));
        static const auto lightmap_texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_t, lightmap_texcoord));

		//bind buffers
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); glCheckError();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer); glCheckError();

		//position
		glEnableVertexAttribArray(position_location); glCheckError();
		glVertexAttribPointer(position_location, 3, GL_FLOAT, false, vertex_size, position_offset); glCheckError();

		//diffuse_texcoord
		glEnableVertexAttribArray(diffuse_texcoord_location); glCheckError();
		glVertexAttribPointer(diffuse_texcoord_location, 2, GL_FLOAT, false, vertex_size, diffuse_texcoord_offset); glCheckError();

		//lightmap_texcoord
		glEnableVertexAttribArray(lightmap_texcoord_location); glCheckError();
		glVertexAttribPointer(lightmap_texcoord_location, 2, GL_FLOAT, false, vertex_size, lightmap_texcoord_offset); glCheckError();

		//world
		glUniformMatrix4fv(world_location, 1, GL_FALSE, glm::value_ptr(mat4_t())); glCheckError();

		//view_projection
		glUniformMatrix4fv(view_projection_location, 1, GL_FALSE, glm::value_ptr(camera.projection_matrix * camera.view_matrix)); glCheckError();

		//diffuse_texture
		glUniform1i(diffuse_texture_location, 0); glCheckError();

		//lightmap_texture
		glUniform1i(lightmap_texture_location, 1); glCheckError();

		//lightmap_gamma
		glUniform1f(lightmap_gamma_location, 1.0f); glCheckError();

		auto& leaf_pvs = leaf_pvs_map.at(leaf_index);

		for (size_t i = 0; i < leaf_pvs.size(); ++i)
		{
			//if (!leaf_pvs[i])
			//{
			//	continue;
			//}

			auto& leaf = leafs[i];

			for (auto j = 0; j < leaf.mark_surface_count; ++j)
			{
				auto face_index = mark_surfaces[leaf.mark_surface_start_index + j];
				auto& face = faces[face_index];
				auto& diffuse_texture = textures[texture_infos[face.texture_info_index].texture_index];
				auto& lightmap_texture = face_lightmap_textures[face_index];

                gpu.textures.bind(0, diffuse_texture);
                gpu.textures.bind(1, lightmap_texture);

				glDrawElements(GL_TRIANGLE_FAN, face.surface_edge_count, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>((face_start_indices[face_index] * sizeof(uint32_t)))); glCheckError();
			}
		}

		glDisableVertexAttribArray(position_location); glCheckError();
		glDisableVertexAttribArray(diffuse_texcoord_location); glCheckError();
		glDisableVertexAttribArray(lightmap_texcoord_location); glCheckError();

		glBindBuffer(GL_ARRAY_BUFFER, 0); glCheckError();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); glCheckError();

		gpu.programs.pop();

		//cull face
		if (is_cull_face_enabled)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		//cull face mode
		glCullFace(cull_face_mode);

		//blend
		if (is_blend_enabled)
		{
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	int32_t bsp_t::get_leaf_index_from_position(const vec3_t& position) const
	{
		int32_t node_index = 0;

		while (node_index >= 0)
		{
			const auto& node = nodes[node_index];
			const auto& plane = planes[node.plane_index].plane;

			if (glm::dot(plane.normal, (position - plane.origin())) >= 0)
			{
				node_index = node.children[0];
			}
			else
			{
				node_index = node.children[1];
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
};