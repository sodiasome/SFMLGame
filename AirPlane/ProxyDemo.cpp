// Qt代理模式演示程序
// 演示ResourceProxy、InputProxy和RenderProxy的基本用法
// sodiasome@163.com

#include <iostream>
#include "ResourceProxy.h"
#include "InputProxy.h"
#include "RenderProxy.h"
#include "Resources.h"

// 简单的演示程序，不依赖SFML图形界面
void demonstrateResourceProxy()
{
    std::cout << "\n=== Resource Proxy Demo ===\n";
    
    try
    {
        // 创建资源代理管理器
        ResourceProxyManager<sf::Texture, Textures::ID> proxyManager;
        
        // 加载资源（延迟加载）
        proxyManager.loadResource(Textures::Airplane, "source/player/plane0.png");
        std::cout << "Resource proxy created for airplane texture\n";
        
        // 预加载资源
        proxyManager.preloadResource(Textures::Airplane);
        std::cout << "Airplane texture preloaded\n";
        
        // 获取资源（这会触发实际加载）
        try 
        {
            auto& texture = proxyManager.getResource(Textures::Airplane);
            std::cout << "Texture accessed successfully\n";
            std::cout << "Texture size: " << texture.getSize().x << "x" << texture.getSize().y << "\n";
        }
        catch (const std::exception& e)
        {
            std::cout << "Could not load texture: " << e.what() << "\n";
            std::cout << "This is expected if the texture file doesn't exist\n";
        }
        
        // 打印资源统计
        proxyManager.printResourceStats();
        
        // 演示垃圾回收
        std::cout << "Running garbage collection...\n";
        proxyManager.garbageCollect(0.0f); // 立即回收所有未使用的资源
        
    }
    catch (const std::exception& e)
    {
        std::cout << "Resource proxy demo error: " << e.what() << "\n";
    }
}

void demonstrateInputProxy()
{
    std::cout << "\n=== Input Proxy Demo ===\n";
    
    try
    {
        // 创建输入代理管理器
        InputProxyManager inputManager;
        auto& inputProxy = inputManager.getActiveProxy();
        
        // 设置自定义键位映射
        inputProxy.setKeyMapping(GameAction::Fire, sf::Keyboard::Space);
        inputProxy.setKeyMapping(GameAction::Exit, sf::Keyboard::Escape);
        std::cout << "Custom key mappings configured\n";
        
        // 设置输入过滤器
        inputProxy.setInputFilter([](GameAction action) -> bool {
            // 示例：禁用Exit动作
            return action != GameAction::Exit;
        });
        std::cout << "Input filter set (Exit action disabled)\n";
        
        // 模拟一些输入事件
        sf::Event event;
        event.type = sf::Event::KeyPressed;
        event.key.code = sf::Keyboard::W;
        inputProxy.update(event);
        
        event.key.code = sf::Keyboard::A;
        inputProxy.update(event);
        
        inputProxy.processFrame();
        
        // 检查输入状态
        std::cout << "Move Up pressed: " << (inputProxy.isActionPressed(GameAction::MoveUp) ? "Yes" : "No") << "\n";
        std::cout << "Move Left pressed: " << (inputProxy.isActionPressed(GameAction::MoveLeft) ? "Yes" : "No") << "\n";
        
        // 打印输入统计
        inputProxy.printInputStats();
        
        // 测试输入过滤
        std::cout << "Testing input filter (Exit should be blocked)...\n";
        event.key.code = sf::Keyboard::Escape;
        inputProxy.update(event);
        std::cout << "Exit pressed (filtered): " << (inputProxy.isActionPressed(GameAction::Exit) ? "Yes" : "No") << "\n";
        
    }
    catch (const std::exception& e)
    {
        std::cout << "Input proxy demo error: " << e.what() << "\n";
    }
}

void demonstrateRenderProxy()
{
    std::cout << "\n=== Render Proxy Demo ===\n";
    
    try
    {
        // 由于RenderProxy需要sf::RenderWindow，这里我们演示概念
        std::cout << "Render proxy provides:\n";
        std::cout << "- Batched rendering for performance\n";
        std::cout << "- Priority-based render queue\n";
        std::cout << "- Layer-based rendering\n";
        std::cout << "- Render statistics collection\n";
        std::cout << "- Automatic render state management\n";
        
        std::cout << "\nRender proxy features:\n";
        std::cout << "1. Deferred rendering - commands are queued and executed in optimal order\n";
        std::cout << "2. Priority system - important objects render first\n";
        std::cout << "3. Depth sorting - objects render from back to front automatically\n";
        std::cout << "4. Statistics - track draw calls, frame times, and performance\n";
        std::cout << "5. Layer management - separate UI, game, background rendering\n";
        
        // 在实际使用中，你会这样使用：
        // sf::RenderWindow window(sf::VideoMode(800, 600), "Game");
        // LayeredRenderProxy renderProxy(window);
        // renderProxy.getLayer(LayeredRenderProxy::Game).draw(sprite);
        // renderProxy.display();
        
    }
    catch (const std::exception& e)
    {
        std::cout << "Render proxy demo error: " << e.what() << "\n";
    }
}

void printProxyPatternExplanation()
{
    std::cout << "\n=== Qt Proxy Pattern Explanation ===\n";
    std::cout << "代理模式 (Proxy Pattern) 是一种结构型设计模式，它为另一个对象提供一个替身或占位符以控制对这个对象的访问。\n\n";
    
    std::cout << "在Qt中，代理模式广泛应用于：\n";
    std::cout << "1. QNetworkProxy - 网络代理，控制网络访问\n";
    std::cout << "2. QAbstractProxyModel - 数据模型代理，用于MVC架构\n";
    std::cout << "3. QSortFilterProxyModel - 排序和过滤代理\n";
    std::cout << "4. QGraphicsProxyWidget - 图形代理组件\n\n";
    
    std::cout << "代理模式的主要类型：\n";
    std::cout << "1. 虚代理 (Virtual Proxy) - 延迟加载昂贵对象\n";
    std::cout << "2. 保护代理 (Protection Proxy) - 控制访问权限\n";
    std::cout << "3. 缓存代理 (Cache Proxy) - 缓存昂贵操作的结果\n";
    std::cout << "4. 远程代理 (Remote Proxy) - 代表远程对象\n\n";
    
    std::cout << "在这个SFML游戏项目中实现的代理类：\n";
    std::cout << "1. ResourceProxy - 实现虚代理，延迟加载游戏资源\n";
    std::cout << "2. InputProxy - 实现保护代理，控制输入访问和过滤\n";
    std::cout << "3. RenderProxy - 实现缓存代理，优化渲染性能\n\n";
}

// 主演示函数
int demonstrateProxyPattern()
{
    std::cout << "Qt Proxy Pattern Demonstration\n";
    std::cout << "==============================\n";
    
    // 打印代理模式说明
    printProxyPatternExplanation();
    
    // 演示各种代理
    demonstrateResourceProxy();
    demonstrateInputProxy();
    demonstrateRenderProxy();
    
    std::cout << "\n=== Demonstration Complete ===\n";
    std::cout << "代理模式演示完成！查看源代码了解详细实现。\n";
    std::cout << "Proxy pattern demonstration complete! Check source code for detailed implementation.\n";
    
    return 0;
}