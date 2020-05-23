#define BOOST_TEST_MODULE TEXT_ANALYZER
#include <boost/test/included/unit_test.hpp>

#include <cstdio>
#include <iostream>

#include "../src/text-analyzer.hpp"

BOOST_AUTO_TEST_SUITE(CrossReference)

const std::string inFilename = "test-in.txt";

const std::string outFilename = "test-out.txt";

void prepareFile(const std::string & filename, const std::string lines[], size_t count)
{
  auto out = std::ofstream{ filename };
  if (!out) {
    throw std::invalid_argument{ "Can't create file for testing" };
  }
  for (size_t i = 0u; i < count; ++i) {
    out << lines[i] << '\n';
  }
  out.close();
}

void testFile(const std::string & filename, const std::string expected[], size_t count)
{
  auto in = std::ifstream{ filename };
  if (!in) {
    throw std::invalid_argument{ "Can't open file for testing" };
  }
  auto str = std::string{ };
  std::getline(in, str, '\n'); // skip table header
  for (size_t i = 0u; i < count; ++i) {
    std::getline(in, str, '\n');
    BOOST_CHECK_EQUAL(str, expected[i]);
  }
  in.close();
}

BOOST_AUTO_TEST_CASE(Words_AreInAlphabeticOrder)
{
  std::string lines[] = { "b c a d", "e s g a" };
  prepareFile(inFilename, lines, 2u);
  auto a = TextAnalyzer{};
  a.analyze(inFilename);
  a.printAnalysis(outFilename); 
  std::string expected[] = {
      "a     1 2 ",
      "b     1 ",
      "c     1 ",
      "d     1 ",
      "e     2 ",
      "g     2 ",
      "s     2 "
  };
  testFile(outFilename, expected, 7u);
}

BOOST_AUTO_TEST_CASE(Lines_AreSorted)
{
  std::string lines[] = { "a b", "a", "a" };
  prepareFile(inFilename, lines, 3u);
  auto a = TextAnalyzer{};
  a.analyze(inFilename);
  a.printAnalysis(outFilename); 
  std::string expected[] = {
      "a     1 2 3 ",
      "b     1 "
  };
  testFile(outFilename, expected, 2u);
}

BOOST_AUTO_TEST_CASE(EmptyString_CountsTowardLineCount)
{
  std::string lines[] = { "a b c d a", "", "a b c d e f g h g", "", "b c e" };
  prepareFile(inFilename, lines, 5u);
  auto a = TextAnalyzer{};
  a.analyze(inFilename);
  a.printAnalysis(outFilename); 
  std::string expected[] = {
      "a     1 3 ",
      "b     1 3 5 ",
      "c     1 3 5 ",
      "d     1 3 ",
      "e     3 5 ",
      "f     3 ",
      "g     3 ",
      "h     3 "
  };
  testFile(outFilename, expected, 8);
}

BOOST_AUTO_TEST_CASE(LetterCase_DoesntMatter)
{
  std::string lines[] = { "ab AB", "Ab aB AB" };
  prepareFile(inFilename, lines, 2u);
  auto a = TextAnalyzer{};
  a.analyze(inFilename);
  a.printAnalysis(outFilename); 
  std::string expected[] = {
      "ab    1 2 ",
  };
  testFile(outFilename, expected, 1);
}

BOOST_AUTO_TEST_CASE(OtherSymbols_DontMatter)
{
  std::string lines[] = { "a?-<a a?в.a,_a" };
  prepareFile(inFilename, lines, 1u);
  auto a = TextAnalyzer{};
  a.analyze(inFilename);
  a.printAnalysis(outFilename); 
  std::string expected[] = {
      "a     1 ",
  };
  testFile(outFilename, expected, 1);
}

BOOST_AUTO_TEST_CASE(InvalidFileName_ThrowsInvalidArgument)
{
  auto a = TextAnalyzer{};
  BOOST_CHECK_THROW(a.analyze("nosuchfile"), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
