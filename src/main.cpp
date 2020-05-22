#include <string>
#include <limits>
#include <iostream>
#include <stdexcept>
#include "text-analyzer.hpp"

void analyzeText(const std::string & inFilename);

void enumerateText(const std::string & inFilename);

int main(int, char * [])
{
  std::cout << "Enter 1 to analyze a text file, enter 2 to enumerate lines in a text file:\n";
  char ch = 0;
  while (std::cin && (ch != '1') && (ch != '2')) {
    std::cin >> ch;
  }
  if (!std::cin) {
    std::cerr << "Unknown error reading input\n";
    return 1;
  }
  std::cin.ignore(std::numeric_limits<int>::max(), '\n');

  std::cout << "Enter text file name including extension (e.g. input.txt):\n";
  auto inFilename = std::string{};
  std::cin >> inFilename;
  if (!std::cin) {
    std::cerr << "Unknown error reading input\n";
    return 1;
  }

  try {
    if (ch == '1') {
      analyzeText(inFilename);
    } else {
      enumerateText(inFilename);
    }
  } catch (const std::invalid_argument & exc) {
    std::cerr << exc.what() << '\n';
    return 1;
  }

  return 0;
}

void analyzeText(const std::string & inFilename)
{
  TextAnalyzer textAnalyzer{ };
  textAnalyzer.analyze(inFilename);
  std::cout << "Enter 1 to output analysis to terminal "
            << "or enter output file name including extension: \n";
  auto outFilename = std::string{};
  std::cin >> outFilename;
  if (!std::cin) {
    throw std::invalid_argument{ "Unknown error reading input" };
  }
  if (outFilename == "1") {
    textAnalyzer.printAnalysis(std::cout);
  } else {
    textAnalyzer.printAnalysis(outFilename);
  }
}

void enumerateText(const std::string & inFilename)
{
  std::cout << "Enter output text file name including extension:\n";
  auto outFilename = std::string{};
  std::cin >> outFilename;
  if (!std::cin) {
    throw std::invalid_argument{ "Unknown error reading input" };
  }
  TextAnalyzer::enumerateLines(inFilename, outFilename);
}
