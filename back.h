#ifndef back_hpp
#define back_hpp

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include <sys/types.h>
 #include <sys/stat.h>
using namespace std;


void back(vector<string> &args, bool runInBackground);


#endif
