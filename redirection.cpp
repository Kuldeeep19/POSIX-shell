#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "redirection.h"

using namespace std;

void execute_cmd_with_redirection(const vector<string>& args) {

    int inpfd = -1, outfd = -1;
    bool inredirect = false, outredirect = false;
    bool append = false;
    vector<const char*> cmd_args;

    for (size_t indx = 0; indx < args.size(); ++indx) {
        if (args[indx] == "<") {
            if (indx + 1 < args.size()) {
                inpfd = open(args[indx + 1].c_str(), O_RDONLY);
                if (inpfd < 0) {
                    cerr << "Error: Input file " << args[indx + 1] << " cannot be opened." << endl;
                    return;
                }
                inredirect = true;
                indx++; // Skip the filename
            } else {
                cerr << "Error: Missing input file." << endl;
                return;
            }
        } else if (args[indx] == ">") {
            if (indx + 1 < args.size()) {
                outfd = open(args[indx + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                outredirect = true;
                indx++; // Skip the filename
            } else {
                cerr << "Error: Missing output file." << endl;
                return;
            }
        } else if (args[indx] == ">>") {
            if (indx + 1 < args.size()) {
                outfd = open(args[indx + 1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
                outredirect = true;
                append = true;
                indx++; // Skip the filename
            } else {
                cerr << "Error: Missing output file." << endl;
                return;
            }
        } else {
            cmd_args.push_back(args[indx].c_str());
        }
    }

    cmd_args.push_back(nullptr);

    pid_t child_pid = fork();
    if (child_pid == 0) {  // Child process
        if (inredirect) {
            dup2(inpfd, STDIN_FILENO);
            close(inpfd);
        }
        if (outredirect) {
            dup2(outfd, STDOUT_FILENO);
            close(outfd);
        }

        execvp(cmd_args[0], const_cast<char* const*>(cmd_args.data()));
        cerr << "Error: Command execution failed." << endl;
        exit(1);
    } else if (child_pid > 0) {  // Parent process
        wait(nullptr);
    } else {
        cerr << "Error: Fork failed." << endl;
    }
}
