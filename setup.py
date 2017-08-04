fom setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

class CustomBuildExtCommand(build_ext):
	"""build_ext command for use when numpy headers are needed."""
	def run(self):
		
		import numpy

		self.include_dirs.append(numpy.get_include())

		build_ext.run(self)


hsimodule = Extension(
        'hsimage',
        libraries = ['opencv_core','opencv_highgui','opencv_imgproc','opencv_imgcodecs','boost_python'],
        sources = ['src/hsimage.cpp', 'src/classifiedhsimage.cpp', 'src/target.cpp', 'src/colormap.cpp','src/main.cpp','src/python_utils.cpp','src/pyboost_cv3_converter.cpp','src/labelfile.cpp','src/jsoncpp/jsoncpp.cpp'],
        extra_compile_args=['-std=c++11'],
        )

setup(
        name='hsimage',
	version='1.0.0',
        description='Interactivity class for hyperspectral ENVI-BIL images',
        author='Ryan Brown',
        author_email='brownrc@vt.edu',
        
        url='https://osf.io/b5t3x',
	download_url = "https://github.com/dtchuck/HSImage/archive/0.1.targ.gz",

        include_package_data=True,
	keywords = ['hyperspectral','image','interface'],
        zip_safe=False,

	cmdclass = {'build_ext': CustomBuildExtCommand},

	install_requires=['numpy'],

        ext_modules=[hsimodule],
        )
                
