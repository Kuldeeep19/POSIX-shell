#ifndef redirection_hpp
#define redirection_hpp

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

using namespace std;


void execute_cmd_with_redirection(const vector<string>& args);

#endif