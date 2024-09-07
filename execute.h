#ifndef execute_hpp
#define execute_hpp

#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <string>
#include <vector>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include "cd.h"
#include "echo.h"
#include "pwd.h"
#include "ls.h"
#include "pinfofile.h"
#include "search.h"
#include "back.h"
#include "redirection.h"
#include "his.h"
#include "pipe.h"
using namespace std;

void execute( string command);

#endif