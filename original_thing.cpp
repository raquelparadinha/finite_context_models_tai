#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <cmath> // For log2 function
#include <numeric> // For std::accumulate
#include <algorithm> // For std::transform
#include <set>

namespace fs = std::filesystem;

std::string ALPHABET;
int ALPHABET_SIZE;

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

std::unordered_map<std::string, std::unordered_map<char, double>> trainModel(const std::vector<std::string>& texts, int k, double alpha) {
    std::unordered_map<std::string, std::unordered_map<char, int>> counts;
    std::unordered_map<std::string, std::unordered_map<char, double>> probabilities;
    std::set<char> alphabetSet;
    
    // Collect counts and build the alphabet
    for (const auto& text : texts) {
        if (text.size() < k) continue;
        for (size_t i = 0; i + k < text.size(); ++i) {
            std::string context = text.substr(i, k);
            char next_char = text[i + k];
            counts[context][next_char]++;
            alphabetSet.insert(next_char);
        }
    }

    // Convert the alphabet set to a string
    ALPHABET = std::string(alphabetSet.begin(), alphabetSet.end());
    ALPHABET_SIZE = ALPHABET.size();

    // Calculate probabilities
    for (const auto& context_entry : counts) {
        const auto& context = context_entry.first;
        const auto& char_counts = context_entry.second;
        int total_count = std::accumulate(char_counts.begin(), char_counts.end(), 0, [](int sum, const auto& p) { return sum + p.second; });
        total_count += alpha * ALPHABET_SIZE;
        for (char c : ALPHABET) {
            probabilities[context][c] = (char_counts.count(c) ? char_counts.at(c) : 0) + alpha;
            probabilities[context][c] /= total_count;
        }
    }

    // Handle unseen contexts
    for (const auto& context_entry : counts) {
        const auto& context = context_entry.first;
        if (probabilities.find(context) == probabilities.end()) {
            for (char c : ALPHABET) {
                probabilities[context][c] = alpha / (alpha * ALPHABET_SIZE);
            }
        }
    }

    for (auto& context_entry : probabilities) {
        double sum = 0;
        for (const auto& char_prob : context_entry.second) {
            sum += char_prob.second;
        }
        for (auto& char_prob : context_entry.second) {
            char_prob.second /= sum;
        }
    }
    return probabilities;
}

// Function to calculate the total number of bits required for compression
int compressFileSize(const std::string& text, const std::unordered_map<std::string, std::unordered_map<char, double>>& probabilities, int k) {
    int totalBits = 0;
    for (size_t i = 0; i + k < text.size(); ++i) {
        std::string context = text.substr(i, k);
        char next_char = text[i + k];
        if (probabilities.find(context) != probabilities.end() &&
            probabilities.at(context).find(next_char) != probabilities.at(context).end()) {
            double prob = probabilities.at(context).at(next_char);
            totalBits += -std::log2(prob);
        } else {
            double prob = 1.0 / ALPHABET_SIZE; // Even distribution if unknown
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
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <path_to_not_rewritten_texts_folder> <path_to_rewritten_texts_folder> <path_to_target_text_file> <alpha>\n";
        return 1;
    }

    // Load texts from directories and the target text file
    std::vector<std::string> rh = readTextsFromDirectory(argv[1]);
    std::vector<std::string> rc = readTextsFromDirectory(argv[2]);
    std::string t = readTextFromFile(argv[3]);
    double alpha = std::stod(argv[4]);

    // Define the context size
    int contextSize = 4;  // Adjust based on empirical data or performance requirements
    
    // Train models
    auto rhProbabilities = trainModel(rh, contextSize, alpha);
    auto rcProbabilities = trainModel(rc, contextSize, alpha);

    // Calculate compression ratios and percentages
    auto [rhCompressionRatio, rhCompressionPercentage] = calculateCompressionStats(t, rhProbabilities, contextSize);
    auto [rcCompressionRatio, rcCompressionPercentage] = calculateCompressionStats(t, rcProbabilities, contextSize);

    // Output the results
    std::cout << "Alpha: " << alpha << std::endl;
    std::cout << "Compression ratio for 'not rewritten by ChatGPT': " << rhCompressionRatio << std::endl;
    std::cout << "Compression percentage for 'not rewritten by ChatGPT': " << rhCompressionPercentage << "%" << std::endl;
    std::cout << "Compression ratio for 'rewritten by ChatGPT': " << rcCompressionRatio << std::endl;
    std::cout << "Compression percentage for 'rewritten by ChatGPT': " << rcCompressionPercentage << "%" << std::endl;
    std::cout << "Classification result: " << (rhCompressionRatio > rcCompressionRatio ? "Not rewritten by ChatGPT" : "Rewritten by ChatGPT") << std::endl;

    std::cout << "ALPHABET: " << ALPHABET << std::endl;
    std::cout << "ALPHABET_SIZE: " << ALPHABET_SIZE << std::endl;

    return 0;
}
