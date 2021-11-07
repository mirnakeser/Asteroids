#include "game.h"
#include <stdexcept>


Game::Game() : mWindow(sf::VideoMode(1280, 1024), "SFML window") {
    mBackgroundSprite.setTexture(mTextureHolder.getTexture(Textures::background));
    
    mDtFixed = sf::seconds(1.0f/60.0f);

    mList.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(Textures::meteorBig1), 1)));
    mList.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(Textures::meteorBig3), 1)));
    mList.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(Textures::meteorMedium1), 2)));
    mList.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(Textures::meteorMedium2), 2)));
    mList.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(Textures::meteorSmall1), 3)));
    mList.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(Textures::meteorSmall2), 3)));


    mPlayer.reset(new Player (mTextureHolder.getTexture(Textures::player)));
    mPlayer->setPosition(mBackgroundSprite.getLocalBounds().width/2, mBackgroundSprite.getLocalBounds().height/2);

    //mBullet.reset(new Bullet(mTextureHolder.getTexture(Textures::bullet), mPlayer));

    mText1.setCharacterSize(25);
    mText1.setColor(sf::Color::White);
    mText1.setString(str1 + std::to_string(mLives));
    sf::FloatRect rect1 = mText1.getLocalBounds();
    mText1.setOrigin(rect1.width/2, rect1.height/2);
    mText1.setPosition(50, 20);

    mText2.setCharacterSize(25);
    mText2.setColor(sf::Color::White);
    mText2.setString(str2 + std::to_string(mScore));
    sf::FloatRect rect2 = mText2.getLocalBounds();
    mText2.setOrigin(rect2.width/2, rect2.height/2);
    mText2.setPosition(1000, 20);

    mText3.setCharacterSize(25);
    mText3.setColor(sf::Color::White);
    mText3.setString(str3);
    sf::FloatRect rect3 = mText3.getLocalBounds();
    mText3.setOrigin(rect3.width/2, rect3.height/2);
    mText3.setPosition(50, 20);


    if (!font.loadFromFile("neuropol_x_rg.ttf"))
        throw std::runtime_error("Cannot open font.");

    mText1.setFont(font);
    mText2.setFont(font);
    mText3.setFont(font);
}

void Game::run(){
    // Start the game loop
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate >= mDtFixed){
            timeSinceLastUpdate -= mDtFixed;
            processEvents();
            update(mDtFixed);
        }       
        render();
    }

}

void Game::processEvents(){
        // Process events
        sf::Event event;
        while (mWindow.pollEvent(event))
        {
            // Close window : exit
            switch(event.type){
                case sf::Event::Closed: 
                    mWindow.close();
                    break;

                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Key::Space && isGameOver)
                    {
                        newGame();
                        return;
                    }
                    mPlayer->handlePlayerInput(event.key.code, true);
                    break;
                case sf::Event::KeyReleased:
                    mPlayer->handlePlayerInput(event.key.code, false);
                    break;
            }
            
        }
}

void Game::update(sf::Time dt){

    mPlayer->update(dt);

    mList.erase(std::remove_if( mList.begin(), mList.end(), [](std::unique_ptr<Entity> const& it )
    {
        return it->isAlive == false;
    }), mList.end());

    for (auto & it : mList)
    {
        if (typeid(*it) == typeid(Asteroid))
        {
            Asteroid *newasteroid = dynamic_cast<Asteroid *>(it.get());
            if(mPlayer->collision(*newasteroid))
            {
                mList.push_back(std::unique_ptr<Animation>(new Animation(mTextureHolder.getTexture(Textures::explosion), 
                4, 4, 256, 256, 0.2f, it->getPosition().x, it->getPosition().y )));
               
                mLives--;
                mText1.setString(str1 + std::to_string(mLives));
                if (!mLives)
                    isGameOver = true;
                mPlayer->reset();
                it->setPosition(rand()%1280, rand()%1024);
                
            }
        }
    }

    if( mPlayer->bullet() ){
        mList.push_back(std::unique_ptr<Bullet>(new Bullet(mTextureHolder.getTexture( Textures::bullet ),
				mPlayer->pos(), mPlayer->rot())));
    }

    for (auto & entity : mList)
        entity->update(dt);
    
    
}

void Game::render(){
       // Clear screen
        mWindow.clear();
        mWindow.draw(mBackgroundSprite);
        for (auto & it : mList)
            if (it->isAlive)
                mWindow.draw(*it);

        if (!isGameOver)
        {
            mWindow.draw(*mPlayer);
            mWindow.draw(mText1); 
        }
        else
            mWindow.draw(mText3);
        mWindow.draw(mText2);

        mWindow.display();   
        
}

void Game::newGame()
{
    isGameOver = false;
    
    mLives = 3;
    mScore = 0;
    mText1.setString( str1 + std::to_string( mLives ) );
    mText2.setString(str2 + std::to_string(mScore));
    
    mPlayer->reset();

    mList.clear();
    mList.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(Textures::meteorBig1), 1)));
    mList.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(Textures::meteorBig3), 1)));
    mList.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(Textures::meteorMedium1), 2)));
    mList.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(Textures::meteorMedium2), 2)));
    mList.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(Textures::meteorSmall1), 3)));
    mList.push_back(std::unique_ptr<Asteroid>(new Asteroid(mTextureHolder.getTexture(Textures::meteorSmall2), 3)));
}