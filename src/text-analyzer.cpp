#include "text-analyzer.hpp"

#include <regex>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <functional>

#include "list.hpp"
#include "map.hpp"

TextAnalyzer::TextAnalyzer() :
    dictionary{ }
{ }

TextAnalyzer::TextAnalyzer(TextAnalyzer && other) noexcept:
    dictionary{ std::move(other.dictionary) }
{ }

TextAnalyzer & TextAnalyzer::operator=(TextAnalyzer && other) noexcept
{
  dictionary = std::move(other.dictionary);
  return *this;
}

const Map<std::string, List<int>> & TextAnalyzer::getDictionary() const
{
  return dictionary;
}

void TextAnalyzer::analyze(const std::string & filename)
{
  dictionary = Map<std::string, List<int>>{ };
  auto is = std::ifstream{ filename };
  if (!is) {
    throw std::invalid_argument{ "Can't open file " + filename };
  }

  analyze(is);

  is.close();
}

void TextAnalyzer::analyze(std::istream & is)
{
  dictionary = Map<std::string, List<int>>{ };

  auto word_regex = std::regex{ "[a-zA-Z0-9]+" };
  auto line = std::string{ };

  for (int i = 1; is; ++i) {

    std::getline(is, line, '\n');

    auto words_begin = std::sregex_iterator{ line.begin(), line.end(), word_regex };
    auto words_end = std::sregex_iterator{ };

    for (auto itr = words_begin; itr != words_end; ++itr) {
      auto word = itr->str();
      std::transform(word.begin(), word.end(), word.begin(),
          [ ] (char c) { return std::tolower(c); });

      if (dictionary.contains(word)) {
        dictionary[word].push_back(i);
      } else {
        dictionary.insert(word, { i });
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

  auto is = std::ifstream{ inFilename };
  if (!is) {
    throw std::invalid_argument{ "Can't open file " + inFilename };
  }

  auto os = std::ofstream{ outFileName };
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
  auto line = std::string{ };

  for (int i = 1; is; ++i) {
    std::getline(is, line, '\n');
    os << i << ") " << line << '\n';
  }
}

void TextAnalyzer::printAnalysis(const std::string & filename)
{
  auto os = std::ofstream{ filename };
  if (!os) {
    throw std::invalid_argument{ "Can't create output file " + filename };
  }

  printAnalysis(os);

  os.close();
}

namespace
{
  std::string generateSpaces(size_t length);
}

void TextAnalyzer::printAnalysis(std::ostream & os)
{
  auto word = std::string{ "Word" };
  auto colwidth = word.length();
  for (auto itr = dictionary.begin(); itr != dictionary.end(); ++itr) {
    colwidth = std::max(itr.key().length(), colwidth);
  }
  const auto margin = size_t{ 2u };
  os << "Word" << generateSpaces(colwidth - word.length() + margin) << "Lines\n"; 
  for (auto itr = dictionary.begin(); itr != dictionary.end(); ++itr) {
    os << itr.key() << generateSpaces(colwidth - itr.key().length() + margin);
    std::for_each(itr.value().begin(), itr.value().end(),
        [&os] (int e) { os << e << ' '; });
    os << '\n';
  }
}

namespace
{
  std::string generateSpaces(size_t length)
  {
    auto spaces = std::string{ };
    for (size_t i = 0u; i < length; ++i) {
      spaces += " ";
    }
    return spaces;
  }
}
