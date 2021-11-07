#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "textureHolder.h"
#include "entities.h"


class Game{
public:
    Game();
    void run();
private:
    void processEvents();
    void update(sf::Time dt);
    void render();
    void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);
    void newGame();

    sf::RenderWindow mWindow;
    TextureHolder mTextureHolder;  
    sf::Sprite mSprite;
    sf::Sprite mBackgroundSprite;
    std::unique_ptr<Animation> mExplosion;
    std::unique_ptr<Player> mPlayer; 
    std::list<std::unique_ptr<Entity>> mList;

    //float mVelocity = 120.0f; // pixel/sec
    sf::Time mDtFixed;

    bool isGameOver = false;

    sf::Text mText1, mText2, mText3;
    std::string str1 = "Lives: ";
    std::string str2 = "Score: ";
    std::string str3 = "Game over. Press space to play again.";
    sf::Font font;
    int mScore = 0, mLives = 3;
};
