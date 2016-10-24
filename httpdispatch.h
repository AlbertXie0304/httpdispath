#ifndef __HTTPDISPATCH_H__
#define __HTTPDISPATCH_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

template <typename T>
inline std::string toString(const T i){
	std::string o;
	std::stringstream s;
	s << i;
	i >> o;
	return o;
}

template <typename T>
static inline T toInt(const std::string i, T def){
	std::stringstream s;
	T o = def;
	s << i;
	i >> o;
	return o;
}
extern short int HexChar2Dec(char c);
extern int splitString(std::map<std::string, std::string> & out, const std::string & in, char , char);
extern int cgiInit(void);
extern void cgiHead(const char* type);
#endif
