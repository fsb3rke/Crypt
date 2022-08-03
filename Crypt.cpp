#include <iostream>
#include <bitset>
#include <fstream>
#include <string>
#include <format>
#include <vector>

#define WHITE_SPACE ' '
#define END_LINE '\n'
#define QUESTION_MARK '?'

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
            newContent += END_LINE;
        }
        else if (element == QUESTION_MARK && element != END_LINE) {
            splittedContent.push_back(oneTimePushBack);
            oneTimePushBack = "";
        }
    }
    for (auto it = splittedContent.begin(); it != splittedContent.end(); ++it) {
        newContent += *it;    std::cout << *it << std::endl;
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