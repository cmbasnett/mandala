import os
import subprocess
import shutil

from os.path import basename

cwd = os.getcwd()
raw_dir = ''.join([cwd, "\\raw\\"]);
deploy_dir = ''.join([cwd, "\\deploy\\"]);

def deploy_bitmap_fonts():
	bmfont_path = os.path.normpath(''.join([cwd, "\\..\\tools\\bmfont\\source\\Release\\bmfont.exe"]))

	if (os.path.isfile(bmfont_path) == False):
		print "Cannot find bmfont.exe, terminating"
		return

	raw_bitmap_fonts_dir = ''.join([raw_dir, "bitmap_fonts\\"])
	deploy_bitmap_fonts_dir = ''.join([deploy_dir, "bitmap_fonts\\"])
	deploy_textures_dir = ''.join([deploy_dir, "textures\\"])

	for file in os.listdir(raw_bitmap_fonts_dir):
		if (file.endswith(".bmfc")):
			config_path = ''.join([raw_bitmap_fonts_dir, file])

			print ''.join(["Processing ", config_path, "..."])

			process = subprocess.call([bmfont_path, "-c", config_path, "-o", os.path.splitext(basename(file))[0] + ".fnt"])

	for file in os.listdir(cwd):
		if (file.endswith(".fnt")):
			shutil.copy(file, deploy_bitmap_fonts_dir)
			os.remove(file)
		elif (file.endswith(".png")):
			shutil.copy(file, deploy_textures_dir)
			os.remove(file)

def deploy_sprite_sets():
	sprite_sets_dir = ''.join([cwd, "\\raw\\sprite_sets\\"])
	sprite_sets_deploy_dir = ''.join([cwd, "\\deploy\\sprite_sets\\"])
	tpsb_path = os.path.normpath(''.join([cwd, "\\..\\tools\\tpsb\\bin\\tpsb.exe"]))
	
	for root, directories, files in os.walk(sprite_sets_dir):
		for file in files:
			if (file.endswith(".tps")):
				tps_file = os.path.abspath(os.path.join(root, file))
				process = subprocess.call(["TexturePacker", tps_file, "--quiet"])
				json_file = tps_file.replace('.tps', '.json')
				process = subprocess.call([tpsb_path, json_file, sprite_sets_deploy_dir])

def deploy_models():
	md5b_path = os.path.normpath(''.join([cwd, "\\..\\tools\\md5b\\bin\\md5b.exe"]))

	if (os.path.isfile(md5b_path) == False):
		print "Cannot find md5b.exe, terminating"
		return

	raw_models_dir = ''.join([raw_dir, "models\\"])

	for root, directories, files in os.walk(raw_models_dir):
		for file in files:
			if (file.endswith(".md5mesh") or file.endswith(".md5anim")):
				md5x_path = ''.join([raw_models_dir, file])

				print ''.join(["Processing ", md5x_path, "..."])

				process = subprocess.call([md5b_path, os.path.abspath(os.path.join(root, file))])

def main():
	deploy_bitmap_fonts()
	deploy_sprite_sets()
	deploy_models()
	return

if __name__ == '__main__':
	main()