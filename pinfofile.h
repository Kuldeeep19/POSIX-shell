#ifndef pinfofile_hpp
#define pinfofile_hpp

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <limits.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


using namespace std;


void pinfo(pid_t pid) ;
#endif