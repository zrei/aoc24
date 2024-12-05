#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "../fileRead.h"

long long Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
void BuildAdjacencyList(const std::vector<std::string>& outputLines, std::unordered_map<int, std::vector<int>>& adjacencyList, int& listStart);
std::pair<int, int> GetOrder(const std::string& line);
void BuildPrinterList(const std::string& s, std::vector<int>& printerList);
int GetMiddleInstruction(const std::vector<int>& printList);
bool CanAppearAfter(int secondElem, int firstElem, std::unordered_map<int, std::vector<int>>& adjacencyList);
bool CanAppearAfter(int secondElem, int firstElem, std::unordered_map<int, std::vector<int>>& adjacencyList);
bool CannotAppearAfter(int secondElem, int firstElem, std::unordered_map<int, std::vector<int>>& adjacencyList);
int GetNewMiddle(const std::vector<int>& numbers, std::unordered_map<int, std::vector<int>>& adjacencyList);
void GetSortedIndexVector(std::vector<std::pair<int, int>>& v, const std::vector<int>& numbers, std::unordered_map<int, std::vector<int>>& adjacencyList);
int GetNumDependencies(int initialIndex, const std::vector<int>& numbers, std::unordered_map<int, std::vector<int>>& adjacencyList);

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
    long long totalNum = 0;

    int listStart = 0;
    std::unordered_map<int, std::vector<int>> adjacencyList;
    BuildAdjacencyList(outputLines, adjacencyList, listStart);

    for (size_t l = listStart; l < outputLines.size(); ++l)
    {
        std::vector<int> printerList;
        BuildPrinterList(outputLines[l], printerList);
        bool isValid = true;
        std::cout << "Testing line: " << l << std::endl;
        for (size_t i = 0; i < printerList.size(); ++i)
        {
            for (size_t j = i + 1; j < printerList.size(); ++j)
            {
                // directly check the available rules and whether they're broken
                if (!CanAppearAfter(printerList[j], printerList[i], adjacencyList) || CannotAppearAfter(printerList[j], printerList[i], adjacencyList))
                {
                    isValid = false;
                    break;
                }
            }
            if (!isValid)
                break;
        }
        if (isValid)
        {
            std::cout << "Valid line: " << l << std::endl;
            totalNum += GetMiddleInstruction(printerList);
        }
            
    }
    return totalNum;
}

long long Part2(const std::vector<std::string>& outputLines)
{
    long long totalNum = 0;

    int listStart = 0;
    std::unordered_map<int, std::vector<int>> adjacencyList;
    BuildAdjacencyList(outputLines, adjacencyList, listStart);

    for (size_t l = listStart; l < outputLines.size(); ++l)
    {
        std::vector<int> printerList;
        BuildPrinterList(outputLines[l], printerList);
        bool isValid = true;
        std::cout << "Testing line: " << l << std::endl;
        for (size_t i = 0; i < printerList.size(); ++i)
        {
            for (size_t j = i + 1; j < printerList.size(); ++j)
            {
                if (!CanAppearAfter(printerList[j], printerList[i], adjacencyList) || CannotAppearAfter(printerList[j], printerList[i], adjacencyList))
                {
                    isValid = false;
                    break;
                }
            }
            if (!isValid)
                break;
        }
        if (!isValid)
        {
            std::cout << "Invalid line: " << l << std::endl;
            totalNum += GetNewMiddle(printerList, adjacencyList);
        }

    }
    return totalNum;
}

void BuildAdjacencyList(const std::vector<std::string>& outputLines, std::unordered_map<int, std::vector<int>>& adjacencyList, int& listStart)
{
    for (size_t i = 0; i < outputLines.size(); ++i)
    {
        if (outputLines[i] == "")
        {
            listStart = i + 1;
            return;
        }
        auto [start, end] = GetOrder(outputLines[i]);

        if (adjacencyList.find(start) == adjacencyList.end())
        {
            adjacencyList[start] = {};
            adjacencyList[start].push_back(end);
        }
        else
        {
            adjacencyList[start].push_back(end);
        }
    }
}

void BuildPrinterList(const std::string& line, std::vector<int>& printerList)
{
    std::string s = line;
    int commaIndex = s.find(',');
    while (commaIndex != -1)
    {
        printerList.push_back(std::stoi(s.substr(0, commaIndex)));
        s = s.substr(commaIndex + 1);
        commaIndex = s.find(',');
    }
    printerList.push_back(std::stoi(s));
}

int GetMiddleInstruction(const std::vector<int>& printList)
{
    return printList[printList.size() / 2];
}

std::pair<int, int> GetOrder(const std::string& line)
{
    int dividerIndex = line.find('|');
    return { std::stoi(line.substr(0, dividerIndex)), std::stoi(line.substr(dividerIndex + 1)) };
}

bool CanAppearAfter(int secondElem, int firstElem, std::unordered_map<int, std::vector<int>>& adjacencyList)
{
    if (adjacencyList.find(firstElem) == adjacencyList.end())
        return true;

    for (int elem : adjacencyList[firstElem])
    {
        if (elem == secondElem)
            return true;
    }

    return false;
}

bool CannotAppearAfter(int secondElem, int firstElem, std::unordered_map<int, std::vector<int>>& adjacencyList)
{
    if (adjacencyList.find(secondElem) == adjacencyList.end())
        return false;

    for (int elem : adjacencyList[secondElem])
    {
        if (elem == firstElem)
            return true;
    }

    return false;
}

int GetNewMiddle(const std::vector<int>& numbers, std::unordered_map<int, std::vector<int>>& adjacencyList)
{
    std::vector<std::pair<int, int>> v;
    GetSortedIndexVector(v, numbers, adjacencyList);
    std::vector<int> newNumbers;

    // sort the numbers by their dependencies - the largest number of dependencies should show up first
    for (size_t i = 0; i < v.size(); ++i)
    {
        newNumbers.push_back(numbers[v[i].first]);
    }
    return GetMiddleInstruction(newNumbers);
}

// initial index, dependencies
void GetSortedIndexVector(std::vector<std::pair<int, int>>& v, const std::vector<int>& numbers, std::unordered_map<int, std::vector<int>>& adjacencyList)
{
    for (size_t i = 0; i < numbers.size(); ++i)
    {
        v.emplace_back(i, GetNumDependencies(i, numbers, adjacencyList));
    }

    std::sort(v.begin(), v.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;
    });
}

// for each element of the vector, find the number of elements it should appear before
int GetNumDependencies(int initialIndex, const std::vector<int>& numbers, std::unordered_map<int, std::vector<int>>& adjacencyList)
{
    int number = numbers[initialIndex];
    std::vector<int>& list = adjacencyList[number];

    int numDependencies = 0;
    for (size_t i = 0; i < numbers.size(); ++i)
    {
        if (i == initialIndex)
            continue;

        for (int ele : list)
        {
            if (ele == numbers[i])
            {
                ++numDependencies;
                break;
            }
        }
    }

    return numDependencies;
}