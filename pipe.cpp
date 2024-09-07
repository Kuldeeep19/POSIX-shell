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
#include "pipe.h"
using namespace std;

// Function to run a series of commands connected by pipes
void runpipedcommands(vector<string>& cmdList) {
    int numCommands = cmdList.size();
    int pipefds[2 * (numCommands - 1)];

    // Create the necessary pipes
    for (int i = 0; i < numCommands - 1; ++i) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    int j = 0;
    for (int i = 0; i < numCommands; ++i) {
        // Tokenize the command to detect redirection
        vector<string> args;
        stringstream ss(cmdList[i]);
        string arg;
        string inputFile, outputFile;
        bool isInputRedirect = false, isOutputRedirect = false, append = false;

        while (ss >> arg) {
            if (arg == "<") {
                ss >> inputFile;
                isInputRedirect = true;
            } else if (arg == ">") {
                ss >> outputFile;
                isOutputRedirect = true;
                append = false;
            } else if (arg == ">>") {
                ss >> outputFile;
                isOutputRedirect = true;
                append = true;
            } else {
                args.push_back(arg);
            }
        }

        pid_t pid = fork();
        if (pid == 0) {
            // Child process

            // Handle input redirection
            if (isInputRedirect) {
                int inputFd = open(inputFile.c_str(), O_RDONLY);
                if (inputFd < 0) {
                    perror("open input file");
                    exit(EXIT_FAILURE);
                }
                dup2(inputFd, STDIN_FILENO);
                close(inputFd);
            }

            // Handle output redirection
            if (isOutputRedirect) {
                int outputFd;
                if (append) {
                    outputFd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
                } else {
                    outputFd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                }
                if (outputFd < 0) {
                    perror("open output file");
                    exit(EXIT_FAILURE);
                }
                dup2(outputFd, STDOUT_FILENO);
                close(outputFd);
            }

            // Redirect the pipes
            if (i > 0) {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }
            if (i < numCommands - 1) {
                dup2(pipefds[j + 1], STDOUT_FILENO);
            }

            // Close all pipe file descriptors
            for (int i = 0; i < 2 * (numCommands - 1); ++i) {
                close(pipefds[i]);
            }

            // Convert args to a format execvp can use
            vector<char*> cargs;
            for (size_t i = 0; i < args.size(); ++i) {
                cargs.push_back(&args[i][0]);
            }
            cargs.push_back(nullptr);

            execvp(cargs[0], cargs.data());
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        j += 2;
    }

    // Close all pipe file descriptors in the parent
    for (int i = 0; i < 2 * (numCommands - 1); ++i) {
        close(pipefds[i]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < numCommands; ++i) {
        wait(nullptr);
    }
}
