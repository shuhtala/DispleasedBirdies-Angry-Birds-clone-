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
#include <fstream>
#include <cstdint>
#include <utility>
#include "DynamicObject.hpp"
#include <Box2D/Box2D.h>
#include "Graphics.hpp"

class Map : public b2ContactListener{
public:
    Map();
    ~Map();
    size_t getBirdCount();
    
    //load the map from a file provided
    void createmap(const std::string &mapInfo);

    //create a new Box2D object according to the parameters provided
    void createObstacle( float x, float y, float xsize, float ysize);
    void createBird(float x, float y, float xsize, float ysize);
    void createEnemy(float x, float y, float xsize, float ysize);
    void Map::createStatic(float x, float y, float xsize, float ysize, int type);
    //get methods for the game objects
    b2Body* boxgetBird();
    std::vector<b2Body*> boxgetEnemies();
    std::vector<b2Body*> boxgetObstacles();
    std::vector<b2Body*> boxgetStatic();

    //called when a bird dies
    void nextBird();
    
    
    //methods for the physics simulation and killing off objects
    void step();
    void killBird(b2Body *b);
    void killObjects();

    //create a new bird
    void addBird();

    //write current statistics
    sf::Text writeStats();
    
    //get methods for birds, enemies and score
    int birdsLeft();
    int enemiesLeft(){
        return no_of_enemies_;
    }

    //return the current score
    int getScore(){
        return score_;
    }

    //add the amount to the current score
    void addScore(int add){
        score_ += add;
    }
    
    //get the type of the current bird
    int getBirdType();
private:
    std::vector<int> birdvector_;
    std::vector<DynamicObject> objects_;
    int score_;
    int no_of_birds_;
    int current_bird_;
    int no_of_enemies_;
    std::vector<b2Body*> obstacles_;
    std::vector<b2Body*> birds_;
    std::vector<b2Body*> enemies_;
    std::vector<b2Body*> static_;
    sf::Font font_;
    sf::Music enemyDiesSound;
    b2World* World_;
    std::vector<b2Body*> remove_;
};
