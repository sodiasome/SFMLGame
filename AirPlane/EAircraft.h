#pragma once
#include "Entity.h"
#include "Resources.h"

//实体飞行器
class EAircraft :public Entity
{
public:
    explicit EAircraft(Textures::Plane type, Resources<sf::Texture, Textures::Plane>& resources);
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
private:
    Textures::Plane mType;
    sf::Sprite mSprite;
};

