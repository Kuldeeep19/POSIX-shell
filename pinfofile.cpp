#include <iostream>
#include <fstream>
#include <unistd.h>
#include <limits.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "pinfofile.h"

using namespace std;
void pinfo(pid_t pid) {
    // construct the path to the /proc/[pid]/status file
    string proc_dir = "/proc/" + to_string(pid);
    string status_path = proc_dir + "/status";
    string stat_path = proc_dir + "/stat";
    string exe_path = proc_dir + "/exe";

    // open and read the /proc/[pid]/status file
    ifstream status_file(status_path);
    if (!status_file) {
        cerr << "Could not open status file for PID " << pid << endl;
        return;
    }

    string line;
    string process_state;
    string vm_size;
    while (getline(status_file, line)) {
        if (line.find("State:") == 0) {
            process_state = line.substr(7);
        } else if (line.find("VmSize:") == 0) {
            vm_size = line.substr(8);
        }
    }

    status_file.close();

    // read the process status from /proc/[pid]/stat file for foreground determination
    ifstream stat_file(stat_path);
    if (!stat_file) {
        cerr << "Could not open stat file for PID " << pid << endl;
        return;
    }

    string stat_line;
    getline(stat_file, stat_line);
    stringstream stat_stream(stat_line);
    string token;
    string process_status;
    int count = 1;
    int foreground_pgid;

    while (stat_stream >> token) {
        if (count == 3) {
            process_status = token;  // Process status
        }
        if (count == 8) {
            foreground_pgid = stoi(token);
        }
        count++;
    }

    stat_file.close();

    // Add '+' if the process is in the foreground
    if (foreground_pgid == getpgid(0)) {
        process_status += "+";
    }

    // resolve the executable path using /proc/[pid]/exe
    char exe_realpath[PATH_MAX];
    ssize_t len = readlink(exe_path.c_str(), exe_realpath, sizeof(exe_realpath) - 1);
    if (len != -1) {
        exe_realpath[len] = '\0';
    } else {
        cerr << "Could not resolve executable path for PID " << pid << endl;
        return;
    }

    // print the required process information
    cout << "pid -- " << pid << endl;
    cout << "Process Status -- " << process_status << endl;
    cout << "memory -- " << vm_size << endl;
    cout << "Executable Path -- " << exe_realpath << endl;
}