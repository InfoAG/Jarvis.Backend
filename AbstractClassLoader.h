#ifndef _ABSTRACTCLASSLOADER_H_
#define _ABSTRACTCLASSLOADER_H_

#include <string>
#include <map>
#include "Library.h"

using namespace std; 

namespace CAS
{

class AbstractClassLoader 
{
private: 
	virtual const string prefix; 
	virtual const string greetingFunction; 
	map<string,Library> libs; 

public: 
	AbstractClassLoader(); 
	void update(); 
	~AbstractClassLoader();
	//TODO: Lib Accessors
};

}; 

#endif
