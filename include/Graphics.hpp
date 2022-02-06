#pragma once
#include "config.h"
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <Box2D/Box2D.h>
#include "DynamicObject.hpp"
#include "Map.hpp"

/*
The Graphics class implementation is done in two different cpp-files, Game.cpp and Menu.cpp
due to the complexity of the methods
*/

class Graphics {
public:
    Graphics();
    ~Graphics(){}
    
    //Functions related to crating and running different game states
    //Rungame does everything related to actually playing
    void runGame(const std::string &mapInfo);

    //These functions are used to display the different menus, they are located in Menu.cpp
    void mapMenu();
    void menu();
    void results(int level, int score, bool completed);
    void highScores(int level, int score, bool completed);
    void refreshScores(std::vector<std::pair<int, std::string>> scores, std::pair<int, std::string> new_score);

    //functions for drawing objects during the game
    void draw(class Map *m);
    void drawSling();

    //the "main" loop of the game, use methods according to the current state 
    void run() {
      while (state != 2){
        std::ifstream istr;
        if(state == 0){
            menu();
        } else if(state == 3){
            mapMenu();
        } else if (state == 4){
          results(level_, last_score_, completed_);
        }  else if (state == 5){
          highScores(level_, last_score_, completed_);
        } else if(state == 10){
          level_ = 1;
          runGame("../content/map1");
        } else if(state == 20){
          level_ = 2;
          runGame("../content/map2");
        } else if(state == 30){
          level_ = 3;
          runGame("../content/map3");
        }
      }
    }

    void use_Ability(b2Body *currentbird, int type);

    

    
        

private:
    int last_stars_;
    int level_;
    int last_score_;
    bool completed_;
    int state = 0;
    sf::Clock clock;
    sf::RenderWindow window;
    sf::View view;

    //text displayed during the game
    std::vector<std::string> menutext = {"Play","Highscores", "Quit"};
    std::vector<std::string> controls_ = {"Shoot: Left mouse button", "Move view: Arrowkeys", "Zoom: M and N", "To menu: Esc"};
    std::vector<std::string> maptext = {"Map 1", "Map 2", "Map 3"};
    std::vector<std::string> endtext = {"Level completed", "Level failed","Retry","Next level","Main menu", "Check highscores"};
    std::vector<std::string> high = {"Highscores", "Enter a name","to add your score:" , "Main menu", "Your score: "};

    std::vector<sf::Text> options;
    std::vector<sf::Text> controls_sf_;
    std::vector<sf::Text> options2;
    std::vector<sf::Text> options3;
    std::vector<sf::Text> options4;
    std::vector<sf::Text> scores_sf_;

    sf::Font font_;

    sf::Text title_;
    sf::Text exit_; 
    sf::Text play_; 

    sf::Music menuMusic;
    sf::Music winSound;
    sf::Music shootSound;
    sf::Music gameOverSound;
    
    sf::View view1;
    std::vector<sf::Texture> textures_;
    
    

};
