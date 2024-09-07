#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sstream>
#include <vector>
#include <signal.h>
#include <sys/types.h>
 #include <sys/stat.h>
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
#include "execute.h"

using namespace std;

string homeDir;


pid_t foreground_pid = -1;  // global variable to store the PID of the foreground job

void sigtstp_handler(int signum) {
    if (foreground_pid != -1) {
        kill(foreground_pid, SIGTSTP);  // Send SIGTSTP to the foreground job
        // Implement logic to move the job to the background and mark it as stopped
        cout << "\nJob " << foreground_pid << " stopped and moved to background" << endl;
        foreground_pid = -1;
    }
}

void sigtstp_handlerc(int signum) {
    if (foreground_pid != -1) {
        kill(foreground_pid, SIGTSTP);  // Send SIGTSTP to the foreground job
        // Implement logic to move the job to the background and mark it as stopped
        cout << "\nJob " << foreground_pid << " stopped and moved to background" << endl;
        foreground_pid = -1;
    }
}
void printPrompt() {
    char hostname[1024];
    char cwd[1024];
    struct passwd *pw = getpwuid(getuid());
    string un = getenv("USER");
    gethostname(hostname, sizeof(hostname));
    getcwd(cwd, sizeof(cwd));
    
    string currdir(cwd);
    if (currdir.find(homeDir) == 0) {
        currdir.replace(0, homeDir.size(), "~");
    }

    cout << un << "@" << hostname << ":" << currdir << "> ";
}

vector<string> tokenize(const string &str, char delim) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    homeDir = cwd;                                        

    signal(SIGTSTP, sigtstp_handler);
         signal(SIGTSTP, sigtstp_handler);
    //    cout<<"i am in main\n";
    
    while (true) {
        printPrompt();
        string commandLine;
        
        // getline(cin, commandLine);  // Read user input
        if (commandLine == "exit") break;  // Simple exit command
         if (!getline(cin, commandLine)) {  // Check for EOF
            cout << "\nLogging out..." << endl;
            break;  // Exit the shell
            
        }
       
         
        vector<string> commands = tokenize(commandLine, ';');
        //  cout<<"iam passing\n";
        for (const string &cmd : commands) {
            
            execute(cmd);
             add_history( cmd);
        }
    }


    return 0;
}
