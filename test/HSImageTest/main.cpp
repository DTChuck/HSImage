#include <iostream>
#include <string>
#include <vector>

#include "hsimage.h"
#include "classifiedhsimage.h"

int main()
{
    std::string header_loc = "/media/ryan/HDD/Perception/TO8_SVN/TO8-docs/HS_Images/10_26_15/image1_ThinTree_VNIR.hdr";
    std::string img_loc = "/media/ryan/HDD/Perception/TO8_SVN/TO8-docs/HS_Images/10_26_15/image1_ThinTree_VNIR.raw";

    std::string vis_loc = "/media/ryan/HDD/Perception/TO8_SVN/TO8-docs/HS_Images/10_26_15/Spectrometers/VIS00000.txt";
    std::string nir_loc = "/media/ryan/HDD/Perception/TO8_SVN/TO8-docs/HS_Images/10_26_15/Spectrometers/NIR00000.txt";

    std::vector<std::string> spec_location;
    spec_location.push_back(vis_loc);
    spec_location.push_back(nir_loc);

    HSImage img(header_loc,img_loc,spec_location);

    std::cout << "Img Loaded" << std::endl;
    return 0;
}

