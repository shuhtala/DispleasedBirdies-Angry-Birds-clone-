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
#include <fstream>
#include <utility>
#include "DynamicObject.hpp"
#include <Box2D/Box2D.h>
#include "Map.hpp"

static const float SCALE = 30.f;
Map::Map() {
    //create a new Box2D world for game objects
    b2Vec2 Gravity(0.0f, 9.8f);
    b2World* World = new b2World(Gravity);
    World_ = World;
    score_ = 0;
    current_bird_ = 0;
    no_of_birds_ = 0;
    no_of_enemies_ = 0;
}
Map::~Map(){}



//create a new dynamic obstacle, (type 1) and add it to the list of obstacles
void Map::createObstacle(float x, float y, float xsize, float ysize)
{
    //create a new BynamicObject to store additional information, set it as the
    //userdata of the b2Body
    DynamicObject *ptr = new DynamicObject(1, 100, true, xsize, ysize);
    

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/30.f, y/30.f);
    BodyDef.type = b2_dynamicBody;
    uintptr_t ptrt = reinterpret_cast<uintptr_t>(ptr);
    b2Body* Body = World_->CreateBody(&BodyDef);
    
    Body->GetUserData().pointer = ptrt; 
    b2PolygonShape Shape;
    Shape.SetAsBox((xsize)/SCALE, (ysize)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    obstacles_.push_back(Body);

}

//create a new bird, (type 2) and add it to the list of birds
void Map::createBird(float x, float y, float xsize, float ysize)
{
    //create a new BynamicObject to store additional information, set it as the
    //userdata of the b2Body
    DynamicObject *ptr = new DynamicObject(2, 100, true, xsize, ysize);
    

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/30.f, y/30.f);
    BodyDef.type = b2_dynamicBody;
    uintptr_t ptrt = reinterpret_cast<uintptr_t>(ptr);
    b2Body* Body = World_->CreateBody(&BodyDef);
    
    Body->GetUserData().pointer = ptrt; 
    b2CircleShape Shape;
    Shape.m_p.Set(0,0);
    Shape.m_radius = (32.f/2)/SCALE;
    b2FixtureDef FixtureDef;
    FixtureDef.density = 2.f;
    FixtureDef.friction = 5.f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    birds_.push_back(Body);
}

//create a new enemy, (type 3) and add it to the list of enemies
void Map::createEnemy(float x, float y, float xsize, float ysize)
{
    //create a new BynamicObject to store additional information, set it as the
    //userdata of the b2Body
    DynamicObject *ptr = new DynamicObject(3, 100, true, xsize, ysize);
    
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/30.f, y/30.f);
    BodyDef.type = b2_dynamicBody;
    uintptr_t ptrt = reinterpret_cast<uintptr_t>(ptr);
    b2Body* Body = World_->CreateBody(&BodyDef);
    
    Body->GetUserData().pointer = ptrt; 
    b2CircleShape Shape;
    Shape.m_p.Set(0,0);
    Shape.m_radius = (32.f/2)/SCALE;
    b2FixtureDef FixtureDef;
    FixtureDef.density = 3.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    enemies_.push_back(Body);
    no_of_enemies_++;
}

//create a static object, if type = 5, it is a static obstacle
void Map::createStatic(float x, float y, float xsize, float ysize, int type){
    //create a new BynamicObject to store additional information, set it as the
    //userdata of the b2Body
    DynamicObject *ptr = new DynamicObject(type, 100, true, xsize, ysize);

    uintptr_t ptrt = reinterpret_cast<uintptr_t>(ptr);
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x/30.f, y/30.f);
    BodyDef.type = b2_staticBody;
    b2Body* Body = World_->CreateBody(&BodyDef);
    Body->GetUserData().pointer = ptrt; 
    b2PolygonShape Shape;
    Shape.SetAsBox((xsize)/SCALE, (ysize)/SCALE); 
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.f;  
    FixtureDef.shape = &Shape; 
    Body->CreateFixture(&FixtureDef);
    static_.push_back(Body);
}

//get the current bird
b2Body* Map::boxgetBird(){
    return birds_[current_bird_];
}

//get a vector of all the other game objects
std::vector<b2Body*> Map::boxgetEnemies(){
    return enemies_;
}
std::vector<b2Body*> Map::boxgetObstacles(){
    return obstacles_;
}

std::vector<b2Body*> Map::boxgetStatic(){
    return static_;
}

//load a new level from a text file
void Map::createmap(const std::string &mapInfo){
    std::ifstream istr;
    istr.open(mapInfo);
    std::string line;
    while(std::getline(istr, line)){
        std::stringstream stream(line); 
        char type;
        float xpos, ypos, xsize, ysize;
        int staticType;
        stream >> type;
        int birdtype;
        if(type =='O'){
            stream >> xpos >> ypos >> xsize >> ysize;
            createObstacle(xpos, ypos, xsize, ysize);
        } else if(type =='E'){
            stream >> xpos >> ypos >> xsize >> ysize;
            createEnemy(xpos, ypos, xsize, ysize);
        } else if(type == 'S'){
            stream >> xpos >> ypos >> xsize >> ysize >> staticType;
            createStatic(xpos, ypos, xsize, ysize, staticType);
        }
        else if(type == 'B'){
            while(!stream.eof()){
                no_of_birds_++; 
                stream >> birdtype;
                birdvector_.push_back(birdtype);
            }
        }
   
    }

}


//add a new bird
void Map::addBird(){
    createBird(50.f, 600.f, 100.f, 200.f);
}

//
int Map::getBirdType(){
    if (birdsLeft() != -1)
        return birdvector_[current_bird_];
    else
        return 100;
}

/*
Advance the simulation and take care of all the collitions that happened
if something is hit hard enough, add it to the list of objects to be killed and update the 
stats accordingly
*/
void Map::step(){
    for (b2Contact* c = World_->GetContactList(); c; c = c->GetNext())
    {
        b2Fixture * A =c->GetFixtureA();
        b2Fixture * B =c->GetFixtureB();
        b2Body *bodyA = A->GetBody();
        b2Body *bodyB = B->GetBody();
        //bird hits an enemy
        if ((A->GetDensity() == 2 && B->GetDensity() == 3) || (A->GetDensity() == 3 && B->GetDensity() == 2)){
            
            if (A->GetDensity() == 3)
            {
                if (bodyA->GetLinearVelocity().x > 1 || bodyA->GetLinearVelocity().y > 1){
                    enemyDiesSound.openFromFile("../content/enemyDies.ogg");
                    sf::Time t = sf::milliseconds(3850);
                    enemyDiesSound.setPlayingOffset(t); 
                    enemyDiesSound.play();
                    remove_.push_back(bodyA);
                    score_ += 100;
                }
                
            }
            else{
                if (bodyB->GetLinearVelocity().x > 1 || bodyB->GetLinearVelocity().y > 1){
                    enemyDiesSound.openFromFile("../content/enemyDies.ogg");
                    sf::Time t = sf::milliseconds(3850);
                    enemyDiesSound.setPlayingOffset(t); 
                    enemyDiesSound.play();
                    remove_.push_back(bodyB);
                    score_ += 100;
                }
            }

        }
        //bird hits something else
        else if (B->GetDensity() == 3 || A->GetDensity() == 3){
             if (A->GetDensity() == 3){
                 if (bodyA->GetLinearVelocity().x > 1 || bodyA->GetLinearVelocity().y > 1 || bodyB->GetLinearVelocity().x > 1 || bodyB->GetLinearVelocity().y > 1){
                    enemyDiesSound.openFromFile("../content/enemyDies.ogg");
                    sf::Time t = sf::milliseconds(3850);
                    enemyDiesSound.setPlayingOffset(t); 
                    enemyDiesSound.play();
                    remove_.push_back(bodyA);
                    score_ += 100;
                }
            if (B->GetDensity() == 3){
                if (bodyB->GetLinearVelocity().x > 1 || bodyB->GetLinearVelocity().y > 1||bodyA->GetLinearVelocity().x > 1 || bodyA->GetLinearVelocity().y > 1 ){
                    enemyDiesSound.openFromFile("../content/enemyDies.ogg");
                    sf::Time t = sf::milliseconds(3850);
                    enemyDiesSound.setPlayingOffset(t); 
                    enemyDiesSound.play();
                    remove_.push_back(bodyB);
                    score_ += 100;
                }
                }
             }

        }
        //something hits an obstacle
        else if (B->GetDensity() == 1 || A->GetDensity() == 1){
             if (A->GetDensity() == 1){
                 if (bodyA->GetLinearVelocity().x > 10 || bodyA->GetLinearVelocity().y > 10 || bodyB->GetLinearVelocity().x > 10 || bodyB->GetLinearVelocity().y > 10){
                    remove_.push_back(bodyA);
                    bodyB->SetLinearVelocity(b2Vec2(bodyB->GetLinearVelocity().x*0.5f, bodyB->GetLinearVelocity().y*0.5f));
                    score_ += 50;
                }
            if (B->GetDensity() == 1){
                if (bodyB->GetLinearVelocity().x > 10 || bodyB->GetLinearVelocity().y > 10 ||bodyA->GetLinearVelocity().x > 10 || bodyA->GetLinearVelocity().y > 10 ){
                    remove_.push_back(bodyB);
                    bodyA->SetLinearVelocity(b2Vec2(bodyA->GetLinearVelocity().x*0.5f, bodyA->GetLinearVelocity().y*0.5f));
                    score_ += 50;
                }
                }
             }

        }
        // Enemy dies when hitting the ground
        if ((A->GetDensity() == 3 && B->GetDensity() == 0) || (A->GetDensity() == 0 && B->GetDensity() == 3) || (A->GetDensity() == 3 && B->GetDensity() == 5) || (A->GetDensity() == 0 && B->GetDensity() == 5)){
            
            if (A->GetDensity() == 3)
            {
                if (bodyA->GetLinearVelocity().x > 10 || bodyA->GetLinearVelocity().y > 10){
                    enemyDiesSound.openFromFile("../content/enemyDies.ogg");
                    sf::Time t = sf::milliseconds(3850);
                    enemyDiesSound.setPlayingOffset(t); 
                    enemyDiesSound.play();
                    remove_.push_back(bodyA);
                    score_ += 100;
                }
                
            }
            else{
                if (bodyB->GetLinearVelocity().x > 10 || bodyB->GetLinearVelocity().y > 10){
                    enemyDiesSound.openFromFile("../content/enemyDies.ogg");
                    sf::Time t = sf::milliseconds(3850);
                    enemyDiesSound.setPlayingOffset(t); 
                    enemyDiesSound.play();
                    remove_.push_back(bodyB);
                    score_ += 100;
                }
            }

        }
    }

    //advance the simulation
    float timeStep = 1.0f / 60.f;
    int32 velocityIterations = 10;
    int32 positionIterations = 10;
    World_->Step(timeStep, velocityIterations, positionIterations);
    World_->ClearForces();

}

//if something dies, set it as disabled
void Map::killObjects(){
    for (auto i : remove_){
        if (i->GetFixtureList()->GetDensity() == 3 && i->IsEnabled() == true){
            no_of_enemies_--;
        }

        i->SetEnabled(false);
    }
}

//next bird
void Map::nextBird(){
    current_bird_++;
}

//return the amount of birds left
int Map::birdsLeft(){
    
    return (no_of_birds_ - current_bird_ - 1);
}


//kill a bird
void Map::killBird(b2Body *b){
    World_->DestroyBody(b);
}

//create sf::Text objects from the current statistics
sf::Text Map::writeStats() {
    
   sf::Text text;
   std::ostringstream ss;
   ss << "Score: " << getScore() << std::endl;
   ss << "Birds left: " << birdsLeft() + 1 << std::endl;
   ss << "Enemies left: " << no_of_enemies_ << std::endl; 
   font_.loadFromFile("../content/sansation.ttf");
   text.setString(ss.str());
   text.setFont(font_);
   text.setOrigin(50,50);
text.setPosition(50,50);
text.setCharacterSize(24); 
text.setFillColor(sf::Color::Red);
text.setStyle(sf::Text::Bold | sf::Text::Underlined);
return text;
}








