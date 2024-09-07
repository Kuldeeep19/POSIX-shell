#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <iomanip>
#include <ctime>
#include "ls.h"

using namespace std;

void parseArguments(const vector<string>& args, bool& showAll, bool& longFormat, vector<string>& paths) {
    for (const auto& arg : args) {
        if (arg == "-a") {
            showAll = true;
        } else if (arg == "-l") {
            longFormat = true;
        } else if (arg == "-la" || arg == "-al") {
            showAll = true;
            longFormat = true;
        } else {
            paths.push_back(arg);
        }
    }
    if (paths.empty()) {
        paths.push_back("."); // default to current directory if no path is provided
    }
}

void printPermissions(mode_t mode) {
    // print file type
    if (S_ISDIR(mode)) {
        cout << "d";
    } else {
        cout << "-";
    }

    // print user permissions
    if (mode & S_IRUSR) {
        cout << "r";
    } else {
        cout << "-";
    }
    if (mode & S_IWUSR) {
        cout << "w";
    } else {
        cout << "-";
    }
    if (mode & S_IXUSR) {
        cout << "x";
    } else {
        cout << "-";
    }

    // print group permissions
    if (mode & S_IRGRP) {
        cout << "r";
    } else {
        cout << "-";
    }
    if (mode & S_IWGRP) {
        cout << "w";
    } else {
        cout << "-";
    }
    if (mode & S_IXGRP) {
        cout << "x";
    } else {
        cout << "-";
    }

    // print others permissions
    if (mode & S_IROTH) {
        cout << "r";
    } else {
        cout << "-";
    }
    if (mode & S_IWOTH) {
        cout << "w";
    } else {
        cout << "-";
    }
    if (mode & S_IXOTH) {
        cout << "x";
    } else {
        cout << "-";
    }
}

void listDirectory(const string& path, bool showAll, bool longFormat) {
    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir(path.c_str())) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (!showAll && entry->d_name[0] == '.') continue;  // Skip hidden files if -a is not specified

            if (longFormat) {
                struct stat fileStat;
                string fullPath = path + "/" + entry->d_name;
                if (stat(fullPath.c_str(), &fileStat) == -1) {
                    perror("stat");
                    continue;
                }

                // print file permissions
                printPermissions(fileStat.st_mode);

                // print number of links, owner, group, size, and last modification time
                cout << " " << fileStat.st_nlink;
                cout << " " << getpwuid(fileStat.st_uid)->pw_name;
                cout << " " << getgrgid(fileStat.st_gid)->gr_name;
                cout << " " << setw(8) << fileStat.st_size;

                char timeBuf[80];
                struct tm lt;
                localtime_r(&fileStat.st_mtime, &lt);
                strftime(timeBuf, sizeof(timeBuf), "%b %d %H:%M", &lt);
                cout << " " << timeBuf;

                // print the file/directory name
                cout << " " << entry->d_name << endl;
            } else {
                // simple listing without -l
                cout << entry->d_name << endl;
            }
        }
        closedir(dir);
    } else {
        cout << "Error opening directory" << endl;
        perror("opendir");
    }
}

void lsCommand(const vector<string>& args) {
    bool showAll = false;
    bool longFormat = false;
    vector<string> paths;

    parseArguments(args, showAll, longFormat, paths);

    for (const auto& path : paths) {
        if (paths.size() > 1) {
            cout << path << ":" << endl;
        }

        listDirectory(path, showAll, longFormat);
        if (&path != &paths.back()) {
            cout << endl;
        }
    }
}
