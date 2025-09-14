#pragma once

// 游戏主逻辑 - 集成代理模式示例
// 演示ResourceProxy、InputProxy和RenderProxy的使用
// sodiasome@163.com

#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "ResourceProxy.h"
#include "InputProxy.h"
#include "RenderProxy.h"

using namespace sf;

class GameWithProxies
{
public:
    GameWithProxies();
    ~GameWithProxies();
    void Run();                    // 游戏主循环
    
private:
    void ProcessEvent();           // 处理事件
    void Update();                 // 游戏操作更新
    void Render();                 // 游戏渲染
    void InitializeProxies();      // 初始化代理类
    void PrintProxyStats();        // 打印代理统计信息
    
private:
    sf::RenderWindow mWindow;      // 游戏窗口
    sf::Sprite mPlaySprite;
    
    // 传统资源管理（用于对比）
    Resources<sf::Texture, Textures::ID> mResources;
    
    // 代理类实例
    std::unique_ptr<ResourceProxyManager<sf::Texture, Textures::ID>> mResourceProxy;
    std::unique_ptr<InputProxyManager> mInputProxy;
    std::unique_ptr<LayeredRenderProxy> mRenderProxy;
    
    sf::Time mTimeFame;            // 每帧时间
    double mSpeedFame;             // 每帧移动速度
    
    // 用于演示的额外精灵
    sf::Sprite mBackgroundSprite;
    sf::Text mStatsText;
    sf::Font mFont;
    
    // 统计信息
    int mFrameCount;
    sf::Clock mStatsClock;
};

// 演示如何在现有游戏中集成代理模式
class ProxyDemoGame
{
private:
    sf::RenderWindow mWindow;
    std::unique_ptr<LayeredRenderProxy> mRenderProxy;
    std::unique_ptr<InputProxyManager> mInputProxy;
    std::unique_ptr<ResourceProxyManager<sf::Texture, Textures::ID>> mResourceProxy;
    
    // 游戏对象
    std::vector<sf::Sprite> mGameObjects;
    sf::Text mInfoText;
    sf::Font mFont;
    
    bool mShowStats;
    sf::Clock mFrameClock;

public:
    ProxyDemoGame() : mWindow(sf::VideoMode(800, 600), "Proxy Pattern Demo"), mShowStats(false)
    {
        initializeProxies();
        loadResources();
        createGameObjects();
    }

    void run()
    {
        while (mWindow.isOpen())
        {
            processEvents();
            update();
            render();
        }
    }

private:
    void initializeProxies()
    {
        // 初始化渲染代理
        mRenderProxy = std::make_unique<LayeredRenderProxy>(mWindow);
        
        // 初始化输入代理
        mInputProxy = std::make_unique<InputProxyManager>();
        
        // 设置输入过滤器 - 演示权限控制
        mInputProxy->getActiveProxy().setInputFilter([this](GameAction action) -> bool {
            // 示例：在显示统计信息时禁用游戏控制
            if (mShowStats && (action == GameAction::Fire || action == GameAction::MoveLeft || 
                              action == GameAction::MoveRight || action == GameAction::MoveUp || 
                              action == GameAction::MoveDown))
            {
                return false;
            }
            return true;
        });
        
        // 初始化资源代理
        mResourceProxy = std::make_unique<ResourceProxyManager<sf::Texture, Textures::ID>>();
    }

    void loadResources()
    {
        // 使用代理加载资源
        mResourceProxy->loadResource(Textures::Airplane, "source/player/plane0.png");
        
        // 演示预加载
        mResourceProxy->preloadResource(Textures::Airplane);
        
        // 加载字体（传统方式，因为代理主要针对纹理）
        if (!mFont.loadFromFile("source/arial.ttf"))
        {
            // 如果无法加载字体，使用默认字体
            mInfoText.setFont(mFont);
        }
    }

    void createGameObjects()
    {
        // 创建演示用的游戏对象
        for (int i = 0; i < 5; ++i)
        {
            sf::Sprite sprite;
            try 
            {
                sprite.setTexture(mResourceProxy->getResource(Textures::Airplane));
                sprite.setPosition(100.0f + i * 120.0f, 200.0f);
                sprite.setScale(0.5f, 0.5f);
                mGameObjects.push_back(sprite);
            }
            catch (const std::exception& e)
            {
                std::cout << "Error loading texture: " << e.what() << std::endl;
            }
        }

        // 设置信息文本
        mInfoText.setFont(mFont);
        mInfoText.setCharacterSize(16);
        mInfoText.setFillColor(sf::Color::White);
        mInfoText.setPosition(10, 10);
    }

    void processEvents()
    {
        sf::Event event;
        while (mWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                mWindow.close();

            // 更新输入代理
            mInputProxy->updateAll(event);
        }
        
        // 处理每帧输入
        mInputProxy->processFrameAll();
        
        // 检查代理化的输入
        auto& inputProxy = mInputProxy->getActiveProxy();
        
        if (inputProxy.isActionJustPressed(GameAction::Exit))
        {
            mWindow.close();
        }
        
        if (inputProxy.isActionJustPressed(GameAction::Pause))
        {
            mShowStats = !mShowStats;
        }
    }

    void update()
    {
        auto& inputProxy = mInputProxy->getActiveProxy();
        
        // 移动游戏对象
        sf::Vector2f movement(0, 0);
        if (inputProxy.isActionPressed(GameAction::MoveLeft))
            movement.x -= 100.0f;
        if (inputProxy.isActionPressed(GameAction::MoveRight))
            movement.x += 100.0f;
        if (inputProxy.isActionPressed(GameAction::MoveUp))
            movement.y -= 100.0f;
        if (inputProxy.isActionPressed(GameAction::MoveDown))
            movement.y += 100.0f;

        float deltaTime = mFrameClock.restart().asSeconds();
        movement *= deltaTime;

        for (auto& sprite : mGameObjects)
        {
            sprite.move(movement);
        }

        // 垃圾回收未使用的资源
        mResourceProxy->garbageCollect(30.0f); // 30秒后卸载未使用的资源

        // 更新统计信息文本
        if (mShowStats)
        {
            updateStatsText();
        }
    }

    void render()
    {
        mRenderProxy->clear();

        // 渲染到不同层级
        auto& gameLayer = mRenderProxy->getLayer(LayeredRenderProxy::Game);
        auto& uiLayer = mRenderProxy->getLayer(LayeredRenderProxy::UI);

        // 渲染游戏对象到游戏层
        for (const auto& sprite : mGameObjects)
        {
            gameLayer.draw(sprite);
        }

        // 渲染UI到UI层
        if (mShowStats)
        {
            uiLayer.draw(mInfoText);
        }

        mRenderProxy->display();
    }

    void updateStatsText()
    {
        auto& inputProxy = mInputProxy->getActiveProxy();
        
        std::string stats = "Proxy Pattern Demo - Statistics\n";
        stats += "================================\n";
        stats += "Controls: WASD - Move, P - Toggle Stats, ESC - Exit\n\n";
        
        stats += "Input Proxy Stats:\n";
        stats += "Move Left pressed: " + std::to_string(inputProxy.getActionCount(GameAction::MoveLeft)) + " times\n";
        stats += "Move Right pressed: " + std::to_string(inputProxy.getActionCount(GameAction::MoveRight)) + " times\n";
        stats += "Move Up pressed: " + std::to_string(inputProxy.getActionCount(GameAction::MoveUp)) + " times\n";
        stats += "Move Down pressed: " + std::to_string(inputProxy.getActionCount(GameAction::MoveDown)) + " times\n\n";
        
        stats += "Resource Proxy Stats:\n";
        // 这里可以添加资源代理的统计信息
        
        stats += "\nRender Proxy Stats:\n";
        // 这里可以添加渲染代理的统计信息
        
        mInfoText.setString(stats);
    }
};