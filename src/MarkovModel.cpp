#include "MarkovModel.hpp"
#include <cmath>
#include <numeric>
#include <algorithm>
#include <set>

std::string MarkovModel::ALPHABET;
int MarkovModel::ALPHABET_SIZE;

MarkovModel::MarkovModel(int contextSize, double alpha)
    : contextSize(contextSize), alpha(alpha) {}

void MarkovModel::trainModel(const std::vector<std::string>& texts) {
    std::unordered_map<std::string, std::unordered_map<char, int>> counts;
    std::set<char> alphabetSet;

    // Collect counts and build the alphabet
    for (const auto& text : texts) {
        if (text.size() < contextSize) continue;
        for (size_t i = 0; i + contextSize < text.size(); ++i) {
            std::string context = text.substr(i, contextSize);
            char next_char = text[i + contextSize];
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
}

int MarkovModel::compressFileSize(const std::string& text) {
    int totalBits = 0;
    for (size_t i = 0; i + contextSize < text.size(); ++i) {
        std::string context = text.substr(i, contextSize);
        char next_char = text[i + contextSize];
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

std::pair<double, double> MarkovModel::calculateCompressionStats(const std::string& text) {
    int originalSize = text.size() * 8; // Original size in bits (8 bits per character)
    int compressedSize = compressFileSize(text);
    double compressionRatio = static_cast<double>(originalSize) / compressedSize;
    double compressionPercentage = (1 - (static_cast<double>(compressedSize) / originalSize)) * 100;
    return {compressionRatio, compressionPercentage};
}

std::string MarkovModel::classify(const std::string& text, const std::vector<std::string>& rhTexts, const std::vector<std::string>& rcTexts) {
    MarkovModel rhModel(contextSize, alpha);
    MarkovModel rcModel(contextSize, alpha);

    rhModel.trainModel(rhTexts);
    rcModel.trainModel(rcTexts);

    auto [rhCompressionRatio, rhCompressionPercentage] = rhModel.calculateCompressionStats(text);
    auto [rcCompressionRatio, rcCompressionPercentage] = rcModel.calculateCompressionStats(text);

    return (rhCompressionRatio > rcCompressionRatio ? "Human Text" : "ChatGPT Text");
}

std::string MarkovModel::getAlphabet() {
    return ALPHABET;
}

int MarkovModel::getAlphabetSize() {
    return ALPHABET_SIZE;
}
