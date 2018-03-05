from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

class CustomBuildExtCommand(build_ext):
	"""build_ext command for use when numpy headers are needed."""
	def run(self):
		
		import numpy

		self.include_dirs.append(numpy.get_include())

		build_ext.run(self)


hsimodule = Extension(
        'HSI',
        libraries = ['opencv_core','opencv_highgui','opencv_imgproc','opencv_imgcodecs'],
        sources = ['src/hsimage.cpp','src/pybind11_opencv_numpy/ndarray_converter.cpp','src/classifiedhsimage.cpp', 'src/target.cpp', 'src/colormap.cpp','src/main.cpp','src/labelfile.cpp','src/jsoncpp/jsoncpp.cpp'],
        extra_compile_args=['-std=c++11'],
        )

setup(
        name='HSI',
	version='1.2.0',
        description='Interactivity class for hyperspectral ENVI-BIL images',
        author='Ryan Brown',
        author_email='brownrc@vt.edu',
        
        url='https://osf.io/b5t3x',
	download_url = "https://github.com/dtchuck/HSImage/archive/0.1.targ.gz",

        license='MIT',

        classifiers=[
            'Development Status :: 4 - Beta',
            'Intended Audience :: Science/Research',
            'Topic :: Scientific/Engineering :: Image Recognition',
            'License :: OSI Approved :: MIT License',
            'Programming Language :: Python :: 3 :: Only'
            ],

        python_requires='>=3',

        include_package_data=True,
	keywords = ['hyperspectral','image','interface'],
        zip_safe=False,

	cmdclass = {'build_ext': CustomBuildExtCommand},

	install_requires=['numpy'],

        setup_requires=['pytest-runner'],
        tests_require=['pytest'],

        ext_modules=[hsimodule],
        )
                
