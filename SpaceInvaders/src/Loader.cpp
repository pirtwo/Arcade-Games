#include "Loader.h"
#include <string>
#include <fstream>
#include <sstream>

bool loadAtlasData(const char path[], std::map<std::string, sf::IntRect> &data)
{
    std::string line = "", token = "";
    std::fstream fs(path, std::fstream::in);
    if (!fs.is_open())
        return false;

    /*
        read atlas data file line by line 
    */
    while (std::getline(fs, line))
    {
        int i = 0;
        std::string textureName;
        sf::IntRect textureArea{0, 0, 0, 0};
        std::istringstream ss(line);

        /*
            read each field and fill current record
        */
        while (std::getline(ss, token, ','))
        {
            switch (i)
            {
            case 0:
                textureName = token;
                data[textureName] = textureArea;
                break;
            case 1:
                data[textureName].left = std::stoi(token);
                break;
            case 2:
                data[textureName].top = std::stoi(token);
                break;
            case 3:
                data[textureName].width = std::stoi(token);
                break;
            case 4:
                data[textureName].height = std::stoi(token);
                break;
            default:
                break;
            }
            i++;
        }
    }

    fs.close();
    return true;
}

bool loadLevelData(const char path[], std::vector<std::vector<int>> &data)
{
    std::string line = "", token = "";
    std::fstream fs(path, std::fstream::in);
    if (!fs.is_open())
        return false;

    /*
        read level data file line by line 
    */
    while (std::getline(fs, line))
    {
        std::vector<int> record;
        std::istringstream ss(line);

        /*
            read each field and fill current record
        */
        while (std::getline(ss, token, ','))
        {
            record.push_back(std::stoi(token));
        }

        data.push_back(record);
    }

    fs.close();
    return true;
}