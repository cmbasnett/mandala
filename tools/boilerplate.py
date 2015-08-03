import os
import argparse
import pprint
import re
import zlib

def main():
	print 'boilerplate'

	UNIFORMS_PATTERN = '[\;|\n?|\s]uniform\s+(\S+)\s+(\S+)\s?\;'
	VERTEX_ATTRIBUTES_PATTERN = '[\;|\n?|\s]in\s+(\S+)\s+(\S+)\s?\;'
	SUBROUTINE_UNIFORMS_PATTERN = '[\;|\n?|\s]subroutine\s+\S+\s+(\S+)\s?\(\s?\)\s?\;'
	SUBROUTINES_PATTERN = '[\;|\n?|\s+]+subroutine\s+\((\S+)\)\s+\S+\s+(\S+)\s?\([^)]*\)'

	parser = argparse.ArgumentParser()
	parser.add_argument('input_dir', help='directory that contains shader files')
	parser.add_argument('output_dir', nargs='?', default='.', help='directory that contains shader files')
	parser.add_argument('-f', '--force', dest='force', required=False, action='store_true', help='force rebuild of shader files')
	parser.add_argument('-q', '--quiet', dest='quiet', required=False, action='store_true', help='no output')
	args = parser.parse_args()

	for directory, subdirectories, files in os.walk(args.input_dir):
		files = [f for f in files if f.endswith('.frag') or f.endswith('.vert')]
		gpu_programs = set(map(lambda x: os.path.splitext(x)[0], files))

		for gpu_program in gpu_programs:
			class_name = '{}_gpu_program_t'.format(gpu_program)
			vertex_attributes = []
			uniforms = []
			subroutine_uniforms = []
			subroutines = []

			with open(os.path.join(args.input_dir, gpu_program + '.vert')) as f:
				vertex_shader_source = f.read()

				vertex_attributes = re.findall(VERTEX_ATTRIBUTES_PATTERN, vertex_shader_source)
				uniforms = uniforms + re.findall(UNIFORMS_PATTERN, vertex_shader_source)
				subroutine_uniforms = subroutine_uniforms + re.findall(SUBROUTINE_UNIFORMS_PATTERN, vertex_shader_source)
				subroutines = subroutines + re.findall(SUBROUTINES_PATTERN, vertex_shader_source)

			with open(os.path.join(args.input_dir, gpu_program + '.frag')) as f:
				fragment_shader_source = f.read()

				uniforms = uniforms + re.findall(UNIFORMS_PATTERN, fragment_shader_source)
				subroutine_uniforms = subroutine_uniforms + re.findall(SUBROUTINE_UNIFORMS_PATTERN, fragment_shader_source)
				subroutines = subroutines + re.findall(SUBROUTINES_PATTERN, fragment_shader_source)

			crc = 0

			try:
				with open(os.path.join(args.output_dir, gpu_program + '_gpu_program.hpp'), 'r') as f:
					crc = zlib.crc32(f.read())
			except IOError:
				pass

			# values = {}
			# values['name'] = gpu_program
			# values['vertex_args'] = ', '.join((' '.join(arg) for arg in vertex_attributes))
			# values['vertex_ctor'] = '\n'.join('this->{0} = {0};'.format(vertex_attribute[1]) for vertex_attribute in vertex_attributes)

			# continue

			s = []

			s.append('#pragma once\n')
			s.append('\n')
			s.append('#include \"gpu_program.hpp\"\n')
			s.append('#include \"glm.hpp\"\n')
			s.append('\n')
			s.append('using namespace glm;\n')
			s.append('\n')
			s.append('namespace mandala\n')
			s.append('{\n')
			s.append('    struct {} : gpu_program_t\n'.format(class_name))
			s.append('    {\n')

			for subroutine_uniform in subroutine_uniforms:
				s.append('        enum class {}_subroutine_e\n'.format(subroutine_uniform))
				s.append('        {\n')

				for subroutine in subroutines:
					uniform, function = subroutine

					if uniform == subroutine_uniform:
						s.append('             {},\n'.format(function.upper()))

				s.append('        };\n')
				s.append('\n')

			s.append('        struct vertex_t\n')
			s.append('        {\n')
			s.append('            vertex_t() = default;\n')
			s.append('            vertex_t(' + ', '.join((' '.join(arg) for arg in vertex_attributes)) + ')\n')
			s.append('            {\n')
			s.append(                 '\n'.join('this->{0} = {0};'.format(vertex_attribute[1]) for vertex_attribute in vertex_attributes))
			s.append('            }\n')
			s.append('\n')

			for vertex_attribute in vertex_attributes:
				type, name = vertex_attribute
				s.append('            {} {};\n'.format(type, name))

			s.append('        };\n')
			s.append('\n')
			s.append('        typedef vertex_t vertex_type;\n')
			s.append('\n')
			s.append('        {}() :\n'.format(class_name))
			s.append('            gpu_program_t(R\"({})\", R\"({})\")\n'.format(vertex_shader_source, fragment_shader_source))
			s.append('        {\n')

			for vertex_attribute in vertex_attributes:
				type, name = vertex_attribute
				s.append('            {0}_location = gpu.get_attribute_location(get_id(), \"{0}\");\n'.format(name))

			for subroutine_uniform in subroutine_uniforms:
				s.append('            {0}_subroutine_uniform_location = gpu.get_subroutine_uniform_location(get_id(), gpu_t::shader_type_e::FRAGMENT, \"{0}\");\n'.format(subroutine_uniform))

				for subroutine in subroutines:
					uniform, function = subroutine
					
					if uniform == subroutine_uniform:
						s.append('            {0}_subroutine_index = gpu.get_subroutine_index(get_id(), gpu_t::shader_type_e::FRAGMENT, \"{0}\");\n'.format(function))

			s.append('        }\n')
			s.append('\n')
			s.append('        void on_bind() override\n')
			s.append('        {\n')

			for vertex_attribute in vertex_attributes:
				type, name = vertex_attribute
				s.append('            gpu.enable_vertex_attribute_array({0}_location);\n'.format(name))

			for vertex_attribute in vertex_attributes:
				type, name = vertex_attribute
				s.append('            gpu.set_vertex_attrib_pointer({0}_location, sizeof({1}) / sizeof({1}::value_type), gpu_data_type_<{1}::value_type>::VALUE, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, {0})));\n'.format(name, type))

			s.append('        }\n')
			s.append('\n')
			s.append('        void on_unbind() override\n')
			s.append('        {\n')

			for vertex_attribute in vertex_attributes:
				type, name = vertex_attribute
				s.append('            gpu.disable_vertex_attribute_array({0}_location);\n'.format(name))

			s.append('        }\n')
			s.append('\n')

			for subroutine_uniform in subroutine_uniforms:
				s.append('        void set_{0}_subroutine({0}_subroutine_e e)\n'.format(subroutine_uniform))
				s.append('        {\n')
				s.append('            switch(e)\n')
				s.append('            {\n')

				for subroutine in subroutines:
					uniform, function = subroutine

					if uniform == subroutine_uniform:
						s.append('                case {}_subroutine_e::{}:'.format(uniform, function.upper()))
						s.append('                    gpu.set_uniform_subroutine(gpu_t::shader_type_e::FRAGMENT, {}_subroutine_index);'.format(function))
						s.append('                    break;\n')

				s.append('            }\n')

				s.append('        }\n')

			s.append('\n')
			s.append('    private:\n')

			for vertex_attribute in vertex_attributes:
				type, name = vertex_attribute
				s.append('       gpu_location_t {}_location;\n'.format(name))

			for subroutine_uniform in subroutine_uniforms:
				s.append('       gpu_location_t {}_subroutine_uniform_location\n;'.format(subroutine_uniform))

				for subroutine in subroutines:
					uniform, function = subroutine

					if uniform == subroutine_uniform:
						s.append('       gpu_index_t {}_subroutine_index;\n'.format(function))

			s.append('    };\n')
			s.append('}\n')

			s = ''.join(s)

			# if file contents changed, write new contents to file
			if (args.force or zlib.crc32(s) != crc):
				with open(os.path.join(args.output_dir, gpu_program + '_gpu_program.hpp'), 'w+') as f:
					if crc != zlib.crc32(s):
						print s
						f.write(s)

if __name__ == '__main__':
	main()