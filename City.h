//
// Created by antoine on 2020-08-20.
//
#include <glm/glm.hpp>
#include "vector";
#ifndef PA3_CITY_H
#define PA3_CITY_H

struct MainStreet;
struct SmallStreet;
struct District;
struct Building;
struct Block;

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
    std::vector<int> generateGrid();
    void generateDistricts();
    void generateMainStreets();
    void generateSmallStreets();
    void generateStreetBlocks();
};


#endif //PA3_CITY_H
