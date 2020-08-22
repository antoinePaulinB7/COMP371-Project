//
// Created by antoine on 2020-08-20.
//

#include "City.h"
#include <iostream>
#include <algorithm>

City::City() : City(25, 25, glm::vec3(0)) {}
City::City(int w, int h, glm::vec3 offset) {
    this->gridWidth = w;
    this->gridHeight = h;
    this->offset = offset;

    pn = PerlinNoise();

    this->mainStreets = std::vector<MainStreet*>();
    this->smallStreets = std::vector<SmallStreet*>();
    this->cityBlocks = std::vector<Block*>();
    this->cityDistricts = std::vector<District*>();
    this->buildings = std::vector<Building*>();

    grid = generateGrid();

    generateDistricts();

    generateMainStreets();

    generateSmallStreets();

    generateStreetBlocks();

    generateBuildings();

	/*
    for(int i = 0; i < this->gridWidth; i++) {
        for(int j = 0; j < this->gridHeight; j++) {
            std::cout << this->grid[(j * this->gridWidth) + i];
        }
        std::cout << std::endl;
    }
	*/
}

std::vector<char> City::generateGrid() {
    std::vector<char> g = std::vector<char>();

    for(int i = 0; i < this->gridWidth; i++) {
        for(int j = 0; j < this->gridHeight; j++) {
            g.push_back(' ');
        }
    }

    return g;
}

void City::generateDistricts() {
    this->district_1 = new District();
    this->district_1->type = '1';
    this->cityDistricts.push_back(this->district_1);

    this->district_2 = new District();
    this->district_2->type = '2';
    this->cityDistricts.push_back(this->district_2);

    this->district_3 = new District();
    this->district_3->type = '3';
    this->cityDistricts.push_back(this->district_3);


    for (int i = 0; i < this->gridWidth; i++) {
        for (int j = 0; j < this->gridHeight; j++) {
            float noiseValue = pn.noise (
                    (float) (i + offset.x) / (float) this->gridWidth,
                    0.1f + offset.y,
                    (float) (j + offset.z) / (float) this->gridHeight
                    );

            noiseValue = (noiseValue + 1.0f) / 2.0f;

            if (noiseValue > 0.6f) {
                this->grid[(j * this->gridWidth) + i] = DISTRICT_1;
            } else if (noiseValue > 0.35f) {
                this->grid[(j * this->gridWidth) + i] = DISTRICT_2;
            } else {
                this->grid[(j * this->gridWidth) + i] = DISTRICT_3;
            }
        }
    }
}

void City::generateMainStreets() {
    int lastRoad = -1;

    for (int i = 0; i < this->gridWidth; i++) {
        float noiseValue = pn.noise(
                (float)(i + offset.x) * 35.0f / (float)this->gridWidth,
                0.1f + offset.y,
                0.1f + offset.z
        );

        noiseValue = (noiseValue + 1.0f) / 2.0f;

        noiseValue *= (i - lastRoad);

        if (noiseValue > 2.0f) {
            traceLine(i, -1, -1, true, MAIN_STREET);
            lastRoad = i;
            MainStreet* road = new MainStreet();
            road->start = glm::vec2(i, 0);
            road->end = glm::vec2(i, this->gridHeight);
            this->mainStreets.push_back(road);
        }
    }

    MainStreet* stopper = new MainStreet();
    stopper->start = glm::vec2(this->gridWidth, 0);
    stopper->end = glm::vec2(this->gridWidth, this->gridHeight);

    this->mainStreets.push_back(stopper); //quick fix to allow for small streets to be generated after the last main street
}

void City::generateSmallStreets() {
    for (int i = 0; i < this->mainStreets.size(); i++) {
        int lastSmallRoad = -1;

        for (int j = 0; j < this->gridWidth; j++) {
            float noiseValue = pn.noise(
                    (float)this->mainStreets[i]->start.x * 35.0f / (float)this->gridWidth,
                    (float)j * 15.0f / (float)this->gridHeight,
                    0
            );

            noiseValue = (noiseValue + 1.0f) / 2.0f;

            noiseValue *= (j - lastSmallRoad);

            if (noiseValue > 2.0f) {
                int start_x, start_y, end_x, end_y;
                start_x = (int)(((i - 1 >= 0) ? this->mainStreets[i-1]->start.x:-1) + 1);
                start_y = j;

                end_x = (int)((i < this->mainStreets.size() - 1) ? (this->mainStreets[i]->start.x - 1) : (this->gridWidth - 1));
                end_y = j;

                traceLine(j, start_x, end_x, false, SMALL_STREET);
                lastSmallRoad = j;

                SmallStreet* road = new SmallStreet();
                road->start = glm::vec2(
                        start_x,
                        j
                        );

                road->end = glm::vec2(
                        end_x,
                        j
                        );

                if(i-1>=0) {
                    this->mainStreets[i-1]->eastStreets.push_back(road);
                    road->westStreet = this->mainStreets[i-1];
                }

                this->mainStreets[i]->westStreets.push_back(road);
                road->eastStreet = this->mainStreets[i];

                this->smallStreets.push_back(road);
            }
        }
    }
}

void City::generateStreetBlocks() {
    for(int i = 0; i < this->mainStreets.size(); i++) {
        MainStreet* ms = this->mainStreets[i];

        for(int j = 0; j < ms->westStreets.size() + 1; j++) {
            SmallStreet* s1 = (j - 1 >= 0) ? ms->westStreets[j-1] : nullptr;
            SmallStreet* s2 = (j < ms->westStreets.size()) ? ms->westStreets[j] : nullptr;

            int start_x, start_y, end_x, end_y;

            if(s1 == nullptr) {
                start_x = (int) s2->start.x;
                start_y = 0;
                end_x = (int) s2->end.x;
                end_y = (int) s2->end.y - 1;
            } else if(s1 && s2){
                start_x = (int) s1->start.x;
                start_y = (int) s1->start.y + 1;
                end_x = (int) s1->end.x;
                end_y = (int) s2->start.y - 1;
            } else if(s1->start.y < this->gridHeight - 1){
                start_x = (int) s1->start.x;
                start_y = (int) s1->start.y + 1;
                end_x = (int) s1->end.x;
                end_y = (int) this->gridHeight - 1;
            } else {
                //std::cout << "skip because no start case" << std::endl;
                continue;
            }

            start_x = std::min(start_x, this->gridWidth-1);
            start_y = std::min(start_y, this->gridHeight-1);
            end_x = std::min(end_x, this->gridWidth-1);
            end_y = std::min(end_y, this->gridHeight-1);

            District* type = getDistrict(this->grid[(start_y * this->gridWidth) + start_x]);

            //std::cout << type << std::endl;


            Block* block = new Block();

            if (type == nullptr) {
				/*
                for(int i = 0; i < this->gridWidth; i++) {
                    for(int j = 0; j < this->gridHeight; j++) {
                        std::cout << this->grid[(j * this->gridWidth) + i];
                    }
                    std::cout << std::endl;
                }

                std::cout << start_x << " " << start_y << std::endl;
                std::cout << this->grid[(start_y * this->gridWidth) + start_x] << std::endl;
                std::cout << ((this->gridHeight-1) * this->gridWidth) + (this->gridWidth-1) << std::endl;
                std::cout << this->grid.size() << std::endl;
                std::cout << this->grid[((this->gridHeight-1) * this->gridWidth) + (this->gridWidth-1)] << std::endl;
				*/
            } else {
                type->blocks.push_back(block);

                block->type = type;
                block->start = glm::vec2(start_x, start_y);
                block->end = glm::vec2(end_x, end_y);

                this->cityBlocks.push_back(block);
            }
        }
    }
}

void City::generateBuildings() {
    int numBuildings = 0;

    for (int b = 0; b < this->cityBlocks.size(); b++) {
        Block* block = this->cityBlocks[b];


        for(int i = block->start.x; i <= block->end.x; i++) {
            for(int j = block->start.y; j <= block->end.y; j++) {
                if (i != block->start.x && i != block->end.x
                && j != block->start.y && j != block->end.y) {
                    this->grid[(j * this->gridWidth) + i] = ' ';
                } else {
                    numBuildings++;

                    Building* building = new Building();
                    building->pos = glm::vec2((float)i, (float) j);
                    building->size.y += 1.0f + pn.noise((float)i + offset.x,
                                                        0.1f + offset.y,
                                                        (float)j + offset.z);
                    building->type = block->type;

                    this->grid[(j * this->gridWidth) + i] = BUILDING;

                    block->buildings.push_back(building);
                    this->buildings.push_back(building);
                }
            }
        }
    }
}

void City::traceLine(int line, float start, float end, bool vertical, char c) {
    if (vertical) {
        for (int j = (int)(start > 0 ? start : 0); j <= (end > 0 ? end : this->gridHeight - 1); j++) {
            this->grid[(j * this->gridWidth) + line] = c;
        }
    } else { // horizontal
        for (int i = (int)(start > 0 ? start : 0); i <= (end > 0 ? end : this->gridWidth - 1); i++) {
            this->grid[(line * this->gridWidth) + i] = c;
        }
    }

}

District* City::getDistrict(char type) {
    switch (type) {
        case '1':
            return this->district_1;
        case '2':
            return this->district_2;
        case '3':
            return this->district_3;
    }

    return nullptr;
}