#ifndef TARGET_H
#define TARGET_H

#include <fstream>
#include <memory>

#include <opencv2/core.hpp>

#include "stdint.h"
#include "annEnums.h"



class target
{
	
public:
	//Data for parsing
	static const char* startTag;	//data to begin a target definition in a file
	static const char* endTag;		//data to end a target definition in a file
	static const int startTagSize;
	static const int endTagSize;
	
	static const int maxTitleSize;
	static const int maxDescSize;
	
	target();
	~target();
	target(uint8_t, uint8_t, uint8_t,
           std::string titleIn = "Test",
		   targetType::types typeIn = targetType::targetClass);
	bool operator ==(const target&) const;
	bool operator !=(const target&) const;
	
    void setTitle(std::string);
    void setDescription(std::string);
	void setType(targetType::types);
	void setR(uint8_t);
	void setG(uint8_t);
	void setB(uint8_t);
	
    std::string getTitle() const;
    std::string getDescription() const;
	targetType::types getType() const;
	uint8_t getR() const;
	uint8_t getG() const;
	uint8_t getB() const;
	
	
	//parsing
	void toFile(std::fstream &);
	bool fromFile(std::fstream &);
	static bool nextTargetExist(std::fstream &);
	std::string parseTitle(std::fstream, std::fstream::pos_type);
	
	
	
private:
	
	char* title;      //identifier for the target
	int titleLen;       //length of the title string
	char* description;//short description for the target
	int descLen;        //length of the description string
	uint8_t r;
	uint8_t g;
	uint8_t b;
	targetType::types type;  //Whether the target is a class, instance, or other
	
	
};



#endif // TARGET_H
