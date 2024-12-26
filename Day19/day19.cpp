#include <iostream>
#include <vector>
#include <unordered_map>
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
unsigned long long Part2(const std::vector<std::string>& outputLines);
bool IsCombinationPossible(const std::string& s, int currIndex, const std::string& towelCombi);
bool ForceFind(const std::string& s, int currIndex, const std::vector<std::string>& towels, std::unordered_map<int, bool>& cache);
void ParseTowels(const std::string& s, std::vector<std::string>& towels);
unsigned long long ForceFindNumber(const std::string& s, int currIndex, const std::vector<std::string>& towels, std::unordered_map<int, unsigned long long>& cache);

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
    std::vector<std::string> towels;
    ParseTowels(outputLines[0], towels);

    unsigned long numPossible = 0;

    for (size_t i = 2; i < outputLines.size(); ++i)
    {
        std::cout << outputLines[i] << std::endl;
        std::unordered_map<int, bool> cache;
        if (ForceFind(outputLines[i], 0, towels, cache))
            ++numPossible;
    }

    return numPossible;
}

unsigned long long Part2(const std::vector<std::string>& outputLines)
{
    std::vector<std::string> towels;
    ParseTowels(outputLines[0], towels);

    unsigned long long numCombinations = 0;

    for (size_t i = 2; i < outputLines.size(); ++i)
    {
        std::cout << outputLines[i] << std::endl;
        std::unordered_map<int, unsigned long long> cache;
        numCombinations += ForceFindNumber(outputLines[i], 0, towels, cache);
        std::cout << numCombinations << std::endl;
    }

    return numCombinations;
}

void ParseTowels(const std::string& s, std::vector<std::string>& towels)
{
    int startIndex = 0;
    int commaIndex = s.find(',');
    while (commaIndex != -1)
    {
        towels.push_back(s.substr(startIndex, commaIndex - startIndex));
        startIndex = commaIndex + 2;
        commaIndex = s.find(',', startIndex);
    }
    towels.push_back(s.substr(startIndex));
}

bool ForceFind(const std::string& s, int currIndex, const std::vector<std::string>& towels, std::unordered_map<int, bool>& cache)
{
    if (currIndex >= s.size())
        return true;

    if (cache.find(currIndex) != cache.end())
        return cache[currIndex];

    for (const std::string& towel : towels)
    {
        // std::cout << s << ", " << currIndex << ", " << towel << std::endl;
        if (IsCombinationPossible(s, currIndex, towel))
        {
            if (currIndex + towel.size() == currIndex)
            {
                std::cout << "ERROR: " << s << ", " << currIndex << ", " << towel << std::endl;
            }
            if (ForceFind(s, currIndex + towel.size(), towels, cache))
            {
                cache[currIndex] = true;
                return true;
            }
        }
    }

    cache[currIndex] = false;
    return false;
}

unsigned long long ForceFindNumber(const std::string& s, int currIndex, const std::vector<std::string>& towels, std::unordered_map<int, unsigned long long>& cache)
{
    if (currIndex >= s.size())
        return 1;

    if (cache.find(currIndex) != cache.end())
        return cache[currIndex];

    unsigned long long numCombinations = 0;

    for (const std::string& towel : towels)
    {
        if (IsCombinationPossible(s, currIndex, towel))
        {
            numCombinations += ForceFindNumber(s, currIndex + towel.size(), towels, cache);
        }
    }

    cache[currIndex] = numCombinations;
    return numCombinations;
}

bool IsCombinationPossible(const std::string& s, int currIndex, const std::string& towelCombi)
{
    return s.find(towelCombi, currIndex) == currIndex;
}