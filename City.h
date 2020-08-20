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

    MainStreet() :
        start(glm::vec2(0)),
        end(glm::vec2(0)),
        westStreets(std::vector<SmallStreet*>()),
        eastStreets(std::vector<SmallStreet*>())
    {}
};

struct District {
    char type;
    std::vector<Block*> blocks;

    District() :
        type('0'),
        blocks(std::vector<Block*>())
    {}
};

struct Building {
    glm::vec2 pos; // vec2 is implying a position on the grid, vec3 implies in world space
    glm::vec3 size;
    District* type;

    Building() :
        pos(glm::vec2(0)),
        size(glm::vec3(1))
    {}
};

struct Block {
    glm::vec2 start, end;
    District* type;
    std::vector<Building*> buildings;

    Block() :
            start(glm::vec2(0)),
            end(glm::vec2(0)),
            buildings(std::vector<Building*>())
    {}
};

class City {
    char MAIN_STREET = 'M';
    char SMALL_STREET = 's';
    char DISTRICT_1 = '1';
    char DISTRICT_2 = '2';
    char DISTRICT_3 = '3';

    District* district_1;
    District* district_2;
    District* district_3;

public:
    City();
    City(int w, int h);

    std::vector<MainStreet*> mainStreets;
    std::vector<SmallStreet*> smallStreets;
    std::vector<Block*> cityBlocks;
    std::vector<District*> cityDistricts;
    std::vector<Building*> buildings;
private:
    std::vector<char> grid;
    int gridWidth;
    int gridHeight;

    PerlinNoise pn;

    std::vector<char> generateGrid();
    void generateDistricts();
    void generateMainStreets();
    void generateSmallStreets();
    void generateStreetBlocks();
    void generateBuildings();

    void traceLine(int line, float start, float end, bool vertical, char c);
    District* getDistrict(char type);
};


#endif //PA3_CITY_H
