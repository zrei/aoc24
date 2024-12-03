#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void ReadFromFile(std::string filePath, std::vector<std::string>& outputLines) {
    std::string line;
    std::ifstream rfile;
    std::vector<std::string> lines;
    rfile.open(filePath);
    if (rfile.is_open()) {
        while (std::getline(rfile, line)) {
            outputLines.push_back(std::move(line));
        }
    }
    rfile.close();
}
