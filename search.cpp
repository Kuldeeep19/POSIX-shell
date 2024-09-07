#include <iostream>
#include <dirent.h>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include "search.h"

using namespace std;

bool search_in_directory(const string& path, const string& tar) {
    DIR* directory = opendir(path.c_str());
    if (!directory) {
        cerr << "Error: Unable to open directory " << path << endl;
        return false;
    }

    struct dirent* file_entry;
    while ((file_entry = readdir(directory)) != nullptr) {
        string filename = file_entry->d_name;

        // Skip the "." and ".." entries
        if (filename == "." || filename == "..") {
            continue;
        }

        string fullpath = path + "/" + filename;

        // Check if the current entry matches the target name
        if (filename == tar) {
            closedir(directory);
            return true;
        }

        // If the entry is a directory, recurse into it
        struct stat filestat;
        if (stat(fullpath.c_str(), &filestat) == 0 && S_ISDIR(filestat.st_mode)) {
            if (search_in_directory(fullpath, tar)) {
                closedir(directory);
                return true;
            }
        }
    }

    closedir(directory);
    return false;
}
