#include <iostream>
#include <unistd.h>
#include <sstream>
#include <pwd.h>
#include <string>
#include <vector>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include "execute.h"

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

void handle_cd(const vector<string> &args);
void handle_pwd();
void handle_echo(const vector<string> &args);

void execute(string command) {
    int x = 0;
    // cout<<" iam above\n";

    vector<string> pipedCommands;
    stringstream ss(command);
    string segment;
    while (getline(ss, segment, '|')) {
        pipedCommands.push_back(segment);
    }

    if (pipedCommands.size() > 1) {
        runpipedcommands(pipedCommands);
        return;
        // addCommandToHistory(s);
        //  saveHistory();
    } else {
        vector<string> args;
        string token;
        istringstream tokenStream(command);
        while (tokenStream >> token) {
            if (token == ">>" || token == ">" || token == "<") {
                x++;
            }
            args.push_back(token);
        }

        if (!args.empty() && x) {
            execute_cmd_with_redirection(args);
            return;
        }
    }

    // cout<<" iam below\n";

    int i;
    // removing the first spaces
    for (i = 0; i < command.size() && command[i] == ' '; i++);  // i pointing to first letter of first word
    int j = i;

    //  cout<<"i="<<i<<endl;

    //   j is pointing to next character after first word
    string first;
    while (j < command.size() && command[j] != ' ') j++;
    first = command.substr(i, j - i);

    //  bool flag= (i==command.size() or command[i]==32 ) and j==command.size();

    while (j < command.size() && command[j] == ' ') j++; // j is pointing to first letter of second word

    int k = command.size() - 1;
    while (k >= j && command[k] == ' ') k--; // k is pointing to last character of last word

    vector<string> args;
    char *cmd = new char[command.length() + 1];
    strcpy(cmd, command.c_str());

    char *token = strtok(cmd, " \t");
    while (token != nullptr) {
        args.push_back(string(token));
        token = strtok(nullptr, " \t");
    }

    if (args.empty()) {
        delete[] cmd;
        return;
    }

    string cmd_name = args[0];

    if (command[k] == '&') {
        bool runInBackground = false;
        vector<string> args;
        string arg;
        istringstream iss(command);
        while (iss >> arg) {
            if (arg == "&") {
                runInBackground = true;  // Check if the command should run in background
            } else {
                args.push_back(arg);
            }
        }

        if (args.empty()) {
            cout << "nothing to do\n";
        }

        back(args, runInBackground);
        sleep(1);
    } else if (cmd_name == "cd") {
        handle_cd(args);
    } else if (cmd_name == "pwd") {
        handle_pwd();
    } else if (first == "echo") {
        if (!echo(command.substr(j, command.size() - j)))
            cout << "Invalid command";
    } else if (first == "ls") {
        //  cout<<"iam here"<<endl;

        vector<string> tokens;
        stringstream ss(command.substr(j, command.size() - j));
        string token;
        while (ss >> token) {
            tokens.push_back(token);
        }
        lsCommand(tokens);
    } else if (first == "pinfo") {
        vector<string> tokens;
        stringstream ss(command.substr(j, command.size() - j));
        string token;
        while (ss >> token) {
            tokens.push_back(token);
        }

        int pid = getpid();
        if (j != command.size())
            pid = stoi(command.substr(j, command.size() - j));

        pinfo(pid);
    } else if (first == "search") {
        string target_name = command.substr(j, k - j + 1);
        string current_dir = ".";

        if (search_in_directory(current_dir, target_name)) {
            cout << "True" << endl;
        } else {
            cout << "False" << endl;
        }
    } else if (first == "history") {
        int num = 10;
        int start = j, end = k;
        while (start <= end) {
            if (command[start] < '0' || command[start] > '9') {
                cout << "Invalid pid entered - pid should be an integer\n";
                return;
            }
            start++;
        }

        if (k >= j)
            num = stoi(command.substr(j, k - j + 1));
        //   displayHistory(num);
        show_history(num);
    } else if (command.substr(j, k - j + 1) == "fg") {
        // cout<<"foreground="<<command.substr(j,k-j+1)<<"\n";
        // Handle external commands
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            vector<char *> c_args;
            for (const auto &arg : args) {
                c_args.push_back(const_cast<char *>(arg.c_str()));
            }
            c_args.push_back(nullptr);
            execvp(c_args[0], c_args.data());
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            // Parent process
            waitpid(pid, nullptr, 0);
        } else {
            perror("fork failed");
        }
    } else {
        command += " &";

        bool runInBackground = false;
        vector<string> args;
        string arg;
        istringstream iss(command);
        while (iss >> arg) {
            if (arg == "&") {
                runInBackground = true;  // Check if the command should run in background
            } else {
                args.push_back(arg);
            }
        }

        if (args.empty()) {
            cout << "nothing to do\n";
        }

        back(args, runInBackground);
    }

    delete[] cmd;
}
