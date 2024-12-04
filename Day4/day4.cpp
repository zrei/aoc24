#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../fileRead.h"

enum Directions : char
{
    DOWN,
    UP,
    RIGHT,
    LEFT,
    UP_RIGHT,
    UP_LEFT,
    DOWN_RIGHT,
    DOWN_LEFT
};

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
int XmasBfs(const std::pair<int, int>& dimensions, Directions dir, std::pair<int, int> currCoordinates, int currXmasPointer, const std::vector<std::string>& matrix);
bool CheckValidCoordinates(const std::pair<int, int>& dimensions, const std::pair<int, int>& currCoordinates);
std::pair<int, int> GetNewCoordinates(const std::pair<int, int>& currCoordinates, Directions dir);
int GetBottomLeftOfCross(int startOfCrossRow);
int GetBottomRightOfCross(int startOfCrossCol);
bool CheckForMasCross(const std::pair<int, int>& dimensions, std::pair<int, int> currCoordinates, const std::vector<std::string>& matrix);

const std::string XMAS = "XMAS";
const std::string MAS = "MAS";

// bfs starting from any X
// optimisation: have a secondary map that marks with a number how many XMAS can be formed from this spot and -1 if it hasn't been explored
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
    long long totalNum = 0;
    
    std::pair<int, int> dimensions {outputLines.size(), outputLines[0].size()};

    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            std::pair<int, int> currCoordinates {r, c};
            totalNum += XmasBfs(dimensions, UP, currCoordinates, 0, outputLines)
            + XmasBfs(dimensions, DOWN, currCoordinates, 0, outputLines)
            + XmasBfs(dimensions, RIGHT, currCoordinates, 0, outputLines)
            + XmasBfs(dimensions, LEFT, currCoordinates, 0, outputLines)
            + XmasBfs(dimensions, UP_RIGHT, currCoordinates, 0, outputLines)
            + XmasBfs(dimensions, UP_LEFT, currCoordinates, 0, outputLines)
            + XmasBfs(dimensions, DOWN_RIGHT, currCoordinates, 0, outputLines)
            + XmasBfs(dimensions, DOWN_LEFT, currCoordinates, 0, outputLines);
        }
    }

    return totalNum;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    long long totalNum = 0;

    std::pair<int, int> dimensions {outputLines.size(), outputLines[0].size()};

    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            std::pair<int, int> currCoordinates {r, c};
            if (CheckForMasCross(dimensions, currCoordinates, outputLines))
                ++totalNum;
        }
    }

    return totalNum;
}

int XmasBfs(const std::pair<int, int>& dimensions, Directions dir, std::pair<int, int> currCoordinates, int currXmasPointer, const std::vector<std::string>& matrix)
{
    if (!CheckValidCoordinates(dimensions, currCoordinates))
        return 0;

    auto[currRow, currCol] = currCoordinates;

    if (matrix[currRow][currCol] != XMAS[currXmasPointer])
        return 0;

    ++currXmasPointer;

    if (currXmasPointer == XMAS.size())
    {
        return 1;
    }

    return XmasBfs(dimensions, dir, GetNewCoordinates(currCoordinates, dir), currXmasPointer, matrix);
}

bool CheckValidCoordinates(const std::pair<int, int>& dimensions, const std::pair<int, int>& currCoordinates)
{
    auto[maxRow, maxCol] = dimensions;
    auto[currRow, currCol] = currCoordinates;

    return currRow >= 0 && currRow < maxRow && currCol >= 0 && currCol < maxCol;
}

std::pair<int, int> GetNewCoordinates(const std::pair<int, int>& currCoordinates, Directions dir)
{
    auto[currRow, currCol] = currCoordinates;
    switch (dir)
    {
        case UP:
            return {currRow - 1, currCol};
        case DOWN:
            return {currRow + 1, currCol};
        case RIGHT:
            return {currRow, currCol + 1};
        case LEFT:
            return {currRow, currCol - 1};
        case UP_LEFT:
            return {currRow - 1, currCol - 1};
        case UP_RIGHT:
            return {currRow - 1, currCol + 1};
        case DOWN_LEFT:
            return {currRow + 1, currCol - 1};
        case DOWN_RIGHT:
            return {currRow + 1, currCol + 1};
        default:
            return {currRow, currCol};
    }
}

int GetBottomLeftOfCross(int startOfCrossRow)
{
    return startOfCrossRow + 2;
}

int GetBottomRightOfCross(int startOfCrossCol)
{
    return startOfCrossCol + 2;
}

bool CheckForMasCross(const std::pair<int, int>& dimensions, std::pair<int, int> currCoordinates, const std::vector<std::string>& matrix)
{
    auto[currRow, currCol] = currCoordinates;

    int endRow = GetBottomLeftOfCross(currRow);
    int endCol = GetBottomRightOfCross(currCol);
    if (!CheckValidCoordinates(dimensions, {endRow, endCol}))
        return false;

    if (matrix[currRow][currCol] == MAS.front())
    {
        if (matrix[currRow + 1][currCol + 1] != MAS[1])
            return false;
        if (matrix[currRow + 2][currCol + 2] != MAS.back())
            return false;
    }
    else if (matrix[currRow][currCol] == MAS.back())
    {
        if (matrix[currRow + 1][currCol + 1] != MAS[1])
            return false;
        if (matrix[currRow + 2][currCol + 2] != MAS.front())
            return false;
    }
    else
    {
        return false;
    }

    if (matrix[endRow][currCol] == MAS.front())
    {
        if (matrix[endRow - 1][currCol + 1] != MAS[1])
            return false;
        if (matrix[endRow - 2][currCol + 2] != MAS.back())
            return false;
    }
    else if (matrix[endRow][currCol] == MAS.back())
    {
        if (matrix[endRow - 1][currCol + 1] != MAS[1])
            return false;
        if (matrix[endRow - 2][currCol + 2] != MAS.front())
            return false;
    }
    else
    {
        return false;
    }

    return true;
}