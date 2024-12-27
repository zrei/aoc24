#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "../fileRead.h"

using Connections =
    std::unordered_map<std::string, std::unordered_set<std::string>>;
using Computers = std::unordered_set<std::string>;

unsigned long long Part1(const std::vector<std::string>& outputLines);
void Part2(const std::vector<std::string>& outputLines);
void ParseLine(const std::string& s);
void ParseLinePart2(const std::string& s);
bool CheckCycles(const std::string& node1, const std::string& node2, const std::string& node3);
bool Contains(const std::vector<std::string>& list, const std::string& node);
bool HasDirectConnection(const std::string& node1, const std::string& node2);
Computers LargestNetworkSize(Computers &computers,
                             const Connections &connections,
                             Computers &connectedSet);

static std::unordered_map<std::string, std::vector<std::string>> AdjacencyList;
static std::unordered_map<std::string, int> AllNodes;
static Connections NodeConnections;
static Computers Nodes;

constexpr char LAN_CHAR = 't';

template <typename T>
std::unordered_set<T> findIntersection(const std::unordered_set<T> &set1,
                                       const std::unordered_set<T> &set2) {
  std::unordered_set<T> intersection;

  const std::unordered_set<T> &smaller =
      (set1.size() < set2.size()) ? set1 : set2;
  const std::unordered_set<T> &larger =
      (set1.size() < set2.size()) ? set2 : set1;

  for (const T &element : smaller) {
    if (larger.find(element) != larger.end()) {
      intersection.insert(element);
    }
  }

  return intersection;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }

    std::vector<std::string> outputLines;
    ReadFromFile(argv[1], outputLines);

    //std::cout << Part1(outputLines) << std::endl;
    Part2(outputLines);
}

unsigned long long Part1(const std::vector<std::string>& outputLines)
{
    for (const std::string& line : outputLines)
    {
        ParseLine(line);
    }

    std::vector<std::string> nodes;
    for (const auto& pair : AllNodes)
    {
        nodes.push_back(pair.first);
    }

    unsigned long long numCycles = 0;
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        for (size_t j = i + 1; j < nodes.size(); ++j)
        {
            for (size_t k = j + 1; k < nodes.size(); ++k)
            {
                if (CheckCycles(nodes[i], nodes[j], nodes[k]))
                {
                    if (nodes[i][0] == LAN_CHAR || nodes[j][0] == LAN_CHAR || nodes[k][0] == LAN_CHAR)
                        ++numCycles;
                }
            }
        }
    }
   return numCycles;
}

void Part2(const std::vector<std::string>& outputLines)
{
    for (const std::string& line : outputLines)
    {
        ParseLinePart2(line);
    }

    Computers connectedSet;
    Computers largestSet{
      LargestNetworkSize(Nodes, NodeConnections, connectedSet)};

    std::vector<std::string> network{largestSet.begin(), largestSet.end()};
    std::sort(network.begin(), network.end());
    
    for (const std::string& node : network)
    {
        std::cout << node << ",";
    }
    std::cout << std::endl;
}

void ParseLine(const std::string& s)
{
    int dashIndex = s.find('-');
    std::string first = s.substr(0, dashIndex);
    std::string second = s.substr(dashIndex + 1);
    AdjacencyList[first].push_back(second);
    AllNodes[first] = 1;
    AllNodes[second] = 1;
}

void ParseLinePart2(const std::string& s)
{
    int dashIndex = s.find('-');
    std::string first = s.substr(0, dashIndex);
    std::string second = s.substr(dashIndex + 1);
    NodeConnections[first].insert(second);
    NodeConnections[second].insert(first);
    Nodes.insert(first);
    Nodes.insert(second);
}

bool CheckCycles(const std::string& node1, const std::string& node2, const std::string& node3)
{
    return HasDirectConnection(node1, node2) && HasDirectConnection(node2, node3) && HasDirectConnection(node3, node1);
}

bool Contains(const std::vector<std::string>& list, const std::string& node)
{
    for (const std::string& s : list)
    {
        if (s == node)
            return true;
    }
    return false;
}

bool HasDirectConnection(const std::string& node1, const std::string& node2)
{
    return Contains(AdjacencyList[node1], node2) || Contains(AdjacencyList[node2], node1);
}

Computers LargestNetworkSize(Computers &computers,
                             const Connections &connections,
                             Computers &connectedSet) {
  if (computers.empty())
    return connectedSet;

  Computers largestSet{connectedSet};
  auto it{computers.begin()};

  while (it != computers.end()) {
    connectedSet.insert(*it);
    auto set{findIntersection(computers, connections.at(*it))};
    auto resSet{LargestNetworkSize(set, connections, connectedSet)};

    if (resSet.size() > largestSet.size()) {
      largestSet = resSet;
    }

    connectedSet.erase(*it);
    it = computers.erase(it);
  }

  return largestSet;
}
