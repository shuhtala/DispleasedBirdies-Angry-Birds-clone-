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
#include "DynamicObject.hpp"

/*
Class to store additional information about the Box2D objects
*/

DynamicObject::DynamicObject(int type, int hp, bool state, float xsize, float ysize)
    : type_(type), hp_(hp), isalive_(state), xsize_(xsize), ysize_(ysize){}

float DynamicObject::getXsize(){
    return xsize_;
}

float DynamicObject::getYsize(){
    return ysize_;
}

int DynamicObject::getType(){
    return type_;
}

int DynamicObject::gethp(){
    return hp_;
}

bool DynamicObject::getState(){
    return isalive_;
}

void DynamicObject::setState(bool state){
    isalive_ = state;
    return;
}