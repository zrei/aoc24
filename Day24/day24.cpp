#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "../fileRead.h"

enum Operation : char
{
    XOR,
    OR,
    AND
};

struct Connection
{
    std::string FirstConnection;
    std::string SecondConnection;
    Operation ConnectionOperation;

    Connection() : FirstConnection(""), SecondConnection(""), ConnectionOperation(XOR) {}

    Connection(std::string firstConnection, std::string secondConnection, Operation operation) : FirstConnection(firstConnection), SecondConnection(secondConnection), ConnectionOperation(operation) {}

    Connection(const Connection& other) : FirstConnection(other.FirstConnection), SecondConnection(other.SecondConnection), ConnectionOperation(other.ConnectionOperation) {}

    bool ContainsConnection(const std::string& node) const
    {
        return FirstConnection == node || SecondConnection == node;
    }
};

unsigned long long Part1(const std::vector<std::string>& outputLines);
void Part2(const std::vector<std::string>& outputLines);
void ParseSource(const std::string& source);
char GetBitValue(char c);
void ParseConnections(const std::string& connection);
Operation GetOperation(const std::string& op);
char PerformOperation(char operand1, char operand2, Operation operation);
bool TryFillOutBitValues(std::unordered_map<std::string, char>& bitValues, std::unordered_map<std::string, Connection>& connections);
unsigned long long GetValue(std::unordered_map<std::string, char>& bitValues, char firstChar);
void OutputSortedNodes(std::vector<std::string>& nodes);
void FindNextZNode(const std::string& node);
bool IsWrongZNode(const std::string& node);

static std::unordered_map<std::string, char> BitValues;
static std::unordered_map<std::string, Connection> Connections;
static std::unordered_map<std::string, Connection> CorrectConnections;
static std::vector<std::string> OutputNodes;
static std::vector<std::string> WrongZNodes;
static std::vector<std::string> WrongOutputNodes; 

constexpr int FIRST_SPACE_INDEX = 3;
constexpr char Z_CHAR = 'z';
constexpr char X_CHAR = 'x';
constexpr char Y_CHAR = 'y';
const std::string FINAL_GATE = "z45";
constexpr char ONE_BIT = 1;

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
    size_t index = 0;
    while (outputLines[index] != "")
    {
        ParseSource(outputLines[index]);
        ++index;
    }

    for (size_t i = index + 1; i < outputLines.size(); ++i)
    {
        ParseConnections(outputLines[i]);
    }

    TryFillOutBitValues(BitValues, Connections);

    return GetValue(BitValues, Z_CHAR);
}

/*
Our goal is to have a Ripple Carry Adder. To find the 8 wrong gates in a ripple carry adder circuit, we can break it down into three groups: The first 3 wrong gates can be found by looking at gates that produce a z output. In a correct ripple carry adder, any gate producing z must use an XOR operation. So we identify 3 gates that output z but don't use XOR and also are not gate z45, that one can be a bit different because it's the last gate - these are wrong. For the next 3 wrong gates, we need to understand that in a ripple carry adder, XOR operations should only be used when dealing with inputs and outputs, not for intermediate calculations. We can find 3 gates that use XOR but don't have x, y, or z in their inputs or outputs. Since these gates are using XOR in the wrong place, they are incorrect.

Now we have 6 gates, but we don't know which ones to swap, to find the correct pairings, we want the number behind the first z-output in the recursive chain, so we write a recursive function. Say we have a chain of gates like this: a, b -> c where c is the output of one of our non-z XOR gates. Then c, d -> e then e, f -> z09 and we know we want to get to z09. Our recursive function would start with the first gate (a, b -> c), see that its output 'c' is used as input in the next gate, follow this to (c, d -> e), see that its output 'e' is used as input in the z09 gate, and finally reach (e, f -> z09). Now we just swap c and z09 - 1, so we swap c and z08. The -1 is there because this function finds the NEXT z gate, not the current one we need.

The final 2 wrong gates require comparing the circuit's actual output with what we expect (x+y). When we convert both numbers to binary and compare them, we'll find a section where they differ. If we XOR these two binary numbers, we'll get zeros where the bits match and ones where they differ. Count the number of zeros at the start of this XOR result - let's call this number N. The last two wrong gates will be the ones that use inputs xN and yN, swap them. 
*/

void Part2(const std::vector<std::string>& outputLines)
{
    size_t index = 0;
    while (outputLines[index] != "")
    {
        ParseSource(outputLines[index]);
        ++index;
    }

    unsigned long long xVal = GetValue(BitValues, X_CHAR);

    unsigned long long yVal = GetValue(BitValues, Y_CHAR);

    unsigned long long desiredZVal = xVal + yVal;

    for (size_t i = index + 1; i < outputLines.size(); ++i)
    {
        ParseConnections(outputLines[i]);
    }

    CorrectConnections = Connections;

    for (const std::string& node : WrongZNodes)
    {
        std::cout << node << ",";
    }
    std::cout << std::endl;

    for (const std::string& node : WrongOutputNodes)
    {
        std::cout << node << ",";
    }
    std::cout << std::endl;

    for (const std::string& wrongNode : WrongOutputNodes)
    {
        FindNextZNode(wrongNode);
    }

    TryFillOutBitValues(BitValues, CorrectConnections);

    char bitValues[46];

    for (const auto& pair : BitValues)
    {
        if (pair.first[0] == Z_CHAR)
        {
            bitValues[std::stoi(pair.first.substr(1))] = pair.second;
        }
    }

    for (int i = 45; i >= 0; --i)
    {
        std::cout << (int) bitValues[i];
    }
    std::cout << std::endl;

    std::bitset<46> x(desiredZVal);
    std::cout << x << '\n';

    // bit 34

    std::string xInput = "x" + std::to_string(34);
    std::string yInput = "y" + std::to_string(34);

    for (const auto& pair: CorrectConnections)
    {
        if (pair.second.ContainsConnection(xInput) && pair.second.ContainsConnection(yInput))
            std::cout << pair.first << ",";
    }

    std::cout << std::endl;
}

/*
Needs to look into the vector as well
*/
void FindNextZNode(const std::string& node)
{
    std::vector<std::string> q;
    q.push_back(node);

    while (!q.empty())
    {
        std::string currNode = q[0];
        q.erase(q.begin());
        
        if (currNode[0] == Z_CHAR && IsWrongZNode(currNode))
        {
            std::string zNode = "z" + std::to_string(std::stoi(currNode.substr(1)) - 1);
            std::cout << "Swap " << node  << " with " << zNode << std::endl;
            Connection first = CorrectConnections[node];
            Connection second = CorrectConnections[zNode];
            CorrectConnections[node] = second;
            CorrectConnections[zNode] = first;

            return;
        }   
        else if (currNode[0] == Z_CHAR)
            continue;
        for (const auto& pair : Connections)
        {
            if (pair.second.ContainsConnection(currNode))
                q.push_back(pair.first);
        }
    }
}

bool IsWrongZNode(const std::string& node)
{
    if (node[0] != Z_CHAR)
        return false;
    
    int number = std::stoi(node.substr(1));
    for (const std::string& zNode : WrongZNodes)
    {
        if (std::stoi(zNode.substr(1)) == number - 1)
            return true;
    }
    return false;
}

void ParseSource(const std::string& source)
{
    int colonIndex = source.find(':');
    std::string node = source.substr(0, colonIndex);
    char bit = GetBitValue(source[colonIndex + 2]);
    BitValues[node] = bit;
}

char GetBitValue(char c)
{
    return c - '0';
}

void ParseConnections(const std::string& connection)
{
    std::string firstConnection = connection.substr(0, FIRST_SPACE_INDEX);
    int secondSpaceIndex = connection.find(' ', FIRST_SPACE_INDEX + 1);
    std::string secondConnection = connection.substr(secondSpaceIndex + 1, 3);
    Operation op = GetOperation(connection.substr(FIRST_SPACE_INDEX + 1, secondSpaceIndex - (FIRST_SPACE_INDEX + 1)));
    int arrowIndex = connection.find('>');
    std::string node = connection.substr(arrowIndex + 2);
    Connections.emplace(std::piecewise_construct,
              std::forward_as_tuple(node),
              std::forward_as_tuple(firstConnection, secondConnection, op));
    std::cout << "Nodes " << firstConnection << " with node " << secondConnection << " connects to node " << node << " with operation " << (int) op << std::endl;
    OutputNodes.push_back(node);

    if (node[0] == Z_CHAR && op != XOR && node != FINAL_GATE)
        WrongZNodes.push_back(node);

    if ((node[0] != Z_CHAR && ((firstConnection[0] != X_CHAR && firstConnection[0] != Y_CHAR) && (secondConnection[0] != X_CHAR && secondConnection[0] != Y_CHAR))) && op == XOR)
        WrongOutputNodes.push_back(node);
}

Operation GetOperation(const std::string& op)
{
    if (op == "AND")
        return AND;
    else if (op == "OR")
        return OR;
    else
        return XOR;
}

char PerformOperation(char operand1, char operand2, Operation operation)
{
    switch (operation)
    {
    case AND:
        return operand1 & operand2;
    case OR:
        return operand1 | operand2;
    case XOR:
        return operand1 ^ operand2;
    }
}

/*
Returns true only if it successfully fills out all bit values
*/
bool TryFillOutBitValues(std::unordered_map<std::string, char>& bitValues, std::unordered_map<std::string, Connection>& connections)
{
    while (true)
    {
        bool hasFoundAll = true;
        bool hasFilledAny = false;
        for (const auto& pair : connections)
        {
            if (bitValues.find(pair.first) != bitValues.end())
                continue;

            if (bitValues.find(pair.second.FirstConnection) == bitValues.end() || bitValues.find(pair.second.SecondConnection) == bitValues.end())
            {
                hasFoundAll = false;
                continue;
            }

            bitValues[pair.first] = PerformOperation(bitValues[pair.second.FirstConnection], bitValues[pair.second.SecondConnection], pair.second.ConnectionOperation);
            hasFilledAny = true;
        }

        if (hasFoundAll)
            break;

        if (!hasFilledAny)
            return false;
    }

    return true;
}

unsigned long long GetValue(std::unordered_map<std::string, char>& bitValues, char firstChar)
{
    unsigned long long val = 0;

    for (const auto& pair : bitValues)
    {
        if (pair.first[0] == firstChar && pair.second == ONE_BIT)
        {
            val += std::pow(2, std::stoi(pair.first.substr(1)));
        }
    }

    return val;
}

void OutputSortedNodes(std::vector<std::string>& nodes)
{
    std::sort(nodes.begin(), nodes.end());
    for (const std::string& s : nodes)
        std::cout << s << ",";
}