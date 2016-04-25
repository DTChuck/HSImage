#ifndef ANNENUMS_H
#define ANNENUMS_H

/*!
 *\file annEnums.h Defines enumerations for annotations
 * Version 1.0.0
 *
 */

/*!
 * \brief The camera class This class defines the enum for camera prefixes
 * Instantiate the camera class as a: QMetaobject &mo = namespace::staticMetaObject
 * Find the index of the enumerator: int enumIndex = mo.indexOfEnumerator("enum name")
 * Instantiate the enum: QMetaEnum enumVar = mo.enumerator(enumIndex)
 */
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
    /*!
 * \brief The targetType enum This enum defines the type of target defined.
 * A target of the type class is the type of object (e.g. vegetation)
 * A target of the type instance is a
 */
    enum types{
        targetClass = 0,
        targetInstance,
    };

};



/*!
 * \brief The colorMap class This class
 */
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
