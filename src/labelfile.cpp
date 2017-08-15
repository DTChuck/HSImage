#include "labelfile.h"

LabelFile::LabelFile()
{
}

LabelFile::LabelFile(std::string filename)
{
    loadFile(filename);
}

void LabelFile::loadFile(std::string filename)
{
    //load .lif file
    std::ifstream lif_file;
    lif_file.open(filename);

    //grab json data
    Json::Value json_data;
    lif_file >> json_data;

    //grab shapes
    Json::Value shapes = json_data["shapes"];

    //prepare instance vectors
    obj_vector.clear();
    class_info.clear();
    class_map.clear();

    //process each shape individually
    for(auto shape : shapes)
    {
        //process each shape here
        LabeledObject obj;

        //get object label
        std::string label = shape["label"].asString();
	
	//get object color
	cv::Vec3b color;

	//look to see if label already present in image
	auto search = class_map.find(label);
	if(search != class_map.end())
	{
		//if label is present, get color from color_map
		color = class_map[label];
	}
	else
	{
		//if label is not present, get random color and add it to color map
		color = getRandomColor();
		class_map[label] = color;
	}
       
        //get polygon points
        std::vector<cv::Point> poly;
        Json::Value points = shape["points"];
        for(auto point : points)
        {
            cv::Point p;
            p.x = point[0].asInt();
            p.y = point[1].asInt();
            poly.push_back(p);
        }

        //create Object
        obj.setName(label);
        obj.setColor(color);
        obj.setPolygon(poly);

        //create classColor object
        classColor c;
        c.first = label;
        c.second = color;

        //push into vectors
        obj_vector.push_back(obj);
        class_info.push_back(c);
    }

    //create rgb image
    Json::Value img_data = json_data["imageData"];
    rgb_image = createRGBImage(img_data);

    //create label image
    label_overlay = createLabelImage();

}

cv::Vec3b LabelFile::getRandomColor()
{
    std::random_device r;
    std::mt19937 e(r());

    std::uniform_int_distribution<> color_dist(1,255);

    cv::Vec3b color;
    color[0] = color_dist(e);
    color[1] = color_dist(e);
    color[2] = color_dist(e);

    //check for conflicts with used colors
    if(class_info.size() > 0)
    {
        for(unsigned int idx = 0; idx < class_info.size(); idx++)
        {
            cv::Vec3b c = class_info[idx].second;
            while(c[0] == color[0] && c[1] == color[1] && c[2] == color[2])
            {
                color[0] = color_dist(e);
                color[1] = color_dist(e);
                color[2] = color_dist(e);

                idx = -1;
            }
        }
    }

    return color;
}

cv::Mat LabelFile::createLabelImage()
{
    cv::Mat overlay = cv::Mat::zeros(rgb_image.rows,rgb_image.cols,CV_8UC3);

    for(LabeledObject obj : obj_vector)
    {
        std::vector<std::vector<cv::Point>> polygon;
        polygon.push_back(obj.getPolygon());
        cv::Vec3b c= obj.getColor();
        cv::fillPoly(overlay,polygon,cv::Scalar(c[0],c[1],c[2]),cv::LINE_AA);
    }

    return overlay;
}

cv::Mat LabelFile::createRGBImage(Json::Value img_data)
{
    //load image data from json as string
    std::string string_data = img_data.asString();

    //decode base64 data that the image is stored in
    //get padding length
    unsigned int paddChars = std::count(string_data.begin(),string_data.end(), '=');
    //move padding to correct alphabet for conversion
    std::replace(string_data.begin(),string_data.end(),'=','A');
    //copy to new string using base64 iterators from boost
    std::string result(it_binary_t(string_data.begin()),it_binary_t(string_data.end()));
    //remove padding for use
    result.erase(result.end() - paddChars, result.end());
    //convert to char vector
    std::vector<char> data(result.begin(),result.end());
    //decode image into cv::Mat
    cv::Mat rgb = cv::imdecode(data,cv::IMREAD_COLOR);

    return rgb;
}

cv::Mat LabelFile::getRGBImage()
{
    return rgb_image;
}

cv::Mat LabelFile::getLabelImage()
{
    return label_overlay;
}

cv::Mat LabelFile::getViewingImage()
{
    cv::Mat view;
    cv::addWeighted(rgb_image,0.90,label_overlay,0.50,0,view);

    return view;
}

std::vector<classColor> LabelFile::getClassInfo()
{
    return class_info;
}

//////////////////////////////////////////////  LabeledObject Class Definitions ////////////////////////////////////

LabeledObject::LabeledObject()
{

}

void LabeledObject::setColor(cv::Vec3b new_color)
{
    color = new_color;
}

void LabeledObject::setName(std::string new_name)
{
    name = new_name;
}

void LabeledObject::setPolygon(std::vector<cv::Point> new_polygon)
{
    polygon = new_polygon;
}

cv::Vec3b LabeledObject::getColor()
{
    return color;
}

std::string LabeledObject::getName()
{
    return name;
}

classColor LabeledObject::getInfo()
{
    classColor c;
    c.first = name;
    c.second = color;

    return c;
}

std::vector<cv::Point> LabeledObject::getPolygon()
{
    return polygon;
}

void export_labelfile(pybind11::module m)
{
    namespace py = pybind11;

//    py::module m2 = m.def_submodule("labelfile","CSAIL/LabelMe label file interface module");

    py::class_<LabelFile> labelfile (m, "labelfile");
    labelfile
            .def(py::init<>())
            .def(py::init<std::string>())
            .def("load", &LabelFile::loadFile)
            .def("getRGBImage", &LabelFile::getRGBImage)
            .def("getLabelImage", &LabelFile::getLabelImage)
            .def("getOverlayImage", &LabelFile::getViewingImage)
            .def("getClassInfo", &LabelFile::getClassInfo);

}

//void export_labelfile()
//{
//    namespace bp = boost::python;
//    //map the namespace to a submodule
//    //make "from myPackage.class1 import <anything>" work
//    bp::object hsimageModule(bp::handle<>(bp::borrowed(PyImport_AddModule("hsi.labelfile"))));
//    //make "from myPackage import class1 work
//    bp::scope().attr("labelfile") = hsimageModule;
//    //set current scope to the new sub-module
//    bp::scope io_scope = hsimageModule;

//    bp::class_<LabelFile>("labelfile")
//	    .def(bp::init<std::string>())
//	    .def("load", &LabelFile::loadFile)
//	    .def("getRGBImage", &LabelFile::getRGBImage)
//	    .def("getLabelImage", &LabelFile::getLabelImage)
//	    .def("getOverlayImage", &LabelFile::getViewingImage)
//	    .def("getClassInfo", &LabelFile::getClassInfo);
//}
