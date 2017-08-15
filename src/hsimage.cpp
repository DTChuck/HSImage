#include "hsimage.h"
HSImage::HSImage()
{
    has_spec_data = false;
}

HSImage::HSImage(std::string header_location, std::string image_location)
{
    has_spec_data = false;
    load(header_location,image_location);
}

HSImage::HSImage(std::string header_location, std::string image_location, std::vector<std::string> &spec_location)
{
    has_spec_data = false;
    load(header_location,image_location,spec_location);
}

HSImage::HSImage(const HSImage& other)
{
    acquisition_date = other.acquisition_date;
    tint = other.tint;
    samples = other.samples;
    lines = other.lines;
    bands = other.bands;
    fps = other.fps;
    binning[0] = other.binning[0];
    binning[1] = other.binning[1];

    hdr_file = other.hdr_file;
    img_file = other.img_file;
    vis_spec_file = other.vis_spec_file;
    nir_spec_file = other.nir_spec_file;
    has_spec_data = other.has_spec_data;

    wavelengths = other.wavelengths;
    fwhm = other.fwhm;

    ambient_intensities = other.ambient_intensities;

//    image_data.reset(new u_int16_t[lines*samples*bands]);
//    memcpy((void*)image_data.get(),(void*)other.image_data.get(),lines*bands*samples*sizeof(u_int16_t));
    image_data = other.image_data;

//    pixel_data.reset(new u_int16_t[lines*samples*bands]);
//    memcpy((void*)pixel_data.get(),(void*)other.pixel_data.get(),lines*bands*samples*sizeof(u_int16_t));
    pixel_data = other.pixel_data;

    //create map of wavelengths -> OpenCV Mat data pointers
    for(unsigned int i=0;i<wavelengths.size();i++)
        image_map.emplace(wavelengths[i],(uchar*)image_data.data()+lines*samples*i*sizeof(u_int16_t));
}

HSImage& HSImage::operator=(const HSImage& other)
{
    HSImage tmp(other);
    std::swap(acquisition_date,tmp.acquisition_date);
    std::swap(tint,tmp.tint);
    std::swap(samples,tmp.samples);
    std::swap(lines,tmp.lines);
    std::swap(bands,tmp.bands);
    std::swap(fps,tmp.fps);
    std::swap(binning[0],tmp.binning[0]);
    std::swap(binning[1],tmp.binning[1]);

    std::swap(hdr_file,tmp.hdr_file);
    std::swap(img_file,tmp.img_file);
    std::swap(vis_spec_file,tmp.vis_spec_file);
    std::swap(nir_spec_file,tmp.nir_spec_file);
    std::swap(has_spec_data,tmp.has_spec_data);

    std::swap(wavelengths,tmp.wavelengths);
    std::swap(fwhm,tmp.fwhm);
    std::swap(ambient_intensities,tmp.ambient_intensities);
    std::swap(image_data,tmp.image_data);
    std::swap(pixel_data, tmp.pixel_data);
    std::swap(image_map,tmp.image_map);

    return *this;
}

void HSImage::load(std::string header_location, std::string image_location)
{
    loadHeader(header_location);

    loadRawImage(image_location);

    if(has_spec_data)
    {
        std::vector<std::string> filenames;
        filenames.push_back(createAbsoluteSpecFilepath( vis_spec_file ));
        filenames.push_back(createAbsoluteSpecFilepath( nir_spec_file ));

        loadSpectrometerData(filenames);
    }
}

void HSImage::load(std::string header_location, std::string image_location, std::vector<std::string> spec_locations)
{
    loadHeader(header_location);
    loadRawImage(image_location);
    if(vis_spec_file.empty() && vis_spec_file.empty())
    {
        addSpecDataToHeader(spec_locations);
        loadSpectrometerData(spec_locations);
    }
    else
    {
        std::vector<std::string> filenames;
        filenames.push_back(createAbsoluteSpecFilepath( vis_spec_file ));
        filenames.push_back(createAbsoluteSpecFilepath( nir_spec_file ));

        loadSpectrometerData(filenames);
    }
}

void HSImage::loadHeader(std::string header_location)
{
    //load in header file
    std::string line;
    std::ifstream in_stream;
    in_stream.open(header_location);
    if(in_stream.is_open())
    {
        int line_number = 1;

        in_stream >> line;
        if(!line.compare("ENVI"))
        {
            bool in_wavelengths = false;
            bool in_fwhm = false;
            bool nir_spec = false,vis_spec = false;
            while(std::getline(in_stream,line))
            {
                line_number++;
                std::vector<std::string> words = split(line,' ');

                if(in_wavelengths)
                {
                    if(words[0].compare("}\r"))
                        wavelengths.push_back(stof(words.front()));
                    else
                        in_wavelengths = false;
                }

                if(in_fwhm)
                {
                    if(words[0].compare("}\r"))
                        fwhm.push_back(stof(words.front()));
                    else
                        in_fwhm = false;
                }
                if(!words[0].compare("NIR_spec_file"))
                {
                    nir_spec_file = words.back();
                    nir_spec = true;
                }

                if(!words[0].compare("VIS_spec_file"))
                {
                    vis_spec_file = words.back();
                    vis_spec = true;
                }

                if(!words[0].compare("acquisition"))
                    acquisition_date = words.back();

                if(!words[0].compare("samples"))
                    samples = stoi(words.back());

                if(!words[0].compare("lines"))
                    lines = stoi(words.back());

                if(!words[0].compare("bands"))
                    bands = stoi(words.back());

                if(!words[0].compare("fps"))
                    fps = stoi(words.back());

                if(!words[0].compare("tint"))
                    tint = stoi(words.back());

                if(!words[0].compare("binning"))
                {
                    binning[0] = stoi(words[3]);
                    binning[1] = stoi(words[4]);
                }

                if(!words[0].compare("Wavelength"))
                {
                    in_wavelengths = true;
                }

                if(!words[0].compare("fwhm"))
                {
                    in_fwhm = true;
                }
            }
            if(vis_spec && nir_spec)
                has_spec_data = true;

            hdr_file = header_location;
        }
        else
        {
            std::cout << "Incorrect Header File! Failed at Line " << line_number << std::endl;
        }
    }
    else
    {
        std::cout << "Could not open " << header_location << " for reading." << std::endl;
    }
}

void HSImage::loadRawImage(std::string image_location)
{
    //load in image file
    std::ifstream raw_file;

    raw_file.open(image_location,std::ios::binary | std::ios::ate);

    if(raw_file.is_open())
    {
        unsigned int size = raw_file.tellg();
        if(size == sizeof(u_int16_t)*lines*samples*bands)
        {
            //image_data.reset(new u_int16_t[lines*samples*bands]);
            //pixel_data.reset(new u_int16_t[lines*samples*bands]);

            image_data.reserve(lines*samples*bands);
            pixel_data.reserve(lines*samples*bands);

            raw_file.seekg(0,std::ios::beg);
            int storage_spot = 0;
            for(int b=0;b<bands;b++)
            {
                for(int l=0;l<lines;l++)
                {
                    raw_file.seekg((l*bands*samples+b*samples)*sizeof(u_int16_t),std::ios::beg);
                    //raw_file.read((char*)image_data.get()+storage_spot,samples*sizeof(u_int16_t));
                    raw_file.read((char*)image_data.data()+storage_spot,samples*sizeof(u_int16_t));
                    storage_spot+=samples*sizeof(u_int16_t);
                }
            }
            raw_file.close();

            storage_spot = 0;
            for(int l=0;l<lines;l++)
            {
                for(int s=0;s<samples;s++)
                {
                    for(int b=0;b<bands;b++)
                    {
                        pixel_data[storage_spot] = image_data[b*lines*samples+l*samples+s];//
                        storage_spot++;
                    }
                }
            }

            //create map of wavelengths -> OpenCV Mat data pointers
            for(unsigned int i=0;i<wavelengths.size();i++)
                //image_map.emplace(wavelengths[i],(uchar*)image_data.get()+lines*samples*i*sizeof(u_int16_t));
                image_map.emplace(wavelengths[i],(uchar*)image_data.data()+lines*samples*i*sizeof(u_int16_t));

            img_file = image_location;
        }
        else
        {
            std::cout << "Header File contains wrong data size for .raw image file." << std::endl
                      << "Header size: " << sizeof(u_int16_t)*lines*samples*bands << std::endl
                      << ".raw file size: " << size << std::endl;
        }

    }
    else
    {
        std::cout << "Cannot Open .raw Image File!" << std::endl;
    }
}

void HSImage::loadSpectrometerData(std::vector<std::string> filenames)
{
    std::vector<float> w,val;

    for(auto filename : filenames)
    {
        std::ifstream file;
        file.open(filename);

        if(file.is_open())
        {
            std::string line;

            for(int i=0;i<18-1;i++)
                file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

            while(std::getline(file,line))
            {
                line.erase(line.end()-1,line.end());
                std::vector<std::string> words = split(line,'\t');

                if(words.size() < 3 && line.compare(">>>>>End Processed Spectral Data<<<<<"))
                {
                    if(w.size() < 1 || stof(words.front()) > w.back())
                    {
                        w.push_back(stof(words.front()));
                        val.push_back(stof(words.back()));
                    }
                }
            }                
        }
        else
        {
            std::cout << strerror(errno) << std::endl;
            std::cout << "Cannot Open Spectrometer Data File!" << std::endl;
            return;
        }

    }

    ambient_intensities.resize(wavelengths.size());
    for(unsigned int i=0;i<ambient_intensities.size();i++)
    {
        std::vector<float>::iterator w_it = std::lower_bound(w.begin(),w.end(),wavelengths[i]);
        ambient_intensities[i] = val[std::distance(w.begin(),w_it)];
    }
    has_spec_data = true;

}

void HSImage::addSpecDataToHeader(std::vector<std::string> filenames)
{
    std::ofstream out_stream;
    out_stream.open(hdr_file,std::ios_base::app | std::ios_base::out);

    if(out_stream.is_open())
    {
        out_stream << "VIS_spec_file " << createRelativeSpecFilepath( filenames[0] ) << std::endl;
        out_stream << "NIR_spec_file " << createRelativeSpecFilepath( filenames[1] ) << std::endl;
        out_stream.close();
    }
}

bool HSImage::hasSpecFiles(std::string header_location)
{
    //load in header file
    std::string line;
    std::ifstream in_stream;
    in_stream.open(header_location);
    if(in_stream.is_open())
    {
        in_stream >> line;
        if(!line.compare("ENVI"))
        {
            bool nir_spec = false,vis_spec = false;
            while(std::getline(in_stream,line))
            {
                std::vector<std::string> words = split(line,' ');

                if(!words[0].compare("NIR_spec_file"))
                    nir_spec = true;

                if(!words[0].compare("VIS_spec_file"))
                    vis_spec = true;
            }
            if(vis_spec && nir_spec)
                return true;
            else
                return false;
        }
        else
        {
            return false;
        }
    }
    else
    {
        std::cout << "Could not open " << header_location << " for reading." << std::endl;
        return false;
    }
}

std::vector<u_int16_t> HSImage::getPixelSpectra(int row, int col)
{
    int loc = row*samples*bands + col*bands;
    std::vector<u_int16_t> output(&pixel_data[loc],&pixel_data[loc + bands]);
    return output;
}

std::vector<u_int16_t> HSImage::getNormalizedPixelSpectra(int row, int col)
{
    int loc = row*samples*bands + col*bands;
    std::vector<u_int16_t> output(&pixel_data[loc],&pixel_data[loc + bands]);

    for(unsigned int i=0;i<output.size();i++)
        output[i] = output[i] - ambient_intensities[i];

    return output;
}

std::vector<float> HSImage::getWavelengths()
{
    return wavelengths;
}

std::vector<float> HSImage::getAmbientIntensities()
{ 
    return ambient_intensities;
}

std::vector<double> HSImage::getPixelTransferFunction(int row, int col)
{
    std::vector<u_int16_t> output = getPixelSpectra(row,col);
    std::vector<double> tf(output.size());

    for(unsigned int i = 0;i<output.size();i++)
    {
        tf[i] = (double)output[i]/(double)ambient_intensities[i];
    }

    return tf;
}

std::vector<cv::Mat> HSImage::getRange(const float lower_wavelength, const float upper_wavelength)
{
    auto const key_1 = std::lower_bound(wavelengths.begin(), wavelengths.end(), lower_wavelength);
    auto const key_2 = std::lower_bound(wavelengths.begin(), wavelengths.end(), upper_wavelength);

    std::vector<float>::iterator it;

    std::vector<cv::Mat> output;

    for(it = key_1;it != key_2;it++)
    {
        cv::Mat test_mat(lines,samples,CV_16UC1, image_map[*it]);
        cv::Mat ret_mat;
        test_mat.copyTo(ret_mat);
        output.push_back(ret_mat);
    }

    return output;
}

std::vector<cv::Mat> HSImage::getSet(const std::vector<float> wavelength_set)
{
    std::vector<cv::Mat> output;
    for(auto wavelength : wavelength_set)
    {
        float key = closest<float>(wavelengths,wavelength);
        if(key < 0)
        {
            key = wavelengths.back();
        }
        cv::Mat test_mat(lines,samples,CV_16UC1, image_map[key]);

        cv::Mat ret_mat;
        test_mat.copyTo(ret_mat);
//        cv::Mat temp,show_mat;
//        cv::transpose(ret_mat,temp);
//        cv::flip(temp,show_mat,0);
        output.push_back(ret_mat);
    }

    return output;
}

cv::Mat HSImage::operator[] (const float wavelength)
{
    float key = closest<float>(wavelengths,wavelength);
    if(key < 0)
    {
        key = wavelengths.back();
    }
    cv::Mat test_mat(lines,samples,CV_16UC1, image_map[key]);
    cv::Mat ret_mat;
    test_mat.copyTo(ret_mat);
//    cv::Mat temp,show_mat;
//    cv::transpose(ret_mat,temp);
//    cv::flip(temp,show_mat,0);

    return ret_mat;

}

std::vector<u_int16_t>&  HSImage::getRawPixelData()
{
    return pixel_data;
}

std::string HSImage::createRelativeSpecFilepath(std::string abs_spec_filepath)
{
    std::pair<std::string::iterator, std::string::iterator> diff_pair;
    diff_pair = std::mismatch(img_file.begin(),img_file.end(),abs_spec_filepath.begin());
    int count = std::distance(diff_pair.second,abs_spec_filepath.end());

    std::string rel_spec_filepath;
    rel_spec_filepath.resize(count);

    std::copy(diff_pair.second,abs_spec_filepath.end(),rel_spec_filepath.begin());

    return rel_spec_filepath;
}

std::string HSImage::createAbsoluteSpecFilepath(std::string rel_spec_filepath)
{
    std::string::reverse_iterator it;
    it = std::find(img_file.rbegin(),img_file.rend(),'/');
    int count = std::distance(img_file.begin(),it.base());

    std::string abs_spec_filepath;
    abs_spec_filepath.resize(count);

    std::copy(img_file.begin(),it.base(),abs_spec_filepath.begin());

    abs_spec_filepath += rel_spec_filepath;

    return abs_spec_filepath;
}

void export_hsimage(pybind11::module m)
{
    namespace py = pybind11;

//    py::module m2 = m.def_submodule("hsimage","ENVI-BIL Hyperspectral Image Interface Module");

    py::class_<HSImage> hsimage (m, "hsimage");
    hsimage
            .def(py::init<>())
            .def(py::init<std::string, std::string>())
            .def(py::init<std::string, std::string, std::vector<std::string>>())
            .def(py::init<const HSImage&>())

            .def("load", (void (HSImage::*)(std::string, std::string)) &HSImage::load, "Load data into HSImage instance", py::arg("header_filename"), py::arg("image_filename"))

            .def("load", (void (HSImage::*)(std::string, std::string, std::vector<std::string>)) &HSImage::load, "Load data into HSImage instance", py::arg("header_filename"), py::arg("image_filename"),py::arg("spec_filename_vector"))

            .def("loadSpectrometerData", &HSImage::loadSpectrometerData)
            .def_static("hasSpecFiles", &HSImage::hasSpecFiles)
            .def("getPixelSpectra", &HSImage::getPixelSpectra)
            .def("getWavelengths", &HSImage::getWavelengths)
            .def("getAmbientIntensities",&HSImage::getAmbientIntensities)
            .def("getPixelTransferFunction", &HSImage::getPixelTransferFunction)
            .def("getRange", &HSImage::getRange)
            .def("getSet", &HSImage::getSet)
            .def("getBand", &HSImage::operator [])
            .def("getPixelArray",&HSImage::getRawPixelData);

}

//void export_hsimage()
//{
//    namespace bp = boost::python;
//    // map the IO namespace to a sub-module
//    // make "from myPackage.class1 import <whatever>" work
//    bp::object hsimageModule(bp::handle<>(bp::borrowed(PyImport_AddModule("hsi.hsimage"))));
//    // make "from mypackage import class1" work
//    bp::scope().attr("hsimage") = hsimageModule;
//    // set the current scope to the new sub-module
//    bp::scope io_scope = hsimageModule;


//    void (HSImage::*d1)(std::string, std::string) = &HSImage::load; // Dealing with overloaded function
//    void (HSImage::*d2)(std::string, std::string, std::vector<std::string>) = &HSImage::load;

//    bp::class_<HSImage>("hsimage")
//        .def(bp::init<std::string, std::string>())
//        .def(bp::init<std::string, std::string, std::vector<std::string>>())
//        .def(bp::init<const HSImage&>())

//        //.def("operator=", &HSImage::operator =) //Member Functions
//        .def("load", d1)
//        .def("load", d2)
////        .def("loadHeader", &HSImage::loadHeader)
////        .def("loadRawImage", &HSImage::loadRawImage)
//        .def("loadSpectrometerData", &HSImage::loadSpectrometerData)
////        .def("addSpecDataToHeader", &HSImage::addSpecDataToHeader)
//        .def("hasSpecFiles", &HSImage::hasSpecFiles)
//        .staticmethod("hasSpecFiles")
//        .def("getPixelSpectra", &HSImage::getPixelSpectra)
//	.def("getWavelengths", &HSImage::getWavelengths)
//	.def("getAmbientIntensities",&HSImage::getAmbientIntensities)
////        .def("getNormalizedPixelSpectra", &HSImage::getNormalizedPixelSpectra)
//        .def("getPixelTransferFunction", &HSImage::getPixelTransferFunction)
//        .def("getRange", &HSImage::getRange)
//        .def("getSet", &HSImage::getSet)
//        .def("getBand", &HSImage::operator [])
//        .def("getPixelArray",&HSImage::getRawPixelData,
//             boost::python::return_value_policy<boost::python::reference_existing_object>());
//}
