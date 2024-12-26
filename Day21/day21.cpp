#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>
#include "../fileRead.h"

enum Direction : char
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

constexpr std::pair<int, int> DIGIT_KEYPAD_STARTPOS = {3, 2};
constexpr std::pair<int, int> DIGIT_KEYPAD_DIMENSIONS = { 4, 3 };
constexpr std::pair<int, int> DIGIT_KEYPAD_FORBIDDEN = { 3, 0 };
constexpr std::pair<int, int> DIGIT_KEYPAD_ACTIVATE = { 3, 2 };

constexpr std::pair<int, int> DIRECTIONAL_KEYPAD_STARTPOS = { 0, 2 };
constexpr std::pair<int, int> DIRECTIONAL_KEYPAD_DIMENSIONS = { 2, 3 };
constexpr std::pair<int, int> DIRECTIONAL_KEYPAD_FORBIDDEN = { 0, 0 };
constexpr std::pair<int, int> DIRECTIONAL_KEYPAD_ACTIVATE = { 0, 2 };
constexpr int NUM_DIRECTIONAL_BUTTONS = 5;

struct PosRepresentation
{
    std::pair<int, int> TopLevelRobotPos;
    std::pair<int, int> BottomLevelKeypadPos;

    unsigned long long NumMoves;
    bool IsPress;

    PosRepresentation(std::pair<int, int> topLevelKeypadPos, std::pair<int, int> bottomLevelKeypadPos) : TopLevelRobotPos(topLevelKeypadPos), BottomLevelKeypadPos(bottomLevelKeypadPos), NumMoves(0), IsPress(false) {}

    PosRepresentation(std::pair<int, int> topLevelKeypadPos, std::pair<int, int> bottomLevelKeypadPos, unsigned long long numMoves, bool isPress) : TopLevelRobotPos(topLevelKeypadPos), BottomLevelKeypadPos(bottomLevelKeypadPos), NumMoves(numMoves), IsPress(isPress) {}

    bool operator==(const PosRepresentation& other) const {
        return BottomLevelKeypadPos == other.BottomLevelKeypadPos && TopLevelRobotPos == other.TopLevelRobotPos;
    }
};

struct PosPair
{
    std::pair<int, int> Pair1;
    std::pair<int, int> Pair2;

    PosPair(std::pair<int, int> pair1, std::pair<int, int> pair2) : Pair1(pair1), Pair2(pair2) {}
    PosPair(int pair1Row, int pair1Col, int pair2Row, int pair2Col) : Pair1(pair1Row, pair1Col), Pair2(pair2Row, pair2Col) {}

    bool operator==(const PosPair& other) const {
        return Pair1 == other.Pair1 && Pair2 == other.Pair2;
    }
};


namespace std
{
    template <>
    class hash<PosPair>
    {
    public:
        std::uint64_t operator()(const PosPair& x) const
        {
            size_t h = std::hash<int>()(x.Pair1.first) ^ std::hash<int>()(x.Pair1.second) ^ std::hash<int>()(x.Pair2.first) ^ std::hash<int>()(x.Pair2.second);
            return h;
        }
    };

    template <>
    class hash<PosRepresentation>
    {
    public:
        std::uint64_t operator()(const PosRepresentation& x) const
        {
            size_t h = std::hash<int>()(x.TopLevelRobotPos.first) ^ std::hash<int>()(x.TopLevelRobotPos.second) ^ std::hash<int>()(x.BottomLevelKeypadPos.first) ^ std::hash<int>()(x.BottomLevelKeypadPos.second);
            return h;
        }
    };
}

// referring to the one controlled by the robot directly controlled by us
static std::unordered_map<PosPair, unsigned long long> TopLevelDirectionalKeypadCosts;
// referring to the one controlled by the robot controlled by another robot
//static std::unordered_map<PosPair, unsigned long> BottomLevelDirectionalKeypadCosts;

constexpr int NUM_MID_DIRECTIONAL_ROBOTS = 25 - 1;

struct ShortestKeypadCosts
{
    std::unordered_map<PosPair, unsigned long long> Costs[NUM_MID_DIRECTIONAL_ROBOTS];

    std::unordered_map<PosPair, unsigned long long>& GetMap(int level)
    {
        return Costs[level];
    }

    std::unordered_map<PosPair, unsigned long long>& GetTopLevelMap(int level)
    {
        if (level == 0)
            return TopLevelDirectionalKeypadCosts;
        else
            return Costs[level - 1];
    }
};

static ShortestKeypadCosts CostsCache;

long long Part1(const std::vector<std::string>& outputLines);
unsigned long long FindShortestOverallMoves(const std::string& targetDigit);
void FindTopLevelDirectionalKeypadShortestPaths();
void FindMidDirectionalKeypadShortestPaths(int level);
void Setup();
bool IsDirectionalKeypadOutOfBounds(const std::pair<int, int>& coordinates);
bool IsDigitKeypadOutOfBounds(const std::pair<int, int>& coordinates);
std::pair<int, int> Move(const std::pair<int, int>& initialCoordinates, Direction dir);
std::pair<int, int> GetTargetDigitPosition(char target);
std::pair<int, int> GetTargetDirectionalPosition(Direction dir);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }

    std::vector<std::string> outputLines;
    ReadFromFile(argv[1], outputLines);

    std::cout << Part1(outputLines) << std::endl;
}

long long Part1(const std::vector<std::string>& outputLines)
{
    Setup();

    for (size_t r = 0; r < DIRECTIONAL_KEYPAD_DIMENSIONS.first; ++r)
    {
        for (size_t c = 0; c < DIRECTIONAL_KEYPAD_DIMENSIONS.second; ++c)
        {
            for (size_t i = 0; i < DIRECTIONAL_KEYPAD_DIMENSIONS.first; ++i)
            {
                for (size_t j = 0; j < DIRECTIONAL_KEYPAD_DIMENSIONS.second; ++j)
                {
                    std::cout << "[" << r << ", " << c << "]" << " [" << i << ", " << j << "] : " << TopLevelDirectionalKeypadCosts[PosPair{{r, c}, {i, j}}] << std::endl;
                }
            }
        }
    }

    std::cout << std::endl << std::endl << std::endl;

    for (size_t robot = 0; robot < NUM_MID_DIRECTIONAL_ROBOTS; ++robot)
    {
        for (size_t r = 0; r < DIRECTIONAL_KEYPAD_DIMENSIONS.first; ++r)
        {
            for (size_t c = 0; c < DIRECTIONAL_KEYPAD_DIMENSIONS.second; ++c)
            {
                for (size_t i = 0; i < DIRECTIONAL_KEYPAD_DIMENSIONS.first; ++i)
                {
                    for (size_t j = 0; j < DIRECTIONAL_KEYPAD_DIMENSIONS.second; ++j)
                    {
                        std::cout << "[" << r << ", " << c << "]" << " [" << i << ", " << j << "] : " << CostsCache.GetMap(robot)[PosPair{ {r, c}, {i, j} }] << std::endl;
                    }
                }
            }
        }
    }
    
    
    long long totalSum = 0;
    for (const std::string& s : outputLines)
        totalSum += FindShortestOverallMoves(s) * std::stoi(s.substr(0, s.size() - 1));
    return totalSum;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    return 0;
}

unsigned long long FindShortestOverallMoves(const std::string& targetDigit)
{
    int digitIndex = 0;
    std::pair<int, int> startPos = DIGIT_KEYPAD_STARTPOS;
    unsigned long long totalMoves = 0;

    std::unordered_map<PosPair, unsigned long long>& robotCosts = CostsCache.GetMap(NUM_MID_DIRECTIONAL_ROBOTS - 1);

    while (digitIndex < targetDigit.size())
    {
        auto cmp = [](const PosRepresentation& x, const PosRepresentation& y) { return x.NumMoves > y.NumMoves; };
        std::priority_queue<PosRepresentation, std::vector<PosRepresentation>, decltype(cmp)> pq(cmp);
        std::unordered_map<PosRepresentation, int> cache;

        pq.emplace(DIRECTIONAL_KEYPAD_ACTIVATE, startPos);

        while (!pq.empty())
        {
            PosRepresentation pos = pq.top();
            pq.pop();

            if (IsDigitKeypadOutOfBounds(pos.BottomLevelKeypadPos))
                continue;

            if (pos.IsPress)
            {
                totalMoves += pos.NumMoves;
                startPos = pos.BottomLevelKeypadPos;
                break;
            }

            if (cache.find(pos) != cache.end())
                continue;
            
            cache[pos] = 1;

            if (GetTargetDigitPosition(targetDigit[digitIndex]) == pos.BottomLevelKeypadPos)
                pq.emplace(DIRECTIONAL_KEYPAD_ACTIVATE, pos.BottomLevelKeypadPos, pos.NumMoves + robotCosts[PosPair{pos.TopLevelRobotPos, DIRECTIONAL_KEYPAD_ACTIVATE}], true);
            
            pq.emplace(GetTargetDirectionalPosition(UP), Move(pos.BottomLevelKeypadPos, UP), pos.NumMoves + robotCosts[PosPair{pos.TopLevelRobotPos, GetTargetDirectionalPosition(UP)}], false);
            pq.emplace(GetTargetDirectionalPosition(DOWN), Move(pos.BottomLevelKeypadPos, DOWN), pos.NumMoves + robotCosts[PosPair{pos.TopLevelRobotPos, GetTargetDirectionalPosition(DOWN)}], false);
            pq.emplace(GetTargetDirectionalPosition(LEFT), Move(pos.BottomLevelKeypadPos, LEFT), pos.NumMoves + robotCosts[PosPair{pos.TopLevelRobotPos, GetTargetDirectionalPosition(LEFT)}], false);
            pq.emplace(GetTargetDirectionalPosition(RIGHT), Move(pos.BottomLevelKeypadPos, RIGHT), pos.NumMoves + robotCosts[PosPair{pos.TopLevelRobotPos, GetTargetDirectionalPosition(RIGHT)}], false);
        }

        ++digitIndex;
    }

    return totalMoves;
}

/*
Note: This counts the single button press on 'A' as well
For this top level, since we have free range to press any button, the shortest distance is just the manhatten distance
*/
void FindTopLevelDirectionalKeypadShortestPaths()
{
    for (size_t r = 0; r < DIRECTIONAL_KEYPAD_DIMENSIONS.first; ++r)
    {
        for (size_t c = 0; c < DIRECTIONAL_KEYPAD_DIMENSIONS.second; ++c)
        {
            for (size_t i = 0; i < DIRECTIONAL_KEYPAD_DIMENSIONS.first; ++i)
            {
                for (size_t j = 0; j < DIRECTIONAL_KEYPAD_DIMENSIONS.second; ++j)
                {
                    TopLevelDirectionalKeypadCosts[PosPair{{r, c}, { i, j }}] = std::abs((int) (i - r)) + std::abs((int) (j - c)) + 1;
                }
            }
        }
    }
}

/*
Note: This takes into account additional button presses required for this robot to actually press the button it arrives at
This will use dijkstra starting from every tile to find the smallest number of moves to every other tile, making the assumption that
the top level keypad starts at the 'A' tile every single time
It will therefore need to keep track of both the first robot and second robot positions
*/
void FindMidDirectionalKeypadShortestPaths(int level) {
    std::ofstream File{ "MyFile.out" };
    std::unordered_map<PosPair, unsigned long long>& TopLevelKeypadCosts = CostsCache.GetTopLevelMap(level);
    std::unordered_map<PosPair, unsigned long long>& CurrLevelKeypadCosts = CostsCache.GetMap(level);
    for (size_t r = 0; r < DIRECTIONAL_KEYPAD_DIMENSIONS.first; ++r)
    {
        for (size_t c = 0; c < DIRECTIONAL_KEYPAD_DIMENSIONS.second; ++c)
        {
            File << "Iteration " << r << ", " << c << std::endl;
            auto cmp = [](const PosRepresentation& x, const PosRepresentation& y) { return x.NumMoves > y.NumMoves; };
            std::priority_queue<PosRepresentation, std::vector<PosRepresentation>, decltype(cmp)> pq(cmp);
            std::unordered_map<PosRepresentation, int> cache;

            pq.emplace(DIRECTIONAL_KEYPAD_STARTPOS, std::pair<int, int>{ r, c });

            int numPressesFound = 0;
            while (!pq.empty())
            {
                PosRepresentation pos = pq.top();
                pq.pop();

                if (IsDirectionalKeypadOutOfBounds(pos.BottomLevelKeypadPos))
                    continue;

                if (pos.IsPress)
                {
                    if (CurrLevelKeypadCosts.find(PosPair{ {r, c}, pos.BottomLevelKeypadPos }) != CurrLevelKeypadCosts.end())
                        continue;
                    else
                    {
                        File << "Found press for: " << pos.BottomLevelKeypadPos.first << ", " << pos.BottomLevelKeypadPos.second << " with moves " << pos.NumMoves << std::endl;
                        CurrLevelKeypadCosts[PosPair{ {r, c}, pos.BottomLevelKeypadPos }] = pos.NumMoves;
                        ++numPressesFound;
                        if (numPressesFound == NUM_DIRECTIONAL_BUTTONS)
                            break;
                    }
                        
                }

                if (cache.find(pos) != cache.end())
                    continue;

                
                cache[pos] = 1;

                pq.emplace(DIRECTIONAL_KEYPAD_ACTIVATE, pos.BottomLevelKeypadPos, pos.NumMoves + TopLevelKeypadCosts[PosPair{pos.TopLevelRobotPos, DIRECTIONAL_KEYPAD_ACTIVATE}], true);
                pq.emplace(GetTargetDirectionalPosition(UP), Move(pos.BottomLevelKeypadPos, UP), pos.NumMoves + TopLevelKeypadCosts[PosPair{pos.TopLevelRobotPos, GetTargetDirectionalPosition(UP)}], false);
                pq.emplace(GetTargetDirectionalPosition(DOWN), Move(pos.BottomLevelKeypadPos, DOWN), pos.NumMoves + TopLevelKeypadCosts[PosPair{pos.TopLevelRobotPos, GetTargetDirectionalPosition(DOWN)}], false);
                pq.emplace(GetTargetDirectionalPosition(LEFT), Move(pos.BottomLevelKeypadPos, LEFT), pos.NumMoves + TopLevelKeypadCosts[PosPair{pos.TopLevelRobotPos, GetTargetDirectionalPosition(LEFT)}], false);
                pq.emplace(GetTargetDirectionalPosition(RIGHT), Move(pos.BottomLevelKeypadPos, RIGHT), pos.NumMoves + TopLevelKeypadCosts[PosPair{pos.TopLevelRobotPos, GetTargetDirectionalPosition(RIGHT)}], false);
            }

            File << std::endl;
        }
    }

    File.close();
}

void Setup()
{
    FindTopLevelDirectionalKeypadShortestPaths();
    for (size_t i = 0; i < NUM_MID_DIRECTIONAL_ROBOTS; ++i)
    {
        FindMidDirectionalKeypadShortestPaths(i);
        std::cout << "Setup " << i << " done" << std::endl;
    }
        
}

bool IsDirectionalKeypadOutOfBounds(const std::pair<int, int>& coordinates)
{
    return coordinates.first < 0 || coordinates.first >= DIRECTIONAL_KEYPAD_DIMENSIONS.first || coordinates.second < 0 || coordinates.second >= DIRECTIONAL_KEYPAD_DIMENSIONS.second || (coordinates.first == DIRECTIONAL_KEYPAD_FORBIDDEN.first && coordinates.second == DIRECTIONAL_KEYPAD_FORBIDDEN.second);
}

bool IsDigitKeypadOutOfBounds(const std::pair<int, int>& coordinates)
{
    return coordinates.first < 0 || coordinates.first >= DIGIT_KEYPAD_DIMENSIONS.first || coordinates.second < 0 || coordinates.second >= DIGIT_KEYPAD_DIMENSIONS.second || (coordinates.first == DIGIT_KEYPAD_FORBIDDEN.first && coordinates.second == DIGIT_KEYPAD_FORBIDDEN.second);
}

std::pair<int, int> Move(const std::pair<int, int>& initialCoordinates, Direction dir)
{
    switch (dir)
    {
    case UP:
        return { initialCoordinates.first - 1, initialCoordinates.second };
    case DOWN:
        return { initialCoordinates.first + 1, initialCoordinates.second };
    case LEFT:
        return { initialCoordinates.first, initialCoordinates.second - 1 };
    case RIGHT:
        return { initialCoordinates.first, initialCoordinates.second + 1 };
    default:
        return { -1, -1 };
    }
}

std::pair<int, int> GetTargetDigitPosition(char target)
{
    switch (target)
    {
    case '0':
        return { 3, 1 };
    case '1':
        return { 2, 0 };
    case '2':
        return { 2, 1 };
    case '3':
        return { 2, 2 };
    case '4':
        return { 1, 0 };
    case '5':
        return { 1, 1 };
    case '6':
        return { 1, 2 };
    case '7':
        return { 0, 0 };
    case '8':
        return { 0, 1 };
    case '9':
        return { 0, 2 };
    default:
        return DIGIT_KEYPAD_ACTIVATE;
    }
}

std::pair<int, int> GetTargetDirectionalPosition(Direction dir)
{
    switch (dir)
    {
    case UP:
        return { 0, 1 };
    case DOWN:
        return { 1, 1 };
    case LEFT:
        return { 1, 0 };
    case RIGHT:
        return { 1, 2 };
    default:
        return DIRECTIONAL_KEYPAD_ACTIVATE;
    }
}