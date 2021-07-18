#ifndef ATLAS_H_INCLUDE
#define ATLAS_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <map>

struct Atlas
{
    sf::Texture texture;
    std::map<std::string, sf::IntRect> data;
};

#endif