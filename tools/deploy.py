import os
import argparse
import subprocess
import shutil
import fnmatch
import zlib
import json
import re
import shlex

def iequal(a, b):
    try:
        return a.upper() == b.upper()
    except AttributeError:
        return a == b

cwd = os.path.dirname(os.path.realpath(__file__))
resources_dir = ''
deploy_dir = ''

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

	deploycrc = dict()
	deploycrc['scripts'] = dict()
	deploycrc['files'] = dict()

	# read .deployconfig
	try:
		with open(os.path.join(cwd, ".deploy")) as f:
			deployconfig = json.loads(f.read())
	except IOError as e:
		raise

	subprocesses = deployconfig['subprocesses']

	# read .deploycrc
	try:
		with open(os.path.join(deploy_dir, '.deploycrc')) as f:
			deploycrc = json.loads(f.read())
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

				if relpath not in deploycrc['files'] or deploycrc['files'][relpath] != crc:
					deploycrc['files'][relpath] = crc

					absdir = os.path.dirname(abspath)
					name = os.path.splitext(os.path.basename(relpath))[0]
					ext = os.path.splitext(relpath)[-1]

					processes = None

					# resolve processes to run on this file
					for subprocess_ in subprocesses:
						for pattern in subprocess_['patterns']:
							if re.match(pattern, relpath) is not None:
								if isinstance(subprocess_['processes'], list):
									processes = subprocess_['processes']
								break

					# run preprocessor
					if isinstance(processes, list):
						for process in processes:
							process = process % {'relpath': relpath, 'deploydir': deploy_dir, 'name': name, 'ext': ext, 'workingdir': cwd, 'abspath': abspath, 'absdir': absdir }
							args = shlex.split(process)
							subprocess.call(args)
					else:
						shutil.copy(abspath, deploy_dir)

	# write .deploycrc
	with open(os.path.join(deploy_dir, '.deploycrc'), 'w+') as f:
		f.write(json.dumps(deploycrc, indent=4, sort_keys=True))

if __name__ == '__main__':
	main()