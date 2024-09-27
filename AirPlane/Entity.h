#pragma once

//实体基类
//sodiasome@163.com

#include <SFML/Graphics.hpp>
using namespace sf;

class Entity
{
public:
    void setVelocity(sf::Vector2f velocity);
    void setVelocity(float vx, float vy);
    sf::Vector2f getVelocity() const;
private:
	sf::Vector2f mVelocity;//速度
};

