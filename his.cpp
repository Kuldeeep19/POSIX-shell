#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#define HISTORY_FILE "/tmp/.shell_history"
#define MAX_HISTORY 20

using namespace std;

void add_history(const string& command) {
    if (command.empty()) return;

    vector<string> lines;
    ifstream infile(HISTORY_FILE);
    string line;

    // read existing history into the vector
    while (getline(infile, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    infile.close();

    // avoid duplicate consecutive commands
    if (!lines.empty() && lines.back() == command) {
        return;
    }

    // add the new command to the history
    lines.push_back(command);

    // maintain only the last MAX_HISTORY commands
    if (lines.size() > MAX_HISTORY) {
        lines.erase(lines.begin());  // Remove the oldest command
    }

    // write the history back to the file
    ofstream outfile(HISTORY_FILE, ios::trunc);
    for (const auto& cmd : lines) {
        outfile << cmd << endl;
    }
    outfile.close();
}

void show_history(int count = 10) {
    ifstream historyFile(HISTORY_FILE);
    vector<string> historyLines;
    string currentLine;

    // Read history from the file
    while (getline(historyFile, currentLine)) {
        if (!currentLine.empty()) {
            historyLines.push_back(currentLine);
        }
    }
    historyFile.close();

    int totalEntries = historyLines.size();
    int displayCount = min(count, totalEntries);

    // Calculate the start index for displaying lines
    int startIndex = totalEntries - displayCount;

    // Output the last `displayCount` commands
    int currentIndex = startIndex;
    while (currentIndex < totalEntries) {
        cout << historyLines[currentIndex] << endl;
        ++currentIndex;
    }
}

