import os
import argparse
import subprocess
import shutil
import fnmatch
import zlib
import json

def iequal(a, b):
    try:
        return a.upper() == b.upper()
    except AttributeError:
        return a == b

cwd = os.path.dirname(os.path.realpath(__file__))

bmfont_path = os.path.normpath(os.path.join(cwd, 'bmfont', 'source', 'Release', 'bmfont.exe'))
tpsb_path = os.path.normpath(os.path.join(cwd, 'tpsb', 'bin', 'tpsb.exe'))
md5b_path = os.path.normpath(os.path.join(cwd, 'md5b.py'))
stringpack_path = os.path.normpath(os.path.join(cwd, 'stringpack.py'))

resources_dir = ''
deploy_dir = ''

def deploy_bitmap_font(p):
	process = subprocess.call([bmfont_path, '-c', p, '-o', os.path.join(deploy_dir, os.path.splitext(os.path.basename(p))[0] + '.fnt')])

def deploy_sprite_set(p):
	#TODO: create custom texturepacker export format to eliminate need for this!
	process = subprocess.call(['TexturePacker', p, '--quiet'])
	json_file = os.path.splitext(p)[0] + '.json'
	process = subprocess.call([tpsb_path, json_file, deploy_dir])
	#TODO: remove intermediate .json file (maybe?)

def deploy_model(p):
	process = subprocess.call(['python', md5b_path, p, '-o', deploy_dir])

def deploy_strings(p):
	process = subprocess.call(['python', stringpack_path, '--input_file', p, '--output_dir', deploy_dir])

ignore_patterns = []

def should_ignore(relpath):
	global ignore_patterns

	for ignore_pattern in ignore_patterns:
		if fnmatch.fnmatch(relpath, ignore_pattern):
			return True

	return False

def main():
	global resources_dir
	global deploy_dir
	global ignore_patterns

	parser = argparse.ArgumentParser()
	parser.add_argument('resources_dir', help='directory to be deployed (can contain .deployignore)')
	parser.add_argument('-f', '--force', dest='force', required=False, action='store_true', help='force redeploy of all resources')
	parser.add_argument('-q', '--quiet', dest='quiet', required=False, action='store_true', help='no output')
	args = parser.parse_args()

	resources_dir = os.path.abspath(args.resources_dir)

	# ensure resources_dir is a valid directory
	if not os.path.isdir(resources_dir):
		raise RuntimeError('resources_dir ' + resources_dir + ' is not a valid directory')

	# read .deployignore
	with open(os.path.join(resources_dir, '.deployignore')) as f:
		ignore_patterns = f.read().splitlines()
		ignore_patterns = filter(lambda s: not s.strip().startswith('#') and len(s.strip()) > 0, ignore_patterns)	#TODO: find a prettier way to do this?

	deploy_dir = os.path.join(resources_dir, '.deploy')

	if args.force:
		try:
			shutil.rmtree(deploy_dir)
		except OSError:
			pass

	# create .deploy directory if it doesn't exist
	try:
		os.makedirs(deploy_dir)
	except OSError:
		if not os.path.isdir(deploy_dir):
			raise

	crcs = dict()

	# read .deploycrc
	try:
		with open(os.path.join(deploy_dir, '.deploycrc')) as f:
			crcs = json.loads(f.read())
	except IOError:
		pass

	# WINDOWS: mark deploy directory as hidden
	if os.name is 'nt':
		os.system('attrib +h \"{}\"'.format(deploy_dir))

	# iterate over all files in resource directory
	for root, dirs, files in os.walk(resources_dir):
		relpath = os.path.relpath(root, resources_dir)
		if should_ignore(relpath):
			continue

		for f in files:
			abspath = os.path.join(root, f)
			relpath = os.path.relpath(abspath, resources_dir)

			if should_ignore(relpath):
				continue

			with open(abspath, 'rb') as g:
				crc = zlib.crc32(g.read())

				if relpath not in crcs or crcs[relpath] != crc:
					crcs[relpath] = crc

					ext = os.path.splitext(relpath)[-1]

					# TODO: load in a pre-processing rule-sheet
					if iequal(ext, '.bmfc'):
						deploy_bitmap_font(abspath)
					elif iequal(ext, '.md5anim') or iequal(ext, '.md5mesh'):
						deploy_model(abspath)
					elif iequal(ext, '.csv'):	#TODO: will cause issues if other non-string csvs exist, figure out a way to mitigate this
						deploy_strings(abspath)
					elif iequal(ext, '.tps'):
						deploy_sprite_set(abspath)
					else:
						shutil.copy(abspath, deploy_dir)

	# write .deploycrc
	with open(os.path.join(deploy_dir, '.deploycrc'), 'w+') as f:
		f.write(json.dumps(crcs, indent=4, sort_keys=True))

if __name__ == '__main__':
	main()