#include <iostream>
#include <vector>
#include <unordered_map>
#include "../fileRead.h"

unsigned long long Part1(const std::vector<std::string>& outputLines);
unsigned long long Part2(const std::vector<std::string>& outputLines);

static std::unordered_map<std::string, std::vector<std::string>> AdjacencyList;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }

    std::vector<std::string> outputLines;
    ReadFromFile(argv[1], outputLines);

    std::cout << Part2(outputLines) << std::endl;
}

unsigned long long Part1(const std::vector<std::string>& outputLines)
{
    return 0;
}

unsigned long long Part2(const std::vector<std::string>& outputLines)
{
    return 0;
}

void ParseLine(const std::string& s)
{
    int dashIndex = s.find('-');
    std::string first = s.substr(0, dashIndex);
    std::string second = s.substr(dashIndex + 1);
    AdjacencyList[first].push_back(second);
}

/*
Only returns true if one node 
*/
bool CheckCycles(std::string std::unordered_map<std::string, int>& checkedNodes, std::unordered_map<std::string, int>& nodeCache)