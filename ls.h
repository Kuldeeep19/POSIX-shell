#ifndef ls_hpp
#define ls_hpp

#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <iomanip>
#include <ctime>

using namespace std;


void parseArguments(const vector<string>& args, bool& showAll, bool& longFormat, std::vector<std::string>& paths);
void listDirectory(const string& path, bool showAll, bool longFormat) ;
void lsCommand(const vector<string>& args);



#endif
