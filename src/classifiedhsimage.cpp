#include "classifiedhsimage.h"


template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());

    std::vector<T> result;
    result.reserve(a.size());

    std::transform(a.begin(), a.end(), b.begin(),
                   std::back_inserter(result), std::plus<T>());
    return result;
}


ClassifiedHSImage::ClassifiedHSImage(){}

ClassifiedHSImage::ClassifiedHSImage(HSImage hsimage, cv::Mat labels, std::vector<classColor> c_names)
{
    load(hsimage,labels,c_names);
}

ClassifiedHSImage::ClassifiedHSImage(std::string raw_file, std::string hdr_file, std::string label_file, std::string class_hdr_file)
{
    HSImage im(hdr_file,raw_file);
    cv::Mat labels = cv::imread(label_file);

    colorMap c;
    std::fstream in(class_hdr_file,std::ios_base::in);
    c.fromFile(in);

    std::vector<classColor> class_list;

    //get class colors and names, then load into vector
    const std::vector<target> *target_list = c.getTargetList(targetType::targetClass);
    for(auto t : *target_list)
    {
        std::string name = t.getTitle();
        cv::Vec3b color;
        color[0] = t.getB();
        color[1] = t.getG();
        color[2] = t.getR();

        classColor c;
        c.first = name;
        c.second = color;

        if(std::find(class_list.begin(),class_list.end(),c)==class_list.end())
            class_list.push_back(c);
    }

    load(im,labels,class_list);
}

ClassifiedHSImage::ClassifiedHSImage(std::string raw_file, std::string hdr_file, std::string lif_file)
{
    if (PyObject *err = PyErr_Occurred()) PyErr_Clear();
    HSImage im(hdr_file,raw_file);
    LabelFile lf(lif_file);
    load(im,lf.getLabelImage(),lf.getClassInfo());
}

void ClassifiedHSImage::load(HSImage hsimage, cv::Mat labels, std::vector<classColor> c_names)
{
    image = hsimage;
    labels.copyTo(label);

    for(auto c : c_names)
    {
        class_names.emplace(c.first,c.second);
        class_keys.emplace(c.second,c.first);
    }
}

std::vector<std::vector<u_int16_t> > ClassifiedHSImage::getClassSpectra(std::string class_label, unsigned int num_spectra)
{
    std::vector<std::vector<u_int16_t>> output;

    if(class_names.count(class_label) > 0)
    {
        cv::Vec3b class_color = class_names[class_label];
        cv::Mat mask;
        std::vector<cv::Point> idx;
        cv::inRange(label,class_color,class_color,mask);
        cv::findNonZero(mask,idx);

        std::cout << idx.size() << std::endl;

        if(idx.size() > 0)
        {
            cv::randShuffle(idx);

            if(num_spectra > 0 && num_spectra < idx.size())
            {
                for(unsigned int i=0; i< num_spectra; i++)
                {
                    cv::Point p = idx[i];
                    std::vector<u_int16_t> tmp = image.getPixelSpectra(p.y, p.x);
                    output.push_back(tmp);
                }
            }
            else
            {
                for(unsigned int i=0; i< idx.size(); i++)
                {
                    cv::Point p = idx[i];
                    std::vector<u_int16_t> tmp = image.getPixelSpectra(p.y, p.x);

                    output.push_back(tmp);
                }
            }
        }
    }
    return output;
}

std::vector<std::vector<double> > ClassifiedHSImage::getClassTF(std::string class_label, unsigned int num_spectra)
{
    std::vector<std::vector<double>> output;

    if(class_names.count(class_label) > 0)
    {
        cv::Vec3b class_color = class_names[class_label];
        cv::Mat mask,idx;
        cv::inRange(label,class_color,class_color,mask);
        cv::findNonZero(mask,idx);
        if(cv::countNonZero(mask) > 0)
        {
            cv::randShuffle(idx);

            if(num_spectra > 0 && num_spectra < idx.total())
            {
                for(unsigned int i=0; i< num_spectra; i++)
                {
                    cv::Point p = idx.at<cv::Point>(i);
                    std::vector<double> tmp = image.getPixelTransferFunction(p.y, p.x);
                    output.push_back(tmp);
                }
            }
            else
            {
                for(unsigned int i=0; i< idx.total(); i++)
                {
                    cv::Point p = idx.at<cv::Point>(i);
                    std::vector<double> tmp = image.getPixelTransferFunction(p.y, p.x);

                    output.push_back(tmp);
                }
            }

        }
    }
    return output;
}

std::vector<double> ClassifiedHSImage::getAvgClassTF(std::string class_label)
{
    cv::Vec3b class_color = class_names[class_label];
    cv::Mat mask,idx;
    cv::inRange(label,class_color,class_color,mask);
    cv::findNonZero(mask,idx);
    std::vector<double> transfer_function,var;
    transfer_function.resize(image.wavelengths.size());
    transfer_function.assign(transfer_function.size(),0);

    for(unsigned int i=0;i<idx.total();i++)
    {
        cv::Point p = idx.at<cv::Point>(i);
        std::vector<double> tmp = image.getPixelTransferFunction(p.y, p.x);
        var = tmp + transfer_function;
        transfer_function = var;
    }

    transform(transfer_function.begin(), transfer_function.end(), transfer_function.begin(),
                std::bind2nd( std::multiplies<double>(), 1.0/(double)idx.total()));

    return transfer_function;
}

std::string ClassifiedHSImage::getPixelClass(int row, int col)
{
    cv::Vec3b val = label.at<cv::Vec3b>(row,col);

    std::string name = class_keys[val];

    if(name.empty())
        name = "Void";

    return name;
}

void ClassifiedHSImage::setSpectraClass(int row, int col, std::string class_label)
{
    cv::Vec3b val;
    val = class_names[class_label];

    label.at<cv::Vec3b>(row, col) = val;
}

void ClassifiedHSImage::setSpectraClass(std::vector<std::pair< int, int > > pixel_locs, std::string class_label)
{
    for(auto loc : pixel_locs)
    {
        cv::Vec3b val;
        val = class_names[class_label];

        label.at<cv::Vec3b>(loc.first, loc.second) = val;
    }
}

void ClassifiedHSImage::setImageClass(cv::Mat class_labels, std::vector<classColor> class_list)
{
    class_labels.copyTo(label);

    class_names.clear();
    for(auto c : class_list)
        class_names.emplace(c.first,c.second);
}

cv::Mat ClassifiedHSImage::getImageClass()
{
    return label;
}

void export_classifiedhsimage(pybind11::module m)
{
    namespace py = pybind11;

//    py::module m2 = m.def_submodule("classified_hsimage","Labeled ENVI-BIL Hyperspectral Image Interface Module using CSAIL/LabelMe label-style.");

    py::class_<ClassifiedHSImage> classified_hsimage(m, "classified_hsimage");
    classified_hsimage
                .def(py::init<>())
                .def(py::init<HSImage, cv::Mat, std::vector<classColor> >()) //Constructors
                .def(py::init<std::string, std::string, std::string>())
                .def_readwrite("hsimage",&ClassifiedHSImage::image)

                .def("getClassSpectra", &ClassifiedHSImage::getClassSpectra)
                .def("getClassTF",&ClassifiedHSImage::getClassTF)
                .def("getAvgClassTF", &ClassifiedHSImage::getAvgClassTF)
                .def("getPixelClass", &ClassifiedHSImage::getPixelClass)
                .def("setSpectraClass", (void (ClassifiedHSImage::*)(int,int,std::string)) &ClassifiedHSImage::setSpectraClass, "Set Spectra Class", py::arg("row"), py::arg("column"), py::arg("class_name"))
                .def("setSpectraClass", (void (ClassifiedHSImage::*)(std::vector<std::pair<int,int>>,std::string)) &ClassifiedHSImage::setSpectraClass, "Set Spectra Class", py::arg("point_vector"), py::arg("class_name"))
                .def("setImageClass", &ClassifiedHSImage::setImageClass)
                .def("getImageClass", &ClassifiedHSImage::getImageClass);



}

//void export_classifiedhsimage(pybind11::module m)
//{
//    namespace bp = boost::python;
//    // map the IO namespace to a sub-module
//    // make "from myPackage.class1 import <whatever>" work
//    bp::object classified_hsimageModule(bp::handle<>(bp::borrowed(PyImport_AddModule("hsi.classified_hsimage"))));
//    // make "from mypackage import class1" work
//    bp::scope().attr("classified_hsimage") = classified_hsimageModule;
//    // set the current scope to the new sub-module
//    bp::scope io_scope = classified_hsimageModule;

//    void (ClassifiedHSImage::*d1)(int, int, std::string) = &ClassifiedHSImage::setSpectraClass; // Dealing with overloaded function
//    void (ClassifiedHSImage::*d2)(std::vector<std::pair<int, int > >, std::string) = &ClassifiedHSImage::setSpectraClass;
//    void (ClassifiedHSImage::*d3)(cv::Mat, std::vector<classColor>) = &ClassifiedHSImage::setImageClass;

//    bp::class_<ClassifiedHSImage>("classified_hsimage")
//    .def(bp::init<HSImage, cv::Mat, std::vector<classColor> >()) //Constructors
////    .def(bp::init<std::string, std::string, std::string, std::string>())
//    .def(bp::init<std::string, std::string, std::string>())
//    .def_readwrite("hsimage",&ClassifiedHSImage::image)

////    .def("load",&ClassifiedHSImage::load) //Member Functions
//    .def("getClassSpectra", &ClassifiedHSImage::getClassSpectra)
//    .def("getClassTF",&ClassifiedHSImage::getClassTF)
//    .def("getAvgClassTF", &ClassifiedHSImage::getAvgClassTF)
//    .def("getPixelClass", &ClassifiedHSImage::getPixelClass)
//    .def("setSpectraClass", d1)
//    .def("setSpectraClass", d2)
//    .def("setSpectraClass", d3)
//    .def("setImageClass", &ClassifiedHSImage::setImageClass)
//    .def("getImageClass", &ClassifiedHSImage::getImageClass);
//}
