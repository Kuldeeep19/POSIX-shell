#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <cstring>
#include "pwd.h"

using namespace std;

extern string homeDir;

void handle_pwd() {
    char currentdir[1024];
    char homedirPath[1024];
    
    if (getcwd(currentdir, sizeof(currentdir)) == nullptr) {
        perror("Failed to get current working directory");
        return;
    }

    struct passwd *pwEntry = getpwuid(getuid());
    if (pwEntry == nullptr) {
        perror("Failed to get password entry");
        return;
    }
    strcpy(homedirPath, pwEntry->pw_dir);

    string currdirStr(currentdir);

    if (currdirStr.find(homeDir) == 0) {
        currdirStr.replace(0, homeDir.size(), "");


    }
    if(currdirStr.size()==0) currdirStr+="/";

    cout << currdirStr << endl;
}
