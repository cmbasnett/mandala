import os
import subprocess
import shutil

from os.path import basename

def main():
	root = os.getcwd()
	bitmap_fonts_path = ''.join([root, "\\raw\\bitmap_fonts\\"])
	bmfont_path = os.path.normpath(''.join([root, "\\..\\tools\\bmfont\\source\\Release\\bmfont.exe"]))
	fnt_dir = ''.join([root, "\\deploy\\bitmap_fonts\\"])
	png_dir = ''.join([root, "\\deploy\\textures\\"])

	if (os.path.isfile(bmfont_path) == False):
		print "cannot find bmfont.exe"
		return

	for file in os.listdir(bitmap_fonts_path):
		print file

		if (file.endswith(".bmfc")):
			process = subprocess.call([bmfont_path, "-c", bitmap_fonts_path + file, "-o", "..\\" + os.path.splitext(basename(file))[0] + ".fnt"])

	for file in os.listdir(root):
		if (file.endswith(".fnt")):
			shutil.copy(file, fnt_dir)
			os.remove(file)
		elif (file.endswith(".png")):
			shutil.copy(file, png_dir)
			os.remove(file)


if __name__ == '__main__':
	main()
	