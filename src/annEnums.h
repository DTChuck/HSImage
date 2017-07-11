#ifndef ANNENUMS_H
#define ANNENUMS_H


class cameras{

public:
	//All camera IDs must be 4 chars
    enum camID {RVIS = 0, RNIR, LVIS, LNIR, LWIR, SWIR, PLR0, PLR1, PLR2, PLR3, STK0, STK1, STK2, STK3, CLAS, INST,HYPR};

	static const uint camValueLength = 4;	//number of chars in the camera names e.g. RVIS
	static const uint imgTimeStampLength_sec  = 10;	//no. of chars in timestamp portion of name if in seconds e.g.1387398513
	static const uint imgTimeStampLength_msec = 13;	//no. of chars in timestamp portion of name if in milliseconds


};


class targetType{
public:
    enum types{
        targetClass = 0,
        targetInstance,
    };

};

class colorMapHierarchy{
public:
    //targetClass are the targets for object classes (e.g. vegetation)
    //targetInstance are the targets for object number (e.g. first, ... , Nth vegetation)
    //conversion is the map from the current color definitions to the next definition)
    enum hierarchy{version};
    enum versionFields{major, minor};
    enum versionGroups {targetClass, targetInstance, conversion};

private:
};









#endif // ANNENUMS_H
