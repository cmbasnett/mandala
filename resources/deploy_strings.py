import csv
import struct

version = 1

def get_byte_count(strings):
	byte_count = 0
	for string in strings:
		byte_count += len(string)

	return byte_count

if __name__ == '__main__':
	keys = dict()
	strings = []
	string_offsets = []
	languages = []

	with open('C:/Users/Colin Basnett/Desktop/strings.csv') as file:
		reader = csv.DictReader(file)

		languages = reader.fieldnames[1:] # omit first column, should be empty

		for row in reader:
			string_indices = []

			for language in languages:
				strings_index = -1

				# get string index
				try:
					strings_index = strings.index(row[language])
				except:
					strings_index = len(strings)
					strings.append(row[language] + '\0')

				string_indices.append(strings_index)

			keys[row['']] = string_indices

	with open('C:/Users/Colin Basnett/Documents/Visual Studio 2013/Projects/mandala/resources/deploy/strings/armada.strings', 'wb') as file:
		# magic, version, language count, key count
		s = struct.Struct('4s I I I')
		file.write(s.pack('LANG', version, len(languages), len(keys)))

		# languages
		for language in languages:
			file.write(language + '\0')

		# strings length
		s = struct.Struct('I')
		file.write(s.pack(get_byte_count(strings)))

		# strings
		for string in strings:
			string_offsets.append(file.tell())
			file.write(string)

		# keys
		for key in keys.keys():
			file.write(key + '\0')

			string_indices = keys[key]

			for string_index in string_indices:
				s = struct.Struct('I')
				file.write(s.pack(string_offsets[string_index]))