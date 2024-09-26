#pragma once
#include<map>
#include<memory>
#include<SFML/Graphics.hpp>

using namespace sf;

//资源加载
//sodiasome@163.com

namespace Textures
{
    enum ID { Landscape, Airplane, Missile };//景观、飞机、飞弹
}

class Resources
{
public:
    Resources();
    ~Resources();
    void Load(Textures::ID id, const std::string& filename);
    sf::Texture& Get(Textures::ID id);
    const sf::Texture& Get(Textures::ID id) const;
private:
    std::map<Textures::ID, std::unique_ptr<sf::Texture>> mTextureMap;
};

