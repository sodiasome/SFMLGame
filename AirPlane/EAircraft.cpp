#include "EAircraft.h"

EAircraft::EAircraft(Textures::Plane type, Resources<sf::Texture, Textures::Plane>& textureResource)
	: mType(type)
	, mSprite(textureResource.Get(Textures::Plane::Host))
{
}

void EAircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}
