import os
import argparse
import zlib
import sys
import struct

def main():
	parser = argparse.ArgumentParser()
	parser.add_argument('--input', dest='input', required=True, default=os.getcwd())
	parser.add_argument('--output', dest='output', required=True, default=os.getcwd())
	args = parser.parse_args()

	resources = list()

	# gather resources to be packed
	for root, dirs, files in os.walk(args.input, topdown=False):
	    for file in files:
	    	if file == 'Thumbs.db':	#HACK: figure out a cross-platform way to check for hidden files
	    		continue

	    	resource = {}

	    	if file in resources:
	    		print 'duplicate file name (' + file + '), terminating'
	    		continue
	    		#sys.exit(1)

	    	resource['name'] = file
	    	resource['path'] = os.path.join(root, file)

	    	with open(os.path.join(root, file), 'rb') as f:
	    		resource['length'] = os.path.getsize(os.path.join(root, file))
	    		resource['crc32'] = zlib.crc32(f.read())

	    	resources.append(resource)

	# create pack file
	with open(os.path.join(root, args.output), 'wb') as file:
		magic = 'PACK'
		version = 1
		header_format = '<4s I I'

		# write header
		s = struct.Struct(header_format)	# big endian

		file.write(s.pack(magic, version, len(resources)))

		# precalculate file data offset
		file_offset = file.tell()

		for resource in resources:
			file_offset += len(resource['name'] + '\0')
			file_offset += struct.calcsize(header_format)

		# write FAT
		for resource in resources:
			file.write(resource['name'] + '\0')

			s = struct.Struct('<I I i')

			file.write(s.pack(file_offset, resource['length'], resource['crc32']))

			file_offset += os.path.getsize(resource['path'])

		for resource in resources:
			with open(resource['path'], 'rb') as resource_file:
				data = resource_file.read()
				file.write(data)

		print file.tell()

if __name__ == '__main__':
	main()