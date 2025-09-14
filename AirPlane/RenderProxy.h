#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <queue>
#include <functional>

// 渲染代理类 - Qt风格的代理模式实现
// 提供渲染优化、批处理和状态管理功能
// sodiasome@163.com

// 渲染命令结构
struct RenderCommand
{
    std::function<void(sf::RenderTarget&, sf::RenderStates)> renderFunc;
    int priority;
    float depth;

    RenderCommand(std::function<void(sf::RenderTarget&, sf::RenderStates)> func, 
                 int prio = 0, float d = 0.0f)
        : renderFunc(func), priority(prio), depth(d) {}

    // 用于优先级排序
    bool operator<(const RenderCommand& other) const
    {
        if (priority != other.priority)
            return priority < other.priority;
        return depth > other.depth; // 深度从远到近
    }
};

// 抽象渲染接口
class IRenderTarget
{
public:
    virtual ~IRenderTarget() = default;
    virtual void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default) = 0;
    virtual void clear(const sf::Color& color = sf::Color::Black) = 0;
    virtual void display() = 0;
    virtual sf::Vector2u getSize() const = 0;
};

// 真实渲染目标
class RealRenderTarget : public IRenderTarget
{
private:
    sf::RenderWindow& mWindow;
    mutable int mDrawCallCount;

public:
    RealRenderTarget(sf::RenderWindow& window) : mWindow(window), mDrawCallCount(0) {}

    void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default) override
    {
        mWindow.draw(drawable, states);
        mDrawCallCount++;
    }

    void clear(const sf::Color& color = sf::Color::Black) override
    {
        mWindow.clear(color);
    }

    void display() override
    {
        mWindow.display();
    }

    sf::Vector2u getSize() const override
    {
        return mWindow.getSize();
    }

    int getDrawCallCount() const { return mDrawCallCount; }
    void resetDrawCallCount() { mDrawCallCount = 0; }
};

// 渲染代理类
class RenderProxy : public IRenderTarget
{
private:
    std::unique_ptr<RealRenderTarget> mRealTarget;
    std::priority_queue<RenderCommand> mRenderQueue;
    sf::RenderStates mCurrentStates;
    bool mBatchingEnabled;
    mutable int mTotalDrawCalls;
    mutable int mBatchedDrawCalls;
    sf::Clock mFrameClock;

    // 渲染统计
    struct RenderStats
    {
        int frameCount;
        float totalFrameTime;
        int peakDrawCalls;
        
        RenderStats() : frameCount(0), totalFrameTime(0.0f), peakDrawCalls(0) {}
        
        float getAverageFrameTime() const 
        { 
            return frameCount > 0 ? totalFrameTime / frameCount : 0.0f; 
        }
    } mStats;

public:
    RenderProxy(sf::RenderWindow& window) 
        : mBatchingEnabled(true), mTotalDrawCalls(0), mBatchedDrawCalls(0)
    {
        mRealTarget = std::make_unique<RealRenderTarget>(window);
    }

    void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default) override
    {
        if (mBatchingEnabled)
        {
            // 将渲染命令加入队列
            auto renderFunc = [&drawable, states](sf::RenderTarget& target, sf::RenderStates)
            {
                target.draw(drawable, states);
            };
            
            // 简单的深度计算（基于变换矩阵的位置）
            float depth = states.transform.transformPoint(0, 0).y;
            
            mRenderQueue.emplace(renderFunc, 0, depth);
            mBatchedDrawCalls++;
        }
        else
        {
            // 直接渲染
            mRealTarget->draw(drawable, states);
        }
        mTotalDrawCalls++;
    }

    void clear(const sf::Color& color = sf::Color::Black) override
    {
        mRealTarget->clear(color);
        mFrameClock.restart();
    }

    void display() override
    {
        if (mBatchingEnabled)
        {
            flushRenderQueue();
        }
        
        mRealTarget->display();
        
        // 更新统计信息
        updateStats();
    }

    sf::Vector2u getSize() const override
    {
        return mRealTarget->getSize();
    }

    // 代理特有功能
    void setBatchingEnabled(bool enabled)
    {
        if (!enabled && mBatchingEnabled)
        {
            flushRenderQueue();
        }
        mBatchingEnabled = enabled;
    }

    bool isBatchingEnabled() const { return mBatchingEnabled; }

    // 立即执行所有排队的渲染命令
    void flushRenderQueue()
    {
        while (!mRenderQueue.empty())
        {
            const auto& command = mRenderQueue.top();
            command.renderFunc(*mRealTarget, mCurrentStates);
            mRenderQueue.pop();
        }
    }

    // 添加带优先级的渲染命令
    void drawWithPriority(const sf::Drawable& drawable, 
                         const sf::RenderStates& states, 
                         int priority, 
                         float depth = 0.0f)
    {
        auto renderFunc = [&drawable, states](sf::RenderTarget& target, sf::RenderStates)
        {
            target.draw(drawable, states);
        };
        
        mRenderQueue.emplace(renderFunc, priority, depth);
        mTotalDrawCalls++;
        mBatchedDrawCalls++;
    }

    // 获取渲染统计
    int getTotalDrawCalls() const { return mTotalDrawCalls; }
    int getBatchedDrawCalls() const { return mBatchedDrawCalls; }
    int getQueueSize() const { return static_cast<int>(mRenderQueue.size()); }

    void resetStats()
    {
        mTotalDrawCalls = 0;
        mBatchedDrawCalls = 0;
        mStats = RenderStats();
        if (mRealTarget)
        {
            mRealTarget->resetDrawCallCount();
        }
    }

    void printRenderStats() const
    {
        std::cout << "Render Statistics:" << std::endl;
        std::cout << "  Total Draw Calls: " << mTotalDrawCalls << std::endl;
        std::cout << "  Batched Draw Calls: " << mBatchedDrawCalls << std::endl;
        std::cout << "  Queue Size: " << getQueueSize() << std::endl;
        std::cout << "  Batching Enabled: " << (mBatchingEnabled ? "Yes" : "No") << std::endl;
        std::cout << "  Frame Count: " << mStats.frameCount << std::endl;
        std::cout << "  Average Frame Time: " << mStats.getAverageFrameTime() << "ms" << std::endl;
        std::cout << "  Peak Draw Calls: " << mStats.peakDrawCalls << std::endl;
    }

private:
    void updateStats()
    {
        mStats.frameCount++;
        mStats.totalFrameTime += mFrameClock.getElapsedTime().asMilliseconds();
        
        int currentDrawCalls = mRealTarget->getDrawCallCount();
        if (currentDrawCalls > mStats.peakDrawCalls)
        {
            mStats.peakDrawCalls = currentDrawCalls;
        }
    }
};

// 多层渲染代理管理器
class LayeredRenderProxy
{
public:
    enum Layer
    {
        Background = 0,
        Game = 1,
        UI = 2,
        Debug = 3,
        LayerCount
    };

private:
    std::vector<std::unique_ptr<RenderProxy>> mLayerProxies;
    sf::RenderWindow& mWindow;

public:
    LayeredRenderProxy(sf::RenderWindow& window) : mWindow(window)
    {
        // 为每个层创建渲染代理
        for (int i = 0; i < LayerCount; ++i)
        {
            mLayerProxies.push_back(std::make_unique<RenderProxy>(window));
        }
    }

    RenderProxy& getLayer(Layer layer)
    {
        return *mLayerProxies[layer];
    }

    void clear(const sf::Color& color = sf::Color::Black)
    {
        mWindow.clear(color);
    }

    void display()
    {
        // 按层级顺序渲染
        for (auto& proxy : mLayerProxies)
        {
            proxy->flushRenderQueue();
        }
        mWindow.display();
    }

    void setBatchingEnabled(bool enabled)
    {
        for (auto& proxy : mLayerProxies)
        {
            proxy->setBatchingEnabled(enabled);
        }
    }

    void printAllStats() const
    {
        for (int i = 0; i < LayerCount; ++i)
        {
            std::cout << "Layer " << i << " ";
            mLayerProxies[i]->printRenderStats();
            std::cout << std::endl;
        }
    }

    void resetAllStats()
    {
        for (auto& proxy : mLayerProxies)
        {
            proxy->resetStats();
        }
    }
};