#include "Resources.h"
#include <assert.h>

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
    assert(texture->loadFromFile(filename));

    // 使用 std：：move（） 从变量 texture 中获取所有权，并将其作为参数传递给 std：：make_pair（）
    // 将纹理插入到地图 mTextureMap 中
    mTextureMap.insert(std::make_pair(id,std::move(texture)));
    //我们必须插入一行，我们期望 std：：map：：insert（） 返回的对的布尔成员为 true：
    
}

sf::Texture& Resources::Get(Textures::ID id)
{
    auto found = mTextureMap.find(id);

    assert(found != mTextureMap.end());
    // std::make_pair
    return *found->second;
}
