#include <iostream>
#include <vector>
#include <unordered_map>
#include "../fileRead.h"

enum Direction : char
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

enum Obstacle : char
{
    BOX,
    WALL,
    NONE,
    LEFT_BOX,
    RIGHT_BOX,
};

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
bool IsOutOfBounds(const std::pair<int, int>& coordinates, const std::pair<int, int>& dimensions, const std::vector<std::vector<Obstacle>>& obstacles);
void ShiftPoints(const std::pair<int, int>& start, const std::pair<int, int>& end, std::vector<std::vector<Obstacle>>& boxPos);
std::pair<int, int> GetFinalBoxPoint(const std::pair<int, int>& point, Direction dir, const std::vector<std::vector<Obstacle>>& boxPos, const std::pair<int, int>& dimensions);
std::pair<int, int> DoMove(const std::pair<int, int>& coordinates, Direction dir);
void PerformInstruction(Direction dir, std::pair<int, int>& robotPosition, std::vector<std::vector<Obstacle>>& boxPos, const std::pair<int, int>& dimensions);
void ParseInstructions(const std::string& line, std::vector<Direction>& dirs);
bool AllPointsInBound(const std::vector<std::pair<int, int>>& shiftBoxPoints, Direction dir, const std::pair<int, int>& dimensions, const std::vector<std::vector<Obstacle>>& boxPos);
bool IsBoxPoint(const std::pair<int, int>& point, const std::vector<std::vector<Obstacle>>& boxPos);
void GetShiftedBoxPoints(const std::pair<int, int>& point, Direction dir, const std::vector<std::vector<Obstacle>>& boxPos, const std::pair<int, int>& dimensions, std::vector<std::pair<int, int>>& shiftedBoxPoints);
void ShiftPointsPart2(std::vector<std::pair<int, int>>& shiftBoxPoints, std::vector<std::vector<Obstacle>>& boxPos, Direction dir);
void PrintStatus(const std::pair<int, int>& dimensions, const std::vector<std::vector<Obstacle>>& pointDesc, const std::pair<int, int>& robotPos);
void PerformInstructionPart2(Direction dir, std::pair<int, int>& robotPosition, std::vector<std::vector<Obstacle>>& boxPos, const std::pair<int, int>& dimensions);

constexpr char BOX_SYMBOL = 'O';
constexpr char ROBOT_SYMBOL = '@';
constexpr char WALL_SYMBOL = '#';
constexpr std::pair<int, int> NO_BOX = { -1, -1 };

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
    int col = outputLines[0].size();
    std::vector<std::vector<Obstacle>> pointDesc;
    int rowPointer = 0;
    std::pair<int, int> robotPos;
    while (true)
    {
        const std::string& s = outputLines[rowPointer];
        std::cout << s << std::endl;
        if (s == "")
        {
            break;
        }
        pointDesc.push_back({});
        for (size_t c = 0; c < col; ++c)
        {
            if (s[c] == BOX_SYMBOL)
            {
                pointDesc[rowPointer].push_back(BOX);
                std::cout << "Found box at: " << rowPointer << ", " << c << std::endl;
            }
            else if (s[c] == WALL_SYMBOL)
            {
                pointDesc[rowPointer].push_back(WALL);
                std::cout << "Found wall at: " << rowPointer << ", " << c << std::endl;
            }
            else
            {
                pointDesc[rowPointer].push_back(NONE);
            }

            if (s[c] == ROBOT_SYMBOL)
            {
                robotPos = { rowPointer, c };
            }
        }
        ++rowPointer;
    }
    std::pair<int, int> dimensions = { rowPointer, col };

    std::cout << "Rows: " << dimensions.first << ", Cols: " << dimensions.second << std::endl;

    ++rowPointer;
    std::vector<Direction> dirs;
    while (rowPointer < outputLines.size())
    {
        ParseInstructions(outputLines[rowPointer], dirs);
        ++rowPointer;
    }

    std::cout << std::endl;

    for (Direction dir : dirs)
    {
        std::cout << "Initial pos: " << robotPos.first << ", " << robotPos.second << std::endl;
        PerformInstruction(dir, robotPos, pointDesc, dimensions);
        std::cout << "Final pos: " << robotPos.first << ", " << robotPos.second << std::endl;
        std::cout << std::endl;
    }

    unsigned long GPSSum = 0;

    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            if (pointDesc[r][c] == NONE)
            {
                std::cout << ".";
            }
            else if (pointDesc[r][c] == WALL)
            {
                std::cout << "#";
            }
            else if (pointDesc[r][c] == BOX)
            {
                std::cout << "O";
                GPSSum += r * 100 + c;
            }
        }
        std::cout << std::endl;
    }

    return GPSSum;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    int col = outputLines[0].size();
    std::vector<std::vector<Obstacle>> pointDesc;
    int rowPointer = 0;
    std::pair<int, int> robotPos;
    while (true)
    {
        const std::string& s = outputLines[rowPointer];
        std::cout << s << std::endl;
        if (s == "")
        {
            break;
        }
        pointDesc.push_back({});
        for (size_t c = 0; c < col; ++c)
        {
            if (s[c] == BOX_SYMBOL)
            {
                pointDesc[rowPointer].push_back(LEFT_BOX);
                pointDesc[rowPointer].push_back(RIGHT_BOX);
                std::cout << "Found box at: " << rowPointer << ", " << c << std::endl;
            }
            else if (s[c] == WALL_SYMBOL)
            {
                pointDesc[rowPointer].push_back(WALL);
                pointDesc[rowPointer].push_back(WALL);
                std::cout << "Found wall at: " << rowPointer << ", " << c << std::endl;
            }
            else
            {
                pointDesc[rowPointer].push_back(NONE);
                pointDesc[rowPointer].push_back(NONE);
            }

            if (s[c] == ROBOT_SYMBOL)
            {
                robotPos = { rowPointer, c * 2};
            }
        }
        ++rowPointer;
    }
    std::pair<int, int> dimensions = { rowPointer, col * 2 };
    std::cout << "Rows: " << dimensions.first << ", Cols: " << dimensions.second << std::endl;

    PrintStatus(dimensions, pointDesc, robotPos);

    ++rowPointer;
    std::vector<Direction> dirs;
    while (rowPointer < outputLines.size())
    {
        ParseInstructions(outputLines[rowPointer], dirs);
        ++rowPointer;
    }

    std::cout << std::endl;

    for (Direction dir : dirs)
    {
        std::cout << "Initial pos: " << robotPos.first << ", " << robotPos.second << std::endl;
        PerformInstructionPart2(dir, robotPos, pointDesc, dimensions);
        std::cout << "Final pos: " << robotPos.first << ", " << robotPos.second << std::endl;
        //PrintStatus(dimensions, pointDesc, robotPos);
        std::cout << std::endl;
    }

    unsigned long GPSSum = 0;

    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            if (pointDesc[r][c] == LEFT_BOX)
            {
                GPSSum += r * 100 + c;
            }
        }
    }

    PrintStatus(dimensions, pointDesc, robotPos);

    return GPSSum;
}

void PrintStatus(const std::pair<int, int>& dimensions, const std::vector<std::vector<Obstacle>>& pointDesc, const std::pair<int, int>& robotPos)
{
    for (size_t r = 0; r < dimensions.first; ++r)
    {
        for (size_t c = 0; c < dimensions.second; ++c)
        {
            if (r == robotPos.first && c == robotPos.second)
            {
                std::cout << "@";
            }
            else if (pointDesc[r][c] == NONE)
            {
                std::cout << ".";
            }
            else if (pointDesc[r][c] == WALL)
            {
                std::cout << "#";
            }
            else if (pointDesc[r][c] == BOX)
            {
                std::cout << "O";
            }
            else if (pointDesc[r][c] == LEFT_BOX)
            {
                std::cout << "[";
            }
            else if (pointDesc[r][c] == RIGHT_BOX)
            {
                std::cout << "]";
            }
        }
        std::cout << std::endl;
    }
}

void ParseInstructions(const std::string& line, std::vector<Direction>& dirs)
{
    for (const char& c : line)
    {
        switch (c)
        {
        case '^':
            dirs.push_back(UP);
            break;
        case '>':
            dirs.push_back(RIGHT);
            break;
        case '<':
            dirs.push_back(LEFT);
            break;
        default:
            dirs.push_back(DOWN);
            break;
        }
    }
}

void PerformInstruction(Direction dir, std::pair<int, int>& robotPosition, std::vector<std::vector<Obstacle>>& boxPos, const std::pair<int, int>& dimensions)
{
    std::pair<int, int> nextPos = DoMove(robotPosition, dir);
    std::cout << "next pos: " << nextPos.first << ", " << nextPos.second << std::endl;
    if (IsOutOfBounds(nextPos, dimensions, boxPos))
        return;
    std::pair<int, int> finalBoxPos = GetFinalBoxPoint(nextPos, dir, boxPos, dimensions);
    std::cout << "final box pos: " << finalBoxPos.first << ", " << finalBoxPos.second << std::endl;
    if (finalBoxPos == NO_BOX)
    {
        robotPosition = nextPos;
        return;
    }
    else if (IsOutOfBounds(finalBoxPos, dimensions, boxPos))
        return;
    else
    {
        ShiftPoints(nextPos, finalBoxPos, boxPos);
        robotPosition = nextPos;
        return;
    }
}

void PerformInstructionPart2(Direction dir, std::pair<int, int>& robotPosition, std::vector<std::vector<Obstacle>>& boxPos, const std::pair<int, int>& dimensions)
{
    std::pair<int, int> nextPos = DoMove(robotPosition, dir);
    std::cout << "next pos: " << nextPos.first << ", " << nextPos.second << std::endl;
    if (IsOutOfBounds(nextPos, dimensions, boxPos))
        return;
    std::vector<std::pair<int, int>> shiftBoxPos;
    GetShiftedBoxPoints(nextPos, dir, boxPos, dimensions, shiftBoxPos);
    std::cout << "Hello!" << std::endl;
    if (shiftBoxPos.empty())
    {
        robotPosition = nextPos;
        return;
    }
    else if (!AllPointsInBound(shiftBoxPos, dir, dimensions, boxPos))
        return;
    else
    {
        std::cout << "Shift" << std::endl;
        ShiftPointsPart2(shiftBoxPos, boxPos, dir);
        robotPosition = nextPos;
        return;
    }
}

std::pair<int, int> DoMove(const std::pair<int, int>& coordinates, Direction dir)
{
    switch (dir)
    {
    case UP:
        return { coordinates.first - 1, coordinates.second };
    case DOWN:
        return { coordinates.first + 1, coordinates.second };
    case RIGHT:
        return { coordinates.first, coordinates.second + 1 };
    case LEFT:
        return { coordinates.first, coordinates.second - 1 };
    default:
        return { -1, -1 };
    }
}

/*
Returns -1, -1 if there is no box at all
*/
std::pair<int, int> GetFinalBoxPoint(const std::pair<int, int>& point, Direction dir, const std::vector<std::vector<Obstacle>>& boxPos, const std::pair<int, int>& dimensions)
{
    if (boxPos[point.first][point.second] != BOX)
    {
        return { -1, -1 };
    }

    std::pair<int, int> pointer = point;
    while (!IsOutOfBounds(pointer, dimensions, boxPos) && boxPos[pointer.first][pointer.second] == BOX)
    {
        pointer = DoMove(pointer, dir);
    }

    return pointer;
}

bool IsBoxPoint(const std::pair<int, int>& point, const std::vector<std::vector<Obstacle>>& boxPos)
{
    return boxPos[point.first][point.second] == LEFT_BOX || boxPos[point.first][point.second] == RIGHT_BOX;
}

void GetShiftedBoxPoints(const std::pair<int, int>& point, Direction dir, const std::vector<std::vector<Obstacle>>& boxPos, const std::pair<int, int>& dimensions, std::vector<std::pair<int, int>>& shiftedBoxPoints)
{
    std::vector<std::pair<int, int>> q;
    q.push_back(point);
    std::unordered_map<std::pair<int, int>, int> encounteredPoints;

    while (!q.empty())
    {
        std::pair<int, int> pointer = q[0];
        q.erase(q.begin());
        //std::cout << "Tried point: " << pointer.first << ", " << pointer.second << std::endl;
        //std::cout << boxPos[pointer.first][pointer.second] << std::endl;
        if (IsOutOfBounds(pointer, dimensions, boxPos) || !IsBoxPoint(pointer, boxPos))
        {
            continue;
        }
        if (encounteredPoints.find(pointer) != encounteredPoints.end())
            continue;

        shiftedBoxPoints.push_back(pointer);
        encounteredPoints[pointer] = 1;
        std::pair<int, int> otherPoint;
        if (boxPos[pointer.first][pointer.second] == LEFT_BOX)
        {
            otherPoint = { pointer.first, pointer.second + 1 };
        }
        else
        {
            otherPoint = { pointer.first, pointer.second - 1 };
        }
        if (encounteredPoints.find(otherPoint) == encounteredPoints.end())
        {
            shiftedBoxPoints.push_back(otherPoint);
            encounteredPoints[otherPoint] = 1;
        }
        std::cout << "Pointer: " << pointer.first << ", " << pointer.second << " and other: " << otherPoint.first << ", " << otherPoint.second << std::endl;
        q.push_back(DoMove(pointer, dir));
        q.push_back(DoMove(otherPoint, dir));
    }
}

bool AllPointsInBound(const std::vector<std::pair<int, int>>& shiftBoxPoints, Direction dir, const std::pair<int, int>& dimensions, const std::vector<std::vector<Obstacle>>& boxPos)
{
    for (const std::pair<int, int>& point : shiftBoxPoints)
    {
        std::pair<int, int> newPoint = DoMove(point, dir);
        if (IsOutOfBounds(newPoint, dimensions, boxPos))
            return false;
    }
    return true;
}

void ShiftPoints(const std::pair<int, int>& start, const std::pair<int, int>& end, std::vector<std::vector<Obstacle>>& boxPos)
{
    boxPos[start.first][start.second] = NONE;
    boxPos[end.first][end.second] = BOX;
}

void ShiftPointsPart2(std::vector<std::pair<int, int>>& shiftBoxPoints, std::vector<std::vector<Obstacle>>& boxPos, Direction dir)
{
    std::vector<Obstacle> obstacles;
    for (const std::pair<int, int>& p : shiftBoxPoints)
    {
        obstacles.push_back(boxPos[p.first][p.second]);
        boxPos[p.first][p.second] = NONE;
    }

    for (size_t i = 0; i < shiftBoxPoints.size(); ++i)
    {
        std::pair<int, int> newPoint = DoMove(shiftBoxPoints[i], dir);
        boxPos[newPoint.first][newPoint.second] = obstacles[i];
    }
}

bool IsOutOfBounds(const std::pair<int, int>& coordinates, const std::pair<int, int>& dimensions, const std::vector<std::vector<Obstacle>>& obstacles)
{
    return coordinates.first < 0 || coordinates.first >= dimensions.first || coordinates.second < 0 || coordinates.second >= dimensions.second || obstacles[coordinates.first][coordinates.second] == WALL;
}