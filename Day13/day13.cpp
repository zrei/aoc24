#include <iostream>
#include <vector>
#include <optional>
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
unsigned long long Part2(const std::vector<std::string>& outputLines);
std::pair<unsigned long long, unsigned long long> ParseLine(const std::string& s, unsigned long long toAdd = 0);
void ConstructDP(std::vector<std::vector<std::pair<unsigned long long, unsigned long long>>>& dp);
long long DP(const std::pair<unsigned long long, unsigned long long>& goal, const std::pair<unsigned long long, unsigned long long>& A, const std::pair<unsigned long long, unsigned long long>& B, std::vector < std::vector<std::pair<unsigned long long, unsigned long long>>>& dp);
unsigned long long Solve(const std::pair<unsigned long long, unsigned long long>& clawA, const std::pair<unsigned long long, unsigned long long>& clawB, const std::pair<unsigned long long, unsigned long long>& goal);

constexpr int MAX_PRESS_AMOUNT = 100;
constexpr unsigned long long BIG_NUMBER = 10000000000000;
constexpr int CLAWA_TOKENS = 3;
constexpr int CLAWB_TOKENS = 1;

namespace std
{
    template <>
    class hash<std::pair<unsigned long long, unsigned long long>>
    {
    public:
        std::uint64_t operator()(const std::pair<unsigned long long, unsigned long long>& x) const
        {
            size_t h = std::hash<unsigned long long>()(x.first) ^ std::hash<unsigned long long>()(x.second);
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
    long long tokenSum = 0;
    for (size_t pointer = 0; pointer < outputLines.size(); pointer += 4)
    {
        std::cout << outputLines[pointer] << std::endl;
        std::pair<unsigned long long, unsigned long long> clawA = ParseLine(outputLines[pointer]);
        std::pair<unsigned long long, unsigned long long> clawB = ParseLine(outputLines[pointer + 1]);
        std::pair<unsigned long long, unsigned long long> goal = ParseLine(outputLines[pointer + 2]);
        std::vector<std::vector<std::pair<unsigned long long, unsigned long long>>> dp;
        ConstructDP(dp);
        tokenSum += DP(goal, clawA, clawB, dp);
    }
    
    return tokenSum;
}

unsigned long long Part2(const std::vector<std::string>& outputLines)
{
    unsigned long long tokenSum = 0;

    for (size_t pointer = 0; pointer < outputLines.size(); pointer += 4)
    {
        std::cout << outputLines[pointer] << std::endl;
        std::pair<unsigned long long, unsigned long long> clawA = ParseLine(outputLines[pointer]);
        std::pair<unsigned long long, unsigned long long> clawB = ParseLine(outputLines[pointer + 1]);
        std::pair<unsigned long long, unsigned long long> goal = ParseLine(outputLines[pointer + 2], BIG_NUMBER);
        
        std::cout << goal.first << ", " << goal.second << std::endl;

        tokenSum += Solve(clawA, clawB, goal);       
    }

    return tokenSum;
}

std::pair<unsigned long long, unsigned long long> ParseLine(const std::string& s, unsigned long long toAdd)
{
    std::string v = s;
    int xIndex = v.find('X') + 2;
    int commaIndex = v.find(',');
    unsigned long long x = std::stoi(v.substr(xIndex, commaIndex - xIndex)) + toAdd;
    v = v.substr(commaIndex + 1);
    int yIndex = v.find('Y') + 2;
    unsigned long long y = std::stoi(v.substr(yIndex)) + toAdd;
    return { x, y };
}

void ConstructDP(std::vector<std::vector<std::pair<unsigned long long, unsigned long long>>>& dp)
{
    for (size_t i = 0; i < MAX_PRESS_AMOUNT + 1; ++i)
    {
        dp.push_back({});
        for (size_t j = 0; j < MAX_PRESS_AMOUNT + 1; ++j)
        {
            dp[i].push_back({ 0, 0 });
        }
    }
}

long long DP(const std::pair<unsigned long long, unsigned long long>& goal, const std::pair<unsigned long long, unsigned long long>& A, const std::pair<unsigned long long, unsigned long long>& B, std::vector < std::vector<std::pair<unsigned long long, unsigned long long>>>& dp)
{
    std::vector<std::pair<unsigned long long, unsigned long long>> possibleSolutions;

    for (size_t i = 1; i < MAX_PRESS_AMOUNT + 1; ++i)
    {
        std::pair<unsigned long long, unsigned long long> initial = dp[0][i - 1];
        dp[0][i] = { initial.first + B.first, initial.second + B.second };
        if (dp[0][i] == goal)
        {
            possibleSolutions.push_back({ 0, i });
        }
    }

    for (size_t i = 1; i < MAX_PRESS_AMOUNT + 1; ++i)
    {
        std::pair<unsigned long long, unsigned long long> initial = dp[i - 1][0];
        dp[i][0] = { initial.first + A.first, initial.second + A.second };
        if (dp[i][0] == goal)
        {
            possibleSolutions.push_back({ i, 0 });
        }
    }

    for (size_t i = 1; i < MAX_PRESS_AMOUNT + 1; ++i)
    {
        for (size_t j = 1; j < MAX_PRESS_AMOUNT + 1; ++j)
        {
            std::pair<unsigned long long, unsigned long long> initial = dp[i - 1][j];
            dp[i][j] = { initial.first + A.first, initial.second + A.second };
            if (dp[i][j] == goal)
            {
                possibleSolutions.push_back({ i, j });
            }
        }
    }

    unsigned long long minTokens = 0;
    for (const std::pair<unsigned long long, unsigned long long>& p : possibleSolutions)
    {
        if (minTokens == 0)
        {
            minTokens = 3 * p.first + p.second;
        }
        else
        {
            minTokens = std::min(minTokens, 3 * p.first + p.second);
        }
    }

    return minTokens;
}

unsigned long long Solve(const std::pair<unsigned long long, unsigned long long>& clawA, const std::pair<unsigned long long, unsigned long long>& clawB, const std::pair<unsigned long long, unsigned long long>& goal)
{
    long long i = clawB.second * goal.first - clawB.first * goal.second;
    long long j = -clawA.second * goal.first + clawA.first * goal.second;
    long long det = (clawA.first * clawB.second) - (clawA.second * clawB.first);

    if (det == 0 || i % det != 0 || j % det != 0)
        return 0;
    else
        return CLAWA_TOKENS * i / det + CLAWB_TOKENS * j / det;
}