import os
import argparse
import zlib
import sys
import struct
import json

def read_null_terminated_string(f):
	bytes = list()
	byte = f.read(1)

	while byte != '\0':
		bytes.append(byte)
		byte = f.read(1)

	return ''.join(bytes)

class Resource:
	def __init__(self):
		self.name = ''
		self.path = ''
		self.offset = 0
		self.length = 0
		self.crc32 = 0

class Pack:
	def __init__(self, input, output = ''):
		self.resources = dict()

		if os.path.isfile(input):
			# create pack from existing pack file
			with open(input, 'rb') as f:
				bytes = f.read(struct.calcsize('<4s I I'))
				data = struct.unpack('<4s I I', bytes)
				magic = data[0]
				version = data[1]
				resource_count = data[2]

				if magic != 'PACK':
					raise Exception()

				if version != 1:
					raise Exception()

				for i in range(0, resource_count):
					resource = Resource()
					resource.name = read_null_terminated_string(f)
					bytes = f.read(struct.calcsize('<I I i'))
					data = struct.unpack('<I I i', bytes)
					resource.offset = data[0]
					resource.length = data[1]
					resource.crc = data[2]

					self.resources[resource.name] = resource

			print self
		elif os.path.isdir(input):
			# create pack from directory
			for root, dirs, files in os.walk(input, topdown=False):
			    for file in files:
			    	if file == 'Thumbs.db':	#HACK: figure out a cross-platform way to check for and omit hidden files
			    		continue

			    	resource = Resource()

			    	if file in self.resources:
			    		raise RuntimeError('duplicate file name')

			    	resource.name = file
			    	resource.path = os.path.join(root, file)

			    	with open(os.path.join(root, file), 'rb') as f:
			    		resource.length = os.path.getsize(os.path.join(root, file))
			    		resource.crc32 = zlib.crc32(f.read())

			    	self.resources[file] = resource

			with open(output, 'wb') as f:
				magic = 'PACK'
				version = 1
				header_format = '<4s I I'

				# write header
				s = struct.Struct(header_format)

				f.write(s.pack(magic, version, len(self.resources)))

				# precalculate file data offset
				file_offset = f.tell()

				for key, resource in self.resources.iteritems():
					file_offset += len(resource.name) + 1
					file_offset += struct.calcsize(header_format)

				# write FAT
				for key, resource in self.resources.iteritems():
					f.write(resource.name + '\0')
					s = struct.Struct('<I I i')
					f.write(s.pack(file_offset, resource.length, resource.crc32))

					file_offset += os.path.getsize(resource.path)

				for key, resource in self.resources.iteritems():
					with open(resource.path, 'rb') as r:
						data = r.read()
						f.write(data)

				print f.tell()

	def __str__(self):
		s = ''

		keys = sorted(self.resources.keys())

		for key in keys:
			print self.resources[key].name

		return s

def main():
	parser = argparse.ArgumentParser()
	parser.add_argument('-q', '--quiet', dest='quiet', action='store_true', required=False, help='no output')
	parser.add_argument('input_dir', help='directory to be processed')
	parser.add_argument('-o', dest='output_file', help='path of file to be created')
	args = parser.parse_args()

	pack = Pack(args.input_dir, args.output_file)

if __name__ == '__main__':
	main()