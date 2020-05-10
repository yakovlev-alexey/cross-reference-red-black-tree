#ifndef CROSS_REFS_TEXT_ANALYZER
#define CROSS_REFS_TEXT_ANALYZER

#include <ios>
#include <list>
#include <string>

#include "map.hpp"

class TextAnalyzer
{

  public:

    TextAnalyzer();

    const Map<std::string, std::list<int>> & getDictionary() const;

    void analyze(const std::string & filename);

    void analyze(std::istream & is);

    static void enumerateLines(const std::string & inFilename, const std::string & outFileName);

    static void enumerateLines(std::istream & is, std::ostream & os);

    void printAnalysis(const std::string & filename);

    void printAnalysis(std::ostream & os);

  private:

    Map<std::string, std::list<int>> dictionary_;

};


#endif
