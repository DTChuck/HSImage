#include "colormap.h"

const char *colorMap::startTag = "<CMAP_START>\n\0";
const char *colorMap::endTag = "<CMAP_END>\n\0";
const int colorMap::startTagSize = strlen(colorMap::startTag);
const int colorMap::endTagSize = strlen(colorMap::endTag);

//note, find the number of character of tag with std::strlen(char*)


//colorMap::colorMap():colorHMO(colorMapHierarchy::staticMetaObject)
colorMap::colorMap()
{
	this->targetClasses = std::vector<target>();
	this->targetInstances = std::vector<target>();
}

//colorMap::colorMap(int majV, int minV) : colorHMO(colorMapHierarchy::staticMetaObject)
colorMap::colorMap(uint32_t majV, uint32_t minV)
{
	this->version = versionData(majV, minV);
	this->targetClasses = std::vector<target>();
	this->targetInstances = std::vector<target>();
	
}
colorMap::~colorMap(){
	
}

bool colorMap::operator==(const colorMap &other) const{
	if(this->version != other.getVersion()){return false;}
	if(this->getNumTargets(targetType::targetClass) != other.getNumTargets(targetType::targetClass)){return false;}
	if(this->getNumTargets(targetType::targetInstance) != other.getNumTargets(targetType::targetInstance)){return false;}

	targetType::types currType = targetType::targetClass;
	for(int i = 0; i < this->getNumTargets(currType); i++){
		if(this->getTargetList(currType)->at(i) != other.getTargetList(currType)->at(i)){return false;}
	}
	currType = targetType::targetInstance;
	for(int i = 0; i < this->getNumTargets(currType); i++){
		if(this->getTargetList(currType)->at(i) != other.getTargetList(currType)->at(i)){return false;}
	}

	return true;
}
bool colorMap::operator!=(const colorMap &other) const{
	return !(*this==other);
}



bool colorMap::greaterVersion(const colorMap &cMap1, const colorMap &cMap2){
	//The highest value of colorMap.version.major should be first
	//followed by that the minor version
	//To follow 'strict weak ordering' for all x, it is not the case that x < x
	
	if(cMap1.getMajorV() > cMap2.getMajorV()){
		//major is greater,
		return true;
	}
	else if(cMap1.getMajorV() == cMap2.getMajorV()){
		//major is equal, check minor
		if(cMap1.getMinorV() > cMap2.getMinorV()){
			//major values equal, minor1 greater than minor2
			return true;
		}
		else if(cMap1.getMinorV() == cMap2.getMinorV()){
			//definitions equal, return false in order to follow strict ordering
			return false;
		}
		else{
			//major values equal, minor1 less than minor2
			return false;
		}
	}
	else{
		//major1 is less than major2
		return false;
	}
	
}




// *******************  START   TARGETS     ***********//

void colorMap::addTarget(target tarIn){
	switch (tarIn.getType()){
	case targetType::targetClass:
		this->targetClasses.push_back(tarIn);
		break;
	case targetType::targetInstance:
		this->targetInstances.push_back(tarIn);
		break;
	default:
        {

        }
//		qDebug() << "color map addTarget case not handled" << tarIn.getType();
	}
}

void colorMap::addTarget(target tarArray[], int length){
	for(int i = 0; i < length; i++){
		this->addTarget(tarArray[i]);
	}
}

void colorMap::addTarget(std::vector<target> tarV){
	int s = tarV.size();
	this->targetClasses.reserve(s);
	this->targetInstances.reserve(s);
	
	for(int i = 0; i < s; i++){
		this->addTarget(tarV.at(i));
	}
	
}

void colorMap::removeTarget(const target &targetToRemove){
	//Find index
	int tInd = this->findTargetInd(targetToRemove);
	if (tInd < 0){
//		qDebug() << "Error in colorMap::removeTarget(target&). Target invalid";
		return;
	}

	//Find the target list
	std::vector<target> *targetV;
	switch(targetToRemove.getType()){
	case targetType::targetClass:
		targetV = &this->targetClasses;
		break;
	case targetType::targetInstance:
		targetV = &this->targetInstances;
		break;
	default:
//		qDebug() << "Error in colorMap::findTargetInd(target &). Unknown target type: "
//				 << targetToRemove.getType();
		return;
	}

	targetV->erase(targetV->begin() + tInd);
}

int colorMap::findTargetInd(const target &tIn){
	//Find the target list
	std::vector<target> *targetV;
	switch(tIn.getType()){
	case targetType::targetClass:
		targetV = &this->targetClasses;
		break;
	case targetType::targetInstance:
		targetV = &this->targetInstances;
		break;
	default:
//        qDebug() << "Error in colorMap::findTargetInd(target &). Unknown target type ";
		return -1;
	}

	//Find the index
	bool isMatch = false;
	int index = -1;
	for(unsigned int i = 0; i < targetV->size(); i++){
		if(targetV->at(i) == tIn){
			isMatch = true;
			index = i;
			break;
		}

	}

	if(!isMatch){
		//No match found, return -1
		return -1;
	}
	else{
		//Match found
		return index;
	}

}





// *******************  END     TARGETS     ***********//



// *******************  START   PARSING     ***********//

void colorMap::toFile(std::fstream &fs){
	//Start tag for color map data
    fs.write(startTag,strlen(startTag));
	
	//Major and Minor
    fs.write((char*)&this->version._major,sizeof(version._major));
    fs.write((char*)&this->version._minor,sizeof(version._minor));
	
	//Target classes
	unsigned int i;
	for(i = 0; i < this->targetClasses.size(); i++){
		this->targetClasses.at(i).toFile(fs);
	}
	for(i = 0; i < this->targetInstances.size(); i++){
		this->targetInstances.at(i).toFile(fs);
	}
	
	//End tag
	fs.write(endTag,endTagSize);
	fs.flush();
	
}


bool colorMap::fromFile(std::fstream &fs){
	//check for the start tag
    char* tempSTag = new char[strlen(startTag)];
	fs.read(tempSTag, startTagSize);
	bool cont = !strncmp(startTag, tempSTag, startTagSize);  //strncmp returns 0 if the strings are equal, hence the !
//	if(!cont){qDebug() << "Color map start tag not found"; return false;}
	delete tempSTag;
	
	//Initialize vectors of targets
	this->targetClasses.erase(targetClasses.begin(), targetClasses.end());
	this->targetInstances.erase(targetInstances.begin(), targetInstances.end());
	
	//Start tag received successful, parse members
    fs.read((char*)(&this->version._major), sizeof(this->version._major));
    fs.read((char*)(&this->version._minor), sizeof(this->version._minor));
	
	//populate target list
	int cC = 0; //target class counter (class definition)
	int iC = 0; //target instance counter (instance definition)
	target tempTarget = target();
	while(target::nextTargetExist(fs) && !fs.eof()){
		//        qDebug() << "Next target exist" << target::nextTargetExist(fs) << "EOF " << fs.eof();
		if( target::nextTargetExist(fs) ){ tempTarget.fromFile(fs);}    //get next target
		switch (tempTarget.getType()){
		case targetType::targetClass:
			this->targetClasses.push_back(tempTarget);
			cC++;
			break;
		case targetType::targetInstance:
			this->targetInstances.push_back(tempTarget);
			iC++;
		default:
//			qDebug() << "Target of unkown type " << tempTarget.getType() << "not recorded in color map object";
			break;
		}
	}
//	qDebug() << "Color Map:  number targets" << cC+iC;
	
	//read past the end tag
	char* tempEndTag = new char[endTagSize];
	fs.read(tempEndTag, endTagSize);
    cont  = !strncmp(tempEndTag, endTag, endTagSize); //returns 0 if matches
	if(!cont){
		//if end tag is missing/wrong
//		qDebug() << "Improper target tag" << QString::fromLatin1(endTag);
//        qDebug() << "bool" << (strncmp(tempEndTag, endTag, endTagSize));
//		qDebug() << "\tRead tag: " << QString::fromLatin1(tempEndTag);
//		qDebug() << "\tStored tag: " << QString::fromLatin1(endTag);
	}
	delete tempEndTag;
	
	//sort the vectors of targets
	
	return cont;
}



bool colorMap::nextColorMapExist(std::fstream &fs){
	//    qDebug() << "Start color map exist";
	std::fstream::pos_type startPos = fs.tellg();   //read in current pos
	//    qDebug() << "\tcurrent pos " << (int)startPos;
	
	char* tempSTag = new char[startTagSize];   //read in start tag
	fs.read(tempSTag, startTagSize);
	bool exist = !strncmp(tempSTag, startTag, startTagSize);
	//    qDebug() << "\tMatch found" << exist;
	
	fs.seekg(startPos);  //return stream to start state
	//    qDebug() << "\tcurrent pos " << fs.tellg() << "Is good: " << fs.good();
	
	delete tempSTag;
	return exist;
}





// *******************  END     PARSING     ***********//



// *******************  START   GET/SET     ***********//
void colorMap::setMajorV(uint32_t mV){
    this->version._major = mV;
}
void colorMap::setMinorV(uint32_t mV){
    this->version._minor = mV;
}
uint32_t colorMap::getMajorV() const{
    return this->version._major;
}
uint32_t colorMap::getMinorV() const{
    return this->version._minor;
}
versionData colorMap::getVersion() const{
	return this->version;
}
int colorMap::getNumTargets(targetType::types tType) const{
	//Select the target list
	switch(tType){
	case targetType::targetClass:
		return this->targetClasses.size();
		break;
	case targetType::targetInstance:
		return this->targetInstances.size();
		break;
	default:
//		qDebug() << "Unkown target type at colorMap::toQComboBox " << tType;
		return -1;
		break;
	}
}
std::vector<target> const *colorMap::getTargetList(targetType::types tType) const{
	switch(tType){
	case targetType::targetClass:
		return &(this->targetClasses);
		break;
	case targetType::targetInstance:
		return &(this->targetInstances);
		break;
	default:
//		qDebug() << "Unknown target type in colorMap::getTargetList " << tType;
		return NULL;
		break;
	}
}

void colorMap::setTargetVector(const std::vector<target> &inputVector){
//	qDebug() << "Enter colorMap::setTargetVector, new vector has " << inputVector.size() << "targets";
	//Find what target type it is
	targetType::types inputTType = inputVector.at(0).getType();
	std::vector<target> *originalTargets;
	
	switch(inputTType){
	case targetType::targetClass:
		originalTargets = &(this->targetClasses);
		break;
	case targetType::targetInstance:
		originalTargets = &(this->targetInstances);
		break;
	default:
//		qDebug() << "Error in colorMap::setTargetVector unkown target type";
		break;
	}
	*originalTargets = inputVector;
}

// *******************  END     GET/SET     ***********//


