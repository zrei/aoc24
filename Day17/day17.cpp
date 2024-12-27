#include <iostream>
#include <vector>
#include <fstream>
#include <optional>
#include "../fileRead.h"

constexpr char UNKNOWN_BIT = 2;
constexpr char B_XOR = 2 xor 7;
constexpr int MAX_BITS = 16 * 3 + 8;

class BitsHolder
{
private:
    char m_BitHolder[MAX_BITS];

public:
    void PrintBits()
    {
        for (int i = MAX_BITS - 1; i >= 0; --i)
        {
            std::cout << (int)m_BitHolder[i];
        }
        std::cout << std::endl;
    }

    BitsHolder()
    {
        for(size_t i = 0; i < MAX_BITS; ++i)
        {
            m_BitHolder[i] = UNKNOWN_BIT;
        }
    }

    BitsHolder(const BitsHolder& copy)
    {
        for (size_t i = 0; i < MAX_BITS; ++i)
        {
            m_BitHolder[i] = copy.m_BitHolder[i];
        }
    }
    
private:
    std::optional<char> GetBitAtPosition(int bitPosition)
    {
        if (m_BitHolder[bitPosition] == UNKNOWN_BIT)
            return std::nullopt;
        else
            return std::optional<char>{m_BitHolder[bitPosition]};
    }

public:
    bool CheckBitAtPosition(int bitPosition, char bit)
    {
        std::optional<char> bitAtPos = GetBitAtPosition(bitPosition);
        return !bitAtPos.has_value() || bitAtPos.value() == bit;
    }

    void SetBitAtPosition(int bitPosition, char bit)
    {
        m_BitHolder[bitPosition] = bit;
    }

    bool CheckLength(int numBits)
    {
        for (size_t i = MAX_BITS - 1; i >= numBits; --i)
        {
            if (m_BitHolder[i] == 1)
                return false;
        }
        return true;
    }

    unsigned long long GetRepresentativeNumber()
    {
        unsigned long long num = 0;
        for (int i = MAX_BITS - 1; i >= 0; --i)
        {
            num <<= 1;
            std::optional<char> bit = GetBitAtPosition(i);
            if (bit.has_value())
                num += bit.value();
        }
        return num;
    }
};


void Part1(const std::vector<std::string>& outputLines);
long long Part2(const std::vector<std::string>& outputLines);
void adv(char comboOperand);
void bxl(char literalOperand);
void bst(char comboOperand);
bool jnz(char literalOperand);
void bxc();
bool jnz(char literalOperand);
void out(char comboOperand);
char outAlternate(char comboOperand);
void bdv(char comboOperand);
void cdv(char comboOperand);
unsigned long ReadComboOperand(char comboOperand);
unsigned long ParseRegister(const std::string& s);
void ParseInstructions(const std::string& s, std::vector<char>& instructions);
unsigned long long FindMinNumber(const std::vector<char>& instructions, int numInstructions);
bool CheckNumber(std::shared_ptr<BitsHolder> front, int bitPosStart, char number);
std::shared_ptr<BitsHolder> ConstructNodes(std::shared_ptr<BitsHolder> currNode, int bitPosition, char number);
char GetInstruction(char instr);
char eval(unsigned long long a, const std::vector<char>& instructions);
long long dfs(long long in_pos, unsigned long long curr, const std::vector<char>& instructions);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Did not provide file name!\n";
        return 0;
    }

    std::vector<std::string> outputLines;
    ReadFromFile(argv[1], outputLines);

    std::cout << Part2(outputLines) << std::endl;
}

constexpr int ADV = 0;
constexpr int BXL = 1;
constexpr int BST = 2;
constexpr int JNZ = 3;
constexpr int BXC = 4;
constexpr int OUT = 5;
constexpr int BDV = 6;
constexpr int CDV = 7;

static unsigned long long registerA = 0;
static unsigned long long registerB = 0;
static unsigned long long registerC = 0;
static unsigned long instrPointer = 0;

static std::ofstream FileOut;

void Part1(const std::vector<std::string>& outputLines)
{
    registerA = ParseRegister(outputLines[0]);
    registerB = ParseRegister(outputLines[1]);
    registerC = ParseRegister(outputLines[2]);
    instrPointer = 0;

    FileOut = std::ofstream{"execution.out"};

    std::vector<char> instructions;
    ParseInstructions(outputLines[4], instructions);

    int numInstructions = instructions.size();

    while (instrPointer < numInstructions)
    {
        std::cout << "Current instruction pointer: " << instrPointer << std::endl;
        switch (instructions[instrPointer])
        {
        case ADV:
            adv(instructions[instrPointer + 1]);
            instrPointer += 2;
            break;
        case BXL:
            bxl(instructions[instrPointer + 1]);
            instrPointer += 2;
            break;
        case BST:
            bst(instructions[instrPointer + 1]);
            instrPointer += 2;
            break;
        case JNZ:
            if (!jnz(instructions[instrPointer + 1]))
            {
                instrPointer += 2;
            }
            break;
        case BXC:
            bxc();
            instrPointer += 2;
            break;
        case OUT:
            out(instructions[instrPointer + 1]);
            instrPointer += 2;
            break;
        case BDV:
            bdv(instructions[instrPointer + 1]);
            instrPointer += 2;
            break;
        case CDV:
            cdv(instructions[instrPointer + 1]);
            instrPointer += 2;
            break;
        }
    }

    std::cout << "Complete" << std::endl;

    FileOut.close();
}

long long Part2(const std::vector<std::string>& outputLines)
{
    std::vector<char> instructions;
    ParseInstructions(outputLines[4], instructions);

    int numInstructions = instructions.size();
    
    return dfs(numInstructions - 1, 0, instructions);
}

unsigned long ParseRegister(const std::string& s)
{
    int colonPointer = s.find(':');
    return std::stoul(s.substr(colonPointer + 2));
}

void ParseInstructions(const std::string& s, std::vector<char>& instructions)
{
    int commaPointer = s.find(',');
    while (commaPointer != -1)
    {
        instructions.push_back(GetInstruction(s[commaPointer - 1]));
        commaPointer = s.find(',', commaPointer + 1);
    }
    instructions.push_back(GetInstruction(s.back()));
}

char GetInstruction(char instr)
{
    return (char) (instr - '0');
}

void adv(char comboOperand)
{
    unsigned long numerator = registerA;
    unsigned long denominator = std::pow(2, ReadComboOperand(comboOperand));
    registerA = numerator / denominator;
}

void bxl(char literalOperand)
{
    registerB = registerB ^ literalOperand;
}

void bst(char comboOperand)
{
    registerB = ReadComboOperand(comboOperand) % 8;
}

bool jnz(char literalOperand)
{
    if (registerA == 0)
        return false;
    instrPointer = literalOperand;
    return true;
}

void bxc()
{
    registerB = registerB ^ registerC;
}

void out(char comboOperand)
{
    FileOut << ReadComboOperand(comboOperand) % 8 << ",";
}

char outAlternate(char comboOperand)
{
    return ReadComboOperand(comboOperand) % 8;
}

void bdv(char comboOperand)
{
    unsigned long numerator = registerA;
    unsigned long denominator = std::pow(2, ReadComboOperand(comboOperand));
    registerB = numerator / denominator;
}

void cdv(char comboOperand)
{
    unsigned long numerator = registerA;
    unsigned long denominator = std::pow(2, ReadComboOperand(comboOperand));
    registerC = numerator / denominator;
}

unsigned long ReadComboOperand(char comboOperand)
{
    switch (comboOperand)
    {
    case 1:
    case 2:
    case 3:
        return comboOperand;
    case 4:
        return registerA;
    case 5:
        return registerB;
    case 6:
        return registerC;
    default:
        std::cout << "Invalid combo operand: " << comboOperand << std::endl;
        return 0;
    }
}


unsigned long long FindMinNumber(const std::vector<char>& instructions, int numInstructions)
{
    unsigned long long minNumber = 0;
    // pair of instruction to print pointer and the most recent bit node
    std::vector<std::pair<int, std::shared_ptr<BitsHolder>>> q;
    std::shared_ptr<BitsHolder> initial = std::make_shared<BitsHolder>();
    q.emplace_back(0, initial);

    while (!q.empty())
    {
        auto [instructionToPrint, node] = q[0];
        q.erase(q.begin());

        if (instructionToPrint == numInstructions)
        {
            std::cout << "Got to the end" << std::endl;
            std::cout << "Bit representation: ";
            node->PrintBits();

            std::cout << std::endl;
            if (node->CheckLength(numInstructions * 3))
            {
                std::cout << "Final number: " << node->GetRepresentativeNumber() << std::endl;
                if (minNumber == 0)
                    minNumber = node->GetRepresentativeNumber();
                else
                    minNumber = std::min(node->GetRepresentativeNumber(), minNumber);
            }
            continue;
        }

        for (char b = 0; b <= 7; ++b)
        {
            if (!CheckNumber(node, instructionToPrint * 3, b))
            {
                //std::cout << "b check for: " << instructionToPrint << ", at index: " << instructionToPrint * 3 << ", for value: " << (int)b << std::endl;
                //node->PrintBits();
                continue;
            }

            std::shared_ptr<BitsHolder> bFront = ConstructNodes(node, instructionToPrint * 3, b);
            for (char c = 0; c <= 7; ++c)
            {
                if (!CheckNumber(bFront, instructionToPrint * 3 + b, c))
                {
                    //std::cout << "c check for: " << instructionToPrint << ", at index: " << instructionToPrint * 3 + b << ", for value: " << (int)c << std::endl;
                    //bFront->PrintBits();
                    continue;
                }

                if ((b xor B_XOR xor c) == instructions[instructionToPrint])
                {
                    std::shared_ptr<BitsHolder> cFront = ConstructNodes(bFront, instructionToPrint * 3 + b, c);
                    q.emplace_back(instructionToPrint + 1, cFront);
                }

            }
        }
    }

    return minNumber;
}

bool CheckNumber(std::shared_ptr<BitsHolder> front, int bitPosStart, char number)
{
    switch (number)
    {
    case 0:
        return front->CheckBitAtPosition(bitPosStart, 0) && front->CheckBitAtPosition(bitPosStart + 1, 0) && front->CheckBitAtPosition(bitPosStart + 2, 0);
    case 1:
        return front->CheckBitAtPosition(bitPosStart, 1) && front->CheckBitAtPosition(bitPosStart + 1, 0) && front->CheckBitAtPosition(bitPosStart + 2, 0);
    case 2:
        return front->CheckBitAtPosition(bitPosStart, 0) && front->CheckBitAtPosition(bitPosStart + 1, 1) && front->CheckBitAtPosition(bitPosStart + 2, 0);
    case 3:
        return front->CheckBitAtPosition(bitPosStart, 1) && front->CheckBitAtPosition(bitPosStart + 1, 1) && front->CheckBitAtPosition(bitPosStart + 2, 0);
    case 4:
        return front->CheckBitAtPosition(bitPosStart, 0) && front->CheckBitAtPosition(bitPosStart + 1, 0) && front->CheckBitAtPosition(bitPosStart + 2, 1);
    case 5:
        return front->CheckBitAtPosition(bitPosStart, 1) && front->CheckBitAtPosition(bitPosStart + 1, 0) && front->CheckBitAtPosition(bitPosStart + 2, 1);
    case 6:
        return front->CheckBitAtPosition(bitPosStart, 0) && front->CheckBitAtPosition(bitPosStart + 1, 1) && front->CheckBitAtPosition(bitPosStart + 2, 1);
    case 7:
        return front->CheckBitAtPosition(bitPosStart, 1) && front->CheckBitAtPosition(bitPosStart + 1, 1) && front->CheckBitAtPosition(bitPosStart + 2, 1);
    }
    return false;
}

std::shared_ptr<BitsHolder> ConstructNodes(std::shared_ptr<BitsHolder> currNode, int bitPosition, char number)
{
    std::shared_ptr<BitsHolder> newFront = std::make_shared<BitsHolder>(*currNode);
    switch (number)
    {
    case 0:
        newFront->SetBitAtPosition(bitPosition, 0);
        newFront->SetBitAtPosition(bitPosition + 1, 0);
        newFront->SetBitAtPosition(bitPosition + 2, 0);
        break;
    case 1:
        newFront->SetBitAtPosition(bitPosition, 1);
        newFront->SetBitAtPosition(bitPosition + 1, 0);
        newFront->SetBitAtPosition(bitPosition + 2, 0);
        break;
    case 2:
        newFront->SetBitAtPosition(bitPosition, 0);
        newFront->SetBitAtPosition(bitPosition + 1, 1);
        newFront->SetBitAtPosition(bitPosition + 2, 0);
        break;
    case 3:
        newFront->SetBitAtPosition(bitPosition, 1);
        newFront->SetBitAtPosition(bitPosition + 1, 1);
        newFront->SetBitAtPosition(bitPosition + 2, 0);
        break;
    case 4:
        newFront->SetBitAtPosition(bitPosition, 0);
        newFront->SetBitAtPosition(bitPosition + 1, 0);
        newFront->SetBitAtPosition(bitPosition + 2, 1);
        break;
    case 5:
        newFront->SetBitAtPosition(bitPosition, 1);
        newFront->SetBitAtPosition(bitPosition + 1, 0);
        newFront->SetBitAtPosition(bitPosition + 2, 1);
        break;
    case 6:
        newFront->SetBitAtPosition(bitPosition, 0);
        newFront->SetBitAtPosition(bitPosition + 1, 1);
        newFront->SetBitAtPosition(bitPosition + 2, 1);
        break;
    case 7:
        newFront->SetBitAtPosition(bitPosition, 1);
        newFront->SetBitAtPosition(bitPosition + 1, 1);
        newFront->SetBitAtPosition(bitPosition + 2, 1);
        break;
    }
    return newFront;
}

char eval(unsigned long long a, const std::vector<char>& instructions)
{
	registerA = a; 
    registerB = 0; 
    registerC = 0;
    instrPointer = 0;

    char out[16];
    int outPtr = 0;

    while (instrPointer < instructions.size())
    {
        std::cout << "Current instruction pointer: " << instrPointer << std::endl;
        switch (instructions[instrPointer])
        {
        case ADV:
            adv(instructions[instrPointer + 1]);
            instrPointer += 2;
            break;
        case BXL:
            bxl(instructions[instrPointer + 1]);
            instrPointer += 2;
            break;
        case BST:
            bst(instructions[instrPointer + 1]);
            instrPointer += 2;
            break;
        case JNZ:
            if (!jnz(instructions[instrPointer + 1]))
            {
                instrPointer += 2;
            }
            break;
        case BXC:
            bxc();
            instrPointer += 2;
            break;
        case OUT:
            out[outPtr] = outAlternate(instructions[instrPointer + 1]);
            ++outPtr;
            instrPointer += 2;
            break;
        case BDV:
            bdv(instructions[instrPointer + 1]);
            instrPointer += 2;
            break;
        case CDV:
            cdv(instructions[instrPointer + 1]);
            instrPointer += 2;
            break;
        }
    }

	return out[0];
}

long long dfs(long long in_pos, unsigned long long curr, const std::vector<char>& instructions)
{
	if (in_pos < 0) return curr;

	curr <<= 3;
	char target = instructions[in_pos];
	for (char i = 0; i < 8; ++i) {
		unsigned long long n = curr + i;
		if (eval(n, instructions) == target) {
			long long out = dfs(in_pos - 1, n, instructions);
			if (out >= 0) return out;
		}
	}
	return -1;
}
