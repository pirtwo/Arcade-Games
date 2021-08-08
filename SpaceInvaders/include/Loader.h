#ifndef LOADER_H_INCLUDE
#define LOADER_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

bool loadAtlasData(const char path[], std::map<std::string, sf::IntRect> &data);
bool loadLevelData(const char path[], std::vector<std::vector<int>> &data);

#endif