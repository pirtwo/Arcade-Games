#include "Loader.h"
#include <string>
#include <fstream>
#include <sstream>

bool loadAtlasData(const char path[], std::map<std::string, std::vector<int>> &atlas)
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
        std::vector<int> textureArea{0, 0, 0, 0};
        std::istringstream ss(line);

        /*
            read each field and fill current record
        */
        while (std::getline(ss, token, ','))
        {
            if (i == 0)
            {
                textureName = token;
                atlas[textureName] = textureArea;
            }
            else
            {
                atlas[textureName][i - 1] = std::stoi(token);
            }
            i++;
        }
    }

    fs.close();
    return true;
}