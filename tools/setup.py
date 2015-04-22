from distutils.core import setup
from distutils.extension import Extension

setup(name='mandala',
	ext_modules=[
		Extension('mandala',
			['../mandala/string_mgr.cpp',
			 '../mandala/resource_mgr.cpp',
			 '../mandala/string_archive.cpp',
			 '../mandala/pack_mgr.cpp',
			 '../mandala/pack.cpp'],
        include_dirs=['C:\\boost_1_55_0',
        '..\ext\glm-0.9.4.4',
        '..\ext\glew-1.10.0\include',
        '..\ext\glfw-3.0.1.bin.WIN32\include'],
        library_dirs=['C:\\boost_1_55_0\stage\lib',
        '..\ext\glfw-3.0.1.bin.WIN32\lib-msvc110',
        '..\ext\glew-1.10.0\lib\Release\Win32'],
        libraries=['glfw3', 'glew32', 'opengl32', 'user32', 'gdi32'],
        extra_compile_args=['/EHsc']
        )
    ])