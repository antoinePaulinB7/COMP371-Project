//
// Created by antoine on 2020-08-20.
//

#include "City.h"

City::City() : City(50, 50) {}
City::City(int w, int h) {
    this->gridWidth = w;
    this->gridHeight = h;

    pn = PerlinNoise();

    mainStreets = std::vector<MainStreet>();
    smallStreets = std::vector<SmallStreet>();
    cityBlocks = std::vector<Block>();
    cityDistricts = std::vector<District>();

    grid = generateGrid();

    generateDistricts();

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

void City::generateDistricts() {
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            float noiseValue = pn.noise (
                    (float)i / (float) gridWidth,
                    (float) j / (float) gridHeight,
                    0.1f
                    );
            if (noiseValue > 0.6f) {
                grid[i * gridWidth + j] = 1;
            } else if (noiseValue > 0.35f) {
                grid[i * gridWidth + j] = 2;
            } else {
                grid[i * gridWidth + j] = 3;
            }
        }
    }
}