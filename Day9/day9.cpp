#include <fstream>
#include <iostream>
#include <vector>
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
void ConstructMemorySpace(std::vector<int>& memorySpace, const std::string& spaceRep);
void ReconstructMemorySpace(std::vector<int>& memorySpace);
long long GetHashCode(const std::vector<int>& memorySpace, bool ignoreEmptySpace = false);
int FindNextEmptySpace(int startingIndex, const std::vector<int>& memorySpace);
int TranslateCharToInt(char c);
int FindNextOccupiedSpot(int startingIndex, const std::vector<int>& memorySpace);
void ReconstructMemorySpaceNoFragmentation(std::vector<int>& memorySpace);
int FindEmptySpaceAmount(int startingIndex, const std::vector<int>& memorySpace);
int FindFileAmount(int startingIndex, const std::vector<int>& memorySpace);

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
    const std::string& s = outputLines[0];
    std::vector<int> memorySpace;
    ConstructMemorySpace(memorySpace, s);
    std::cout << "Complete" << std::endl;
    ReconstructMemorySpace(memorySpace);
    std::cout << "Reconstruction complete" << std::endl;

    return GetHashCode(memorySpace);
}

long long Part2(const std::vector<std::string>& outputLines)
{
    const std::string& s = outputLines[0];
    std::vector<int> memorySpace;
    ConstructMemorySpace(memorySpace, s);
    std::cout << "Complete" << std::endl;
    ReconstructMemorySpaceNoFragmentation(memorySpace);
    std::cout << "Reconstruction complete" << std::endl;

    return GetHashCode(memorySpace, true);
}

void ConstructMemorySpace(std::vector<int>& memorySpace, const std::string& spaceRep)
{
    for (size_t i = 0; i < spaceRep.size(); ++i)
    {
        int numBlocks = TranslateCharToInt(spaceRep[i]);
        if (i % 2 == 0)
        {
            for (size_t j = 0; j < numBlocks; ++j)
            {
                memorySpace.push_back(i / 2);
            }
        }
        else
        {
            for (size_t j = 0; j < numBlocks; ++j)
            {
                memorySpace.push_back(-1);
            }
        }
    }
}

void ReconstructMemorySpace(std::vector<int>& memorySpace)
{
    int firstPointer = FindNextEmptySpace(0, memorySpace);
    int secondPointer = FindNextOccupiedSpot(memorySpace.size() - 1, memorySpace);
    
    while (firstPointer != -1 && secondPointer != - 1 && firstPointer < secondPointer)
    {
        memorySpace[firstPointer] = memorySpace[secondPointer];
        memorySpace[secondPointer] = -1;

        firstPointer = FindNextEmptySpace(firstPointer + 1, memorySpace);
        secondPointer = FindNextOccupiedSpot(secondPointer - 1, memorySpace);
    }
}

void ReconstructMemorySpaceNoFragmentation(std::vector<int>& memorySpace)
{
    int secondPointer = FindNextOccupiedSpot(memorySpace.size() - 1, memorySpace);

    while (secondPointer != -1)
    {
        int occupiedSpace = FindFileAmount(secondPointer, memorySpace);
        int firstPointer = FindNextEmptySpace(0, memorySpace);
        while (firstPointer != -1 && firstPointer < secondPointer)
        {
            int emptySpace = FindEmptySpaceAmount(firstPointer, memorySpace);

            if (occupiedSpace <= emptySpace)
            {
                for (size_t i = 0; i < occupiedSpace; ++i)
                {
                    memorySpace[firstPointer + i] = memorySpace[secondPointer - i];
                    memorySpace[secondPointer - i] = -1;
                }
                break;
            }
            else
            {
                firstPointer = FindNextEmptySpace(firstPointer + emptySpace, memorySpace);
            }
        }
        secondPointer = FindNextOccupiedSpot(secondPointer - occupiedSpace, memorySpace);
    }
    /*
    while (firstPointer != -1 && secondPointer != - 1 && firstPointer < secondPointer)
    {

        int emptySpace = FindEmptySpaceAmount(firstPointer, memorySpace);
        int occupiedSpace = FindFileAmount(secondPointer, memorySpace);

        if (occupiedSpace <= emptySpace)
        {
            for (size_t i = 0; i < occupiedSpace; ++i)
            {
                memorySpace[firstPointer + i] = memorySpace[secondPointer - i];
                memorySpace[secondPointer - i] = -1;
            }
            firstPointer = FindNextEmptySpace(firstPointer + occupiedSpace, memorySpace);
            secondPointer = FindNextOccupiedSpot(secondPointer - occupiedSpace, memorySpace);
        }
        else
        {
            secondPointer = FindNextOccupiedSpot(secondPointer - occupiedSpace, memorySpace);
        }
    }
    */
}

long long GetHashCode(const std::vector<int>& memorySpace, bool ignoreEmptySpace)
{
    long long hash = 0;
    for (size_t i = 0; i < memorySpace.size(); ++i)
    {
        if (memorySpace[i] == -1)
        {
            if (ignoreEmptySpace)
                continue;
            else
                return hash;
        }
        hash += memorySpace[i] * i;
    }
    return hash;
}

int FindNextEmptySpace(int startingIndex, const std::vector<int>& memorySpace)
{
    int pointer = startingIndex;
    while (pointer < memorySpace.size())
    {
        if (memorySpace[pointer] == -1)
            return pointer;
        ++pointer;
    }
    return -1;
}

int FindEmptySpaceAmount(int startingIndex, const std::vector<int>& memorySpace)
{
    int pointer = startingIndex;
    int length = 0;
    while (pointer < memorySpace.size() && memorySpace[pointer] == -1)
    {
        ++length;
        ++pointer;
    }
    return length;
}

int FindFileAmount(int startingIndex, const std::vector<int>& memorySpace)
{
    int pointer = startingIndex;
    int length = 0;
    while (pointer < memorySpace.size() && memorySpace[pointer] == memorySpace[startingIndex])
    {
        ++length;
        --pointer;
    }
    return length;
}

int FindNextOccupiedSpot(int startingIndex, const std::vector<int>& memorySpace)
{
    int pointer = startingIndex;
    while (pointer >= 0)
    {
        if (memorySpace[pointer] != -1)
            return pointer;
        --pointer;
    }
    return -1;
}

int TranslateCharToInt(char c)
{
    return c - '0';
}