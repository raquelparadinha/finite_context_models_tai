#include "FileReader.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::string toLowercase(const std::string& str) {
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
    return lower_str;
}

std::string FileReader::readTextFromFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return toLowercase(buffer.str());
}

std::vector<std::string> FileReader::readTextsFromDirectory(const std::string& directoryPath) {
    std::vector<std::string> texts;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            std::stringstream buffer;
            buffer << file.rdbuf();
            texts.push_back(toLowercase(buffer.str()));
        }
    }
    return texts;
}
