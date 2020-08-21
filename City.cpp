//
// Created by antoine on 2020-08-20.
//

#include "City.h"
#include <iostream>

City::City() : City(25, 25) {}
City::City(int w, int h) {
    this->gridWidth = w;
    this->gridHeight = h;

    pn = PerlinNoise();

    mainStreets = std::vector<MainStreet*>();
    smallStreets = std::vector<SmallStreet*>();
    cityBlocks = std::vector<Block*>();
    cityDistricts = std::vector<District*>();
    buildings = std::vector<Building*>();

    grid = generateGrid();

    generateDistricts();

    generateMainStreets();

    for(int i = 0; i < gridWidth; i++) {
        for(int j = 0; j < gridHeight; j++) {
            std::cout << grid[j * gridWidth + i];
        }
        std::cout << std::endl;
    }

    generateSmallStreets();

    generateStreetBlocks();

    generateBuildings();

    for(int i = 0; i < gridWidth; i++) {
        for(int j = 0; j < gridHeight; j++) {
            std::cout << grid[j * gridWidth + i];
        }
        std::cout << std::endl;
    }

    std::cout << mainStreets.size() << std::endl;
    std::cout << smallStreets.size() << std::endl;
    std::cout << cityBlocks.size() << std::endl;
    std::cout << buildings.size() << std::endl;

    for(int d = 0; d < cityDistricts.size(); d++) {
        std::cout << cityDistricts[d]->blocks.size() << std::endl;
    }

}

std::vector<char> City::generateGrid() {
    std::vector<char> g = std::vector<char>();

    for(int i = 0; i < gridWidth; i++) {
        for(int j = 0; j < gridHeight; j++) {
            g.push_back(' ');
        }
    }

    return g;
}

void City::generateDistricts() {
    district_1 = new District();
    district_1->type = '1';
    cityDistricts.push_back(district_1);

    district_2 = new District();
    district_2->type = '2';
    cityDistricts.push_back(district_2);

    district_3 = new District();
    district_3->type = '3';
    cityDistricts.push_back(district_3);


    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            float noiseValue = pn.noise (
                    (float) i / (float) gridWidth,
                    (float) j / (float) gridHeight,
                    0.1f
                    );

            noiseValue = (noiseValue + 1.0f) / 2.0f;

            if (noiseValue > 0.6f) {
                grid[i * gridWidth + j] = DISTRICT_1;
            } else if (noiseValue > 0.35f) {
                grid[i * gridWidth + j] = DISTRICT_2;
            } else {
                grid[i * gridWidth + j] = DISTRICT_3;
            }
        }
    }
}

void City::generateMainStreets() {
    int lastRoad = -1;

    for (int i = 0; i < gridWidth; i++) {
        float noiseValue = pn.noise(
                (float)i * 35.0f / (float)gridWidth,
                0.1f,
                0.1f
        );

        noiseValue = (noiseValue + 1.0f) / 2.0f;

        noiseValue *= (i - lastRoad);

        if (noiseValue > 2.0f) {
            traceLine(i, -1, -1, true, MAIN_STREET);
            lastRoad = i;
            MainStreet* road = new MainStreet();
            road->start = glm::vec2(i, 0);
            road->end = glm::vec2(i, gridHeight);
            mainStreets.push_back(road);
        }
    }

    MainStreet* stopper = new MainStreet();
    stopper->start = glm::vec2(gridWidth, 0);
    stopper->end = glm::vec2(gridWidth, gridHeight);

    mainStreets.push_back(stopper); //quick fix to allow for small streets to be generated after the last main street
}

void City::generateSmallStreets() {
    for (int i = 0; i < mainStreets.size(); i++) {
        int lastSmallRoad = -1;

        for (int j = 0; j < gridWidth; j++) {
            float noiseValue = pn.noise(
                    (float)mainStreets[i]->start.x * 35.0f / (float)gridWidth,
                    (float)j * 15.0f / (float)gridHeight,
                    0
            );

            noiseValue = (noiseValue + 1.0f) / 2.0f;

            noiseValue *= (j - lastSmallRoad);

            if (noiseValue > 2.0f) {
                traceLine(j, (i-1>=0?mainStreets[i-1]->start.x:-1)+1, ((i>=gridWidth)?gridWidth-2:mainStreets[i]->start.x), false, SMALL_STREET);
                lastSmallRoad = j;

                SmallStreet* road = new SmallStreet();
                road->start = glm::vec2(
                        (int)((i-1>=0?mainStreets[i-1]->start.x:-1) + 1),
                        j
                        );

                road->end = glm::vec2(
                        (int)((i>=gridWidth)?gridWidth-2:mainStreets[i]->start.x),
                        j
                        );

                std::cout << i << " " << road->start.x << road->end.x << std::endl;
                //new SmallStreet((int)(i-1>=0?listOfMainStreets.get(i-1).start.x:-1)+1,j, (int)listOfMainStreets.get(i).start.x, j);

                if(i-1>=0) {
                    mainStreets[i-1]->eastStreets.push_back(road);
                    road->westStreet = mainStreets[i-1];
                }

                mainStreets[i]->westStreets.push_back(road);
                road->eastStreet = mainStreets[i];

                smallStreets.push_back(road);
            }
        }
    }
}

void City::generateStreetBlocks() {
    for(int i = 0; i < mainStreets.size(); i++) {
        MainStreet* ms = mainStreets[i];

        for(int j = 0; j < ms->westStreets.size() + 1; j++) {
            SmallStreet* s1 = j - 1 >= 0 ? ms->westStreets[j-1] : nullptr;
            SmallStreet* s2 = j < ms->westStreets.size() ? ms->westStreets[j] : nullptr;

            int start_x, start_y, end_x, end_y;

            if(s1 == nullptr) {
                start_x = (int) s2->start.x;
                start_y = 0;
                end_x = (int) s2->end.x - 1;
                end_y = (int) s2->end.y - 1;
            } else if(s1 && s2){
                start_x = (int) s1->start.x;
                start_y = (int) s1->start.y + 1;
                end_x = (int) s2->end.x - 1;
                end_y = (int) s2->end.y - 1;
            } else if(s1->start.y < gridHeight - 1){
                start_x = (int) s1->start.x;
                start_y = (int) s1->start.y + 1;
                end_x = (int) s1->end.x - 1;
                end_y = (int) gridHeight - 1;
            } else {
                std::cout << "skip because no start case" << std::endl;
                continue;
            }

            if(start_y >= gridHeight) {
                std::cout << "skip because start y too big" << std::endl;
                continue;
            } else {
                std::cout << grid[start_x * gridWidth + start_y] << std::endl;

                District* type = getDistrict(grid[start_x * gridWidth + start_y]);

                std::cout << type << std::endl;


                Block* block = new Block();

                type->blocks.push_back(block);

                block->type = type;
                block->start = glm::vec2(start_x, start_y);
                block->end = glm::vec2(end_x, end_y);

                cityBlocks.push_back(block);
            }

        }
    }
}

void City::generateBuildings() {
    for (int b = 0; b < cityBlocks.size(); b++) {
        Block* block = cityBlocks[b];

        for(int i = block->start.x; i <= block->end.x; i++) {
            for(int j = block->start.y; j <= block->end.y; j++) {
                Building* building = new Building();
                building->pos = glm::vec2(i,j);
                building->size.y += 1.0f + pn.noise((float)i,
                                             (float)j,
                                             0.1f);
                building->type = block->type;

                block->buildings.push_back(building);
                buildings.push_back(building);
            }
        }
    }
}

void City::traceLine(int line, float start, float end, bool vertical, char c) {
    if (vertical) {
        for (int j = (int)(start > 0 ? start : 0); j < (end > 0 ? end : gridHeight); j++) {
            grid[line * gridWidth + j] = c;
        }
    } else { // horizontal
        for (int i = (int)(start > 0 ? start : 0); i < (end > 0 ? end : gridWidth); i++) {
            grid[i * gridWidth + line] = c;
        }
    }

}

District* City::getDistrict(char type) {
    switch (type) {
        case '1':
            return district_1;
        case '2':
            return district_2;
        case '3':
            return district_3;
    }
}