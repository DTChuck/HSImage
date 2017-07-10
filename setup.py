from setuptools import setup, Extension

hsimodule = Extension(
        'hsimage',
        libraries = ['opencv_core','opencv_highgui','opencv_imgproc','opencv_imgcodecs'],
        sources = ['hsimage.cpp', 'classifiedhsimage.cpp', 'target.cpp', 'colormap.cpp','main.cpp','python_utils.cpp','pyboost_cv3_converter.cpp','labelfile.cpp','jsoncpp/jsoncpp.cpp'],
        extra_compile_args=['-std=c++11'],
        )

setup(
        name='hsimage',
        version='1.0.0',
        description='Interactivity class for hyperspectral ENVI-BIL images',
        author='Ryan Brown',
        author_email='brownrc@vt.edu',
        
        url='https://osf.io/[tbd]',

        include_package_data=True,

        zip_safe=False,

        ext_modules=[hsimodule],
        )
                
