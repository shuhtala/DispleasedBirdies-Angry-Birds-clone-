#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <list>
#include <cstdint>
#include <utility>

/*
Class to store additional information about the Box2D objects
*/

class DynamicObject {
public:
    DynamicObject(int type, int hp, bool state, float xsize, float ysize);
    int getType();
    int gethp();
    bool getState();
    void setState(bool state);
    float getXsize();
    float getYsize();

protected:
    int type_;
    int hp_;
    bool isalive_;
    float xsize_;
    float ysize_;
};

