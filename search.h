
#ifndef search_hpp
#define search_hpp

#include <iostream>
#include <dirent.h>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

bool search_in_directory(const string& path, const string& tar);


#endif