#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <cmath> // For log2 function
#include <regex>
#include <numeric> // For std::accumulate
#include <algorithm> // For std::transform

namespace fs = std::filesystem;

// Function to convert a string to lowercase
std::string toLowercase(const std::string& str) {
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
    return lower_str;
}

// Function to read texts from a directory
std::vector<std::string> readTextsFromDirectory(const std::string& directoryPath) {
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

// Function to read a single text file, assumes UTF-8 encoding
std::string readTextFromFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return toLowercase(buffer.str());
}

// Function to train the context frequency model and calculate probabilities
std::unordered_map<std::string, std::unordered_map<char, double>> trainModel(const std::vector<std::string>& texts, int k, double alpha) {
    std::unordered_map<std::string, std::unordered_map<char, int>> counts;
    std::unordered_map<std::string, std::unordered_map<char, double>> probabilities;
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
    int alphabetSize = alphabet.size();

    // Collect counts
    std::regex regex("[^a-z0-9]");
    for (const auto& text : texts) {
        std::string filtered_text = std::regex_replace(text, regex, "");
        for (size_t i = 0; i + k < filtered_text.size(); ++i) {
            std::string context = filtered_text.substr(i, k);
            char next_char = filtered_text[i + k];
            counts[context][next_char]++;
        }
    }

    // Calculate probabilities
    for (const auto& context_entry : counts) {
        int total_count = std::accumulate(context_entry.second.begin(), context_entry.second.end(), 0, [](int sum, const auto& p) { return sum + p.second; });
        total_count += alpha * alphabetSize;
        for (char c : alphabet) {
            probabilities[context_entry.first][c] = (counts[context_entry.first].count(c) ? counts[context_entry.first].at(c) : 0) + alpha;
            probabilities[context_entry.first][c] /= total_count;
        }
    }

    // Handle unseen contexts
    for (const auto& text : texts) {
        std::string filtered_text = std::regex_replace(text, regex, "");
        for (size_t i = 0; i + k < filtered_text.size(); ++i) {
            std::string context = filtered_text.substr(i, k);
            if (probabilities.find(context) == probabilities.end()) {
                for (char c : alphabet) {
                    probabilities[context][c] = alpha / (alpha * alphabetSize);
                }
            }
        }
    }

    return probabilities;
}

// Function to calculate the total number of bits required for compression
int compressFileSize(const std::string& text, const std::unordered_map<std::string, std::unordered_map<char, double>>& probabilities, int k) {
    int totalBits = 0;
    std::regex regex("[^a-z0-9]");
    std::string filtered_text = std::regex_replace(toLowercase(text), regex, "");
    for (size_t i = 0; i + k < filtered_text.size(); ++i) {
        std::string context = filtered_text.substr(i, k);
        char next_char = filtered_text[i + k];
        if (probabilities.find(context) != probabilities.end() &&
            probabilities.at(context).find(next_char) != probabilities.at(context).end()) {
            double prob = probabilities.at(context).at(next_char);
            totalBits += -std::log2(prob);
        } else {
            double prob = 1.0 / 36; // Even distribution if unknown (26 letters + 10 digits)
            totalBits += -std::log2(prob);
        }
    }
    return totalBits;
}

// Function to calculate compression statistics
std::pair<double, double> calculateCompressionStats(const std::string& text, const std::unordered_map<std::string, std::unordered_map<char, double>>& probabilities, int k) {
    int originalSize = text.size() * 8; // Original size in bits (8 bits per character)
    int compressedSize = compressFileSize(text, probabilities, k);
    double compressionRatio = static_cast<double>(originalSize) / compressedSize;
    double compressionPercentage = (1 - (static_cast<double>(compressedSize) / originalSize)) * 100;
    return {compressionRatio, compressionPercentage};
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <path_to_not_rewritten_texts_folder> <path_to_rewritten_texts_folder> <path_to_target_text_file>\n";
        return 1;
    }

    // Load texts from directories and the target text file
    std::vector<std::string> rh = readTextsFromDirectory(argv[1]);
    std::vector<std::string> rc = readTextsFromDirectory(argv[2]);
    std::string t = readTextFromFile(argv[3]);

    // Define the context size and smoothing parameter
    int contextSize = 3;  // Adjust based on empirical data or performance requirements
    double alpha = 0.5;  // Smoothing parameter, can be adjusted as needed

    // Train models
    auto rhProbabilities = trainModel(rh, contextSize, alpha);
    auto rcProbabilities = trainModel(rc, contextSize, alpha);

    // Print rhProbabilities
    for (const auto& context_entry : rhProbabilities) {
        std::cout << "Context: " << context_entry.first << std::endl;
        for (const auto& prob_entry : context_entry.second) {
            std::cout << "Character: " << prob_entry.first << ", Probability: " << prob_entry.second << std::endl;
        }
        std::cout << std::endl;
    }

    // Calculate compression ratios and percentages
    auto [rhCompressionRatio, rhCompressionPercentage] = calculateCompressionStats(t, rhProbabilities, contextSize);
    auto [rcCompressionRatio, rcCompressionPercentage] = calculateCompressionStats(t, rcProbabilities, contextSize);

    // Output the results
    std::cout << "Compression ratio for 'not rewritten by ChatGPT': " << rhCompressionRatio << std::endl;
    std::cout << "Compression percentage for 'not rewritten by ChatGPT': " << rhCompressionPercentage << "%" << std::endl;
    std::cout << "Compression ratio for 'rewritten by ChatGPT': " << rcCompressionRatio << std::endl;
    std::cout << "Compression percentage for 'rewritten by ChatGPT': " << rcCompressionPercentage << "%" << std::endl;
    std::cout << "Classification result: " << (rhCompressionRatio > rcCompressionRatio ? "Not rewritten by ChatGPT" : "Rewritten by ChatGPT") << std::endl;

    return 0;
}
