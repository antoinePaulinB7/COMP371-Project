//
// Created by antoine on 2020-08-20.
//
#include <glm/glm.hpp>
#include "vector"
#include "PerlinNoise.h"
#ifndef PA3_CITY_H
#define PA3_CITY_H

struct MainStreet;
struct Block;

struct SmallStreet {
    glm::vec2 start, end;
    MainStreet *westStreet, *eastStreet;
};

struct MainStreet {
    glm::vec2 start, end;
    std::vector<SmallStreet*> westStreets, eastStreets;
};

struct District {
    std::vector<Block*> blocks;
};

struct Building {
    glm::vec2 pos; // vec2 is implying a position on the grid, vec3 implies in world space
    glm::vec3 size;
    District* type;
};

struct Block {
    glm::vec2 start, end;
    District* type;
    std::vector<Building*> buildings;
};

class City {
public:
    City();
    City(int w, int h);

    std::vector<MainStreet> mainStreets;
    std::vector<SmallStreet> smallStreets;
    std::vector<Block> cityBlocks;
    std::vector<District> cityDistricts;
private:
    std::vector<int> grid;
    int gridWidth;
    int gridHeight;

    PerlinNoise pn;

    std::vector<int> generateGrid();
    void generateDistricts();
    void generateMainStreets();
    void generateSmallStreets();
    void generateStreetBlocks();
};


#endif //PA3_CITY_H
