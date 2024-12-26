#include <iostream>
#include <vector>
#include <unordered_map>
#include "../fileRead.h"

enum Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT
};

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
void CreateMap(const std::pair<int, int>& dimensions, std::vector<std::vector<bool>>& map);
unsigned long GetAreaValue(const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix, const std::pair<int, int>& point, std::vector<std::vector<bool>>& map);
std::pair<int, int> GetPoint(const std::pair<int, int>& currPoint, Direction dir);
bool IsOutOfBounds(const std::pair<int, int>& point, const std::pair<int, int>& dimensions);
bool IsPerimeterInDirection(const std::pair<int, int>& currPoint, Direction dir, const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix);
bool HasNextPoint(const std::pair<int, int>& currPoint, const std::pair<int, int>& dimensions, Direction dir, const std::vector<std::string>& matrix);
Direction TurnRight(Direction dir);
unsigned long GetAreaValuePart2(const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix, const std::pair<int, int>& point, std::vector<std::vector<bool>>& map);
std::string TranslateDir(Direction dir);

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
    std::pair<int, int> dimensions{ outputLines.size(), outputLines[0].size() };
    std::vector<std::vector<bool>> map;
    CreateMap(dimensions, map);
    long long totalScore = 0;

    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            totalScore += GetAreaValue(dimensions, outputLines, { r, c }, map);
        }
    }

    return totalScore;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::pair<int, int> dimensions{ outputLines.size(), outputLines[0].size() };
    std::vector<std::vector<bool>> map;
    CreateMap(dimensions, map);
    long long totalScore = 0;

    GetAreaValuePart2(dimensions, outputLines, { 0, 0 }, map);

    /*
    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            totalScore += GetAreaValuePart2(dimensions, outputLines, { r, c }, map);
        }
    }
    */

    return totalScore;
}

void CreateMap(const std::pair<int, int>& dimensions, std::vector<std::vector<bool>>& map)
{
    for (size_t r = 0; r < dimensions.first; ++r)
    {
        map.push_back({});
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            map[r].push_back(false);
        }
    }
}

unsigned long GetAreaValue(const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix, const std::pair<int, int>& point, std::vector<std::vector<bool>>& map)
{
    if (map[point.first][point.second])
        return 0;

    std::unordered_map<std::pair<int, int>, int> visitedPoints;
    std::vector<std::pair<int, int>> q;
    q.push_back(point);

    unsigned long perimeter = 0;

    while (!q.empty())
    {
        std::pair<int, int> currPoint = q[0];
        q.erase(q.begin());
        if (visitedPoints.find(currPoint) != visitedPoints.end())
        {
            continue;
        }
        visitedPoints[currPoint] = 1;
       
        map[currPoint.first][currPoint.second] = true;
        
        if (IsPerimeterInDirection(currPoint, UP, dimensions, matrix))
        {
            ++perimeter;
        }
        else
        {
            q.push_back(GetPoint(currPoint, UP));
        }

        if (IsPerimeterInDirection(currPoint, DOWN, dimensions, matrix))
        {
            ++perimeter;
        }
        else
        {
            q.push_back(GetPoint(currPoint, DOWN));
        }

        if (IsPerimeterInDirection(currPoint, RIGHT, dimensions, matrix))
        {
            ++perimeter;
        }
        else
        {
            q.push_back(GetPoint(currPoint, RIGHT));
        }

        if (IsPerimeterInDirection(currPoint, LEFT, dimensions, matrix))
        {
            ++perimeter;
        }
        else
        {
            q.push_back(GetPoint(currPoint, LEFT));
        }
    }

    unsigned long area = visitedPoints.size();

    std::cout << "Char: " << matrix[point.first][point.second] << ", area: " << area << ", perimeter: " << perimeter << std::endl;
    return area * perimeter;
}

// number of sides == number of corners
// a corner is three L shaped same character that DOES NOT have the same character bordering at least one side
// of the middle character. The number of diff characters bordering the middle section is the number of corners it represents (?)
// the diff chara can be no chara (i.e. it is on the border)
unsigned long GetAreaValuePart2(const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix, const std::pair<int, int>& point, std::vector<std::vector<bool>>& map)
{
    if (map[point.first][point.second])
        return 0;

    std::unordered_map<std::pair<int, int>, int> visitedPoints;
    std::vector<std::pair<int, int>> q;
    q.push_back(point);

    while (!q.empty())
    {
        std::pair<int, int> currPoint = q[0];
        q.erase(q.begin());
        if (visitedPoints.find(currPoint) != visitedPoints.end())
        {
            continue;
        }
        visitedPoints[currPoint] = 1;

        map[currPoint.first][currPoint.second] = true;

        if (!IsPerimeterInDirection(currPoint, UP, dimensions, matrix))
        {
            q.push_back(GetPoint(currPoint, UP));
        }

        if (!IsPerimeterInDirection(currPoint, DOWN, dimensions, matrix))
        {
            q.push_back(GetPoint(currPoint, DOWN));
        }

        if (!IsPerimeterInDirection(currPoint, RIGHT, dimensions, matrix))
        {
            q.push_back(GetPoint(currPoint, RIGHT));
        }

        if (!IsPerimeterInDirection(currPoint, LEFT, dimensions, matrix))
        {
            q.push_back(GetPoint(currPoint, LEFT));
        }
    }

    unsigned long area = visitedPoints.size();

    std::pair<int, int> sidePoint = point;
    Direction currDir = RIGHT;
    std::cout << "Starting from " << sidePoint.first << ", " << sidePoint.second << std::endl;
    unsigned long numSides = 0;
    while (sidePoint != point || currDir != UP)
    {
        if (currDir == DOWN)
        {
            if (HasNextPoint(sidePoint, dimensions, RIGHT, matrix))
            {
                ++numSides;
                currDir = RIGHT;
                sidePoint = GetPoint(sidePoint, currDir);
                continue;
            }
            else if (HasNextPoint(sidePoint, dimensions, LEFT, matrix))
            {
                ++numSides;
                currDir = LEFT;
                sidePoint = GetPoint(sidePoint, currDir);
                continue;
            }
        }
        else if (currDir == UP)
        {
            if (HasNextPoint(sidePoint, dimensions, LEFT, matrix))
            {
                ++numSides;
                currDir = LEFT;
                sidePoint = GetPoint(sidePoint, currDir);
                continue;
            }
            else if (HasNextPoint(sidePoint, dimensions, RIGHT, matrix))
            {
                ++numSides;
                currDir = RIGHT;
                sidePoint = GetPoint(sidePoint, currDir);
                continue;
            }
        }
        else if (currDir == RIGHT)
        {
            if (HasNextPoint(sidePoint, dimensions, UP, matrix))
            {
                ++numSides;
                currDir = UP;
                sidePoint = GetPoint(sidePoint, currDir);
                continue;
            }
            else if (HasNextPoint(sidePoint, dimensions, DOWN, matrix))
            {
                ++numSides;
                currDir = DOWN;
                sidePoint = GetPoint(sidePoint, currDir);
                continue;
            }
        }
        else if (currDir == LEFT)
        {
            if (HasNextPoint(sidePoint, dimensions, DOWN, matrix))
            {
                ++numSides;
                currDir = DOWN;
                sidePoint = GetPoint(sidePoint, currDir);
                continue;
            }
            else if (HasNextPoint(sidePoint, dimensions, UP, matrix))
            {
                ++numSides;
                currDir = UP;
                sidePoint = GetPoint(sidePoint, currDir);
                continue;
            }
        }

        sidePoint = GetPoint(sidePoint, currDir);
        /*
        while (HasNextPoint(sidePoint, dimensions, currDir, matrix))
        {
            std::cout << sidePoint.first << ", " << sidePoint.second << ", direction: " << TranslateDir(currDir) << std::endl;
            sidePoint = GetPoint(sidePoint, currDir);
        }
        std::cout << "No more expansion" << std::endl;
        ++numSides;
        if (sidePoint == point)
        {
            std::cout << "Got back to same point, direction: " << TranslateDir(currDir) << std::endl;
            break;
        }
        if (sidePoint == point && currDir == UP)
            break;
        if (currDir == DOWN)
        {
            if (HasNextPoint(sidePoint, dimensions, RIGHT, matrix))
                currDir = RIGHT;
            else
                currDir = LEFT;
        }
        else if (currDir == UP)
        {
            if (HasNextPoint(sidePoint, dimensions, LEFT, matrix))
                currDir = LEFT;
            else
                currDir = RIGHT;
        }
        else if (currDir == RIGHT)
        {
            if (HasNextPoint(sidePoint, dimensions, UP, matrix))
                currDir = UP;
            else
                currDir = DOWN;
        }
        else
        {
            if (HasNextPoint(sidePoint, dimensions, DOWN, matrix))
                currDir = DOWN;
            else
                currDir = UP;
        }      
        */
    }
    std::cout << "Char: " << matrix[point.first][point.second] << ", area: " << area << ", sides: " << numSides << std::endl;
    return area * numSides;
}

std::pair<int, int> GetPoint(const std::pair<int, int>& currPoint, Direction dir)
{
    switch (dir)
    {
    case UP:
        return { currPoint.first - 1, currPoint.second };
    case DOWN:
        return { currPoint.first + 1, currPoint.second };
    case RIGHT:
        return { currPoint.first, currPoint.second + 1 };
    case LEFT:
        return { currPoint.first, currPoint.second - 1 };
    default:
        return { -1, -1 };
    }
}

bool IsOutOfBounds(const std::pair<int, int>& point, const std::pair<int, int>& dimensions)
{
    return point.first < 0 || point.first >= dimensions.first || point.second < 0 || point.second >= dimensions.second;
}

bool IsPerimeterInDirection(const std::pair<int, int>& currPoint, Direction dir, const std::pair<int, int>& dimensions, const std::vector<std::string>& matrix)
{
    std::pair<int, int> point = GetPoint(currPoint, dir);
    if (IsOutOfBounds(point, dimensions) || matrix[point.first][point.second] != matrix[currPoint.first][currPoint.second])
        return true;
    else
        return false;
}  

bool HasNextPoint(const std::pair<int, int>& currPoint, const std::pair<int, int>& dimensions, Direction dir, const std::vector<std::string>& matrix)
{
    std::pair<int, int> point = GetPoint(currPoint, dir);
    if (IsOutOfBounds(point, dimensions))
        return false;
    return matrix[point.first][point.second] == matrix[currPoint.first][currPoint.second];
}

Direction TurnRight(Direction dir)
{
    switch (dir)
    {
    case UP:
        return RIGHT;
    case DOWN:
        return LEFT;
    case RIGHT:
        return DOWN;
    case LEFT:
        return UP;
    default:
        return UP;
    }
}

std::string TranslateDir(Direction dir)
{
    switch (dir)
    {
    case UP:
        return "UP";
    case DOWN:
        return "DOWN";
    case RIGHT:
        return "RIGHT";
    case LEFT:
        return "LEFT";
    default:
        return "???";
    }
}