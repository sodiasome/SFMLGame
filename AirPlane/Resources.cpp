#include "Resources.h"

Resources::Resources()
{
}

Resources::~Resources()
{
}

void Resources::Load(Textures::ID id, const std::string& filename)
{
    // 首先创建一个 sf：：Texture 对象并将其存储在唯一指针中
    std::unique_ptr<sf::Texture> texture(new sf::Texture());
    // 加载文件
    texture->loadFromFile(filename);
    // 使用 std：：move（） 从变量 texture 中获取所有权，并将其作为参数传递给 std：：make_pair（）
    // 将纹理插入到地图 mTextureMap 中
    mTextureMap.insert(std::make_pair(id, std::move(texture)));
}

sf::Texture& Resources::Get(Textures::ID id)
{
    auto found = mTextureMap.find(id);
    // std::make_pair
    return *found->second;
}
