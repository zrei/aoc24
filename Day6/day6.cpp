#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../fileRead.h"

enum Directions : char
{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

constexpr int NUM_DIRECTIONS = 4;
constexpr char BLOCKED_POS = '#';
constexpr char START_POS = '^';

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
Directions TurnRight(Directions currDirection);
bool IsOutOfMap(const std::pair<int, int>& dimensions, const std::pair<int, int>& currCoordinates);
std::pair<int, int> MoveForward(const std::pair<int, int>& currCoordinates, Directions currDirection);
bool IsPositionBlocked(const std::pair<int, int>& dimensions, const std::pair<int, int>& coordinates, const std::vector<std::string>& matrix, const std::pair<int, int>& extraBlock = {-1, -1});
std::pair<int, int> GetStartingPosition(const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix);
long long Traverse(const std::pair<int, int>& startingPosition, const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix);
bool TraverseCheckLoop(const std::pair<int, int>& startingPosition, const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix, const std::pair<int, int>& extraObstacle);
long long TraverseAndAddObstacles(const std::pair<int, int>& startingPosition, const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix);

namespace std
{
    template <>
    class hash<std::pair<int, int>>
    {
    public:
        std::uint64_t operator()(const std::pair<int, int>& x) const
        {
            size_t h = std::hash<int>()(x.first) ^ std::hash<int>()(x.second);
            return h;
         }
    };

    template <>
    class hash<std::tuple<int, int, Directions>>
    {
    public:
        std::uint64_t operator()(const std::tuple<int, int, Directions>& x) const
        {
            auto[row, col, direction] = x;
            size_t h = std::hash<int>()(row) ^ std::hash<int>()(col) ^ std::hash<char>()(direction);
            return h;
         }
    };
}

// part 2 brute force: you can only place along the original route anyway so.
// put there and then test whether it runs into a loop or not

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
    std::pair<int, int> dimensions {outputLines.size(), outputLines[0].size()};
    std::pair<int, int> startingPos = GetStartingPosition(dimensions, outputLines);

    return Traverse(startingPos, dimensions, outputLines);
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::pair<int, int> dimensions {outputLines.size(), outputLines[0].size()};
    std::pair<int, int> startingPos = GetStartingPosition(dimensions, outputLines);

    return TraverseAndAddObstacles(startingPos, dimensions, outputLines);
}

Directions TurnRight(Directions currDirection)
{
    return (Directions) ((((char) currDirection) + 1) % NUM_DIRECTIONS);
}

bool IsOutOfMap(const std::pair<int, int>& dimensions, const std::pair<int, int>& currCoordinates)
{
    return currCoordinates.first < 0 || currCoordinates.first >= dimensions.first || currCoordinates.second < 0 || currCoordinates.second >= dimensions.second;
}

std::pair<int, int> MoveForward(const std::pair<int, int>& currCoordinates, Directions currDirection)
{
    switch (currDirection)
    {
        case UP:
            return {currCoordinates.first - 1, currCoordinates.second};
        case RIGHT:
            return {currCoordinates.first, currCoordinates.second + 1};
        case DOWN:
            return {currCoordinates.first + 1, currCoordinates.second};
        case LEFT:
            return {currCoordinates.first, currCoordinates.second - 1};
        default:
            return {0, 0};
    }
}

bool IsPositionBlocked(const std::pair<int, int>& dimensions, const std::pair<int, int>& coordinates, const std::vector<std::string>& matrix, const std::pair<int, int>& extraBlock)
{
    if (!IsOutOfMap(dimensions, coordinates))
    {
        if (matrix[coordinates.first][coordinates.second] == BLOCKED_POS)
            return true;    
    }

    if (!IsOutOfMap(dimensions, extraBlock))
    {
        if (coordinates == extraBlock)
            return true;
    }
    
    return false;
}

std::pair<int, int> GetStartingPosition(const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix)
{
    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            if (matrix[r][c] == START_POS)
                return {r, c};
        }
    }

    return {0, 0};
}

long long Traverse(const std::pair<int, int>& startingPosition, const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix)
{
    std::unordered_map<std::pair<int, int>, int> visitedPositions;

    std::pair<int, int> currPosition = startingPosition;
    Directions currDir = UP;
    while (!IsOutOfMap(dimensions, currPosition))
    {
        visitedPositions[currPosition] = 1;
        while (true)
        {
            std::pair<int, int> nextPosition = MoveForward(currPosition, currDir);
            if (IsPositionBlocked(dimensions, nextPosition, matrix))
            {
                currDir = TurnRight(currDir);
            }
            else
            {
                currPosition = nextPosition;
                break;
            }
        }
    }

    return visitedPositions.size();
}

long long TraverseAndAddObstacles(const std::pair<int, int>& startingPosition, const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix)
{
    long long numSuccessfulObstacles = 0;

    // track where we've tried to place obstacles
    std::unordered_map<std::pair<int, int>, int> placedObstacles;

    std::pair<int, int> currPosition = startingPosition;
    Directions currDir = UP;
    while (!IsOutOfMap(dimensions, currPosition))
    {
        while (true)
        {
            std::pair<int, int> nextPosition = MoveForward(currPosition, currDir);
            if (IsPositionBlocked(dimensions, nextPosition, matrix))
            {
                currDir = TurnRight(currDir);
            }
            else
            {
                if (nextPosition != startingPosition && !IsOutOfMap(dimensions, nextPosition) && placedObstacles.find(nextPosition) == placedObstacles.end())
                {
                    placedObstacles[nextPosition] = 1;
                    if (TraverseCheckLoop(startingPosition, dimensions, matrix, nextPosition))
                    {
                        ++numSuccessfulObstacles;
                    }
                }
                currPosition = nextPosition;
                break;
            }
        }
    }

    return numSuccessfulObstacles;
}

/// @brief 
/// @param startingPosition 
/// @param dimensions 
/// @param matrix 
/// @return true if the guard gets stuck in a loop
bool TraverseCheckLoop(const std::pair<int, int>& startingPosition, const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix, const std::pair<int, int>& extraObstacle)
{
    std::unordered_map<std::tuple<int, int, Directions>, int> visitedPositions;
    std::pair<int, int> currPosition = startingPosition;
    Directions currDir = UP;

    while (!IsOutOfMap(dimensions, currPosition))
    {
        if (visitedPositions.find({currPosition.first, currPosition.second, currDir}) != visitedPositions.end())
            return true;
        visitedPositions[{currPosition.first, currPosition.second, currDir}] = 1;
        while (true)
        {
            std::pair<int, int> nextPosition = MoveForward(currPosition, currDir);
            if (IsPositionBlocked(dimensions, nextPosition, matrix, extraObstacle))
            {
                currDir = TurnRight(currDir);
            }
            else
            {
                currPosition = nextPosition;
                break;
            }
        }
    }

    return false;
}
