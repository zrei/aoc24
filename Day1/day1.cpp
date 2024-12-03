#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
void ParseOutput(const std::vector<std::string>& outputLines, std::vector<int>& firstList, std::vector<int>& secondList);

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
    std::vector<int> firstList;
    std::vector<int> secondList;
    
    ParseOutput(outputLines, firstList, secondList);

    long long finalOutput = 0;

    for (size_t i = 0; i < firstList.size(); ++i)
    {
        finalOutput += std::abs(firstList[i] - secondList[i]);
    }

    return finalOutput;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::vector<int> firstList;
    std::vector<int> secondList;
    
    ParseOutput(outputLines, firstList, secondList);

    long long finalOutput = 0;

    size_t pointer1, pointer2 = 0;
    size_t size = firstList.size();

    while (pointer1 < size)
    {
        int numTimes = 0;
        std::cout << firstList[pointer1] << ", " << secondList[pointer2] << std::endl;
        while (pointer2 < size && secondList[pointer2] <= firstList[pointer1])
        {
            if (secondList[pointer2] == firstList[pointer1])
                ++numTimes;
            ++pointer2;
        }
        finalOutput += firstList[pointer1] * numTimes;
        ++pointer1;
        while (pointer1 < size && firstList[pointer1] == firstList[pointer1 - 1])
        {
            finalOutput += firstList[pointer1] * numTimes;
            ++pointer1;
        }
    }

    return finalOutput;
}

void ParseOutput(const std::vector<std::string>& outputLines, std::vector<int>& firstList, std::vector<int>& secondList)
{
    for (size_t i = 0; i < outputLines.size(); ++i)
    {
        std::string s = outputLines[i];
        int spaceIndex = s.find(' ');
        firstList.push_back(std::stoi(s.substr(0, spaceIndex)));
        secondList.push_back(std::stoi(s.substr(spaceIndex + 3)));
    }

    std::sort(firstList.begin(), firstList.end());
    std::sort(secondList.begin(), secondList.end());
}

