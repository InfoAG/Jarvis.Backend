#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <dlfcn.h>
#include <vector>

namespace CAS
{

typedef libhandle void*; 
typedef funchandle void*; 

class Library 
{
private: 
	libhandle hdl; 
	string name; 
	funchandle factory; 
	vector<string> classNames; 
public:
	Library(string); 
	void* getInstanceOf(string);
	~Lirary(); 
}; 

}; 

#endif
