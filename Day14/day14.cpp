#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
void Part2(const std::vector<std::string>& outputLines);
std::pair<int, int> ParsePosition(const std::string& line, int* pointer);
std::pair<int, int> Move(std::pair<int, int> initialCoordinates, const std::pair<int, int>& movement);
std::pair<int, int> GetFinalCoordinates(std::pair<int, int> initialCoordinates);
int GetQuadrantNumber(const std::pair<int, int>& coordinates);
std::pair<int, int> MoveSingle(std::pair<int, int> initialCoordinates, const std::pair<int, int>& movement);

constexpr int ROW_LENGTH = 103;
constexpr int COL_LENGTH = 101;
constexpr int NUM_ITERATIONS = 101 * 103;

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


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }

    std::vector<std::string> outputLines;
    ReadFromFile(argv[1], outputLines);

    // std::cout << Part1(outputLines) << std::endl;
    Part2(outputLines);
}

long long Part1(const std::vector<std::string>& outputLines)
{
    std::vector<unsigned long> quadrantNumbers;
    quadrantNumbers.reserve(4);
    for (size_t i = 0; i < 4; ++i)
    {
        quadrantNumbers.push_back(0);
    }

    for (const std::string& s : outputLines)
    {
        int pointer = 0;
        std::pair<int, int> initialPosition = ParsePosition(s, &pointer);
        std::pair<int, int> movement = ParsePosition(s.substr(pointer), &pointer);
        std::pair<int, int> finalPosition = Move(initialPosition, movement);
        std::cout << "InitialPosition: " << initialPosition.first << ", " << initialPosition.second << std::endl;
        std::cout << "Movement: " << movement.first << ", " << movement.second << std::endl;
        std::cout << "Final position: " << finalPosition.first << ", " << finalPosition.second << std::endl;
        int quadrant = GetQuadrantNumber(finalPosition);
        std::cout << "Quadrant: " << quadrant << std::endl;
        std::cout << std::endl;
        if (quadrant != -1)
        {
            ++quadrantNumbers[quadrant];
        }
    }

    return quadrantNumbers[0] * quadrantNumbers[1] * quadrantNumbers[2] * quadrantNumbers[3];
}

void Part2(const std::vector<std::string>& outputLines)
{
    std::ofstream ChristmasTreeFile{ "ChristmasTree.out" };
    std::vector<std::pair<int, int>> initialPoints;
    initialPoints.reserve(outputLines.size());
    std::vector<std::pair<int, int>> newPoints;
    newPoints.reserve(outputLines.size());
    std::vector<std::pair<int, int>> movement;
    movement.reserve(outputLines.size());

    for (const std::string& s : outputLines)
    {
        int pointer = 0;
        std::pair<int, int> initialPosition = ParsePosition(s, &pointer);
        std::pair<int, int> pointMovement = ParsePosition(s.substr(pointer), &pointer);
        initialPoints.push_back(initialPosition);
        newPoints.push_back(initialPosition);
        movement.push_back(pointMovement);
    }
    
    // iterate for all possible unique positions
    for (size_t i = 0; i < NUM_ITERATIONS; ++i)
    {
        std::unordered_map<std::pair<int, int>, int> encounteredPoints;
        bool foundDuplicate = false;

        for (size_t i = 0; i < initialPoints.size(); ++i)
        {
            newPoints[i] = MoveSingle(newPoints[i], movement[i]);
            if (encounteredPoints.find(newPoints[i]) != encounteredPoints.end())
            {
                foundDuplicate = true;
            }
            else
            {
                encounteredPoints[newPoints[i]] = 1;
            }
        }

        // find any iteration that does not have duplicate robot positions and print out
        // the art to check if it forms a christmas tree
        if (!foundDuplicate)
        {
            ChristmasTreeFile << "Iteration: " << i + 1 << std::endl;
            for (size_t r = 0; r < ROW_LENGTH; ++r)
            {
                for (size_t c = 0; c < COL_LENGTH; ++c)
                {
                    if (encounteredPoints.find({ c, r }) != encounteredPoints.end())
                    {
                        ChristmasTreeFile << "*";
                    }
                    else
                    {
                        ChristmasTreeFile << ".";
                    }
                }
                ChristmasTreeFile << std::endl;
            }

            ChristmasTreeFile << std::endl;
        }
    }

    ChristmasTreeFile.close();
}

std::pair<int, int> ParsePosition(const std::string& line, int* pointer)
{
    int equalIndex = line.find('=');
    int commaIndex = line.find(',');
    int initialCol = std::stoi(line.substr(equalIndex + 1, commaIndex - (equalIndex + 1)));
    int spaceIndex = line.find(' ');
    int initialRow = std::stoi(line.substr(commaIndex + 1, spaceIndex - (commaIndex + 1)));
    *pointer = spaceIndex + 1;
    return { initialCol, initialRow };
}

std::pair<int, int> Move(std::pair<int, int> initialCoordinates, const std::pair<int, int>& movement)
{
    for (size_t i = 0; i < NUM_ITERATIONS; ++i)
    {
        initialCoordinates = MoveSingle(initialCoordinates, movement);
    }
    return initialCoordinates;
}

std::pair<int, int> MoveSingle(std::pair<int, int> initialCoordinates, const std::pair<int, int>& movement)
{
    initialCoordinates = { initialCoordinates.first + movement.first, initialCoordinates.second + movement.second };
    initialCoordinates = GetFinalCoordinates(initialCoordinates);
    return initialCoordinates;
}


std::pair<int, int> GetFinalCoordinates(std::pair<int, int> initialCoordinates)
{
    int finalCol = initialCoordinates.first;
    if (finalCol < 0)
    {
        finalCol = COL_LENGTH + finalCol;
    }
    else if (finalCol >= COL_LENGTH)
    {
        finalCol = finalCol - COL_LENGTH;
    }

    int finalRow = initialCoordinates.second;
    if (finalRow < 0)
    {
        finalRow = ROW_LENGTH + finalRow;
    }
    else if (finalRow >= ROW_LENGTH)
    {
        finalRow = finalRow - ROW_LENGTH;
    }

    return { finalCol, finalRow };
}

/**
Returns -1 if it is not in a quadrant
*/
int GetQuadrantNumber(const std::pair<int, int>& coordinates)
{
    int colMidLine = COL_LENGTH / 2;
    int rowMidLine = ROW_LENGTH / 2;

    if (coordinates.first == colMidLine || coordinates.second == rowMidLine)
        return -1;

    if (coordinates.first < colMidLine && coordinates.second < rowMidLine)
        return 0;
    else if (coordinates.first < colMidLine && coordinates.second > rowMidLine)
        return 1;
    else if (coordinates.first > colMidLine && coordinates.second < rowMidLine)
        return 2;
    else
        return 3;
}