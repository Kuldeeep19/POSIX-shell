#ifndef cd_hpp
#define cd_hpp

#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <vector>
#include <cstring>  // For strcpy and strerror


using namespace std;

void handle_cd(const vector<string> &args);

#endif