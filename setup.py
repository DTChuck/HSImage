from setuptools import setup, Extension

hsimodule = Extension(
        'hsimage',
        libraries = ['opencv_core','opencv_highgui','opencv_imgproc','opencv_imgcodecs'],
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

        include_package_data=True,

        zip_safe=False,

        ext_modules=[hsimodule],
        )
                
