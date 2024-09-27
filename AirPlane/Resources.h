#pragma once
#include<map>
#include<memory>
#include<SFML/Graphics.hpp>
#include <assert.h>

using namespace sf;

//资源加载
//sodiasome@163.com

namespace Textures
{
    enum ID { Landscape, Airplane, Missile};//景观、飞机、飞弹
    enum Plane{ Host, Slaver };
}

template <typename Resource, typename Identifier>
class Resources
{
public:
    Resources() {}
    ~Resources() {}

    
    void Load(Identifier id, const std::string& filename)
    {
        // 首先创建一个 sf：：Texture 对象并将其存储在唯一指针中
        std::unique_ptr<Resource> texture(new Resource());

        // 加载文件
        assert(texture->loadFromFile(filename));

        // 使用 std：：move（） 从变量 texture 中获取所有权，并将其作为参数传递给 std：：make_pair（）
        // 将纹理插入到地图 mTextureMap 中
        mTextureMap.insert(std::make_pair(id, std::move(texture)));
        //我们必须插入一行，我们期望 std：：map：：insert（） 返回的对的布尔成员为 true：

    }
    template <typename Parameter>
    void Load(Identifier id, const std::string& filename, const Parameter& secondParam)
    {
        std::unique_ptr<Resource> texture(new Resource());

        assert(texture->loadFromFile(filename, secondParam));//兼容sf::Shader

        mTextureMap.insert(std::make_pair(id, std::move(texture)));
    }

    Resource& Get(Identifier id)
    {
        auto found = mTextureMap.find(id);

        assert(found != mTextureMap.end());
        // std::make_pair
        return *found->second;
    }


private:
    std::map<Identifier, std::unique_ptr<Resource>> mTextureMap;
};

typedef Resources<sf::Texture, Textures::ID> TextureResource;

