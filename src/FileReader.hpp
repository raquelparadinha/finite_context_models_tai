#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <string>
#include <vector>

class FileReader {
public:
    static std::string readTextFromFile(const std::string& filePath);
    static std::vector<std::string> readTextsFromDirectory(const std::string& directoryPath);
};

#endif
