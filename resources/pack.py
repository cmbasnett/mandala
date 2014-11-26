import os
import argparse
import zlib
import sys

def main():
	parser = argparse.ArgumentParser()
	parser.add_argument('--input', dest='input', required=False, default=os.getcwd())
	parser.add_argument('--output', dest='output', required=False, default=os.getcwd())
	args = parser.parse_args()

	if not os.path.isdir(args.input):
		parser.print_help()

	if not os.path.isdir(args.output):
		parser.print_help()

	resources = set()

	for root, dirs, files in os.walk(args.input, topdown=False):
	    for file in files:
	    	if file in resources:
	    		print 'duplicate file name (' + file + '), terminating'
	    		sys.exit(1)

	    	print file

	    	resources.add(file)

	    	with open(os.path.join(root, file), 'rb') as f:
	    		crc32 = zlib.crc32(f.read())

if __name__ == '__main__':
	main()