#ifndef MARKOVMODEL_HPP
#define MARKOVMODEL_HPP

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class MarkovModel {
public:
    MarkovModel(int contextSize, double alpha);

    void trainModel(const std::vector<std::string>& texts);
    std::pair<double, double> calculateCompressionStats(const std::string& text);
    std::string classify(const std::string& text, const std::vector<std::string>& rhTexts, const std::vector<std::string>& rcTexts);

    static std::string getAlphabet();
    static int getAlphabetSize();

private:
    int contextSize;
    double alpha;
    std::unordered_map<std::string, std::unordered_map<char, double>> probabilities;
    static std::string ALPHABET;
    static int ALPHABET_SIZE;
    
    int compressFileSize(const std::string& text);
};

#endif
