#include <fstream>
#include <iostream>
#include <vector>
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
long long ParseLine(const std::string& line, std::vector<int>& operands);
bool CanBeComputed(const std::vector<int>& operands, int currIndex, long long currRes, long long expectedRes);
bool CanBeComputedWithConcat(const std::vector<int>& operands, int currIndex, long long currRes, long long expectedRes);
long long ComputeConcat(long long first, long long second);

constexpr char COLON = ':';

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }

    std::vector<std::string> outputLines;
    ReadFromFile(argv[1], outputLines);

    std::cout << Part2(outputLines) << std::endl;
}

long long Part1(const std::vector<std::string>& outputLines)
{
    long long numWays = 0;

    for (const std::string& s : outputLines)
    {
        std::vector<int> operands;
        long long expectedRes = ParseLine(s, operands);
        if (CanBeComputed(operands, 0, operands[0], expectedRes))
            numWays += expectedRes;
    }

    return numWays;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    long long numWays = 0;

    for (const std::string& s : outputLines)
    {
        std::vector<int> operands;
        long long expectedRes = ParseLine(s, operands);
        if (CanBeComputedWithConcat(operands, 0, operands[0], expectedRes))
            numWays += expectedRes;
    }

    return numWays;
}

long long ParseLine(const std::string& line, std::vector<int>& operands)
{
    std::string s = line;
    int colonIndex = line.find(COLON);
    long long res = std::stoul(s.substr(0, colonIndex));
    s = s.substr(colonIndex + 2);
    int spaceIndex = s.find(' ');
    while (spaceIndex != -1)
    {
        operands.push_back(std::stoul(s.substr(0, spaceIndex)));
        s = s.substr(spaceIndex + 1);
        spaceIndex = s.find(' ');
    }
    operands.push_back(std::stoul(s));
    return res;
}

bool CanBeComputed(const std::vector<int>& operands, int currIndex, long long currRes, long long expectedRes)
{
    if (currIndex == operands.size() - 1)
    {
        return currRes == expectedRes;
    }

    return CanBeComputed(operands, currIndex + 1, currRes + operands[currIndex + 1], expectedRes) || CanBeComputed(operands, currIndex + 1, currRes * operands[currIndex + 1], expectedRes);
}

bool CanBeComputedWithConcat(const std::vector<int>& operands, int currIndex, long long currRes, long long expectedRes)
{
    if (currIndex == operands.size() - 1)
    {
        return currRes == expectedRes;
    }

    return CanBeComputedWithConcat(operands, currIndex + 1, currRes + operands[currIndex + 1], expectedRes) || CanBeComputedWithConcat(operands, currIndex + 1, currRes * operands[currIndex + 1], expectedRes)
        || CanBeComputedWithConcat(operands, currIndex + 1, ComputeConcat(currRes, operands[currIndex + 1]), expectedRes);
}

long long ComputeConcat(long long first, long long second)
{
    std::string concat = std::to_string(first) + std::to_string(second);
    return std::stoul(concat);
}