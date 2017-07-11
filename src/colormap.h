#ifndef COLORMAP_H
#define COLORMAP_H

#include <fstream>
#include <vector>
#include <stdint.h>
#include <string>

#include "annEnums.h"
#include "target.h"



struct versionData{
    uint32_t _major;
    uint32_t _minor;
	
    versionData(uint32_t _majorV, uint32_t _minorV){
        this->_major = _majorV;
        this->_minor = _minorV;
	}
	//default constructor
    versionData(){
        this->_major = 0;
        this->_minor = 0;
	}
	//default destructor
    ~versionData(){
	}
	//default copy constructor
    versionData(const versionData& other) :
        _major(other._major), _minor(other._minor){}
	//overload the is equal operator
    bool operator==(const versionData &other) const{
        if(this->_major != other._major){return false;}
        if(this->_minor != other._minor){return false;}
		return true;
	}
    bool operator!=(const versionData &other) const{
		return !(*this==other);
	}

};


class colorMap
{
	//future functionality: info to upgrade the version data
	//(mapping previous definitions to current version))
	
	
public:
	static const char* startTag;
	static const char* endTag;
	static const int startTagSize;
	static const int endTagSize;
	
	colorMap();
	colorMap(uint32_t, uint32_t);
	~colorMap();
	
    bool operator==(const colorMap &other) const;
    bool operator!=(const colorMap &other) const;

	//    const QMetaObject colorHMO;     //meta object for colorMapHierarchy enum
	
	//managing target definitions
	void addTarget(target);
	void addTarget(target[], int);
	void addTarget(std::vector<target>);
	void removeTarget(const target &);
	int findTargetInd(const target &);
	
	static bool greaterVersion(const colorMap &cMap1, const colorMap &cMap2);
	
	//Set, Get
    void setMajorV(uint32_t);
    void setMinorV(uint32_t);
    uint32_t getMajorV() const;
    uint32_t getMinorV() const;
	versionData getVersion() const;
	int getNumTargets(targetType::types) const;
	
	std::vector<target> const * getTargetList(targetType::types) const;
	void setTargetVector(const std::vector<target> &);
	
	//Parsing the settings file
	void toFile(std::fstream &);
	bool fromFile(std::fstream &fs);
	bool isEnd(std::fstream &);
	static bool nextColorMapExist(std::fstream &fs);

	
	
	
private:
	
	struct versionData version;
	
	std::vector<target> targetClasses;
	std::vector<target> targetInstances;
	
};






#endif // COLORMAP_H
