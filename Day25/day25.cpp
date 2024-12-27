#include <iostream>
#include <vector>
#include "../fileRead.h"

constexpr int FILL_ROWS = 7;
constexpr int NUM_ROWS = FILL_ROWS - 2;
constexpr int NUM_COLS = 5;
constexpr char FILL = '#';

struct FillRepresentation
{
    int Heights[NUM_COLS];

    FillRepresentation() {
        for (size_t i = 0; i < NUM_COLS; ++i)
            Heights[i] = NUM_ROWS;
    }

    FillRepresentation(const FillRepresentation& other)
    {
        for (size_t i = 0; i < NUM_COLS; ++i)
            Heights[i] = other.Heights[i];
    }
};

unsigned long long Part1(const std::vector<std::string>& outputLines);
unsigned long long Part2(const std::vector<std::string>& outputLines);
bool IsLock(const std::vector<std::string>& set);
FillRepresentation ParseKey(const std::vector<std::string>& key);
FillRepresentation ParseLock(const std::vector<std::string>& lock);
bool CanLockFitKey(FillRepresentation lock, FillRepresentation key);
void PrintLockAndKey(const std::vector<std::string>& set);

static std::vector<FillRepresentation> Locks;
static std::vector<FillRepresentation> Keys;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }

    std::vector<std::string> outputLines;
    ReadFromFile(argv[1], outputLines);

    std::cout << Part1(outputLines) << std::endl;
}

unsigned long long Part1(const std::vector<std::string>& outputLines)
{
    size_t index = 0;
    while (index < outputLines.size())
    {
        std::vector<std::string> set;
        for (size_t i = 0; i < FILL_ROWS; ++i)
        {
            set.push_back(outputLines[index + i]);
        }

        if (IsLock(set))
        {
            Locks.push_back(ParseLock(set));
        }
        else
        {
            Keys.push_back(ParseKey(set));
        }
        index += FILL_ROWS + 1;
    }

    unsigned long long numMatches = 0;

    for (const FillRepresentation& lock : Locks)
    {
        for (const FillRepresentation& key : Keys)
        {
            if (CanLockFitKey(lock, key))
            {
                std::cout << "Lock: " << lock.Heights[0] << ", " << lock.Heights[1] << ", " << lock.Heights[2] << ", " << lock.Heights[3] << ", " << lock.Heights[4] << " fits key: "
                    << key.Heights[0] << ", " << key.Heights[1] << ", " << key.Heights[2] << ", " << key.Heights[3] << ", " << key.Heights[4] << std::endl;
                ++numMatches;
            }
                
        }
    }

    return numMatches;
}

unsigned long long Part2(const std::vector<std::string>& outputLines)
{
    return 0;
}

bool IsLock(const std::vector<std::string>& set)
{
    for (size_t i = 0; i < NUM_COLS; ++i)
    {
        if (set[0][i] != FILL)
            return false;
    }
    return true;
}

FillRepresentation ParseKey(const std::vector<std::string>& key)
{
    FillRepresentation fillRep;
    for (size_t i = 0; i < NUM_COLS; ++i)
    {
        for (size_t j = FILL_ROWS - 2; j >= 1; --j)
        {
            if (key[j][i] != FILL)
            {
                fillRep.Heights[i] = FILL_ROWS - (j + 2);
                break;
            }
        }
    }
    return fillRep;
}

FillRepresentation ParseLock(const std::vector<std::string>& lock)
{
    FillRepresentation fillRep;
    for (size_t i = 0; i < NUM_COLS; ++i)
    {
        for (size_t j = 1; j < FILL_ROWS - 1; ++j)
        {
            if (lock[j][i] != FILL)
            {
                fillRep.Heights[i] = j - 1;
                break;
            }
        }
    }
    return fillRep;
}

bool CanLockFitKey(FillRepresentation lock, FillRepresentation key)
{
    for (size_t i = 0; i < NUM_COLS; ++i)
    {
        if (lock.Heights[i] + key.Heights[i] > NUM_ROWS)
            return false;
    }
    return true;
}

void PrintLockAndKey(const std::vector<std::string>& set)
{
    for (const std::string& s : set)
    {
        std::cout << s << std::endl;
    }
    std::cout << std::endl;
}