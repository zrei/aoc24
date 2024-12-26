#include <iostream>
#include <vector>
#include <unordered_map>
#include "../fileRead.h"

enum Direction : char
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
void ParseMaze(const std::pair<int, int>& dimensions, const std::vector<std::string>& maze, std::pair<int, int>& startPoint, std::pair<int, int>& endPoint, std::unordered_map<std::pair<int, int>, int>& routeCache, std::vector<std::pair<int, int>>& route);
unsigned long FindShortcuts(const std::pair<int, int>& dimensions, const std::vector<std::string>& maze, const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, std::unordered_map<std::pair<int, int>, int>& routeCache, const std::vector<std::pair<int, int>>& route);
bool IsOppositeDirection(Direction direction1, Direction direction2);
std::pair<int, int> Move(const std::pair<int, int>& currPoint, Direction moveDir, int amount = 1);
bool IsOutOfBounds(const std::pair<int, int>& dimensions, const std::pair<int, int>& currPoint);
unsigned long FindShortcutsHigh(const std::pair<int, int>& dimensions, const std::vector<std::string>& maze, const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, std::unordered_map<std::pair<int, int>, int>& routeCache, const std::vector<std::pair<int, int>>& route);

constexpr char START_POINT = 'S';
constexpr char END_POINT = 'E';
constexpr char WALL = '#';
constexpr int NUM_PICOSECONDS_SAVED = 100;
constexpr int NUM_SHORTCUT_TRAVEL = 20;

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
    std::pair<int, int> dimensions = { outputLines.size(), outputLines[0].size() };
    std::vector<std::pair<int, int>> route;
    std::unordered_map<std::pair<int, int>, int> routeCache;
    std::pair<int, int> startPoint;
    std::pair<int, int> endPoint;
    ParseMaze(dimensions, outputLines, startPoint, endPoint, routeCache, route);

    return FindShortcuts(dimensions, outputLines, startPoint, endPoint, routeCache, route);
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::pair<int, int> dimensions = { outputLines.size(), outputLines[0].size() };
    std::vector<std::pair<int, int>> route;
    std::unordered_map<std::pair<int, int>, int> routeCache;
    std::pair<int, int> startPoint;
    std::pair<int, int> endPoint;
    ParseMaze(dimensions, outputLines, startPoint, endPoint, routeCache, route);

    return FindShortcutsHigh(dimensions, outputLines, startPoint, endPoint, routeCache, route);
}

void ParseMaze(const std::pair<int, int>& dimensions, const std::vector<std::string>& maze, std::pair<int, int>& startPoint, std::pair<int, int>& endPoint, std::unordered_map<std::pair<int, int>, int>& routeCache, std::vector<std::pair<int, int>>& route)
{
    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            if (maze[r][c] == START_POINT)
                startPoint = { r, c };
            else if (maze[r][c] == END_POINT)
                endPoint = { r, c };
        }
    }

    std::pair<int, int> currPoint = startPoint;
    Direction currDirection = NONE;
    int pointCount = 0;

    while (currPoint != endPoint)
    {
        routeCache[currPoint] = pointCount;
        route.push_back(currPoint);
        for (char i = 0; i < 4; ++i)
        {
            if (IsOppositeDirection(currDirection, (Direction) i))
            {
                continue;
            }
            std::pair<int, int> newPoint = Move(currPoint, (Direction) i);
            if (IsOutOfBounds(dimensions, newPoint))
            {
                continue;
            }
            if (maze[newPoint.first][newPoint.second] != WALL)
            {
                currPoint = newPoint;
                currDirection = (Direction)i;
                break;
            }
        }
        ++pointCount;
    }
    routeCache[currPoint] = pointCount;
    route.push_back(currPoint);
}

unsigned long FindShortcuts(const std::pair<int, int>& dimensions, const std::vector<std::string>& maze, const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, std::unordered_map<std::pair<int, int>, int>& routeCache, const std::vector<std::pair<int, int>>& route)
{
    unsigned long numberShortcuts = 0;

    for (size_t i = 0; i < route.size(); ++i)
    {
        std::pair<int, int> currPoint = route[i];
        for (char d = 0; d < 4; ++d)
        {
            for (int shortcutCount = 1; shortcutCount <= 2; ++shortcutCount)
            {
                std::pair<int, int> newPoint = Move(currPoint, (Direction) d, shortcutCount);
                if (routeCache.find(newPoint) == routeCache.end() || routeCache[newPoint] <= i)
                {
                    continue;
                }
                int savedSeconds = routeCache[newPoint] - i - shortcutCount;
                if (savedSeconds >= NUM_PICOSECONDS_SAVED)
                    ++numberShortcuts;
            }
        }
    }
    
    return numberShortcuts;
}

unsigned long FindShortcutsHigh(const std::pair<int, int>& dimensions, const std::vector<std::string>& maze, const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, std::unordered_map<std::pair<int, int>, int>& routeCache, const std::vector<std::pair<int, int>>& route)
{
    unsigned long numberShortcuts = 0;

    for (size_t i = 0; i < route.size(); ++i)
    {
        std::cout << "Point " << i << std::endl;
        std::pair<int, int> currPoint = route[i];
        std::unordered_map<std::pair<int, int>, int> cachedEndPoints;
        
        // point, then direction travelled
        std::vector<std::tuple<int, int, int>> q;
        q.emplace_back(currPoint.first, currPoint.second, 0);
        while (!q.empty())
        {
            auto [row, col, distanceTravelled] = q[0];
            q.erase(q.begin());

            if (distanceTravelled > NUM_SHORTCUT_TRAVEL)
                continue;

            std::pair<int, int> newPoint = { row, col };
            if (IsOutOfBounds(dimensions, newPoint))
                continue;

            if (cachedEndPoints.find(newPoint) != cachedEndPoints.end())
                continue;

            cachedEndPoints[newPoint] = 1;

            if (routeCache.find(newPoint) != routeCache.end() && routeCache[newPoint] > i)
            {
                int savedSeconds = routeCache[newPoint] - i - distanceTravelled;
                if (savedSeconds >= NUM_PICOSECONDS_SAVED)
                    ++numberShortcuts;
            }

            for (char d = 0; d < 4; ++d)
            {
                std::pair<int, int> movedPoint = Move(newPoint, (Direction)d);
                q.emplace_back(movedPoint.first, movedPoint.second, distanceTravelled + 1);
            }
        }
    }

    return numberShortcuts;
}

bool IsOppositeDirection(Direction direction1, Direction direction2)
{
    switch (direction1)
    {
    case UP:
        return direction2 == DOWN;
    case DOWN:
        return direction2 == UP;
    case LEFT:
        return direction2 == RIGHT;
    case RIGHT:
        return direction2 == LEFT;
    default:
        return false;
    }
}

bool IsOutOfBounds(const std::pair<int, int>& dimensions, const std::pair<int, int>& currPoint)
{
    return currPoint.first < 0 || currPoint.first >= dimensions.first || currPoint.second < 0 || currPoint.second >= dimensions.second;
}

std::pair<int, int> Move(const std::pair<int, int>& currPoint, Direction moveDir, int amount)
{
    switch (moveDir)
    {
    case UP:
        return { currPoint.first - amount, currPoint.second };
    case DOWN:
        return { currPoint.first + amount, currPoint.second };
    case LEFT:
        return { currPoint.first, currPoint.second - amount };
    case RIGHT:
        return { currPoint.first, currPoint.second + amount };
    default:
        return { -1, -1 };
    }
}