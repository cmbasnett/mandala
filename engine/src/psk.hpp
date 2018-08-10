#pragma once

// std
#include <string>
#include <vector>

// boost
#include <boost\enable_shared_from_this.hpp>

// naga
#include "types.hpp"
#include "pose.hpp"
#include "basic_gpu_program.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

namespace naga
{
	struct CameraParameters;

	struct PSK : boost::enable_shared_from_this<PSK>
	{
		struct JointPosition
		{
			quat rotation;
			vec3 location;
			f32 length;
			vec3 size;
		};

		struct ChunkHeader
		{
			char chunk_id[20];
			i32 type_flag;
			i32 data_size;
			i32 data_count;
		};

		struct Material
		{
			char name[64];
			i32 texture_index;
			u32 poly_flags;
			i32 aux_material;
			u32 aux_flags;
			i32 lod_bias;
			i32 lod_style;
		};

		struct Bone
		{
			char name[64];
			u32 flags;
			i32 num_children;
			i32 parent_index;
			JointPosition bone_position;
		};

		struct Weight
		{
			f32 weight;
			i32 point_index;
			i32 bone_index;
		};

		struct Vertex
		{
			u16 point_index;
			f32 u;
			f32 v;
			u8 material_index;
			u8 reserved;
		};

		struct PaddedVertex
		{
			u16 point_index;
			u16 padding_1;
			f32 u;
			f32 v;
			u8 material_index;
			u8 reserved;
			u16 padding_2;
		};

		struct Face
		{
			u16 vertex_indices[3];
			u8 material_index;
			u8 aux_material_index;
			u32 smoothing_groups;
		};

		PSK(const std::string& path);

		const std::vector<vec3>& get_points() const { return points; }
		const std::vector<Vertex>& get_vertices() const { return vertices; }
		const std::vector<Face>& get_faces() const { return faces; }
		const std::vector<Material>& get_materials() const { return materials; }
		const std::vector<Bone>& get_bones() const { return bones; }
		const std::vector<Weight>& get_weights() const { return weights; }

		virtual void render(CameraParameters& camera_parameters);

	private:
		std::vector<vec3> points;
		std::vector<Vertex> vertices;
		std::vector<Face> faces;
		std::vector<Material> materials;
		std::vector<Bone> bones;
		std::vector<Weight> weights;

		typedef basic_gpu_program::VertexType vertex_type;
		typedef VertexBuffer<vertex_type> VertexBufferType;
		typedef u16 index_type;
		typedef IndexBuffer<index_type> IndexBufferType;

		boost::shared_ptr<VertexBufferType> vertex_buffer;
		boost::shared_ptr<IndexBufferType> index_buffer;
		size_t index_count;
	};
};