#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace sf;

// 添加场景节点
// 保持可绘制的内容
//sodiasome@163.com

class SceneNode
    : public sf::Transformable,
    public sf::Drawable,
    private sf::NonCopyable //私下派生，以声明我们的场景节点无法被复制
                           //（复制构造函数和复制赋值运算符被禁用）
{
public:
    // 非常频繁地使用 std：：unique_ptr 类型，因此我们为它创建一个 typedef 作为 SceneNode 类中的成员。
    typedef std::unique_ptr<SceneNode> Ptr;
public:
    SceneNode();
    // 插入节点
    void attachChild(Ptr child);
    // 删除节点
    Ptr detachChild(const SceneNode& node);
private:
    // 为了存储子项，我们使用 STL 容器 std：：vector
    std::vector<Ptr> mChildren;
    // 存储了指向父节点的指针
    SceneNode* mParent;

    // 覆盖sf：:D rawable 的纯虚拟 draw（） 函数来渲染整个场景节点
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    // 绘制当前对象（而不是子对象）。它可以被派生自 SceneNode 的类覆盖
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
};

