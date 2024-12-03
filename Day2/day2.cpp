#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../fileRead.h"

enum ChangeState : char
{
    NOT_SET,
    INCREASING,
    DECREASING,
    NO_CHANGE
};

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
std::pair<ChangeState, bool> CheckState(int firstNum, int secondNum);
void ParseOutput(const std::string& outputLines, std::vector<int>& parsedOutput);
bool IsValid(const std::vector<int>& numbers, int skippedIndex = -1);

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
    long long numValid = 0;
    for (const std::string& s : outputLines)
    {
        std::vector<int> numbers;
        ParseOutput(s, numbers);
        if (IsValid(numbers))
            ++numValid;
    }
    return numValid;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    long long numValid = 0;

    for (const std::string& s : outputLines)
    {
        std::vector<int> numbers;
        ParseOutput(s, numbers);
        ChangeState state = NOT_SET;
        bool isValid = false;
        isValid = IsValid(numbers, -1);
        for (size_t i = 0; i < numbers.size(); ++i)
        {
            isValid = IsValid(numbers, i) || isValid;
        }
        if (isValid)
            ++numValid;
    }
    return numValid;
}

std::pair<ChangeState, bool> CheckState(int firstNum, int secondNum)
{
    int diff = std::abs(secondNum - firstNum);
    bool valid = diff >= 1 && diff <= 3;
    if (firstNum > secondNum)
    {
        return std::pair<ChangeState, bool>(DECREASING, valid);
    }
    else if (firstNum < secondNum)
    {
        return std::pair<ChangeState, bool>(INCREASING, valid);
    }
    else
    {
        return std::pair<ChangeState, bool>(NO_CHANGE, valid);
    }
}

void ParseOutput(const std::string& outputLines, std::vector<int>& parsedOutput)
{
    std::string s = outputLines;
    int spaceIndex = s.find(' ');
    while (spaceIndex != -1)
    {
        parsedOutput.push_back(std::stoi(s.substr(0, spaceIndex)));
        s = s.substr(spaceIndex + 1);
        spaceIndex = s.find(' ');
        
    }
    parsedOutput.push_back(std::stoi(s));
}

bool IsValid(const std::vector<int>& numbers, int skippedIndex)
{
    ChangeState state = NOT_SET;
    bool isValid = true;
    size_t startingIndex = skippedIndex == 0 ? 2 : 1;
    size_t previousIndex = startingIndex - 1;
    size_t endingIndex = skippedIndex == numbers.size() - 1 ? numbers.size() - 1 : numbers.size();
    for (size_t i = startingIndex; i < endingIndex; ++i)
    {
        if (i == skippedIndex)
        {
            previousIndex = i - 1;
            continue;
        }

        auto[changeState, valid] = CheckState(numbers[previousIndex], numbers[i]);
        if (changeState == NO_CHANGE)
        {
            isValid = false;
            break;
        }
        if (state != NOT_SET && state != changeState)
        {
            isValid = false;
            break;
        }
        if (!valid)
        {
            isValid = false;
            break;
        }
        state = changeState;

        previousIndex = i;
    }
    return isValid;
}
