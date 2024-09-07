#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <vector>
#include <cstring>  

#include "cd.h"

using namespace std;
extern string homeDir;
void handle_cd(const vector<string> &args) {
    // get the current directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        perror("getcwd failed");
        return;
    }

    // get the home directory
    struct passwd *pw = getpwuid(getuid());
    if (pw == nullptr) {
        perror("getpwuid failed");
        return;
    }
    // string homeDir = pw->pw_dir;

    // different cases for cd
    if (args.size() == 1) {
        // No arguments, go to the home directory
        if (chdir(homeDir.c_str()) != 0) {
            perror("chdir failed");
        }
    } else if (args.size() == 2) {
        string path = args[1];
        if (path == "-") {
            // Go to the previous directory
            if (chdir(getenv("OLDPWD")) != 0) {
                perror("chdir failed");
            }
        } else if (path == "--") {
            // Handle -- as a special case if needed (custom behavior)
            // Currently, treating it the same as "-"
            if (chdir(getenv("OLDPWD")) != 0) {
                perror("chdir failed");
            }
        } else if (path == "~") {
            // Go to the home directory
            if (chdir(homeDir.c_str()) != 0) {
                perror("chdir failed");
            }
        }
        else if(path.size()==0)
        {
            cout<<"iam in path size 0"<<endl;
           if( chdir(homeDir.c_str()) != 0)
            cout<<"invalid argumrnt";
        }
         else {
            // Handle relative or absolute paths
            if (chdir(path.c_str()) != 0) {
                perror("chdir failed");
            }
        }
    } else {
        cout << "Invalid arguments for cd" << endl;
    }
    
    // Update OLDPWD environment variable to the current directory before changing
    setenv("OLDPWD", cwd, 1);
}
