#include "PSKModelLoader.hpp"
#include "psk.hpp"
#include "md5b.hpp"

namespace naga
{
	void PSKModelLoader::load(std::istream& istream)
	{
		auto psk = PSK(istream);

		// TODO: make it real simple to start, all one mesh, all vertices on the mesh
		for (const auto& psk_vertex : psk.get_vertices())
		{
			MeshInfo::Vertex vertex;
			//model_vertex.bone_indices_0
			//model_vertex.bone_indices_1
			//model_vertex.bone_weights_0
			//model_vertex.bone_weights_1
			// TODO: we need to find up to 4 thingies
			vertex.weight_count = 0;		// ?
			vertex.weight_index_start = 0;	// ?
			//model_vertex.normal
			//model_vertex.tangent
			vertex.texcoord.x = psk_vertex.u;
			vertex.texcoord.y = psk_vertex.v;
		}

		// bones
		for (size_t i = 0; i < psk.get_bones().size(); ++i)
		{
			const auto& psk_bone = psk.get_bones()[i];
			pose3 pose;
			pose.location = psk_bone.bone_position.location;
			pose.rotation = psk_bone.bone_position.rotation;
			auto bone_info = BoneInfo();
			bone_info.name = psk_bone.name;
			bone_info.parent_index = (i == psk_bone.parent_index) ? 255 : psk_bone.parent_index;
			bone_info.pose = pose;
			bones.push_back(bone_info);
		}
	}
}