#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <list>
#include <cstdint>
#include <utility>
#include "DynamicObject.hpp"
#include "Map.hpp"
#include <Box2D/Box2D.h>

static const float SCALE = 30.f;


//construct a new Graphics-object
Graphics::Graphics() : window(sf::VideoMode(1280, 720), "Displeased Birdies", sf::Style::Default/*, settings*/){
    window.setFramerateLimit(60);
    sf::View view1;
    window.display();
    font_.loadFromFile("../content/sansation.ttf");
    menuMusic.openFromFile("../content/menuMusic.ogg");
    menuMusic.setVolume(80);
    last_stars_ = 0;
    //load textures from their files and add them to a vector textures_
    sf::Texture GroundTexture;
    sf::Texture BoxTexture;
    sf::Texture bird;
    sf::Texture EnemyTexture;
    sf::Texture StaticObsTexture;
    sf::Texture Background;
    sf::Texture Brick;
    sf::Texture Menubg;
    sf::Texture Speedy;
    sf::Texture Diver;
    sf::Texture Star;

    GroundTexture.loadFromFile("../content/GroundTexture.png");
    BoxTexture.loadFromFile("../content/box.png");
    bird.loadFromFile("../content/asd.png");
    EnemyTexture.loadFromFile("../content/Enemy.png");
    StaticObsTexture.loadFromFile("../content/StaticObstacle.png");
    Background.loadFromFile("../content/backround.png");
    Brick.loadFromFile("../content/Brick.png");
    Menubg.loadFromFile("../content/Menubg.png");
    Speedy.loadFromFile("../content/speedybird.png");
    Diver.loadFromFile("../content/diver.png");
    Star.loadFromFile("../content/star.png");

    GroundTexture.setSmooth(true);
    BoxTexture.setSmooth(true);
    bird.setSmooth(true);
    EnemyTexture.setSmooth(true);
    Brick.setSmooth(true);
    Speedy.setSmooth(true);
    Diver.setSmooth(true);
    textures_.push_back(GroundTexture);
    textures_.push_back(BoxTexture);
    textures_.push_back(bird);
    textures_.push_back(EnemyTexture);
    textures_.push_back(StaticObsTexture);
    textures_.push_back(Background);
    textures_.push_back(Brick);
    textures_.push_back(Menubg);
    textures_.push_back(Speedy);
    textures_.push_back(Diver);
    textures_.push_back(Star);
}


//this runs while the state is 10, 20 or 30, according to the level being played
void Graphics::runGame(const std::string &mapInfo){
    //configure the window and a view that can be manipulated
    sf::View def(sf::FloatRect(0, 0, 1280, 720));
    view1.reset(sf::FloatRect(0, 0, 1280, 720));
    view1.setCenter(600.f , 400.f);
    view1.zoom(1.3f);
    window.setView(view1);

    //create a new Map-object that stores information of the game level
    Map* level = new Map();
    level->createmap(mapInfo);

    //booleans to help keep track of the state of the bird
    bool birdupdate = false;
    bool is_bird = false;
    bool flying = false;
    bool birdpressed = false;
    bool ability_used = false;

    struct limits  {
        int first;
        int second;
        int third;
    } stars;


    stars.first = level->enemiesLeft()*100;
    stars.second = level->enemiesLeft()*100 + level_*300;
    stars.third =  level->enemiesLeft()*100 + level_*500;
    
    int birdtype;

    //get our first bird
    b2Body *currentbird;
    level->addBird();
    
    //get the type of the bird
    birdtype = level->getBirdType();
    
    sf::Clock clock;

    int xmove = 0;
    double zoomNum = 1.00;
    double allowedXmove = 0;
    double allowedYmove = 0;

    //SFML vectors for storing information about mouse positions
    sf::Vector2i mousepos;
    sf::Vector2i mouseEnd;
    sf::Vector2f MouseBegF;
    sf::Vector2f MouseEndF;
    sf::Vector2i MouseCurrent;
    sf::Vector2f MouseCurrentF;
    sf::Vector2f BirdPos;

    //music and sounds
    sf::Music gameMusic;
    gameMusic.openFromFile("../content/gameMusic.ogg");
    winSound.openFromFile("../content/winSound.ogg");
    gameOverSound.openFromFile("../content/GameOver.ogg");
    gameMusic.setVolume(50);
    gameMusic.play();

    //a model for the bird, used before it is shot
    sf::Sprite BirdModel;
    switch (birdtype)
    {
    case 0:
        BirdModel.setTexture(textures_[2]);
        break;
    case 1:
        BirdModel.setTexture(textures_[7]);
        break;
    case 2:
        BirdModel.setTexture(textures_[8]);
        break;
    }
    BirdModel.setOrigin(282.f, 282.f);
    BirdModel.setScale(0.0566f, 0.0566f);
    BirdModel.setPosition(50.f, 602.f);
    sf::FloatRect bounds = BirdModel.getGlobalBounds();

    //Background
    sf::Sprite Background; 
    Background.setTexture(textures_[5]);
    Background.setScale(1.1f,1.1f);
    Background.setPosition(-835.f,-835.f);

    sf::Sprite Brick;
    Brick.setTexture(textures_[6]);
    Brick.setScale(0.75f, 0.75);
    Brick.setPosition(-1450.f, -1500.f);


   
    sf::Sprite star;
    star.setTexture(textures_[10]);
    star.setOrigin(50.f, 50.f);
    star.setPosition(100.f, 100.f);
    star.setScale(0.7f, 0.7f);
    star.setColor(sf::Color(128,128,128));
    

    //the while-loop runs until the window is closed or the state changes
    while (window.isOpen() && (state == 10 || state == 20 || state == 30))
        {
            //in the beginning always check if the game should end->
            //are there any enemies left and does the player have any more birds
            if (level->birdsLeft() == -1 || level->enemiesLeft() == 0){
                if (level->birdsLeft() == -1 && level->enemiesLeft() != 0){
                    //no birds, but enemies alive -> level failed
                    completed_ = false;
                    gameOverSound.play();
                    last_stars_ = 0;
                }
                else {
                    //no enemies left -> level completed
                    level->addScore(level->birdsLeft() * 100);
                    completed_ = true;
                    winSound.play();
                    last_score_ = level->getScore();
                    if (last_score_ < stars.second)
                        last_stars_ = 1;
                    else if(last_score_ < stars.third)
                        last_stars_ = 2;
                    else 
                        last_stars_ = 3;
                }
                // state 4 = show results
                state = 4;
                return;
                
            }
            
            //clear window and draw background
            window.clear(sf::Color::Black);
            window.draw(Brick);
            window.draw(Background);
            window.setView(def);

            star.setPosition(1000.f, 100.f);
            if (level->getScore() > stars.first){
                star.setColor(sf::Color(255,255,0));
            }
                
            window.draw(star);
            star.setColor(sf::Color(128,128,128));

            if (level->getScore() > stars.second){
                star.setColor(sf::Color(255,255,0));
            }
                
            star.setPosition(1100.f, 100.f);
            window.draw(star);

            star.setColor(sf::Color(128,128,128));
            if (level->getScore() > stars.third){
                star.setColor(sf::Color(255,255,0));
            }
                
            star.setPosition(1200.f, 100.f);
            window.draw(star);

            star.setColor(sf::Color(128,128,128));
            sf::Text stat = level->writeStats();

            window.draw(stat);
            window.setView(view1);

            //bird is not shot ->draw the model
            if (flying == false){
                window.draw(BirdModel);
            }
            

            sf::Event event;

            //if we are holding the bird, move the model and keep track how far the mouse is dragged
            if (birdpressed == true && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                MouseCurrent = sf::Mouse::getPosition(window);
                MouseCurrentF = window.mapPixelToCoords(MouseCurrent);
                BirdPos = MouseCurrentF;
                
                float dx = (MouseCurrentF.x - MouseBegF.x)*(MouseCurrentF.x - MouseBegF.x);
                float dy = (MouseCurrentF.y - MouseBegF.y)*(MouseCurrentF.y - MouseBegF.y);
               
                float lenx = std::sqrt(dx);
                float leny = std::sqrt(dy);

                //the bird can only be dragged a certain distance from the starting point
                if (MouseCurrentF.x > 250)
                    BirdPos.x = 250;
                if (MouseCurrentF.x < -200)
                    BirdPos.x = -200;
                if (MouseCurrentF.y > 800)
                    BirdPos.y = 800;
                if (MouseCurrentF.y < 400)
                    BirdPos.y = 400;

                //draw lines from the slingshot to the bird
                sf::VertexArray line(sf::Lines, 2);
                line[0].position = BirdPos;
                line[0].color  = sf::Color(128,0,0);
                line[1].position =sf::Vector2f(15.f, 605.f);
                line[1].color = sf::Color(128,0,0);
                sf::VertexArray line2(sf::Lines, 2);
                line2[0].position = BirdPos;
                line2[0].color  = sf::Color(128,0,0);
                line2[1].position =sf::Vector2f(80.f, 600.f);
                line2[1].color = sf::Color(128,0,0);
                if (birdpressed == true){
                    window.draw(line);
                    window.draw(line2);
                }
                

                BirdModel.setPosition(BirdPos);
            }

            //check the input of the player
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                    //close window
                    case sf::Event::Closed:
                        state = 2;
                        window.close();
                        break;

                    //if we press on the bird, set the birdpressed as true
                    //and store the coordinates where the mouse was pressed
                    case sf::Event::MouseButtonPressed:
                        mousepos = sf::Mouse::getPosition(window);
                        MouseBegF = window.mapPixelToCoords(mousepos);
                        if (bounds.contains(MouseBegF.x, MouseBegF.y)){
                            birdpressed = true;
                        }
                        // if a speedy bird is flying and has not used the boost, give it a speedboost
                        else if(flying == true && birdtype != 0)
                            if (ability_used == false)
                                use_Ability(currentbird, birdtype);
                                ability_used = true;
                        
                        break;
                    
                    //if mouse released, set flying to true, and record mouse pos for calculating the force
                    //start the clock for calculating bird's lifespan
                    case sf::Event::MouseButtonReleased:
                        clock.restart();
                        mouseEnd = sf::Mouse::getPosition(window);
                        MouseEndF = window.mapPixelToCoords(mouseEnd);
                        if (birdpressed == true){
                            if (flying == false){

                                currentbird->SetAwake(true);
                                shootSound.openFromFile("../content/birdSound.ogg");
                                shootSound.play();
                                birdpressed = false;
                                birdupdate = true;
                                BirdModel.setPosition(50.f, 602.f);
                            }                               
                            
                            flying = true;
                        }
                        
                        break;

                    /*
                    Keyboard events, if Esc, go to main menu, enter to kill a bird, 
                    arrowkeys, M and N for manipulating the view
                    */
                    case sf::Event::KeyPressed:
                        if (event.key.code == sf::Keyboard::Escape){
                            view1.zoom(1.f);
                            state = 0;
                            return;
                            break;
                        }
                        else if(event.key.code == sf::Keyboard::Enter){
                            is_bird = false;
                            xmove = 0;
                            view1.setCenter(600.f , 400.f );
                            level->killBird(currentbird);
            
                            level->addBird();
                            level->nextBird();
                            break;
                        }
                        else if (event.key.code == sf::Keyboard::N){
                            if(zoomNum >= -40.59){
                                view1.zoom(0.99f);
                                zoomNum -= 0.99;
                            }

                        }
                        else if (event.key.code == sf::Keyboard::M){
                            if(zoomNum <= 44.44){
                                view1.zoom(1.01f);
                                zoomNum += 1.01;
                            }

                        }
                        else if (event.key.code == sf::Keyboard::Up){
                            if(allowedYmove >= -250){
                                view1.move(0, -4.f);
                                allowedYmove -= 4;
                            }

                        }
                        else if (event.key.code == sf::Keyboard::Down){
                            if(allowedYmove <= 150){
                                view1.move(0, 4.f);
                                allowedYmove += 4;
                            }

                        }
                        else if (event.key.code == sf::Keyboard::Left){
                            if(allowedXmove >= -410){
                                view1.move(-4.f,0);
                                allowedXmove -= 4;
                            }
                        }
                        else if (event.key.code == sf::Keyboard::Right){
                            if(allowedXmove <= 1400){
                                view1.move(4.f, 0);
                                allowedXmove += 4;
                            }

                        }                   
                }

            }
           
            //get time from the moment the bird was shot, if it hits 5 seconds, kill the bird
            sf::Time elapsed1 = clock.getElapsedTime();
            if (elapsed1.asSeconds() > 5  && flying == true){
                is_bird = false;
                xmove = 0;
                view1.setCenter(600.f , 400.f );
                level->killBird(currentbird);
                level->addBird();
                level->nextBird();
            }
            
            //if we have no bird, get a new one
            if (is_bird == false){
                flying = false;
                is_bird = true;
                currentbird = level->boxgetBird();
                birdtype = level->getBirdType();
                switch (birdtype)
                {
                case 0:
                    BirdModel.setTexture(textures_[2]);
                    break;
                case 1:
                    BirdModel.setTexture(textures_[8]);
                    break;
                case 2:
                    BirdModel.setTexture(textures_[9]);
                    break;
                }
                currentbird->SetAwake(false);
            }

            //if the bird is flying, update its position and move the view with it
            if (flying == true){
                if((currentbird->GetPosition().x*SCALE + 550 - xmove <= 2250) && (currentbird->GetPosition().x*SCALE + 550 - xmove >= 0)){
                    view1.setCenter(currentbird->GetPosition().x*SCALE + 550 - xmove, 400.f);
                    if (xmove < 300)
                        xmove+= 2;
                }

            }

            //if the bird was shot, apply a force to it, calculated from the mouse pos vectors
            if (birdupdate == true){
                birdupdate = false;
                sf::Vector2f force = MouseBegF - MouseEndF;
                if (force.x > 150.f)
                    force.x = 150.f;
                if (force.y < -150.f)
                    force.y = -150.f;
                currentbird->ApplyForceToCenter(50* currentbird->GetMass() * b2Vec2(force.x/5.f, force.y/5.f), true);
                ability_used = false;
            }
            

            //if the bird is flying, draw it to the screen
            if (flying == true){
                sf::Sprite Sprite;
                switch (birdtype)
                {
                case 0:
                    Sprite.setTexture(textures_[2]);
                    break;
                case 1:
                    Sprite.setTexture(textures_[8]);
                    break;
                case 2:
                    Sprite.setTexture(textures_[9]);
                    break;
                }
                Sprite.setOrigin(282.f, 282.f);
                Sprite.setScale(0.0566f, 0.0566f);
                Sprite.setPosition(SCALE * currentbird->GetPosition().x, SCALE * currentbird->GetPosition().y);
                Sprite.setRotation(currentbird->GetAngle() * 180/b2_pi);
                window.draw(Sprite);
            }
            
            
            
            
            //advance the simulation using the Map-object
            level->step();
        
            //draw objects and display them
            draw(level);    

            
            window.display();

            //if something was hit hard enough, kill it
            level->killObjects();
        }

}

void Graphics::use_Ability(b2Body *currentbird, int type){
    if (type == 1){
        currentbird->SetLinearVelocity(b2Vec2(currentbird->GetLinearVelocity().x*2.5f, currentbird->GetLinearVelocity().y*2.5f));
    }
    else if(type == 2){
        b2MassData massdata = {0.f,b2Vec2(0.f , 0.f), 0.f};
        currentbird->GetMassData(&massdata);
        massdata.mass = massdata.mass*5;
       currentbird->SetMassData(&massdata);
       currentbird->SetLinearVelocity(b2Vec2(currentbird->GetLinearVelocity().x*0.1f, 50.f)); 
    }
}


//method for drawing obstacles, enemies and the slingshot
void Graphics::draw(Map *level){
    //get all obstacles
    for (auto it : level->boxgetObstacles()){
        //if still alive, draw it
        if (it->IsEnabled() == true){
                sf::Sprite Sprite;
                DynamicObject* xsize = reinterpret_cast<DynamicObject*>(it->GetUserData().pointer);
                DynamicObject* ysize = reinterpret_cast<DynamicObject*>(it->GetUserData().pointer);
                Sprite.setTexture(textures_[1]);
                Sprite.setOrigin(50.f, 50.f);
                Sprite.setScale((xsize->getXsize())/50.f, (ysize->getYsize())/50.f);
                Sprite.setPosition(SCALE * it->GetPosition().x, SCALE * it->GetPosition().y);
                Sprite.setRotation(it->GetAngle() * 180/b2_pi);
                window.draw(Sprite);
        }
    }

            
    //get all enemies
     for (auto it : level->boxgetEnemies()){
         //if alive, draw
         if (it->IsEnabled() == true){
             sf::Sprite Sprite;
             Sprite.setTexture(textures_[3]);
             Sprite.setOrigin(150.f, 150.f);
             Sprite.setScale(0.106f, 0.106f);
             Sprite.setPosition(SCALE * it->GetPosition().x, SCALE * it->GetPosition().y);
             Sprite.setRotation(it->GetAngle() * 180/b2_pi);
             window.draw(Sprite);
         }
     
     //get static objects
      for (auto it : level->boxgetStatic()){
         DynamicObject *data = reinterpret_cast<DynamicObject*>(it->GetUserData().pointer);
         //if the type is a static obstacle (5), draw it
         if (data->getType() == 5){
             sf::Sprite Sprite;
             DynamicObject* xsize = reinterpret_cast<DynamicObject*>(it->GetUserData().pointer);
             DynamicObject* ysize = reinterpret_cast<DynamicObject*>(it->GetUserData().pointer);
             Sprite.setTexture(textures_[4]);
             Sprite.setOrigin(50.f, 50.f);
             Sprite.setScale((xsize->getXsize())/50.f, (ysize->getYsize())/50.f);
             Sprite.setPosition(SCALE * it->GetPosition().x, SCALE * it->GetPosition().y);
             Sprite.setRotation(it->GetAngle() * 180/b2_pi);
             window.draw(Sprite);
         }
     
        
     }

     }
     //draw the slingshot
     drawSling();
}


//method for drawing the slingshot
void Graphics::drawSling(){
    sf::RectangleShape rectangle1(sf::Vector2f(10.f, 70.f));
    sf::RectangleShape rectangle2(sf::Vector2f(7.f, 50.f));
    sf::RectangleShape rectangle3(sf::Vector2f(7.f, 50.f));
    rectangle1.setPosition(46.f, 630.f);
    rectangle2.setPosition(15.f, 605.f);
    rectangle3.setPosition(80.f, 600.f);
    rectangle1.setFillColor(sf::Color(139,69,19));
    rectangle2.setFillColor(sf::Color(139,69,19));
    rectangle3.setFillColor(sf::Color(139,69,19));
    rectangle3.setRotation(45.f);
    rectangle2.setRotation(-45.f);
    window.draw(rectangle1);
    window.draw(rectangle2);
    window.draw(rectangle3);
}








