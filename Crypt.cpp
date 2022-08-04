#include <iostream>
#include <bitset>
#include <fstream>
#include <string>
#include <format>
#include <vector>
#include <math.h>

#define WHITE_SPACE ' '
#define END_LINE '\n'
#define QUESTION_MARK '?'

#define DECIMAL_ZERO 48
#define DECIMAL_ONE 49

void reverseStr(std::string& str)
{
    int len = str.length();
    int n = len - 1;
    int i = 0;
    while (i <= n) {
        std::swap(str[i], str[n]);
        n = n - 1;
        i = i + 1;
    }

}

class FILE_IO {
public:
    FILE_IO(std::string fileName) { m_fileName = fileName; }
    std::string GetContent();
    void SetContent(std::string newContent);
private:
    std::string m_fileName;
};
std::string FILE_IO::GetContent() {
    std::string line, content;
    std::ifstream file(m_fileName);
    while (getline(file, line)) {
        content += line + END_LINE;
    }
    file.close();
    return content;
}
void FILE_IO::SetContent(std::string newContent) {
    std::ofstream file(m_fileName);
    file << newContent;
    file.close();
}

class Crypter
{
public:
    Crypter(std::string fileName) { m_fileName = fileName; }
    void Decrypt();
    void Encrypt();
private:
    std::string m_fileName;
};
void Crypter::Decrypt() {
    FILE_IO file(m_fileName);
    std::string content = file.GetContent();
    std::string newContent;
    for (int i = 0; i < content.length(); i++) {
        char element = content[i];
        if (element != WHITE_SPACE && element != END_LINE) {
            std::bitset<8> x(element);
            newContent += x.to_string()+"?";
        }
        else if (element != WHITE_SPACE && element == END_LINE) {
            newContent += END_LINE;
        }
    }
    FILE_IO decryptFile("d." + m_fileName);
    decryptFile.SetContent(newContent);
}
void Crypter::Encrypt() {
    FILE_IO file(m_fileName);
    std::string content = file.GetContent();
    std::string newContent;
    std::vector<std::string> splittedContent;
    std::string oneTimePushBack;
    for (int i = 0; i < content.length(); i++) {
        char element = content[i];
        if (element != QUESTION_MARK && element != END_LINE) {
            oneTimePushBack += element;
        }
        else if (element != QUESTION_MARK && element == END_LINE) {
            newContent += END_LINE; // BUG
        }
        else if (element == QUESTION_MARK && element != END_LINE) {
            reverseStr(oneTimePushBack);
            splittedContent.push_back(oneTimePushBack);
            oneTimePushBack = "";
        }
    }
    int binaryBitSize = 0;
    std::vector<int> hexadecimalNumbers;
    std::vector<char> chars;
    for (auto it = splittedContent.begin(); it != splittedContent.end(); ++it) {
        int hexadecimalNumber = 0;
        for (auto binaryBit : *it) {
            int m_binaryBit = (int)binaryBit;
            std::cout << m_binaryBit << std::endl;
            double bitHexadecimalNumber = .0;
            if (m_binaryBit == DECIMAL_ZERO) {
                bitHexadecimalNumber = pow(2, binaryBitSize) * 0;
            }
            else if (m_binaryBit == DECIMAL_ONE) {
                bitHexadecimalNumber = pow(2, binaryBitSize) * 1;
            }
            std::cout << binaryBit << " 2^" << binaryBitSize << std::endl;
            std::cout << "B: " << bitHexadecimalNumber << std::endl;
            hexadecimalNumber += bitHexadecimalNumber;
            binaryBitSize++;
        }
        std::cout << hexadecimalNumber << std::endl;
        hexadecimalNumbers.push_back(hexadecimalNumber);
        hexadecimalNumber = 0;
        binaryBitSize = 0;
    }
    std::cout << std::endl;
    chars.reserve(hexadecimalNumbers.size());
    for (auto hex = hexadecimalNumbers.begin(); hex != hexadecimalNumbers.end(); hex++) {
        chars.push_back(*hex);
        std::cout << *hex << std::endl;
    }
    for (auto& n : chars) {
        newContent += n;
    }
    FILE_IO decryptFile("e." + m_fileName);
    decryptFile.SetContent(newContent);
}

int main() // TODO: have to ARGVS
{
    /*
        1: Encrypt Mode
        2: Decrypt Mode
    */
    Crypter crypt("d.testFile.txt");
    crypt.Encrypt();
}
