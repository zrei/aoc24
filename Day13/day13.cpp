#include <iostream>
#include <vector>
#include "z3++.h"
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
std::pair<unsigned long, unsigned long> ParseLine(const std::string& s);
void ConstructDP(std::vector<std::vector<std::pair<unsigned long, unsigned long>>>& dp);
long long DP(const std::pair<unsigned long, unsigned long>& goal, const std::pair<unsigned long, unsigned long>& A, const std::pair<unsigned long, unsigned long>& B, std::vector < std::vector<std::pair<unsigned long, unsigned long>>>& dp);
long long Recurse(std::pair<unsigned long, unsigned long> goal, const std::pair<unsigned long, unsigned long>& clawA, const std::pair<unsigned long, unsigned long>& clawB);

constexpr int MAX_PRESS_AMOUNT = 100;
constexpr unsigned long BIG_NUMBER = 10000000000000;
constexpr int CLAWA_TOKENS = 3;
constexpr int CLAWB_TOKENS = 1;

namespace std
{
    template <>
    class hash<std::pair<unsigned long, unsigned long>>
    {
    public:
        std::uint64_t operator()(const std::pair<unsigned long, unsigned long>& x) const
        {
            size_t h = std::hash<unsigned long>()(x.first) ^ std::hash<unsigned long>()(x.second);
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
        std::pair<unsigned long, unsigned long> clawA = ParseLine(outputLines[pointer]);
        std::pair<unsigned long, unsigned long> clawB = ParseLine(outputLines[pointer + 1]);
        std::pair<unsigned long, unsigned long> goal = ParseLine(outputLines[pointer + 2]);
        std::vector<std::vector<std::pair<unsigned long, unsigned long>>> dp;
        ConstructDP(dp);
        tokenSum += DP(goal, clawA, clawB, dp);
    }
    
    return tokenSum;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    long long tokenSum = 0;
    for (size_t pointer = 0; pointer < outputLines.size(); pointer += 4)
    {
        std::cout << outputLines[pointer] << std::endl;
        std::pair<unsigned long, unsigned long> clawA = ParseLine(outputLines[pointer]);
        std::pair<unsigned long, unsigned long> clawB = ParseLine(outputLines[pointer + 1]);
        std::pair<unsigned long, unsigned long> goal = ParseLine(outputLines[pointer + 2]);
        std::cout << goal.first << ", " << goal.second << std::endl;
        Recurse(goal, clawA, clawB);
    }

    return tokenSum;
}

std::pair<unsigned long, unsigned long> ParseLine(const std::string& s)
{
    std::string v = s;
    int xIndex = v.find('X') + 2;
    int commaIndex = v.find(',');
    unsigned long x = std::stoi(v.substr(xIndex, commaIndex - xIndex));
    v = v.substr(commaIndex + 1);
    int yIndex = v.find('Y') + 2;
    unsigned long y = std::stoi(v.substr(yIndex));
    return { x, y };
}

void ConstructDP(std::vector<std::vector<std::pair<unsigned long, unsigned long>>>& dp)
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

long long DP(const std::pair<unsigned long, unsigned long>& goal, const std::pair<unsigned long, unsigned long>& A, const std::pair<unsigned long, unsigned long>& B, std::vector < std::vector<std::pair<unsigned long, unsigned long>>>& dp)
{
    std::vector<std::pair<unsigned long, unsigned long>> possibleSolutions;

    for (size_t i = 1; i < MAX_PRESS_AMOUNT + 1; ++i)
    {
        std::pair<unsigned long, unsigned long> initial = dp[0][i - 1];
        dp[0][i] = { initial.first + B.first, initial.second + B.second };
        if (dp[0][i] == goal)
        {
            possibleSolutions.push_back({ 0, i });
        }
    }

    for (size_t i = 1; i < MAX_PRESS_AMOUNT + 1; ++i)
    {
        std::pair<unsigned long, unsigned long> initial = dp[i - 1][0];
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
            std::pair<unsigned long, unsigned long> initial = dp[i - 1][j];
            dp[i][j] = { initial.first + A.first, initial.second + A.second };
            if (dp[i][j] == goal)
            {
                possibleSolutions.push_back({ i, j });
            }
        }
    }

    unsigned long minTokens = 0;
    for (const std::pair<unsigned long, unsigned long>& p : possibleSolutions)
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

/*
ahhh
*/
long long Recurse(std::pair<unsigned long, unsigned long> goal, const std::pair<unsigned long, unsigned long>& clawA, const std::pair<unsigned long, unsigned long>& clawB)
{
    z3::context c;
    z3::expr a = c.int_const("a");
    z3::expr b = c.int_const("b");
    z3::solver s(c);

    s.add(a * clawA.first + b * clawB.first == goal.first + BIG_NUMBER);
    s.add(a * clawA.second + b * clawB.second == goal.second + BIG_NUMBER);
    
    std::cout << s.check() << std::endl;
    std::cout << s.get_model() << std::endl;

    return 0;
}