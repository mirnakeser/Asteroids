#include "entities.h"
#include <cstdlib>
#include <time.h>
#include <cmath>
#include <math.h>



Entity::Entity(sf::Texture const & texture){
    mSprite.setTexture(texture);
    auto lb = mSprite.getLocalBounds();
    mSprite.setOrigin(lb.width/2, lb.height/2);
    mVelocity.x=mVelocity.y = 0.0f; 
    isAlive = true;

    mRadius = 1.0*std::min(lb.width/2,lb.height/2);
    mCircle.setRadius(mRadius);
    mCircle.setFillColor(sf::Color(0, 255, 0, 0));
    mCircle.setOutlineThickness(1);
    mCircle.setOutlineColor(sf::Color::Yellow);
    mCircle.setOrigin(mRadius, mRadius);
    
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    states.transform *= getTransform();
    target.draw(mSprite, states);
    //target.draw(mCircle, states);
}

void Entity::update(sf::Time dt){
    auto pos = getPosition();
    float x = pos.x + mVelocity.x*dt.asSeconds();
    float y = pos.y + mVelocity.y*dt.asSeconds();
    setPosition(x,y);
}

// ako je objekt izasao iz ekrana
void Entity::outOfWindow()
{
    sf::FloatRect lb = mSprite.getLocalBounds();
    float min = std::min(lb.width/2, lb.height/2);

    if (x + min < 0)
        x = 1280.0f + min;
    else if (x - min > 1280 )
        x = 0.0f - min;
    else if (y + min < 0)
        y = 1024.0f + min;
    else if (y - min > 1024)
        y = 0.0f - min;

    setPosition(x,y);
}

Animation::Animation(sf::Texture const & texture, int rows, int cols, int width, int height, float vel, float x, float y) :
  Entity(texture), mRows(rows), mCols(cols), mWidth(width/cols), mHeight(height/rows), mSpeed(vel),
  mCurrentCol(0.0f),  mCurrentRow(0) {
    auto lb = mSprite.getLocalBounds();
    mSprite.setOrigin(lb.width/8, lb.height/8);
    setPosition(x, y);
  }

 void Animation::update(sf::Time dt){
    if(!isAlive) return;
    mCurrentCol += mSpeed;
    if(static_cast<int>(mCurrentCol) == mCols){
        mCurrentCol = 0.0f;
        ++mCurrentRow;
    }
    if(mCurrentRow == mRows){
        mCurrentCol = 0.0f;
        mCurrentRow = 0;
        isAlive = false;
        return;
    }

    int i = mCurrentCol;
    int j = mCurrentRow; 
    mSprite.setTextureRect(sf::IntRect(i*mWidth, j*mHeight, mWidth, mHeight));
} 



Asteroid::Asteroid(sf::Texture const & texture, int size) : Entity(texture), mSize(size)
{
    setPosition(rand()%1280, rand()%1024);
    sf::Vector2f vel (static_cast<float>(rand()%200), static_cast<float>(rand()%200));
    setVelocity(vel);
    mRotate = rand()%20;
    smjer = rand()%4;
}

void Asteroid::update(sf::Time dt)
{
    rotate(mRotate);
    getDir(dt);
    setPosition(x,y);
    outOfWindow();
    
}
/*
void Asteroid::gameOver()
{
    setRotation(0);
    setVelocity(sf::Vector2f(0.0f, 0.0f));
}*/

void Asteroid::getDir (sf::Time dt)
{
    auto pos = getPosition();

    // "random" određivanje smjera kretanja asteroida
    if ( smjer == 0 )
    {
        x = pos.x + mVelocity.x*dt.asSeconds();
        y = pos.y + mVelocity.y*dt.asSeconds();
    }

    else if ( smjer == 1 )
    {
        x = pos.x - mVelocity.x*dt.asSeconds();
        y = pos.y + mVelocity.y*dt.asSeconds();
    }

    else if ( smjer == 2 )
    {
        x = pos.x + mVelocity.x*dt.asSeconds();
        y = pos.y - mVelocity.y*dt.asSeconds();
    }

    else if ( smjer == 3)
    {
        x = pos.x - mVelocity.x*dt.asSeconds();
        y = pos.y - mVelocity.y*dt.asSeconds();
    }

}


Player::Player(sf::Texture const & texture) : Entity(texture){
    setVelocity(sf::Vector2f(0.0f, 0.0f));
}

void Player::update(sf::Time dt){

    sf::Vector2f pos = getPosition();

    if(mIsMovingUp)
       mVel += 25.0f;
        
    if(mIsMovingDown){
        mVel -= 25.0f;
        if( mVel < 0.0f )
            mVel = 0.0f;
    }
            
    if(mIsMovingLeft)
        setRotation(getRotation()-2);

    if(mIsMovingRight)
        setRotation(getRotation()+2);

    mVelocity.x = sin(getRotation()*pi/180)*mVel;
    mVelocity.y = -cos(getRotation()*pi/180)*mVel;

    x = pos.x + mVelocity.x*dt.asSeconds();
    y = pos.y + mVelocity.y*dt.asSeconds();
  
    setPosition(x,y);
    outOfWindow();
    
}

void Player::reset(){
    setPosition(1280/2, 1024/2);
    setRotation(0);
    setVelocity(sf::Vector2f(0.0f, 0.0f));
    mVel = 0.f;
    isCollision = false;
}


bool Player::collision(Asteroid asteroid)
{
    float minudaljenost = mRadius + asteroid.mRadius;
    float udaljenost = distance ( getPosition(), asteroid.getPosition() );

    if (udaljenost < minudaljenost)
        isCollision = true;
    else
        isCollision = false;
    
    return isCollision;
    
}

float Player::distance (sf::Vector2f x, sf::Vector2f y)
{
    float prvi = x.x - y.x;
    float drugi = x.y - y.y;
    return sqrt( pow(prvi, 2.0f) + pow(drugi, 2.0f) );
}

void Player::handlePlayerInput(sf::Keyboard::Key code, bool isPressed){
    if(code == sf::Keyboard::Key::Up)
        mIsMovingUp = isPressed;
    if(code == sf::Keyboard::Key::Down)
        mIsMovingDown = isPressed;
    if(code == sf::Keyboard::Key::Left)
        mIsMovingLeft = isPressed;
    if(code == sf::Keyboard::Key::Right)
        mIsMovingRight = isPressed;  
    if (code == sf::Keyboard::Key::Space)
        mIsBulletShot = isPressed;          
}

bool Player::bullet(){
    if( mIsBulletShot ){
        mIsBulletShot = false;
        return true;
    }
    return false;
}


Bullet::Bullet(sf::Texture const & texture, sf::Vector2f pos, float rot) : Entity(texture){ 
    setPosition( pos.x, pos.y );
    setRotation( rot - 90 );
    setVelocity(sf::Vector2f(500*cos(getRotation()*pi/180), 500*sin(getRotation()*pi/180)));
}

void Bullet::update(sf::Time dt){
    if(isAlive){
        auto pos = getPosition();
        x = pos.x + mVelocity.x*dt.asSeconds();
        y = pos.y + mVelocity.y*dt.asSeconds();

        auto lb = mSprite.getLocalBounds();
        auto min = std::min(lb.width/2,lb.height/2);
        if( x + min < 0 || x - min > 1280 || y + min < 0 || y - min > 1024 )
            isAlive = false;
        setPosition(x,y);
    }
}