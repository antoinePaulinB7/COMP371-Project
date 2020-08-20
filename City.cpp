//
// Created by antoine on 2020-08-20.
//

#include "City.h"

struct MainStreet {
    glm::vec2 start, end;
    std::vector<SmallStreet> westStreets, eastStreets;
};

struct SmallStreet {
    glm::vec2 start, end;
    MainStreet westStreet, eastStreet;
};

struct District {

};

struct Building {
    glm::vec2 pos; // vec2 is implying a position on the grid, vec3 implies in world space
    glm::vec3 size;
    District type;
};

struct Block {
    glm::vec2 start, end;
    District type;
    std::vector<Building> buildings;
};

City::City() : City(50, 50) {}
City::City(int w, int h) {
    this->gridWidth = w;
    this->gridHeight = h;

    mainStreets = std::vector<MainStreet>();
    smallStreets = std::vector<SmallStreet>();
    cityBlocks = std::vector<Block>();
    cityDistricts = std::vector<District>();

    grid = generateGrid();
}

std::vector<int> City::generateGrid() {
    std::vector<int> g = std::vector<int>();

    for(int i = 0; i < gridWidth; i++) {
        for(int j = 0; j < gridHeight; j++) {
            g.push_back(0);
        }
    }

    return g;
}