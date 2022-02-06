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




//Method for displaying the main menu

void Graphics::menu(){
    sf::Sprite bg;
    bg.setTexture(textures_[7]);
    title_.setFont(font_);
    title_.setString("Displeased Birdies");
    title_.setCharacterSize(80);
    title_.setFillColor(sf::Color::Red);
    title_.setOrigin(title_.getLocalBounds().width /2, title_.getLocalBounds().height /2);
    title_.setPosition(window.getSize().x/2.f, window.getSize().y/8.f);
    view1.reset(sf::FloatRect(0, 0, 1280, 720));
    window.setView(view1);
    if (menuMusic.getStatus() == sf::Music::SoundSource::Stopped) {
        menuMusic.play();
    }
    
    //create a sf::Text object from all of the menutext strings
    int i = 0;
    for (std::vector<std::string>::iterator it=menutext.begin(); it!=menutext.end(); ++it){
        sf::Text t;
        t.setFont(font_);
        t.setString(*it);
        t.setCharacterSize(50);
        t.setFillColor(sf::Color::Red);
        t.setOrigin(t.getLocalBounds().width /2, t.getLocalBounds().height /2);
        t.setPosition(window.getSize().x/2.f, window.getSize().y/3 + 150.f * i);
        options.push_back(t);
        i++;
    }
    i = 0;

    //same for the control strings
    for (std::vector<std::string>::iterator it=controls_.begin(); it!=controls_.end(); ++it){
         sf::Text t;
        t.setFont(font_);
        t.setString(*it);
        t.setCharacterSize(20);
        t.setFillColor(sf::Color::Red);
        t.setPosition(30.f,30.f + 30.f * i);
        controls_sf_.push_back(t);
        i++;
     }

    
    /*
    main loop, if the mouse is hovered over a text object, give it an outline
    if something is clicked, change the game state accordingly
    runs when state = 0
    */
    while (window.isOpen() && state == 0)
        {
           
            window.clear(sf::Color::Black);
            window.draw(bg);
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed){
                    window.close();
                    state = 2;
                    break;
                }

                //if mouse is over a text object, give it an outline
                else if (event.type == sf::Event::MouseMoved){
                    sf::Vector2i mousePos = sf::Mouse::getPosition( window );
                    sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                    if (options[0].getGlobalBounds().contains(mousePosF)){
                        options[0].setOutlineColor(sf::Color::White);
                        options[0].setOutlineThickness(5);
                
                    }
                    else if (options[1].getGlobalBounds().contains(mousePosF)){
                        options[1].setOutlineColor(sf::Color::White);
                        options[1].setOutlineThickness(5);
                        
                    }
                    else if (options[2].getGlobalBounds().contains(mousePosF)){
                        options[2].setOutlineColor(sf::Color::White);
                        options[2].setOutlineThickness(5);
                        
                    }
                    else{
                             options[0].setOutlineThickness(0);
                             options[1].setOutlineThickness(0);
                             options[2].setOutlineThickness(0);
                    } 
                }
                //if an option is selected, change game state
                else if (event.type == sf::Event::MouseButtonPressed){
                    sf::Vector2i mousePos = sf::Mouse::getPosition( window );
                    sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                    if (options[0].getGlobalBounds().contains(mousePosF)){
                        state = 3;
                        return;
                        break;
                    }
                    else if(options[1].getGlobalBounds().contains(mousePosF)){
                        state = 5;
                        return;
                        break;
                    }
                    else if(options[2].getGlobalBounds().contains(mousePosF)){
                        state = 2;
                        return;
                        break;
                    }
                }

            }
            for (auto i: options){
                window.draw(i);
            }

            for (auto i: controls_sf_){
                window.draw(i);
            }
            
            window.draw(title_);
            window.display();
        }
        return;
}



/*
Method for the map selection screen, runs when state = 3
create sf::Text objects from the related strings, do outlines
and change game-state if something is clicked
*/

void Graphics::mapMenu(){
    sf::Sprite bg;
    bg.setTexture(textures_[7]);
    title_.setFont(font_);
    title_.setString("Displeased Birdies");
    title_.setCharacterSize(80);
    title_.setFillColor(sf::Color::Red);
    title_.setOrigin(title_.getLocalBounds().width /2, title_.getLocalBounds().height /2);
    title_.setPosition(window.getSize().x/2.f, window.getSize().y/8.f);
    view1.reset(sf::FloatRect(0, 0, 1280, 720));
    window.setView(view1);
    
    int i = 0;
    for (std::vector<std::string>::iterator it=maptext.begin(); it!=maptext.end(); ++it){
        sf::Text t;
        t.setFont(font_);
        t.setString(*it);
        t.setCharacterSize(50);
        t.setFillColor(sf::Color::Red);
        t.setOrigin(t.getLocalBounds().width /2, t.getLocalBounds().height /2);
        t.setPosition(window.getSize().x/2.f, window.getSize().y/3 + 150.f * i);
        options2.push_back(t);
        i++;
    }
    i = 0;

    for (std::vector<std::string>::iterator it=controls_.begin(); it!=controls_.end(); ++it){
         sf::Text t;
        t.setFont(font_);
        t.setString(*it);
        t.setCharacterSize(20);
        t.setFillColor(sf::Color::Red);
        t.setPosition(30.f,30.f + 30.f * i);
        controls_sf_.push_back(t);
        i++;
     }

    
    
    while (window.isOpen() && state == 3)
        {
            window.clear(sf::Color::Black);
            window.draw(bg);
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed){
                    window.close();
                    state = 2;
                    break;
                }
                //if mouse is over a text object, give it an outline
                else if (event.type == sf::Event::MouseMoved){
                    sf::Vector2i mousePos = sf::Mouse::getPosition( window );
                    sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                    if (options2[0].getGlobalBounds().contains(mousePosF)){
                        options2[0].setOutlineColor(sf::Color::White);
                        options2[0].setOutlineThickness(5);
                
                    }
                    else if (options2[1].getGlobalBounds().contains(mousePosF)){
                        options2[1].setOutlineColor(sf::Color::White);
                        options2[1].setOutlineThickness(5);
                        
                    }else if (options2[2].getGlobalBounds().contains(mousePosF)){
                        options2[2].setOutlineColor(sf::Color::White);
                        options2[2].setOutlineThickness(5);
                        
                    } else{
                        options2[0].setOutlineThickness(0);
                        options2[1].setOutlineThickness(0);
                        options2[2].setOutlineThickness(0);
                    }
                }
                //if an option is selected, change game state
                else if (event.type == sf::Event::MouseButtonPressed){
                    sf::Vector2i mousePos = sf::Mouse::getPosition( window );
                    sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                    if (options2[0].getGlobalBounds().contains(mousePosF)){
                        state = 10;
                        menuMusic.stop();
                        return;
                        break;
                    }
                    else if(options2[1].getGlobalBounds().contains(mousePosF)){
                        state = 20;
                        menuMusic.stop();
                        return;
                        break;
                    }
                    else if(options2[2].getGlobalBounds().contains(mousePosF)){
                        state = 30;
                        menuMusic.stop();
                        return;
                        break;
                    }
                }

            }
            for (auto i: options2){
                window.draw(i);
            }

            for (auto i: controls_sf_){
                window.draw(i);
            }
            
            window.draw(title_);
            window.display();
        }
        return;
}

/*
Method for displaying the result screen, runs when state = 4
take the level, achieved score and boolean whether the level was passed
*/

void Graphics::results(int level, int score, bool completed){
    sf::Sprite star;
    star.setTexture(textures_[10]);
    star.setOrigin(50.f, 50.f);
    sf::Sprite bg;
    bg.setTexture(textures_[7]);
    view1.reset(sf::FloatRect(0, 0, 1280, 720));
    window.setView(view1);
    menuMusic.play();
    
    //create sf::text objects
    int i = 0;
    for (std::vector<std::string>::iterator it=endtext.begin(); it!=endtext.end(); ++it){
        sf::Text t;
        t.setFont(font_);
        t.setString(*it);
        t.setCharacterSize(50);
        t.setFillColor(sf::Color::Red);
        t.setOutlineColor(sf::Color::White);
        t.setOrigin(t.getLocalBounds().width /2, t.getLocalBounds().height /2);
        options3.push_back(t);
        i++;
    }
    i = 0;

    
    
    while (window.isOpen() && state == 4)
        {
           
            window.clear(sf::Color::Black);
            window.draw(bg);
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed){
                    window.close();
                    state = 2;
                    break;
                }
                
                else if (event.type == sf::Event::MouseMoved){
                    sf::Vector2i mousePos = sf::Mouse::getPosition( window );
                    sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                    int j = 0;
                    for (auto i : options3){
                        if (options3[j].getGlobalBounds().contains(mousePosF)){
                            options3[j].setOutlineThickness(5);
                            options3[0].setOutlineThickness(0);
                            options3[1].setOutlineThickness(0);
                        }
                        else{
                            options3[j].setOutlineThickness(0);
                        }
                        j++;
                    }
                }
                else if (event.type == sf::Event::MouseButtonPressed){
                    sf::Vector2i mousePos = sf::Mouse::getPosition( window );
                    sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                    
                    if(options3[2].getGlobalBounds().contains(mousePosF)){
                        state = level * 10;
                        menuMusic.stop();
                        return;
                        break;
                    }
                    else if (options3[3].getGlobalBounds().contains(mousePosF)){
                        state = (level + 1) * 10;
                        menuMusic.stop();
                        return;
                        break;
                    }
                    else if(options3[4].getGlobalBounds().contains(mousePosF)){
                        state = 0;
                        return;
                        break;
                    }
                    else if(options3[5].getGlobalBounds().contains(mousePosF)){
                        state = 5;
                        return;
                        break;
                    }
                }
                

            }
            //modify the text displayed depending if the level was passed
            if (completed == true){

                if(last_stars_ >= 1){
                    star.setPosition(window.getSize().x/2.f - 100.f, 70.f);
                    window.draw(star);
                    if(last_stars_ >= 2){
                        star.setPosition(window.getSize().x/2.f , 70.f);
                        window.draw(star);
                        if(last_stars_ >= 3){
                            star.setPosition(window.getSize().x/2.f + 100.f, 70.f);
                            window.draw(star);
                        }
                    }
                    
                }
                options3[0].setCharacterSize(100);
                options3[0].setOrigin(options3[0].getLocalBounds().width /2, options3[0].getLocalBounds().height /2);
                options3[0].setPosition(window.getSize().x/2.f, window.getSize().y/3 - 100.f);
        
                window.draw(options3[0]);
                std::stringstream ss;
                ss << "Points: " << score << std::endl;
                sf::Text t;
                t.setFont(font_);
                t.setString(ss.str());
                t.setCharacterSize(50);
                t.setFillColor(sf::Color::Red);
                t.setOrigin(t.getLocalBounds().width /2, t.getLocalBounds().height /2);
                t.setPosition(window.getSize().x/2.f, window.getSize().y/3 + 50.f);
                window.draw(t);
                if (level != 3){
                    options3[3].setPosition(window.getSize().x/2.f, window.getSize().y/3 + 150.f);
                    options3[4].setPosition(window.getSize().x/2.f, window.getSize().y/3 + 250.f);
                    options3[5].setPosition(window.getSize().x/2.f, window.getSize().y/3 + 350.f);
                    window.draw(options3[3]);
                    window.draw(options3[4]);
                    window.draw(options3[5]);
                }
                else{
                    options3[4].setPosition(window.getSize().x/2.f, window.getSize().y/3 + 250.f);
                    options3[5].setPosition(window.getSize().x/2.f, window.getSize().y/3 + 350.f);
                    window.draw(options3[4]);
                    window.draw(options3[5]);
                }
            }
            else{
                options3[1].setPosition(window.getSize().x/2.f, window.getSize().y/3 - 200.f);
                window.draw(options3[1]);
                options3[2].setPosition(window.getSize().x/2.f, window.getSize().y/3 - 50.f);
                window.draw(options3[2]);
                options3[4].setPosition(window.getSize().x/2.f, window.getSize().y/3 + 100.f);
                window.draw(options3[4]);
                options3[5].setPosition(window.getSize().x/2.f, window.getSize().y/3 + 250.f);
                window.draw(options3[5]);
            }

            
    
            window.display();
        }
        
        return;
}

void Graphics::highScores(int level, int last_score, bool completed){

    std::vector<sf::Text> lvl1;
    std::vector<sf::Text> lvl2;
    std::vector<sf::Text> lvl3;

    scores_sf_.clear();
    std::pair<int, std::string> new_score;
    sf::String playerInput;
    sf::Text playerText;
    int namelen = 0;
    playerText.setFont(font_);
    playerText.setCharacterSize(50);
    playerText.setFillColor(sf::Color::Red);
    playerText.setPosition(window.getSize().x/2.f, window.getSize().y/2.f + 100.f);
    std::ifstream file1;
    std::ifstream file2;
    std::ifstream file3;
    std::string text;
    std::vector<std::pair<int, std::string>> highscore1;
    std::vector<std::pair<int, std::string>> highscore2;
    std::vector<std::pair<int, std::string>> highscore3;
    if (completed == false){
        file1.open("../content/highscores_1");
        file2.open("../content/highscores_2");
        file3.open("../content/highscores_3");
        while(std::getline(file1, text)){
            std::stringstream stream(text); 
            int score;
            std::string name;
            std::pair<int, std::string> pair;
            stream >> name >> score;
            pair = std::make_pair(score, name);
            highscore1.push_back(pair);
        }
        while(std::getline(file2, text)){
            std::stringstream stream(text); 
            int score;
            std::string name;
            std::pair<int, std::string> pair;
            stream >> name >> score;
            pair = std::make_pair(score, name);
            highscore2.push_back(pair);
        }
        while(std::getline(file3, text)){
            std::stringstream stream(text); 
            int score;
            std::string name;
            std::pair<int, std::string> pair;
            stream >> name >> score;
            pair = std::make_pair(score, name);
            highscore3.push_back(pair);
        }
        int ind = 1;
        for (auto i : highscore1){
            sf::Text t;
            t.setFont(font_);
            t.setString(std::to_string(ind) + ". "+ i.second + " " + std::to_string(i.first));
            t.setCharacterSize(30);
            t.setFillColor(sf::Color::Black);
            t.setOutlineColor(sf::Color::White);
            t.setPosition(window.getSize().x/6.f, window.getSize().y/4 - 70.f + 50.f*ind);
            lvl1.push_back(t);
            ind++;
        }
        ind = 1;
        for (auto i : highscore2){
            sf::Text t;
            t.setFont(font_);
            t.setString(std::to_string(ind) + ". "+ i.second + " " + std::to_string(i.first));
            t.setCharacterSize(30);
            t.setFillColor(sf::Color::Black);
            t.setOutlineColor(sf::Color::White);
            t.setPosition(window.getSize().x/2.f - 100.f, window.getSize().y/4 - 70.f + 50.f*ind);
            lvl2.push_back(t);
            ind++;
        }
        ind = 1;
        for (auto i : highscore3){
            sf::Text t;
            t.setFont(font_);
            t.setString(std::to_string(ind) + ". "+ i.second + " " + std::to_string(i.first));
            t.setCharacterSize(30);
            t.setFillColor(sf::Color::Black);
            t.setOutlineColor(sf::Color::White);
            t.setPosition(window.getSize().x/4.f + 600.f, window.getSize().y/4 - 70.f + 50.f*ind);
            lvl3.push_back(t);
            ind++;
        }
        
    }
    file1.close();
    file2.close();
    file3.close();


    sf::Text level1;
    sf::Text level2;
    sf::Text level3;
    level1.setFont(font_);
    level2.setFont(font_);
    level3.setFont(font_);
    level1.setString("Level 1");
    level2.setString("Level 2");
    level3.setString("Level 3");
    level1.setFillColor(sf::Color::Black);
    level2.setFillColor(sf::Color::Black);
    level3.setFillColor(sf::Color::Black);
    level1.setPosition(window.getSize().x/6.f, window.getSize().y/4 - 70.f);
    level2.setPosition(window.getSize().x/2.f - 100.f, window.getSize().y/4 - 70.f);
    level3.setPosition(window.getSize().x/4.f + 600.f, window.getSize().y/4 - 70.f);
    std::ifstream istr;
    
    istr.open("../content/highscores_" + std::to_string(level));
    std::string line;
    std::vector<std::pair<int, std::string>> scores;
    int placement = 11;
    bool placement_found = false;
    bool score_entered = false;
    while(std::getline(istr, line)){
        std::stringstream stream(line); 
        int score;
        std::string name;
        std::pair<int, std::string> pair;

        stream >> name >> score;
        pair = std::make_pair(score, name);
        scores.push_back(pair);
    }
    istr.close();
    sort(scores.rbegin(), scores.rend());


    int ind = 1;
    for (auto i : scores){
        if (last_score > i.first && placement_found == false){
            placement = ind;
            placement_found = true;    
        } 
        sf::Text t;
        t.setFont(font_);
        t.setString(std::to_string(ind) + ". "+ i.second + " " + std::to_string(i.first));
        t.setCharacterSize(30);
        t.setFillColor(sf::Color::Red);
        t.setOutlineColor(sf::Color::White);
        t.setPosition(window.getSize().x/4.f + 600.f, window.getSize().y/5 - 150.f + 50.f*ind);
        scores_sf_.push_back(t);
        ind++;
    }
    


    sf::Sprite bg;
    sf::Event event;
    bg.setTexture(textures_[7]);
    view1.reset(sf::FloatRect(0, 0, 1280, 720));
    window.setView(view1);
    if (menuMusic.getStatus() == sf::Music::SoundSource::Stopped) {
        menuMusic.play();
    }
    //create sf::text objects
    int i = 0;
    for (std::vector<std::string>::iterator it=high.begin(); it!=high.end(); ++it){
        sf::Text t;
        t.setFont(font_);
        t.setString(*it);
        t.setCharacterSize(50);
        t.setFillColor(sf::Color::Red);
        t.setOutlineColor(sf::Color::White);
        t.setOrigin(t.getLocalBounds().width /2, t.getLocalBounds().height /2);
        options4.push_back(t);
        i++;
    }

    options4[0].setPosition(window.getSize().x/2.f, window.getSize().y/2.f - 300.f);
    options4[3].setPosition(window.getSize().x/2.f, window.getSize().y/2.f + 300.f);
    options4[4].setString("Your score: " + std::to_string(last_score));
    options4[4].setOrigin(options4[4].getLocalBounds().width /2, options4[4].getLocalBounds().height /2);
    options4[4].setPosition(window.getSize().x/2.f, window.getSize().y/2.f - 200.f);
    i = 0;

    if (placement < 11){
        options4[1].setPosition(window.getSize().x/2.f, window.getSize().y/2.f - 100.f);
        options4[2].setPosition(window.getSize().x/2.f, window.getSize().y/2.f );
    }

    while (window.isOpen() && state == 5){
         while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed){
                    window.close();
                    state = 2;
                    break;
                }

                else if (event.type == sf::Event::MouseMoved){
                    sf::Vector2i mousePos = sf::Mouse::getPosition( window );
                    sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );
                    int j = 0;
                    for (auto i : options4){
                        if (options4[j].getGlobalBounds().contains(mousePosF)){
                            options4[j].setOutlineThickness(5);
                            options4[0].setOutlineThickness(0);
                            options4[1].setOutlineThickness(0);
                            options4[2].setOutlineThickness(0);
                        }
                        else{
                            options4[j].setOutlineThickness(0);
                        }
                        j++;
                    }

                    
                }
                else if (event.type == sf::Event::MouseButtonPressed){
                    sf::Vector2i mousePos = sf::Mouse::getPosition( window );
                    sf::Vector2f mousePosF( static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ) );                   
                    if(options4[3].getGlobalBounds().contains(mousePosF)){
                        completed_ = false;
                        state = 0;
                        return;
                        break;
                    }
                    
                } 
                else if(event.key.code == sf::Keyboard::BackSpace){
                    if(!playerInput.isEmpty()){
                        playerInput.erase(playerInput.getSize()-1, 1);
                        namelen--;
                        playerText.setString(playerInput);
                        playerText.setOrigin(playerText.getLocalBounds().width /2, playerText.getLocalBounds().height /2);
                    }
                }
                else if (event.type == sf::Event::TextEntered && score_entered == false && event.key.code != sf::Keyboard::BackSpace){
                    if(namelen < 10){
                        namelen++;
                        playerInput +=event.text.unicode;
                        playerText.setString(playerInput);
                        playerText.setOrigin(playerText.getLocalBounds().width /2, playerText.getLocalBounds().height /2);
                    }    
                }
                else if(event.key.code == sf::Keyboard::Enter && score_entered == false){
                    new_score = std::make_pair(last_score, playerInput);
                    score_entered = true;
                    refreshScores(scores, new_score);
                }

            }

        window.clear();
        window.draw(bg);

        if (completed == true){
            window.draw(options4[4]);
            if (placement < 11)
                window.draw(options4[1]);
                window.draw(options4[2]);
                if(score_entered == false)
                    window.draw(playerText);  
            for (auto i : scores_sf_)
                window.draw(i);

            window.draw(options4[0]);
            window.draw(options4[3]);  
       }
       else{
           window.draw(options4[0]);
           window.draw(options4[3]); 
           window.draw(level1);
           window.draw(level2);
           window.draw(level3);
            for (auto i : lvl1)
                window.draw(i);
            for (auto i : lvl2)
                window.draw(i);
            for (auto i : lvl3)
                window.draw(i);
       }
      


        window.display();
    }
}


void Graphics::refreshScores(std::vector<std::pair<int, std::string>> scores, std::pair<int, std::string> new_score){
    scores.push_back(new_score);
    sort(scores.rbegin(), scores.rend());
    scores.resize(10);
    scores_sf_.clear();
    int ind = 1;
    for (auto i : scores){
        sf::Text t;
        t.setFont(font_);
        t.setString(std::to_string(ind) + ". "+ i.second + " " + std::to_string(i.first));
        t.setCharacterSize(30);
        t.setFillColor(sf::Color::Red);
        t.setOutlineColor(sf::Color::White);
        t.setPosition(window.getSize().x/4.f + 600.f, window.getSize().y/5 - 150.f + 50.f*ind);
        scores_sf_.push_back(t);
        ind++;
    }
    std::ofstream ostr;
    ostr.open("../content/highscores_" + std::to_string(level_));
    for (auto i : scores){
        ostr << i.second << " " <<i.first <<"\n";
    }
    ostr.close();

}


