#include "SceneNode.h"
#include <assert.h>

SceneNode::SceneNode()
{
}

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
    // 借助 lambda 表达式在容器中搜索指定的节点
    // STL 算法 std：：find_if（） 返回找到的元素的迭代器
    // 如果元素的指针 p.get（） 等于所需节点的地址
    auto found = std::find_if(mChildren.begin(), mChildren.end(),
        [&](Ptr& p) -> bool {
            return p.get() == &node;
        });
    assert(found != mChildren.end());
    // 将找到的节点从容器移动到变量 result 中。
    // 我们将节点的父指针设置为 nullptr，从容器中擦除空元素，并返回包含分离节点的智能指针。
    Ptr result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);
    return result;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // 可以使用重载的乘法运算符链接两个转换（一个接一个地应用）。
    // 我们带有 operator\*= 的表达式将父节点的绝对变换与当前节点的相对变换组合在一起。
    // 结果是当前节点的绝对变换，它存储了场景节点在世界中的放置位置。
    states.transform *= getTransform();

    drawCurrent(target, states);
    for (auto itr = mChildren.begin(); itr != mChildren.end(); ++itr)
    {
        (*itr)->draw(target, states);
    }
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
}
