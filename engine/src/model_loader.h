#include <vector>

#include "types.hpp"
#include "pose.hpp"

namespace naga
{
	struct ModelLoader
	{
		struct BoneInfo
		{
			std::string name;
			u8 parent_index = 0;
			pose3 pose;
		};

		struct MeshInfo
		{
			struct Vertex
			{
				vec2 texcoord;
				u16 weight_index_start = 0;
				u8 weight_count = 0;
			};

			struct Weight
			{
				u8 bone_index = 0;
				f32 bias = 0;
				vec3 location;
			};

			std::string shader;
			std::vector<Vertex> vertices;
			std::vector<u16> indices;
			std::vector<Weight> weights;
		};

		virtual void load(std::istream& istream) = 0;

		const std::vector<MeshInfo>& get_meshes() const { return meshes; }
		const std::vector<BoneInfo>& get_bones() const { return bones; }

	protected:
		std::vector<MeshInfo> meshes;
		std::vector<BoneInfo> bones;
	};
}