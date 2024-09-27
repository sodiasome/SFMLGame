#include "EAircraft.h"

EAircraft::EAircraft(Textures::Plane type, Resources<sf::Texture, Textures::Plane>& textureResource)
	: mType(type)
	, mSprite(textureResource.Get(Textures::Plane::Host))
{
	//原点移动到中心
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void EAircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}
