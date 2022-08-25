// AUTHOR: fsb3rke

#include <iostream>
#include <bitset>
#include <fstream>
#include <string>
#include <format>
#include <vector>
#include <math.h>
#include <conio.h>

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
            //std::cout << m_binaryBit << std::endl;
            double bitHexadecimalNumber = .0;
            if (m_binaryBit == DECIMAL_ZERO) {
                bitHexadecimalNumber = pow(2, binaryBitSize) * 0;
            }
            else if (m_binaryBit == DECIMAL_ONE) {
                bitHexadecimalNumber = pow(2, binaryBitSize) * 1;
            }
            //std::cout << binaryBit << " 2^" << binaryBitSize << std::endl;
            //std::cout << "B: " << bitHexadecimalNumber << std::endl;
            hexadecimalNumber += bitHexadecimalNumber;
            binaryBitSize++;
        }
        //std::cout << hexadecimalNumber << std::endl;
        hexadecimalNumbers.push_back(hexadecimalNumber);
        hexadecimalNumber = 0;
        binaryBitSize = 0;
    }
    //std::cout << std::endl;
    chars.reserve(hexadecimalNumbers.size());
    for (auto hex = hexadecimalNumbers.begin(); hex != hexadecimalNumbers.end(); hex++) {
        chars.push_back(*hex);
        //std::cout << *hex << std::endl;
    }
    for (auto& n : chars) {
        newContent += n;
    }
    FILE_IO decryptFile("e." + m_fileName);
    decryptFile.SetContent(newContent);
}

#define RIGH_ARROW_KEY_ASCII 77
#define LEFT_ARROW_KEY_ASCII 75
#define ENTER_KEY_ASCII 13
#define ESCAPE_KEY_ASCII 27

#define ENCRYPT_MODE 0
#define DECRYPT_MODE 1

#define SELECTED_ENCRYPT_MODE "_1: Encrypt Mode_"
#define UNSELECTED_ENCRYPT_MODE " 1: Encrypt Mode"

#define SELECTED_DECRYPT_MODE "_2: Decrypt Mode_"
#define UNSELECTED_DECRYPT_MODE " 2: Decrypt Mode"

int main(int argc, char** argv) // TODO: have to ARGVS
{
    std::string fileName;
    fileName = (std::string)argv[1];
    Crypter crypter(fileName);
    char key_press;
    int ascii_value;
    int mode;
    mode = ENCRYPT_MODE;
    std::cout << "\t\t" << SELECTED_ENCRYPT_MODE << "\t" << UNSELECTED_DECRYPT_MODE << "\r";
    //std::cout << "\r\t\t\t" << SELECTED_ENCRYPT_MODE << "\n\t\t\t" << UNSELECTED_DECRYPT_MODE << "\r";
    while (1)
    {
        key_press = _getch();
        ascii_value = key_press;
        if (ascii_value == ESCAPE_KEY_ASCII)
            break;
        else if (ascii_value == RIGH_ARROW_KEY_ASCII) {
            mode = ENCRYPT_MODE;
        }
        else if (ascii_value == LEFT_ARROW_KEY_ASCII) {
            mode = DECRYPT_MODE;
        }
        else if (ascii_value == ENTER_KEY_ASCII) {
            break;
        }

        if (mode == ENCRYPT_MODE) {
            //std::cout << "\r\t\t\t" << SELECTED_ENCRYPT_MODE << "\r\n\t\t\t" << UNSELECTED_DECRYPT_MODE;
            std::cout << "\t\t" << SELECTED_ENCRYPT_MODE << "\t" << UNSELECTED_DECRYPT_MODE << "\r";
        }
        else if (mode == DECRYPT_MODE) {
            //std::cout << "\r\t\t\t" << UNSELECTED_ENCRYPT_MODE << "\r\n\t\t\t" << SELECTED_DECRYPT_MODE << "\r";
            std::cout << "\t\t" << SELECTED_DECRYPT_MODE << "\t" << UNSELECTED_ENCRYPT_MODE << "\r";
        }
    }
    if (mode == ENCRYPT_MODE) {
        crypter.Encrypt();
    }
    else if (mode == DECRYPT_MODE) {
        crypter.Decrypt();
    }

    return EXIT_SUCCESS;
}
