//mandala
#include "bsp_gpu_program.hpp"
#include "opengl.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
	std::shared_ptr<bsp_gpu_program_t> bsp_gpu_program;

	std::string bsp_gpu_program_t::vertex_shader_source = R"(
#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

precision lowp float;

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;
uniform sampler2D diffuse_texture;
uniform sampler2D lightmap_texture;

in vec3 position;
in vec2 diffuse_texcoord;
in vec2 lightmap_texcoord;

out vec2 out_diffuse_texcoord;
out vec2 out_lightmap_texcoord;

void main() 
{
	gl_Position = view_projection_matrix * (world_matrix * vec4(position, 1));
	
	//out_normal = normal;
	out_diffuse_texcoord = diffuse_texcoord;
	out_lightmap_texcoord = lightmap_texcoord;
})";

	std::string bsp_gpu_program_t::fragment_shader_source = R"(
#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

uniform sampler2D diffuse_texture;
uniform sampler2D lightmap_texture;
uniform float lightmap_gamma;
uniform float alpha;
uniform bool should_test_alpha;

in vec3 out_normal;
in vec2 out_diffuse_texcoord;
in vec2 out_lightmap_texcoord;

out vec4 fragment;

void main() 
{
	vec4 lightmap_term = texture(lightmap_texture, out_lightmap_texcoord);
	lightmap_term.xyz *= lightmap_gamma;
	
	vec4 diffuse_term = texture(diffuse_texture, out_diffuse_texcoord);
	
	fragment = diffuse_term * lightmap_term;
	
	fragment.a *= alpha;
	
	if(should_test_alpha && fragment.a <= 0.25)
	{
	    discard;
	}
})";

	bsp_gpu_program_t::bsp_gpu_program_t() :
		gpu_program_t(vertex_shader_source, fragment_shader_source)
	{
		position_location = glGetAttribLocation(id, "position"); glCheckError();
		diffuse_texcoord_location = glGetAttribLocation(id, "diffuse_texcoord"); glCheckError();
		lightmap_texcoord_location = glGetAttribLocation(id, "lightmap_texcoord"); glCheckError();

		world_matrix_location = glGetUniformLocation(id, "world_matrix"); glCheckError();
		view_projection_matrix_location = glGetUniformLocation(id, "view_projection_matrix"); glCheckError();
		diffuse_texture_location = glGetUniformLocation(id, "diffuse_texture"); glCheckError();
		lightmap_texture_location = glGetUniformLocation(id, "lightmap_texture"); glCheckError();
		lightmap_gamma_location = glGetUniformLocation(id, "lightmap_gamma"); glCheckError();
		alpha_location = glGetUniformLocation(id, "alpha"); glCheckError();
		should_test_alpha_location = glGetUniformLocation(id, "should_test_alpha"); glCheckError();
	}

	void bsp_gpu_program_t::on_bind()
	{
		static const auto position_offset = reinterpret_cast<void*>(offsetof(vertex_t, position));
		static const auto diffuse_texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_t, diffuse_texcoord));
		static const auto lightmap_texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_t, lightmap_texcoord));

		glEnableVertexAttribArray(position_location); glCheckError();
		glEnableVertexAttribArray(diffuse_texcoord_location); glCheckError();
		glEnableVertexAttribArray(lightmap_texcoord_location); glCheckError();

		glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_type), position_offset); glCheckError();
		glVertexAttribPointer(diffuse_texcoord_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), diffuse_texcoord_offset); glCheckError();
		glVertexAttribPointer(lightmap_texcoord_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), lightmap_texcoord_offset); glCheckError();
	}

	void bsp_gpu_program_t::on_unbind()
	{
		glDisableVertexAttribArray(position_location); glCheckError();
		glDisableVertexAttribArray(diffuse_texcoord_location); glCheckError();
		glDisableVertexAttribArray(lightmap_texcoord_location); glCheckError();
	}

	void bsp_gpu_program_t::world_matrix(const mat4_t& world_matrix) const
	{
		glUniformMatrix4fv(world_matrix_location, 1, GL_FALSE, glm::value_ptr(world_matrix)); glCheckError();
	}

	void bsp_gpu_program_t::view_projection_matrix(const mat4_t& view_projection_matrix) const
	{
		glUniformMatrix4fv(view_projection_matrix_location, 1, GL_FALSE, glm::value_ptr(view_projection_matrix)); glCheckError();
	}

	void bsp_gpu_program_t::diffuse_texture_index(uint32_t diffuse_texture_index) const
	{
		glUniform1i(diffuse_texture_location, diffuse_texture_index); glCheckError();
	}

	void bsp_gpu_program_t::lightmap_texture_index(uint32_t lightmap_texture_index) const
	{
		glUniform1i(lightmap_texture_location, lightmap_texture_index); glCheckError();
	}

	void bsp_gpu_program_t::lightmap_gamma(float32_t lightmap_gamma) const
	{
		glUniform1f(lightmap_gamma_location, lightmap_gamma); glCheckError();
	}

	void bsp_gpu_program_t::alpha(float32_t alpha) const
	{
		glUniform1f(alpha_location, alpha); glCheckError();
	}

	void bsp_gpu_program_t::should_test_alpha(bool should_test_alpha) const
	{
		glUniform1i(should_test_alpha_location, should_test_alpha); glCheckError();
	}
};
