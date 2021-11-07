#pragma once
#include <SFML/Graphics.hpp>



class Entity : public sf::Drawable, public sf::Transformable {
public:
    Entity(sf::Texture const & texture);
    void setVelocity(sf::Vector2f const &  vel) { mVelocity = vel;}
    virtual void update(sf::Time dt = sf::Time::Zero); 
    //virtual bool collision ();
    virtual ~Entity() {}
    bool isAlive;
    float mRadius;
    
protected:
    sf::Sprite mSprite;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::Vector2f mVelocity;
    sf::CircleShape mCircle;
    //float mRadius;
    void outOfWindow();
    float x,y;
    float pi = 3.1415926535;

};



class Animation : public Entity{
public:
    Animation(sf::Texture const & texture, int rows, int cols, int width, int height, float vel, float x, float y);
    virtual void update(sf::Time dt = sf::Time::Zero)  override; 
private:
    int mRows, mCols, mWidth, mHeight;
    float mSpeed;
    float mCurrentCol;
    int   mCurrentRow;
};



class Asteroid : public Entity{
public:
    Asteroid(sf::Texture const & texture, int size);
    //void gameOver();
    virtual void update(sf::Time dt = sf::Time::Zero)  override;
private:
    void getDir(sf::Time dt);
    int mSize;
    int mRotate;
    int smjer;
    //float x, y;
};


class Player : public Entity{
public:
    Player(sf::Texture const & texture);
    virtual void update(sf::Time dt = sf::Time::Zero)  override;
    void handlePlayerInput(sf::Keyboard::Key code, bool isPressed);
    bool bullet();
    sf::Vector2f pos() { return getPosition(); }
    float rot() { return getRotation(); }
    bool collision(Asteroid asteroid);
    void reset();
private:
    //float x,y;
    bool mIsMovingUp    = false;
    bool mIsMovingDown  = false;
    bool mIsMovingLeft  = false;
    bool mIsMovingRight = false;
    bool mIsBulletShot = false;
    bool isCollision = false;
    
    float mVel = 0.0f;
    float distance (sf::Vector2f x, sf::Vector2f y);
    
};


class Bullet : public Entity{
public:
    Bullet(sf::Texture const & texture, sf::Vector2f pos, float rot );
    virtual void update(sf::Time dt = sf::Time::Zero)  override;
};