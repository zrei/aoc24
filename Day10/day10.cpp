#include <iostream>
#include <vector>
#include <unordered_map>
#include "../fileRead.h"

enum Direction : char
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
long long BFS(const std::pair<int, int>& startingCoordinates, const std::vector<std::string>& matrix, const std::pair<int, int>& dimensions);
bool OutOfRange(const std::pair<int, int>& coordinates, const std::pair<int, int>& dimensions);
std::pair<int, int> Move(Direction dir, const std::pair<int, int>& initial);
long long BFSUniqueTrails(const std::pair<int, int>& startingCoordinates, const std::vector<std::string>& matrix, const std::pair<int, int>& dimensions);

constexpr char TRAIL_START = '0';
constexpr char TRAIL_END = '9';

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
    std::pair<int, int> dimensions {outputLines.size(), outputLines[0].size()};
    long long totalScore = 0;

    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            if (outputLines[r][c] == TRAIL_START)
            {
                totalScore += BFS({r, c}, outputLines, dimensions);
            }
        }
    }
    return totalScore;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::pair<int, int> dimensions {outputLines.size(), outputLines[0].size()};
    long long totalScore = 0;

    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            if (outputLines[r][c] == TRAIL_START)
            {
                totalScore += BFSUniqueTrails({r, c}, outputLines, dimensions);
            }
        }
    }
    return totalScore;
}

long long BFS(const std::pair<int, int>& startingCoordinates, const std::vector<std::string>& matrix, const std::pair<int, int>& dimensions)
{
    std::vector<std::pair<std::pair<int, int>, char>> q;
    std::unordered_map<std::pair<int, int>, int> uniqueTrailEnds;
    q.emplace_back(startingCoordinates, '0');

    while (!q.empty())
    {
        const auto[coordinates, trailPoint] = q[0];
        q.erase(q.begin());
        if (OutOfRange(coordinates, dimensions))
            continue;
        
        const auto[row, col] = coordinates;
        if (matrix[row][col] != trailPoint)
            continue;
        if (matrix[row][col] == TRAIL_END)
        {
            uniqueTrailEnds[coordinates] = 1;
            continue;
        }
        
        char nextChar = trailPoint + 1;
        q.emplace_back(Move(UP, coordinates), nextChar);
        q.emplace_back(Move(DOWN, coordinates), nextChar);
        q.emplace_back(Move(LEFT, coordinates), nextChar);
        q.emplace_back(Move(RIGHT, coordinates), nextChar);
    }

    return uniqueTrailEnds.size();
}

long long BFSUniqueTrails(const std::pair<int, int>& startingCoordinates, const std::vector<std::string>& matrix, const std::pair<int, int>& dimensions)
{
    long long uniqueTrails = 0;
    std::vector<std::pair<std::pair<int, int>, char>> q;
    //std::unordered_map<std::pair<int, int>, int> visitedSpots;
    q.emplace_back(startingCoordinates, '0');

    while (!q.empty())
    {
        const auto[coordinates, trailPoint] = q[0];

        q.erase(q.begin());
        if (OutOfRange(coordinates, dimensions))
            continue;
        //if (visitedSpots.find(coordinates) != visitedSpots.end())
        //    continue;

        const auto[row, col] = coordinates;
        if (matrix[row][col] != trailPoint)
            continue;
        if (matrix[row][col] == TRAIL_END)
        {
            ++uniqueTrails;
            continue;
        }
        
        //visitedSpots[coordinates] = 1;
        char nextChar = trailPoint + 1;
        q.emplace_back(Move(UP, coordinates), nextChar);
        q.emplace_back(Move(DOWN, coordinates), nextChar);
        q.emplace_back(Move(LEFT, coordinates), nextChar);
        q.emplace_back(Move(RIGHT, coordinates), nextChar);
    }

    return uniqueTrails;
}

bool OutOfRange(const std::pair<int, int>& coordinates, const std::pair<int, int>& dimensions)
{
    return coordinates.first < 0 || coordinates.first >= dimensions.first || coordinates.second < 0 || coordinates.second >= dimensions.second;
}

std::pair<int, int> Move(Direction dir, const std::pair<int, int>& initial)
{
    switch (dir)
    {
        case UP:
            return {initial.first - 1, initial.second};
        case DOWN:
            return {initial.first + 1, initial.second};
        case LEFT:
            return {initial.first, initial.second - 1};
        case RIGHT:
            return {initial.first, initial.second + 1};
        default:
            return {-1, -1};
    }
}