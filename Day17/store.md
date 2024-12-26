struct BitNode
{
    char bit;
    int bitPosition;
    std::shared_ptr<BitNode> prevBit;

    BitNode() : bit(UNKNOWN_BIT), bitPosition(0), prevBit(nullptr) {}
    BitNode(char bit, int bitPosition, std::shared_ptr<BitNode> prevBit) : bit(bit), bitPosition(bitPosition), prevBit(prevBit) {}
    BitNode(int bitPosition, std::shared_ptr<BitNode> prevBit) : bit(UNKNOWN_BIT), bitPosition(bitPosition), prevBit(prevBit) {}
    BitNode(char bit) : bit(bit), bitPosition(0), prevBit(nullptr) {}

private:
    BitNode* GetNodeAtPosition(int bitPosition)
    {
        if (bitPosition == this->bitPosition)
            return this;
        std::shared_ptr<BitNode> ptr = prevBit;
        while (ptr != nullptr && ptr->bitPosition != bitPosition)
        {
            ptr = ptr->prevBit;
        }
        return ptr.get();
    }

    std::optional<char> GetBitAtPosition(int bitPosition)
    {
        if (bitPosition > this->bitPosition)
            return std::nullopt;
        char bit = GetNodeAtPosition(bitPosition)->bit;
        if (bit == UNKNOWN_BIT)
            return std::nullopt;
        else
            return std::optional<char>{bit};
    }

public:
    bool CheckBitAtPosition(int bitPosition, char bit)
    {
        std::optional<char> bitAtPos = GetBitAtPosition(bitPosition);
        return !bitAtPos.has_value() || bitAtPos.value() == bit;
    }

    void SetBitAtPosition(int bitPosition, char bit)
    {
        if (bitPosition > this->bitPosition)
            return;
        GetNodeAtPosition(bitPosition)->bit = bit;
    }

    bool CheckLength(int numBits)
    {
        int bitPosition = this->bitPosition;
        while (bitPosition >= numBits)
        {
            if (GetNodeAtPosition(bitPosition)->bit == 1)
                return false;
            --bitPosition;
        }
        return true;
    }

    long long GetRepresentativeNumber()
    {
        long long num = 0;
        int bitPosition = this->bitPosition;
        while (bitPosition >= 0)
        {
            num <<= 1;
            std::optional<char> bit = GetBitAtPosition(bitPosition);
            if (bit.has_value())
                num += bit.value();
            bitPosition -= 1;
        }
        return num;
    }

    void PrintBits()
    {
        int bitPosition = this->bitPosition;
        while (bitPosition >= 0)
        {
            std::cout << (int) GetNodeAtPosition(bitPosition)->bit;
            bitPosition -= 1;
        }
        std::cout << std::endl;
    }
};

bool CheckNumber(std::shared_ptr<BitNode> front, int bitPosStart, char number);
std::shared_ptr<BitNode> ConstructNodes(std::shared_ptr<BitNode> currFront, int bitPosition, char number);

long long FindMinNumber(const std::vector<int>& instructions, int numInstructions)
{
    long long minNumber = -1;
    // pair of instruction to print pointer and the most recent bit node
    std::vector<std::pair<int, std::shared_ptr<BitNode>>> q;
    std::shared_ptr<BitNode> initial = std::make_shared<BitNode>();
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
            //std::cout << "Final number: " << node->GetRepresentativeNumber() << std::endl;
            std::cout << std::endl;
            if (node->CheckLength(numInstructions * 3))
            {
                std::cout << "Final number: " << node->GetRepresentativeNumber() << std::endl;
                if (minNumber == -1)
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
                std::cout << "b check for: " << instructionToPrint << ", at index: " << instructionToPrint * 3 << ", for value: " << (int) b << std::endl;
                node->PrintBits();
                continue;
            }
                
            std::shared_ptr<BitNode> bFront = ConstructNodes(node, instructionToPrint * 3, b);
            for (char c = 0; c <= 7; ++c)
            {
                if (!CheckNumber(bFront, instructionToPrint * 3 + b, c))
                {
                    std::cout << "c check for: " << instructionToPrint << ", at index: " << instructionToPrint * 3 + b << ", for value: " << (int) c << std::endl;
                    bFront->PrintBits();
                    continue;
                }
                    
                if ((b ^ B_XOR ^ c) == instructions[instructionToPrint])
                {
                    std::cout << (b ^ B_XOR ^ c) << std::endl;
                    std::cout << ((b ^ B_XOR ^ c) == instructions[instructionToPrint]) << std::endl;
                    std::cout << "Index: " << instructionToPrint << ", Instruction to print : " << instructions[instructionToPrint] << std::endl;
                    std::cout << "Char b: " << (int)b << std::endl;
                    std::cout << "Char c: " << (int)c << std::endl;
                    std::cout << std::endl;
                    std::shared_ptr<BitNode> cFront = ConstructNodes(bFront, instructionToPrint * 3 + b, c);
                    q.emplace_back(instructionToPrint + 1, cFront);
                }
                
            }
        }
    }

    return minNumber;
}

bool CheckNumber(std::shared_ptr<BitNode> front, int bitPosStart, char number)
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

std::shared_ptr<BitNode> ConstructNodes(std::shared_ptr<BitNode> currFront, int bitPosition, char number)
{
    std::shared_ptr<BitNode> newFront = currFront;
    while (newFront->bitPosition < bitPosition + 2)
    {
        newFront = std::make_shared<BitNode>(newFront->bitPosition + 1, newFront);
    }
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
