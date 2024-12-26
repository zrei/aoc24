#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include "../fileRead.h"

enum Direction : char
{
    NORTH,
    SOUTH,
    EAST,
    WEST
};

std::pair<int, int> MovePoint(const std::pair<int, int>& currPos, Direction dir);
Direction RotateDirClockwise(Direction initialDir);
Direction RotateDirAnticlockwise(Direction initialDir);

struct Point
{
    std::pair<int, int> m_Position;
    Direction m_Direction;
    unsigned long m_Cost;

    Point(int row, int col, Direction dir, unsigned long cost) : m_Position(row, col), m_Direction(dir), m_Cost(cost) {}
    Point(std::pair<int, int> pos, Direction dir, unsigned long cost) : m_Position(pos), m_Direction(dir), m_Cost(cost) {}

    Point Move() const {
        std::pair<int, int> newPos = MovePoint(m_Position, m_Direction);
        return { newPos, m_Direction, m_Cost + 1 };
    }

    Point RotateClockwise() const
    {
        return { m_Position, RotateDirClockwise(m_Direction), m_Cost + 1000 };
    }

    Point RotateAnticlockwise() const
    {
        return { m_Position, RotateDirAnticlockwise(m_Direction), m_Cost + 1000 };
    }

    std::tuple<int, int, Direction> GetKey() const
    {
        return { m_Position.first, m_Position.second, m_Direction };
    }
};

struct PathPoint
{
    std::shared_ptr<PathPoint> m_PrevPoint;
    std::pair<int, int> m_Position;
    Direction m_Direction;
    unsigned long m_Cost;

    PathPoint(std::pair<int, int> pos, Direction dir, unsigned long cost) : m_PrevPoint(nullptr), m_Position(pos), m_Direction(dir), m_Cost(cost) {}
    PathPoint(std::shared_ptr<PathPoint> prevPoint, std::pair<int, int> pos, Direction dir, unsigned long cost) : m_PrevPoint(prevPoint), m_Position(pos), m_Direction(dir), m_Cost(cost) {}

    std::shared_ptr<PathPoint> Move(std::shared_ptr<PathPoint> pointerToSelf) const {
        std::pair<int, int> newPos = MovePoint(m_Position, m_Direction);
        return std::make_shared<PathPoint>(pointerToSelf, newPos, m_Direction, m_Cost + 1);
    }

    std::shared_ptr<PathPoint> RotateClockwise(std::shared_ptr<PathPoint> pointerToSelf) const
    {
        return std::make_shared<PathPoint>(pointerToSelf, m_Position, RotateDirClockwise(m_Direction), m_Cost + 1000);
    }

    std::shared_ptr<PathPoint> RotateAnticlockwise(std::shared_ptr<PathPoint> pointerToSelf) const
    {
        return std::make_shared<PathPoint>(pointerToSelf, m_Position, RotateDirAnticlockwise(m_Direction), m_Cost + 1000);
    }

    std::tuple<int, int, Direction> GetKey() const
    {
        return { m_Position.first, m_Position.second, m_Direction };
    }
};

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
void ParseMap(const std::vector<std::string>& map, const std::pair<int, int>& dimensions, std::vector<std::vector<bool>>& wallPos);
unsigned long Dijkstra(const std::vector<std::vector<bool>>& wallPos, const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, Direction startDir, bool hasEndDir = false, Direction endDir = NORTH);
unsigned long DijkstraAllPaths(const std::vector<std::vector<bool>>& wallPos, const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, unsigned long shortestPathLength);
void TracePath(std::shared_ptr<PathPoint> pathPoint, std::unordered_map<std::pair<int, int>, int>& pointStore, std::unordered_map<std::tuple<int, int, Direction>, unsigned long>& minDistanceToEnd, unsigned long actualPathCost);
bool DijkstraAllPoints(const std::vector<std::vector<bool>>& wallPos, const std::pair<int, int>& point, const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, unsigned long actualPathCost);
void PrintState(const std::pair<int, int>& dimensions, const std::vector<std::vector<bool>>& wallPos, const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, const std::unordered_map<std::pair<int, int>, int>& pointStore);

constexpr char WALL = '#';
constexpr std::pair<int, int> END_OF_PATH = { -1, -1 };

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
    class hash<std::tuple<int, int, Direction>>
    {
    public:
        std::uint64_t operator()(const std::tuple<int, int, Direction>& x) const
        {
            auto [row, col, direction] = x;
            size_t h = std::hash<int>()(row) ^ std::hash<int>()(col) ^ std::hash<char>()(direction);
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
    std::vector<std::vector<bool>> wallPos;
    ParseMap(outputLines, dimensions, wallPos);
    std::pair<int, int> startingPoint = { dimensions.first - 2, 1 };
    std::pair<int, int> endPoint = { 1, dimensions.second - 2 };
    return Dijkstra(wallPos, startingPoint, endPoint, EAST);
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::pair<int, int> dimensions = { outputLines.size(), outputLines[0].size() };
    std::vector<std::vector<bool>> wallPos;
    ParseMap(outputLines, dimensions, wallPos);
    std::pair<int, int> startingPoint = { dimensions.first - 2, 1 };
    std::pair<int, int> endPoint = { 1, dimensions.second - 2 };
    unsigned long shortestPathCost = Dijkstra(wallPos, startingPoint, endPoint, EAST);
    
    std::cout << "Shortest path cost: " << shortestPathCost << std::endl;
    
    /*
    unsigned long numTiles = 0;

    
    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            std::cout << "Check point: " << r << ", " << c << std::endl;
            if (wallPos[r][c])
                continue;
            if (r == startingPoint.first && c == startingPoint.second)
                ++numTiles;
            else if (r == endPoint.first && c == endPoint.second)
                ++numTiles;
            else if (DijkstraAllPoints(wallPos, { r, c }, startingPoint, endPoint, shortestPathCost))
            {
                std::cout << "Point is okay: " << r << ", " << c << std::endl;
                ++numTiles;
            }
                
        }
    }

    return numTiles;
    */
    
    return DijkstraAllPaths(wallPos, startingPoint, endPoint, shortestPathCost);
    
}

void ParseMap(const std::vector<std::string>& map, const std::pair<int, int>& dimensions, std::vector<std::vector<bool>>& wallPos)
{
    for (size_t r = 0; r < dimensions.first; ++r)
    {
        wallPos.push_back({});
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            wallPos[r].push_back(map[r][c] == WALL);
        }
    }
}

unsigned long Dijkstra(const std::vector<std::vector<bool>>& wallPos, const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, Direction startDir, bool hasEndDir, Direction endDir)
{
    Point initialPoint{ startPoint, startDir, 0 };
    auto cmp = [](const Point& x, const Point& y) { return x.m_Cost > y.m_Cost; };
    std::unordered_map<std::tuple<int, int, Direction>, int> store;
    std::priority_queue<Point, std::vector<Point>, decltype(cmp)> pq(cmp);

    pq.push(initialPoint);

    while (!pq.empty())
    {
        Point point = pq.top();
        pq.pop();

        // is end point
        if (point.m_Position == endPoint)
        {
            if (hasEndDir && point.m_Direction != endDir)
            {
                int clockwiseCost = 0;
                Direction initialDir = point.m_Direction;
                while (initialDir != endDir)
                {
                    initialDir = RotateDirClockwise(initialDir);
                    clockwiseCost += 1000;
                }
                initialDir = point.m_Direction;
                int anticlockwiseCost = 0;
                while (initialDir != endDir)
                {
                    initialDir = RotateDirAnticlockwise(initialDir);
                    anticlockwiseCost += 1000;
                }

                int minCost = std::min(clockwiseCost, anticlockwiseCost);

                return point.m_Cost + minCost;
            }
            else
            {
                return point.m_Cost;
            }
        }

        // in a wall
        if (wallPos[point.m_Position.first][point.m_Position.second])
        {
            continue;
        }
        // has gone to before
        if (store.find(point.GetKey()) != store.end())
        {
            continue;
        }

        store[point.GetKey()] = 1;

        pq.push(point.RotateAnticlockwise());
        pq.push(point.RotateClockwise());
        pq.push(point.Move());
    }

    return -1;
}


unsigned long DijkstraAllPaths(const std::vector<std::vector<bool>>& wallPos, const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, unsigned long shortestPathLength)
{
    std::shared_ptr<PathPoint> initialPoint = std::make_shared<PathPoint>(startPoint, EAST, 0);
    auto cmp = [](const std::shared_ptr<PathPoint> x, const std::shared_ptr<PathPoint> y) { return x->m_Cost > y->m_Cost; };
    std::priority_queue<std::shared_ptr<PathPoint>, std::vector<std::shared_ptr<PathPoint>>, decltype(cmp)> pq(cmp);

    std::unordered_map<std::pair<int, int>, int> pointsOnPaths;
    std::unordered_map<std::tuple<int, int, Direction>, unsigned long> shortestDistanceToPoint;
    std::unordered_map<std::tuple<int, int, Direction>, unsigned long> shortestDistanceToEnd;

    pq.push(initialPoint);

    while (!pq.empty())
    {
        std::shared_ptr<PathPoint> point = pq.top();
        pq.pop();

        bool isPoint = point->m_Position.first == 11 && point->m_Position.second == 4;

        // is end point
        if (point->m_Position == endPoint)
        {   
            TracePath(point, pointsOnPaths, shortestDistanceToEnd, shortestPathLength);
            continue;
        }

        // has a known shortest distance to the end from here
        if (shortestDistanceToEnd.find(point->GetKey()) != shortestDistanceToEnd.end())
        {     
            // shortest distance to end + curr cost meets the shortest path length
            if (shortestDistanceToEnd[point->GetKey()] + point->m_Cost <= shortestPathLength)
            {
                TracePath(point, pointsOnPaths, shortestDistanceToEnd, shortestPathLength);
                continue;
            }
            // is too much, this point is not feasible
            else
            {
                continue;
            }
        }

        // in a wall
        if (wallPos[point->m_Position.first][point->m_Position.second])
        {
            continue;
        }

        // has exceeded cost
        if (point->m_Cost >= shortestPathLength)
        {
            continue;
        }

        // has exceeded previous min cost or is same as previous min cost: we have explored this before
        if (shortestDistanceToPoint.find(point->GetKey()) != shortestDistanceToPoint.end() && shortestDistanceToPoint[point->GetKey()] < point->m_Cost)
        {
            continue;
        }

        shortestDistanceToPoint[point->GetKey()] = point->m_Cost;
        
        pq.push(point->RotateAnticlockwise(point));
        pq.push(point->RotateClockwise(point));
        pq.push(point->Move(point));
    }

    PrintState({ wallPos.size(), wallPos[0].size() }, wallPos, startPoint, endPoint, pointsOnPaths);
    return pointsOnPaths.size();
}

void PrintState(const std::pair<int, int>& dimensions, const std::vector<std::vector<bool>>& wallPos, const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, const std::unordered_map<std::pair<int, int>, int>& pointStore)
{
    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            if (wallPos[r][c])
            {
                std::cout << "#";
            }
            else if (r == startPoint.first && c == startPoint.second)
            {
                std::cout << "S";
            }
            else if (r == endPoint.first && c == endPoint.second)
            {
                std::cout << "E";
            }
            else if (pointStore.find({ r, c }) != pointStore.end())
            {
                std::cout << "O";
            }
            else
            {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
}

bool DijkstraAllPoints(const std::vector<std::vector<bool>>& wallPos, const std::pair<int, int>& point, const std::pair<int, int>& startPoint, const std::pair<int, int>& endPoint, unsigned long actualPathCost)
{
    for (char c = 0; c < 4; ++c)
    {
        unsigned long possiblePathToStart = Dijkstra(wallPos, startPoint, point, EAST, true, (Direction) c);
        unsigned long possiblePathToEnd = Dijkstra(wallPos, point, endPoint, (Direction) c);
        //std::cout << "Point: " << point.first << ", " << point.second << ", direction: " << c << ", cost to start: " << possiblePathToStart << ", cost to end: " << possiblePathToEnd << std::endl;
        if (possiblePathToEnd == -1 || possiblePathToStart == -1)
            continue;
        if (possiblePathToEnd + possiblePathToStart <= actualPathCost)
        {
            return true;
        }
    }
    return false;
}

void TracePath(std::shared_ptr<PathPoint> pathPoint, std::unordered_map<std::pair<int, int>, int>& pointStore, std::unordered_map<std::tuple<int, int, Direction>, unsigned long>& minDistanceToEnd, unsigned long actualPathCost)
{
    std::shared_ptr<PathPoint> currPoint = pathPoint;
    while (true)
    {
        pointStore[currPoint->m_Position] = 1;
        if (minDistanceToEnd.find(currPoint->GetKey()) == minDistanceToEnd.end())
        {
            minDistanceToEnd[currPoint->GetKey()] = actualPathCost - currPoint->m_Cost;
        }
        else
        {
            minDistanceToEnd[currPoint->GetKey()] = std::min(minDistanceToEnd[currPoint->GetKey()], actualPathCost - currPoint->m_Cost);
        }
        if (currPoint->m_PrevPoint == nullptr)
            return;
        else
            currPoint = currPoint->m_PrevPoint;
    }
}

std::pair<int, int> MovePoint(const std::pair<int, int>& currPos, Direction dir)
{
    switch (dir)
    {
    case NORTH:
        return { currPos.first - 1, currPos.second };
    case SOUTH:
        return { currPos.first + 1, currPos.second };
    case EAST:
        return { currPos.first, currPos.second + 1 };
    case WEST:
        return { currPos.first, currPos.second - 1 };
    default:
        return { -1, -1 };
    }
}

Direction RotateDirClockwise(Direction initialDir)
{
    switch (initialDir)
    {
    case NORTH:
        return EAST;
    case SOUTH:
        return WEST;
    case EAST:
        return SOUTH;
    case WEST:
        return NORTH;
    default:
        return NORTH;
    }
}

Direction RotateDirAnticlockwise(Direction initialDir)
{
    switch (initialDir)
    {
    case NORTH:
        return WEST;
    case SOUTH:
        return EAST;
    case WEST:
        return SOUTH;
    case EAST:
        return NORTH;
    default:
        return NORTH;
    }
}