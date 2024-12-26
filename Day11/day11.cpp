#include <iostream>
#include <vector>
#include <unordered_map>
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
int GetNumDigits(unsigned long number);
std::pair<unsigned long, unsigned long> Split(unsigned long number, int length);
void ParseLine(const std::string& line, std::vector<unsigned long>& v);
unsigned long DoRulesSingle(unsigned long number, int numIterations, std::unordered_map<std::pair<unsigned long, int>, unsigned long>& cache);

constexpr int MULT_AMOUNT = 2024;
constexpr int NUM_ITERATIONS_PART1 = 25;
constexpr int NUM_ITERATIONS_PART2 = 75;

namespace std
{
    template <>
    class hash<std::pair<unsigned long, int>>
    {
    public:
        std::uint64_t operator()(const std::pair<unsigned long, int>& x) const
        {
            size_t h = std::hash<unsigned long>()(x.first) ^ std::hash<int>()(x.second);
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
    std::vector<unsigned long> v;
    std::unordered_map<std::pair<unsigned long, int>, unsigned long> cache;
    ParseLine(outputLines[0], v);

    long long length = 0;

    for (unsigned long i : v)
    {
        std::cout << i << std::endl;
        length += DoRulesSingle(i, NUM_ITERATIONS_PART1, cache);
    }

    return length;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::vector<unsigned long> v;
    std::unordered_map<std::pair<unsigned long, int>, unsigned long> cache;
    ParseLine(outputLines[0], v);

    unsigned long length = 0;

    for (unsigned long i : v)
    {
        std::cout << i << std::endl;
        length += DoRulesSingle(i, NUM_ITERATIONS_PART2, cache);
    }

    return length;
}

void ParseLine(const std::string& line, std::vector<unsigned long>& v)
{
    int spacePointer = line.find(' ');
    std::string s = line;
    while (spacePointer != -1)
    {
        v.push_back(std::stoul(s.substr(0, spacePointer)));
        s = s.substr(spacePointer + 1);
        spacePointer = s.find(' ');
    }
    v.push_back(std::stoul(s));
}

unsigned long DoRulesSingle(unsigned long number, int numIterations, std::unordered_map<std::pair<unsigned long, int>, unsigned long>& cache)
{
    if (cache.find({number, numIterations}) != cache.end())
    {
        return cache[{number, numIterations}];
    }
    unsigned long rock = number;
    unsigned long numRocks = 1;
    for (int i = 0; i < numIterations; ++i)
    {
        int len = GetNumDigits(rock);
        if (rock == 0)
        {
            rock = 1;
        }
        else if (len % 2 == 0)
        {
            std::pair<unsigned long, unsigned long> rockPair = Split(rock, len);
            rock = rockPair.first;
            numRocks += DoRulesSingle(rockPair.second, numIterations - i - 1, cache);;
        }
        else
        {
            rock *= MULT_AMOUNT;
        }
    }
    cache[{number, numIterations}] = numRocks;
    return numRocks;
}

int GetNumDigits(unsigned long number)
{
    int digits = 0;
    while (number)
    {
        number /= 10;
        ++digits;
    }
    return digits;
}

std::pair<unsigned long, unsigned long> Split(unsigned long number, int length)
{
    int numDigits = length / 2;
    unsigned long num1 = 0;
    unsigned long num2 = 0;
    
    for (int i = 0; i < numDigits; ++i)
    {
        num2 += (number % 10) * std::pow(10, i);
        number /= 10;
    }

    for (int i = 0; i < numDigits; ++i)
    {
        num1 += (number % 10) * std::pow(10, i);;
        number /= 10;
    }   

    return { num1, num2 };
}
