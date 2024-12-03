#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
bool CheckForMulSequence(int* pointer, const std::string& string);
bool CheckForDoAndDont(int* pointer, const std::string& string, bool& enable);
bool CheckForFirstNumber(int* pointer, const std::string& string, int& outNumber);
bool CheckForSecondNumber(int* pointer, const std::string& string, int& outNumber);
bool IsNumber(char c);

const std::string mulSequence = "mul(";
const std::string doSequence = "do()";
const std::string dontSequence ="don't()";
constexpr char startingNumber = '0';
constexpr char endNumber = '9';
constexpr char breakComma = ',';
constexpr char breakBracket = ')';

// valid instructions are mul, both () and a comma and the numbers. anything else results in instant execution

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
    // Create and open a text file
    std::ofstream MyFile("day3.out");
    std::ofstream MyFile2("day3-2.out");

    long long totalSum = 0;

    for (const std::string& s : outputLines)
    {
        int pointer = 0;
        while (pointer < s.size())
        {
            int beginningValue = pointer;
            if (!CheckForMulSequence(&pointer, s))
                continue;
            int firstNumber = 0;
            if (!CheckForFirstNumber(&pointer, s, firstNumber))
                continue;
            int secondNumber = 0;
            if (!CheckForSecondNumber(&pointer, s, secondNumber))
                continue;
            MyFile << beginningValue << " to " << pointer - 1 << ": " << (firstNumber * secondNumber) << std::endl;
            MyFile2 <<  s.substr(beginningValue, pointer - beginningValue) << std::endl;
            totalSum += firstNumber * secondNumber;
        }
    }

    // Close the file
    MyFile.close();

    return totalSum;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    // Create and open a text file
    std::ofstream MyFile("day3.out");
    std::ofstream MyFile2("day3-2.out");

    long long totalSum = 0;

    bool enabled = true;

    for (const std::string& s : outputLines)
    {
        int pointer = 0;
        while (pointer < s.size())
        {
            int beginningValue = pointer;
            if (CheckForDoAndDont(&pointer, s, enabled))
            {
                continue;
            }
            if (!CheckForMulSequence(&pointer, s))
                continue;
            int firstNumber = 0;
            if (!CheckForFirstNumber(&pointer, s, firstNumber))
                continue;
            int secondNumber = 0;
            if (!CheckForSecondNumber(&pointer, s, secondNumber))
                continue;
            MyFile << (enabled ? "enabled" : "not enabled") << ", " << beginningValue << " to " << pointer - 1 << ": " << (firstNumber * secondNumber) << std::endl;
            MyFile2 << (enabled ? "enabled" : "not enabled") << ", " << s.substr(beginningValue, pointer - beginningValue) << std::endl;
            if (enabled)
                totalSum += firstNumber * secondNumber;
        }
    }

    // Close the file
    MyFile.close();

    return totalSum;
}

bool CheckForMulSequence(int* pointer, const std::string& string)
{
    int mulSequencePointer = 0;
    while (*pointer < string.size() && mulSequencePointer < mulSequence.size())
    {
        if (string[*pointer] != mulSequence[mulSequencePointer])
        {
            ++(*pointer);
            return false;
        }
        ++(*pointer);
        ++mulSequencePointer;
    }
    return mulSequencePointer == mulSequence.size();
}

bool CheckForDoSequence(int* pointer, const std::string& string)
{
    int mulSequencePointer = 0;
    while (*pointer < string.size() && mulSequencePointer < mulSequence.size())
    {
        if (string[*pointer] != mulSequence[mulSequencePointer])
        {
            ++(*pointer);
            return false;
        }
        ++(*pointer);
        ++mulSequencePointer;
    }
    return mulSequencePointer == mulSequence.size();
}

bool CheckForDoAndDont(int* pointer, const std::string& string, bool& enable)
{
    int refPointer = *pointer;
    int sequencePointer = 0;
    while (refPointer < string.size() && sequencePointer < doSequence.size())
    {
        if (string[refPointer] != doSequence[sequencePointer])
        {
            break;
        }
        ++refPointer;
        ++sequencePointer;
    }
    if (sequencePointer == doSequence.size())
    {
        *pointer = refPointer;
        enable = true;
        return true;
    }

    refPointer = *pointer;
    sequencePointer = 0;

    while (refPointer < string.size() && sequencePointer < dontSequence.size())
    {
        if (string[refPointer] != dontSequence[sequencePointer])
        {
            break;
        }
        ++refPointer;
        ++sequencePointer;
    }
    if (sequencePointer == dontSequence.size())
    {
        *pointer = refPointer;
        enable = false;
        return true;
    }
    
    return false;
}

bool CheckForFirstNumber(int* pointer, const std::string& string, int& outNumber)
{
    if (*pointer >= string.size())
        return false;

    int beginningPointer = *pointer;

    if (!IsNumber(string[*pointer]))
        return false;

    ++(*pointer);

    while (*pointer < string.size())
    {
        if (string[*pointer] == breakComma)
        {
            outNumber = std::stoi(string.substr(beginningPointer, *pointer - beginningPointer));
            (++*pointer);
            return true;
        }

        if (!IsNumber(string[*pointer]))
        {
            return false;
        }

        ++(*pointer);
    }
    
    if (string[*pointer - 1] != breakComma)
    {
        outNumber = std::stoi(string.substr(beginningPointer, *pointer - 1 - beginningPointer));
        return true;        
    }

    return false;
}

bool CheckForSecondNumber(int* pointer, const std::string& string, int& outNumber)
{
    if (*pointer >= string.size())
        return false;

    int beginningPointer = *pointer;

    if (!IsNumber(string[*pointer]))
        return false;

    ++(*pointer);

    while (*pointer < string.size())
    {
        if (string[*pointer] == breakBracket)
        {
            outNumber = std::stoi(string.substr(beginningPointer, *pointer - beginningPointer));
            (++*pointer);
            return true;
        }

        if (!IsNumber(string[*pointer]))
        {
            return false;
        }

        ++(*pointer);
    }
    
    if (string[*pointer - 1] != breakBracket)
    {
        outNumber = std::stoi(string.substr(beginningPointer, *pointer - 1 - beginningPointer));
        return true;        
    }

    return false;
}

bool IsNumber(char c)
{
    return c >= startingNumber && c <= endNumber;
}