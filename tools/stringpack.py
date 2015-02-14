import argparse
import csv
import struct
import os

version = 1

def get_byte_count(strings):
	byte_count = 0

	for string in strings:
		byte_count += len(string)

	return byte_count

def main():
	keys = dict()
	strings = []
	string_offsets = []
	languages = []

	parser = argparse.ArgumentParser()
	parser.add_argument('--input_file', dest='input_file', required=True)
	parser.add_argument('--output_dir', dest='output_dir', required=True)
	args = parser.parse_args()

	print args.input_file

	with open(args.input_file) as f:
		reader = csv.DictReader(f)

		# omit first column, should be empty
		languages = reader.fieldnames[1:]

		# traverse all keys
		for row in reader:
			string_indices = []

			# for each language
			for language in languages:
				strings_index = -1

				try:
					# attempt to find identical string
					strings_index = strings.index(row[language])
				except:
					# string is unique, append to strings list
					strings_index = len(strings)
					strings.append(row[language] + '\0')

				# append string index to indices list
				string_indices.append(strings_index)

			keys[row['']] = string_indices

	# create output directory
	try:
		os.makedirs(args.output_dir)
	except OSError:
		if not os.path.isdir(args.output_dir):
			raise

	output_file = os.path.join(args.output_dir, os.path.splitext(os.path.basename(args.input_file))[0] + '.strings')

	with open(output_file, 'wb') as f:
		# magic, version, language count, key count
		s = struct.Struct('4s I I I')
		f.write(s.pack('LANG', version, len(languages), len(keys)))

		# languages
		for language in languages:
			f.write(language + '\0')

		# strings length
		s = struct.Struct('I')
		f.write(s.pack(get_byte_count(strings)))

		# strings
		for string in strings:
			string_offsets.append(f.tell())
			f.write(string)

		# keys
		for key in keys.keys():
			f.write(key + '\0')

			string_indices = keys[key]

			for string_index in string_indices:
				s = struct.Struct('I')
				f.write(s.pack(string_offsets[string_index]))

if __name__ == '__main__':
	main()