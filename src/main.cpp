#include <iostream>
#include <string>
#include <list>
#include "map.hpp"

void output(const Map<std::string, std::list<int>> & map)
{
  for (Map<std::string, std::list<int>>::const_iterator itr = map.begin(); itr != map.end(); ++itr) {
    std::cout << '\n' << itr.key() << '\n';
    std::for_each(itr.value().begin(), itr.value().end(), [] (int e) { std::cout << e << ' '; });
  }
}

int main(int, char *[])
{
  Map<std::string, std::list<int>> map{};
  map.insert("e", std::list<int>{ 1, 3 });
  map.insert("b", std::list<int>{ 3, 5 });
  map.insert("g", std::list<int>{ 3, 5 });
  map.insert("b", std::list<int>{ 3, 5 });
  map.insert("v", std::list<int>{ 3, 5 });
  map.insert("f", std::list<int>{ 3, 5 });
  map.insert("z", std::list<int>{ 3, 5 });
  output(map);
  return 0;
}