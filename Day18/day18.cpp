#include <iostream>
#include <vector>
#include <unordered_map>
#include <optional>
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
void Part2(const std::vector<std::string>& outputLines);
std::optional<unsigned long> BFS(const std::pair<int, int>& startPos, const std::pair<int, int>& endPos, std::vector<std::vector<bool>>& corruptedTiles);
void BuildInitialCorruptedTiles(const std::vector<std::pair<int, int>>& corruptedBytes, int numberCorruptedBytes, std::vector<std::vector<bool>>& corruptedTiles);
std::pair<int, int> ParseLine(const std::string& s);
bool IsOutOfBounds(const std::pair<int, int>& tile, const std::pair<int, int>& dimensions);

constexpr std::pair<int, int> DIMENSIONS = { 71, 71 };
constexpr std::pair<int, int> START_POS = { 0, 0 };
constexpr std::pair<int, int> END_POS = { DIMENSIONS.first - 1, DIMENSIONS.second - 1 };
constexpr int NUM_CORRUPTED_BYTES = 1024;

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

    //std::cout << Part1(outputLines) << std::endl;
    Part2(outputLines);
}

long long Part1(const std::vector<std::string>& outputLines)
{
    std::vector<std::pair<int, int>> corruptedBytes;

    for (const std::string& s : outputLines)
    {
        corruptedBytes.push_back(ParseLine(s));
    }

    std::vector<std::vector<bool>> corruptedTiles;
    BuildInitialCorruptedTiles(corruptedBytes, NUM_CORRUPTED_BYTES, corruptedTiles);
    return BFS(START_POS, END_POS, corruptedTiles).value_or(0);
}

void Part2(const std::vector<std::string>& outputLines)
{
    std::vector<std::pair<int, int>> corruptedBytes;

    for (const std::string& s : outputLines)
    {
        corruptedBytes.push_back(ParseLine(s));
    }

    std::vector<std::vector<bool>> corruptedTiles;
    BuildInitialCorruptedTiles(corruptedBytes, 0, corruptedTiles);

    for (const std::pair<int, int>& tile : corruptedBytes)
    {
        corruptedTiles[tile.first][tile.second] = true;
        if (!BFS(START_POS, END_POS, corruptedTiles).has_value())
        {
            std::cout << tile.first << "," << tile.second << std::endl;
            break;
        }
    }
}

std::pair<int, int> ParseLine(const std::string& s)
{
    int commaIndex = s.find(',');
    int row = std::stoi(s.substr(0, commaIndex));
    int col = std::stoi(s.substr(commaIndex + 1));
    return { row, col };
}

void BuildInitialCorruptedTiles(const std::vector<std::pair<int, int>>& corruptedBytes, int numberCorruptedBytes, std::vector<std::vector<bool>>& corruptedTiles)
{
    for (size_t r = 0; r < DIMENSIONS.first; ++r)
    {
        corruptedTiles.push_back({});
        for (size_t c = 0; c < DIMENSIONS.second; ++c)
        {
            corruptedTiles[r].push_back(false);
        }
    }

    for (size_t i = 0; i < numberCorruptedBytes; ++i)
    {
        corruptedTiles[corruptedBytes[i].first][corruptedBytes[i].second] = true;
    }
}

std::optional<unsigned long> BFS(const std::pair<int, int>& startPos, const std::pair<int, int>& endPos, std::vector<std::vector<bool>>& corruptedTiles)
{
    std::unordered_map<std::pair<int, int>, int> visitedTiles;
    std::vector<std::tuple<int, int, unsigned long>> q;
    q.emplace_back(startPos.first, startPos.second, 0);

    while (!q.empty())
    {
        auto [row, col, cost] = q[0];
        q.erase(q.begin());

        if (IsOutOfBounds({ row, col }, DIMENSIONS))
        {
            continue;
        }
        if (visitedTiles.find({ row, col }) != visitedTiles.end())
        {
            continue;
        }
        if (corruptedTiles[row][col])
        {
            continue;
        }
        if (row == endPos.first && col == endPos.second)
        {
            return std::optional<unsigned long>{cost};
        }

        visitedTiles[{row, col}] = 1;

        q.push_back({ row + 1, col, cost + 1 });
        q.push_back({ row - 1, col, cost + 1 });
        q.push_back({ row, col + 1, cost + 1 });
        q.push_back({ row, col - 1, cost + 1 });
    }

    std::cout << "No solution?" << std::endl;
    return std::nullopt;
}

bool IsOutOfBounds(const std::pair<int, int>& tile, const std::pair<int, int>& dimensions)
{
    return tile.first < 0 || tile.first >= dimensions.first || tile.second < 0 || tile.second >= dimensions.second;
}