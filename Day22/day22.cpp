#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>
#include "../fileRead.h"

struct SeqRepresentation
{
    int Seq1;
    int Seq2;
    int Seq3;
    int Seq4;

    SeqRepresentation(int seq1, int seq2, int seq3, int seq4) : Seq1(seq1), Seq2(seq2), Seq3(seq3), Seq4(seq4) {}

    bool operator==(const SeqRepresentation& other) const {
        return Seq1 == other.Seq1 && Seq2 == other.Seq2 && Seq3 == other.Seq3 && Seq4 == other.Seq4;
    }
};


namespace std
{
    template <>
    struct hash<SeqRepresentation>
    {
        std::size_t operator()(const SeqRepresentation& x) const
        {
            size_t h = std::hash<int>()(x.Seq1) ^ std::hash<int>()(x.Seq2) ^ std::hash<int>()(x.Seq3) ^ std::hash<int>()(x.Seq4);
            return h;
        }
    };
}

unsigned long long Part1(const std::vector<std::string>& outputLines);
unsigned long long Part2(const std::vector<std::string>& outputLines);
unsigned long long Step1(unsigned long long secretNumber);
unsigned long long Step2(unsigned long long secretNumber);
unsigned long long Step3(unsigned long long secretNumber);
unsigned long long Mix(unsigned long long number, unsigned long long secretNumber);
unsigned long long Prune(unsigned long long secretNumber);
char GetPrice(unsigned long long secretNumber);

constexpr int NUM_ITERATIONS = 2000;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }

    std::vector<std::string> outputLines;
    ReadFromFile(argv[1], outputLines);

    std::cout << Part2(outputLines) << std::endl;
}

unsigned long long Part1(const std::vector<std::string>& outputLines)
{
    unsigned long long finalSum = 0;

    for (const std::string& s : outputLines)
    {
        unsigned long secretNumber = std::stoull(s);
        for (size_t i = 0; i < NUM_ITERATIONS; ++i)
        {
            secretNumber = Step1(secretNumber);
            secretNumber = Step2(secretNumber);
            secretNumber = Step3(secretNumber);
        }
        finalSum += secretNumber;
    }

    return finalSum;
}

unsigned long long Part2(const std::vector<std::string>& outputLines)
{
    std::unordered_map<SeqRepresentation, unsigned long long> priceCache;

    for (const std::string& s : outputLines)
    {
        std::vector<char> prices;
        unsigned long long secretNumber = std::stoull(s);
        prices.push_back(GetPrice(secretNumber));
        
        for (size_t i = 0; i < NUM_ITERATIONS; ++i)
        {
            secretNumber = Step1(secretNumber);
            secretNumber = Step2(secretNumber);
            secretNumber = Step3(secretNumber);
            prices.push_back(GetPrice(secretNumber));
        }
        
        std::unordered_map<SeqRepresentation, int> seqCache;
        for (size_t i = 4; i < prices.size(); ++i)
        {
            int seq1 = prices[i - 3] - prices[i - 4];
            int seq2 = prices[i - 2] - prices[i - 3];
            int seq3 = prices[i - 1] - prices[i - 2];
            int seq4 = prices[i] - prices[i - 1];

            SeqRepresentation seq{ seq1, seq2, seq3, seq4 };
                
            if (seqCache.find(seq) != seqCache.end())
                continue;
            seqCache[seq] = 1;

            if (priceCache.find(seq) != priceCache.end())
            {
                priceCache[seq] += prices[i];
            }
            else
            {
                priceCache[seq] = prices[i];
            }
        }
    }

    unsigned long long maxSum = 0;

    for (const auto& it : priceCache)
    {
        maxSum = std::max(it.second, maxSum);
    }

    return maxSum;
}

unsigned long long Step1(unsigned long long secretNumber)
{
    unsigned long long res = secretNumber * 64;
    secretNumber = Mix(res, secretNumber);
    secretNumber = Prune(secretNumber);
    return secretNumber;
}

unsigned long long Step2(unsigned long long secretNumber)
{
    unsigned long long res = secretNumber / 32;
    secretNumber = Mix(res, secretNumber);
    secretNumber = Prune(secretNumber);
    return secretNumber;
}

unsigned long long Step3(unsigned long long secretNumber)
{
    unsigned long long res = secretNumber * 2048;
    secretNumber = Mix(res, secretNumber);
    secretNumber = Prune(secretNumber);
    return secretNumber;
}

unsigned long long Mix(unsigned long long number, unsigned long long secretNumber)
{
    return number ^ secretNumber;
}

unsigned long long Prune(unsigned long long secretNumber)
{
    return secretNumber % 16777216;
}

char GetPrice(unsigned long long secretNumber)
{
    return (char) (secretNumber % 10);
}