#ifndef pipe_hpp
#define pipe_hpp

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <vector>
#include <sstream>
#include <iterator>
#include<algorithm>

using namespace std;

// Executes a sequence of piped commands
void runpipedcommands(vector<string>& cmdList);

#endif
