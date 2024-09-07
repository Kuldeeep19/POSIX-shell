#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include "back.h"
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

void back(vector<string> &args, bool runInBackground) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return;
    }

    if (pid == 0) {  // child process
        // convert the vector of arguments to an array of char pointers
        vector<char*> c_args;
        for (auto &arg : args) {
            c_args.push_back(const_cast<char*>(arg.c_str()));
        }
        c_args.push_back(NULL);  // null terminate the array

        // Execute the command
        if (execvp(c_args[0], c_args.data()) < 0) {
            perror("Execution failed");
            exit(EXIT_FAILURE);
        }
    } else {  // Parent process
        if (runInBackground) {
            cout << "Background PID : " << pid << endl;  // Print PID of the background process
        } else {
            waitpid(pid, NULL, 0);  // Wait for the child process to complete
        }
    }
}
