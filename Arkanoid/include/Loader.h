#ifndef LOADER_H_INCLUDE
#define LOADER_H_INCLUDE

#include <map>
#include <vector>

bool loadAtlasData(const char path[], std::map<std::string, std::vector<int>> &atlas);
bool loadLevelData(const char path[], std::vector<std::vector<int>>);

#endif