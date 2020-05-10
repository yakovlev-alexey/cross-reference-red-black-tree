#include "text-analyzer.hpp"

#include <list>
#include <string>
#include <regex>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <functional>

#include "map.hpp"

TextAnalyzer::TextAnalyzer() : dictionary_{ }
{ }

const Map<std::string, std::list<int>> & TextAnalyzer::getDictionary() const
{
  return dictionary_;
}

void TextAnalyzer::analyze(const std::string & filename)
{
  dictionary_ = Map<std::string, std::list<int>>{ };
  std::ifstream is{ filename };
  if (!is) {
    throw std::invalid_argument{ "Can't open file " + filename };
  }

  analyze(is);

  is.close();
}

void TextAnalyzer::analyze(std::istream & is)
{
  dictionary_ = Map<std::string, std::list<int>>{ };

  std::regex word_regex{ "(\\w+)" };
  std::string line;

  for (int i = 1; is; ++i) {
    std::getline(is, line, '\n');
    auto words_begin = std::sregex_iterator(line.begin(), line.end(), word_regex);
    auto words_end = std::sregex_iterator();
    for (auto itr = words_begin; itr != words_end; ++itr) {
      auto word = itr->str();
      std::transform(word.begin(), word.end(), word.begin(), [ ](char c)
      {
        return std::tolower(c);
      });

      if (dictionary_.contains(word)) {
        dictionary_[word].push_back(i);
      } else {
        dictionary_.insert(word, { i });
      }
    }
  }
}

void TextAnalyzer::enumerateLines(const std::string & inFilename, const std::string & outFileName)
{
  if (inFilename == outFileName) {
    throw std::invalid_argument{
        "Can't output enumerated text to the file with the same name " + inFilename };
  }

  std::ifstream is{ inFilename };
  if (!is) {
    throw std::invalid_argument{ "Can't open file " + inFilename };
  }
  std::ofstream os{ outFileName };
  if (!os) {
    is.close();
    throw std::invalid_argument{ "Can't create output file " + outFileName };
  }

  enumerateLines(is, os);

  is.close();
  os.close();
}

void TextAnalyzer::enumerateLines(std::istream & is, std::ostream & os)
{
  std::string line;

  for (int i = 1; is; ++i) {
    std::getline(is, line, '\n');
    os << i << ") " << line << '\n';
  }
}

void TextAnalyzer::printAnalysis(const std::string & filename)
{
  std::ofstream os{ filename };
  if (!os) {
    throw std::invalid_argument{ "Can't create output file " + filename };
  }

  printAnalysis(os);

  os.close();
}

void TextAnalyzer::printAnalysis(std::ostream & os)
{
  for (auto itr = dictionary_.begin(); itr != dictionary_.end(); ++itr) {
    os << itr.key() << ':';
    std::for_each(itr.value().begin(), itr.value().end(),
        [&os] (int e) { os << ' ' << e; });
    os << '\n';
  }
}
