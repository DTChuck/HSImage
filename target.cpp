#include "target.h"


const char *target::startTag = "<TSTART>\n\0";
const char *target::endTag = "<TEND>\n\0";
const int target::startTagSize = strlen(target::startTag);
const int target::endTagSize = strlen(target::endTag);

const int target::maxTitleSize = 255;
const int target::maxDescSize = 255;



target::target(){
	
}
target::~target(){
	
}


target::target(uint8_t r, uint8_t g, uint8_t b, std::string titleIn, targetType::types typeIn){
	this->r = r;
	this->g = g;
	this->b = b;
	this->setTitle(titleIn);
	this->setDescription("No Description");
	this->type = typeIn;
}

bool target::operator == (const target& other) const{
	//Check type
	if(this->getType() != other.getType()){return false;}

	//Check colors
	if(this->getR() != other.getR()){return false;}
	if(this->getG() != other.getG()){return false;}
	if(this->getB() != other.getB()){return false;}

	//Check title/description
	if( this->getTitle() != other.getTitle()){return false;}
	if( this->getDescription() != other.getDescription()){return false;};

	//Passed all checks
	return true;
}

bool target::operator !=(const target& other) const{
	return !(*this == other);
}


void target::toFile(std::fstream &fs){
//	std::fstream::pos_type outPosStart = fs.tellp();       //current output position
	
	//Start tag
	fs.write(startTag, startTagSize);
	
	//members
	fs.write((char*)&titleLen, sizeof(titleLen));
	fs.write(title,titleLen);       //write +1 for the termination char
	fs.write((char*)&descLen, sizeof(descLen));
	fs.write(description,descLen);   //write +1 for the termination char (IMPORTANT)
	fs.write((char*)&r, sizeof(r));
	fs.write((char*)&g, sizeof(g));
	fs.write((char*)&b, sizeof(g));
	fs.write((char*)&type, sizeof(type));
	
	//End tag
	fs.write(endTag, endTagSize);
//	std::fstream::pos_type inPosEnd = fs.tellg();       //current input pos
	
}


bool target::fromFile(std::fstream &fs){
	//    qDebug() << "ENTER fromFile target";
	
	//Find start
	char* tempStart = new char[startTagSize];
	fs.read(tempStart, startTagSize);
    bool cont = !strncmp(tempStart, startTag, startTagSize);
	delete tempStart;
	//    qDebug() << "\tCont" << cont;
	if(!cont){return cont;}
	
	//Title Length
	fs.read((char*)&(this->titleLen), sizeof(titleLen));
	//    qDebug() << "\ttitle length" << this->titleLen;
	
	//Title value
	this->title = new char[titleLen];
	fs.read(this->title, this->titleLen);
	//    qDebug() << "\ttitle" << QString::fromLatin1(this->title);
	
	//Description Length
	fs.read((char*)&(this->descLen), sizeof(descLen));
	//    qDebug() << "\tdesc length" << this->descLen;
	
	//Description Value
	this->description = new char[descLen];
	fs.read(this->description, this->descLen);
	//    qDebug() << "\tdesc" << QString::fromLatin1(this->description);
	
	//RGB
	fs.read((char*)&this->r, sizeof(this->r));
	fs.read((char*)&this->g, sizeof(this->g));
	fs.read((char*)&this->b, sizeof(this->b));
	//    qDebug() << "\tRGB" << (int) r << (int) g << (int) b;
	
	//Type tag
	fs.read((char*)&this->type, sizeof(this->type));
	//    qDebug() << "\ttype" << this->type;
	
	//Read past end tag
	char* tempEnd = new char[endTagSize];
	fs.read(tempEnd, endTagSize);
    cont  = !strncmp(tempEnd, endTag, endTagSize); //returns 0 if matches

	delete tempEnd;
	
	return false;
}


bool target::nextTargetExist(std::fstream &fs){
	std::fstream::pos_type startPos = fs.tellg();   //store current input pos
	
	char* tempSTag = new char[startTagSize];   //read in start tag
	fs.read(tempSTag, startTagSize);
	bool exist = !strncmp(tempSTag, startTag, startTagSize);
	
	fs.seekg(startPos);  //return stream to start state
	
	delete tempSTag;
	return exist;
}








// *******************  START   GET/SET     ***********//
void target::setTitle(std::string titIn){
	if(this->title == NULL){delete this->title;}    //delete if not null
	this->titleLen = titIn.size() + 1;  //+1 for null char
	this->title = new char[this->titleLen];
    strncpy(this->title, titIn.data(), this->titleLen);
	
}
void target::setDescription(std::string descIn){
	if(this->description == NULL){delete this->description;}    //delete if not null
	this->descLen = descIn.size() + 1;  //+1 for null char
	this->description = new char[descLen];
    strncpy(this->description, descIn.data(), this->descLen);
}
void target::setType(targetType::types typIn){
	this->type = typIn;
}
void target::setR(uint8_t u8In){
	this->r = u8In;
}
void target::setG(uint8_t u8In){
	this->g = u8In;
}
void target::setB(uint8_t u8In){
	this->b = u8In;
}

std::string target::getTitle() const{
    std::shared_ptr<char> p(title, &free);
    std::string title_string(p.get());
    return title_string;
}
std::string target::getDescription() const{
    std::shared_ptr<char> p(description, &free);
    std::string desc_string(p.get());
    return desc_string;
}
targetType::types target::getType() const{
	return this->type;
}

uint8_t target::getR() const{ return this->r;}
uint8_t target::getG() const{ return this->g;}
uint8_t target::getB() const{ return this->b;}




// *******************  END     GET/SET     ***********//

