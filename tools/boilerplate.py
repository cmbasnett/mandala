import os
import argparse
import pprint
import re

def main():
	UNIFORMS_PATTERN = '[\;|\n?|\s]uniform\s+(\S+)\s+(\S+)\s?\;'
	VERTEX_ATTRIBUTES_PATTERN = '[\;|\n?|\s]in\s+(\S+)\s+(\S+)\s?\;'
	SUBROUTINE_UNIFORMS_PATTERN = '[\;|\n?|\s]subroutine\s+\S+\s+(\S+)\s?\(\s?\)\s?\;'
	SUBROUTINES_PATTERN = '[\;|\n?|\s+]+subroutine\s+\((\S+)\)\s+\S+\s+(\S+)\s?\([^)]*\)'

	#parser = argparse.ArgumentParser()
	#parser.add_argument('shaders_dir', help='directory that contains shader files')
	#parser.add_argument('-q', '--quiet', dest='quiet', required=False, action='store_true', help='no output')
	#args = parser.parse_args()

	gpu_program_name = 'model'
	class_name = '{}_gpu_program_t'.format(gpu_program_name)
	vertex_attributes = []
	uniforms = []
	subroutine_uniforms = []
	subroutines = []

	with open('C:\\Users\\Colin Basnett\\Desktop\\model.vert') as f:
		vertex_shader_source = f.read()

		vertex_attributes = re.findall(VERTEX_ATTRIBUTES_PATTERN, vertex_shader_source)
		uniforms = uniforms + re.findall(UNIFORMS_PATTERN, vertex_shader_source)
		subroutine_uniforms = subroutine_uniforms + re.findall(SUBROUTINE_UNIFORMS_PATTERN, vertex_shader_source)
		subroutines = subroutines + re.findall(SUBROUTINES_PATTERN, vertex_shader_source)

	with open('C:\\Users\\Colin Basnett\\Desktop\\model.frag') as f:
		fragment_shader_source = f.read()

		uniforms = uniforms + re.findall(UNIFORMS_PATTERN, fragment_shader_source)
		subroutine_uniforms = subroutine_uniforms + re.findall(SUBROUTINE_UNIFORMS_PATTERN, fragment_shader_source)
		subroutines = subroutines + re.findall(SUBROUTINES_PATTERN, fragment_shader_source)

	print '#include \"gpu_program.hpp\"'
	print '#include \"glm.hpp\"'
	print ''
	print 'using namespace glm;'
	print ''
	print 'namespace mandala'
	print '{'
	print '    struct {} : gpu_program_t'.format(class_name)
	print '    {'

	for subroutine_uniform in subroutine_uniforms:
		print '        enum class {}_subroutine_e'.format(subroutine_uniform)
		print '        {'

		for subroutine in subroutines:
			uniform, function = subroutine

			if uniform == subroutine_uniform:
				print '             {},'.format(function.upper())

		print '        };'
		print ''

	print '        struct vertex_t'
	print '        {'

	for vertex_attribute in vertex_attributes:
		type, name = vertex_attribute

		print '            {} {};'.format(type, name)

	print '        };'
	print ''
	print '        typedef vertex_t vertex_type;'
	print ''
	print '        {}() :'.format(class_name)
	print '            gpu_program_t(R\"({})\", R\"({})\")'.format(vertex_shader_source, fragment_shader_source)
	print '        {'

	for vertex_attribute in vertex_attributes:
		type, name = vertex_attribute
		print '            {0}_location = gpu.get_attribute_location(get_id(), \"{0}\");'.format(name)

	for subroutine_uniform in subroutine_uniforms:
		print '            {0}_subroutine_uniform_location = gpu.get_subroutine_uniform_location(get_id(), gpu_t::shader_type_e::FRAGMENT, \"{0}\");'.format(subroutine_uniform)

		for subroutine in subroutines:
			uniform, function = subroutine
			
			if uniform == subroutine_uniform:
				print '            {0}_subroutine_index = gpu.get_subroutine_index(get_id(), gpu_t::shader_type_e::FRAGMENT, \"{0}\");'.format(function)

	print '        }'
	print ''
	print '        void on_bind() override'
	print '        {'

	for vertex_attribute in vertex_attributes:
		type, name = vertex_attribute
		print '            gpu.enable_vertex_attribute_array({0}_location);'.format(name)

	for vertex_attribute in vertex_attributes:
		type, name = vertex_attribute
		print '            gpu.set_vertex_attrib_pointer({0}_location, sizeof({1}) / sizeof({1}::value_type), gpu_data_type_<{1}::value_type>::VALUE, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, {0})));'.format(name, type)

	print '        }'
	print ''
	print '        void on_unbind() override'
	print '        {'

	for vertex_attribute in vertex_attributes:
		type, name = vertex_attribute
		print '            gpu.disable_vertex_attribute_array({0}_location);'.format(name)

	print '        }'
	print ''

	for subroutine_uniform in subroutine_uniforms:
		print '        void set_{0}_subroutine({0}_subroutine_e e)'.format(subroutine_uniform)
		print '        {'
		print '            switch(e)'
		print '            {'

		for subroutine in subroutines:
			uniform, function = subroutine

			if uniform == subroutine_uniform:
				print '                case {}_subroutine_e::{}:'.format(uniform, function.upper())
				print '                    gpu.set_uniform_subroutine(gpu_t::shader_type_e::FRAGMENT, {}_subroutine_index);'.format(function)
				print '                    break;'

		print '            }'

		print '        }'

	print ''
	print '    private:'

	for vertex_attribute in vertex_attributes:
		type, name = vertex_attribute
		print '       gpu_location_t {}_location;'.format(name)

	for subroutine_uniform in subroutine_uniforms:
		print '       gpu_location_t {}_subroutine_uniform_location;'.format(subroutine_uniform)

		for subroutine in subroutines:
			uniform, function = subroutine

			if uniform == subroutine_uniform:
				print '       gpu_index_t {}_subroutine_index;'.format(function)

	print '    };'
	print '}'

if __name__ == '__main__':
	main()