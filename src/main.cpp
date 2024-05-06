#include <iostream>
#include "FileReader.hpp"
#include "MarkovModel.hpp"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <path_to_not_rewritten_texts_folder> <path_to_rewritten_texts_folder> <path_to_target_text_file> <alpha>\n";
        return 1;
    }

    std::vector<std::string> rh = FileReader::readTextsFromDirectory(argv[1]);
    std::vector<std::string> rc = FileReader::readTextsFromDirectory(argv[2]);
    std::string t = FileReader::readTextFromFile(argv[3]);
    double alpha = std::stod(argv[4]);

    int contextSize = 4;
    MarkovModel model(contextSize, alpha);
    std::string result = model.classify(t, rh, rc);

    std::cout << "Classification result: " << result << std::endl;
    std::cout << "ALPHABET: " << MarkovModel::getAlphabet() << std::endl;
    std::cout << "ALPHABET_SIZE: " << MarkovModel::getAlphabetSize() << std::endl;

    return 0;
}
