#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
void ConstructPositions(std::vector<std::pair<int, int>>& positions, const std::vector<std::string>& matrix, const std::pair<int, int>& dimensions, char symbol);
void FindAntinodes(const std::vector<std::pair<int, int>>& positions, std::unordered_map<std::pair<int, int>, int>& antinodePositions, const std::pair<int, int>& dimensions, bool extend = false);
bool IsWithinMap(const std::pair<int, int>& position, const std::pair<int, int>& dimensions);
std::pair<int, int> GetDistance(const std::pair<int, int>& first, const std::pair<int, int>& second);
std::pair<int, int> NegateDistance(const std::pair<int, int>& distance);
std::pair<int, int> GetCoordinatesFromDistance(const std::pair<int, int>& initialCoordinates, const std::pair<int, int>& distance);
void FindAntinodesInRange(char startRange, char endRange, const std::pair<int, int>& dimensions, std::unordered_map<std::pair<int, int>, int>& uniqueAntinodePositions, const std::vector<std::string>& matrix, bool extend = false);

constexpr char DIGIT_RANGE_START = '0';
constexpr char DIGIT_RANGE_END = '9';
constexpr char LOWERCASE_RANGE_START = 'a';
constexpr char LOWERCASE_RANGE_END = 'z';
constexpr char UPPERCASE_RANGE_START = 'A';
constexpr char UPPERCASE_RANGE_END = 'Z';

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
}

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
    std::pair<int, int> dimensions = {outputLines.size(), outputLines[0].size()};
    std::unordered_map<std::pair<int, int>, int> uniqueAntinodePositions; 

    FindAntinodesInRange(DIGIT_RANGE_START, DIGIT_RANGE_END, dimensions, uniqueAntinodePositions, outputLines);
    FindAntinodesInRange(LOWERCASE_RANGE_START, LOWERCASE_RANGE_END, dimensions, uniqueAntinodePositions, outputLines);
    FindAntinodesInRange(UPPERCASE_RANGE_START, UPPERCASE_RANGE_END, dimensions, uniqueAntinodePositions, outputLines);

    return uniqueAntinodePositions.size();
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::pair<int, int> dimensions = {outputLines.size(), outputLines[0].size()};
    std::unordered_map<std::pair<int, int>, int> uniqueAntinodePositions; 

    FindAntinodesInRange(DIGIT_RANGE_START, DIGIT_RANGE_END, dimensions, uniqueAntinodePositions, outputLines, true);
    FindAntinodesInRange(LOWERCASE_RANGE_START, LOWERCASE_RANGE_END, dimensions, uniqueAntinodePositions, outputLines, true);
    FindAntinodesInRange(UPPERCASE_RANGE_START, UPPERCASE_RANGE_END, dimensions, uniqueAntinodePositions, outputLines, true);

    return uniqueAntinodePositions.size();
}

void ConstructPositions(std::vector<std::pair<int, int>>& positions, const std::vector<std::string>& matrix, const std::pair<int, int>& dimensions, char symbol)
{
    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            if (matrix[r][c] == symbol)
            {
                positions.emplace_back(r, c);
            }
        }
    }
}

void FindAntinodes(const std::vector<std::pair<int, int>>& positions, std::unordered_map<std::pair<int, int>, int>& antinodePositions, const std::pair<int, int>& dimensions, bool extend)
{
    int numPositions = positions.size();
    for (size_t i = 0; i < numPositions; ++i)
    {
        for (size_t j = i + 1; j < numPositions; ++j)
        {
            std::pair<int, int> distance = GetDistance(positions[i], positions[j]);
            std::pair<int, int> firstAntinodePosition = GetCoordinatesFromDistance(positions[i], distance);
            if (extend)
            {
                while (IsWithinMap(firstAntinodePosition, dimensions))
                {
                    antinodePositions[firstAntinodePosition] = 1;
                    firstAntinodePosition = GetCoordinatesFromDistance(firstAntinodePosition, distance);
                }
            }
            else
            {
                if (IsWithinMap(firstAntinodePosition, dimensions))
                {
                    antinodePositions[firstAntinodePosition] = 1;
                }
            }
            
            std::pair<int, int> secondAntinodePosition = GetCoordinatesFromDistance(positions[j], NegateDistance(distance));
            if (extend)
            {
                while (IsWithinMap(secondAntinodePosition, dimensions))
                {
                    antinodePositions[secondAntinodePosition] = 1;
                    secondAntinodePosition = GetCoordinatesFromDistance(secondAntinodePosition, NegateDistance(distance));
                }
            }
            else
            {
                if (IsWithinMap(secondAntinodePosition, dimensions))
                {
                    antinodePositions[secondAntinodePosition] = 1;
                }
            }
            
        }
    }
}

void FindAntinodesInRange(char startRange, char endRange, const std::pair<int, int>& dimensions, std::unordered_map<std::pair<int, int>, int>& uniqueAntinodePositions, const std::vector<std::string>& matrix, bool extend)
{
    for (char c = startRange; c <= endRange; ++c)
    {
        std::vector<std::pair<int, int>> positions;
        ConstructPositions(positions, matrix, dimensions, c);
        FindAntinodes(positions, uniqueAntinodePositions, dimensions, extend);
        if (extend)
        {
            if (positions.size() > 1)
            {
                for (const std::pair<int, int>& p : positions)
                {
                    uniqueAntinodePositions[p] = 1;
                }
            }
        }
    }
}

bool IsWithinMap(const std::pair<int, int>& position, const std::pair<int, int>& dimensions)
{
    return position.first >= 0 && position.first < dimensions.first && position.second >= 0 && position.second < dimensions.second;
}

std::pair<int, int> GetDistance(const std::pair<int, int>& first, const std::pair<int, int>& second)
{
    return {first.first - second.first, first.second - second.second};
}

std::pair<int, int> NegateDistance(const std::pair<int, int>& distance)
{
    return {-distance.first, -distance.second};
}

std::pair<int, int> GetCoordinatesFromDistance(const std::pair<int, int>& initialCoordinates, const std::pair<int, int>& distance)
{
    return {initialCoordinates.first + distance.first, initialCoordinates.second + distance.second};
}