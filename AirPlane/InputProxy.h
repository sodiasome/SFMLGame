#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <functional>
#include <memory>

// 输入代理类 - Qt风格的代理模式实现
// 提供输入抽象、映射配置和事件过滤功能
// sodiasome@163.com

// 游戏动作枚举
enum class GameAction
{
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    Fire,
    Pause,
    Exit
};

// 抽象输入处理接口
class IInputHandler
{
public:
    virtual ~IInputHandler() = default;
    virtual bool isActionPressed(GameAction action) const = 0;
    virtual bool isActionJustPressed(GameAction action) = 0;
    virtual void update(const sf::Event& event) = 0;
    virtual void processFrame() = 0;
};

// 真实输入处理器
class RealInputHandler : public IInputHandler
{
private:
    std::map<GameAction, sf::Keyboard::Key> mKeyMapping;
    std::map<GameAction, bool> mPreviousState;
    std::map<GameAction, bool> mCurrentState;

public:
    RealInputHandler()
    {
        // 默认键位映射
        mKeyMapping[GameAction::MoveLeft] = sf::Keyboard::A;
        mKeyMapping[GameAction::MoveRight] = sf::Keyboard::D;
        mKeyMapping[GameAction::MoveUp] = sf::Keyboard::W;
        mKeyMapping[GameAction::MoveDown] = sf::Keyboard::S;
        mKeyMapping[GameAction::Fire] = sf::Keyboard::Space;
        mKeyMapping[GameAction::Pause] = sf::Keyboard::P;
        mKeyMapping[GameAction::Exit] = sf::Keyboard::Escape;

        // 初始化状态
        for (const auto& mapping : mKeyMapping)
        {
            mPreviousState[mapping.first] = false;
            mCurrentState[mapping.first] = false;
        }
    }

    void setKeyMapping(GameAction action, sf::Keyboard::Key key)
    {
        mKeyMapping[action] = key;
    }

    bool isActionPressed(GameAction action) const override
    {
        auto it = mCurrentState.find(action);
        return it != mCurrentState.end() ? it->second : false;
    }

    bool isActionJustPressed(GameAction action) override
    {
        auto currentIt = mCurrentState.find(action);
        auto previousIt = mPreviousState.find(action);
        
        if (currentIt == mCurrentState.end() || previousIt == mPreviousState.end())
            return false;
            
        return currentIt->second && !previousIt->second;
    }

    void update(const sf::Event& event) override
    {
        // 处理键盘事件
        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            bool pressed = (event.type == sf::Event::KeyPressed);
            
            for (const auto& mapping : mKeyMapping)
            {
                if (mapping.second == event.key.code)
                {
                    mCurrentState[mapping.first] = pressed;
                    break;
                }
            }
        }
    }

    void processFrame() override
    {
        // 更新实时键盘状态
        for (const auto& mapping : mKeyMapping)
        {
            mPreviousState[mapping.first] = mCurrentState[mapping.first];
            mCurrentState[mapping.first] = sf::Keyboard::isKeyPressed(mapping.second);
        }
    }
};

// 输入代理类
class InputProxy : public IInputHandler
{
private:
    std::unique_ptr<RealInputHandler> mRealHandler;
    mutable std::map<GameAction, int> mActionCount; // 动作计数
    std::function<bool(GameAction)> mInputFilter;   // 输入过滤器
    bool mInputEnabled;

public:
    InputProxy() : mInputEnabled(true), mInputFilter(nullptr)
    {
        mRealHandler = std::make_unique<RealInputHandler>();
    }

    // 设置输入过滤器
    void setInputFilter(std::function<bool(GameAction)> filter)
    {
        mInputFilter = filter;
    }

    // 启用/禁用输入
    void setInputEnabled(bool enabled)
    {
        mInputEnabled = enabled;
    }

    bool isInputEnabled() const { return mInputEnabled; }

    // 设置键位映射
    void setKeyMapping(GameAction action, sf::Keyboard::Key key)
    {
        mRealHandler->setKeyMapping(action, key);
    }

    bool isActionPressed(GameAction action) const override
    {
        if (!mInputEnabled) return false;
        
        // 应用输入过滤器
        if (mInputFilter && !mInputFilter(action))
        {
            return false;
        }

        // 增加访问计数
        mActionCount[action]++;
        
        return mRealHandler->isActionPressed(action);
    }

    bool isActionJustPressed(GameAction action) override
    {
        if (!mInputEnabled) return false;
        
        if (mInputFilter && !mInputFilter(action))
        {
            return false;
        }

        mActionCount[action]++;
        return mRealHandler->isActionJustPressed(action);
    }

    void update(const sf::Event& event) override
    {
        if (mInputEnabled)
        {
            mRealHandler->update(event);
        }
    }

    void processFrame() override
    {
        if (mInputEnabled)
        {
            mRealHandler->processFrame();
        }
    }

    // 代理特有功能
    int getActionCount(GameAction action) const
    {
        auto it = mActionCount.find(action);
        return it != mActionCount.end() ? it->second : 0;
    }

    void resetActionCounts()
    {
        mActionCount.clear();
    }

    // 获取所有动作的统计信息
    void printInputStats() const
    {
        std::cout << "Input Statistics:" << std::endl;
        for (const auto& pair : mActionCount)
        {
            std::cout << "Action " << static_cast<int>(pair.first) 
                      << ": " << pair.second << " times" << std::endl;
        }
    }
};

// 输入管理器，支持多个输入代理
class InputProxyManager
{
private:
    std::vector<std::shared_ptr<InputProxy>> mInputProxies;
    int mActiveProxyIndex;

public:
    InputProxyManager() : mActiveProxyIndex(0)
    {
        // 创建默认输入代理
        mInputProxies.push_back(std::make_shared<InputProxy>());
    }

    void addInputProxy(std::shared_ptr<InputProxy> proxy)
    {
        mInputProxies.push_back(proxy);
    }

    void setActiveProxy(int index)
    {
        if (index >= 0 && index < static_cast<int>(mInputProxies.size()))
        {
            mActiveProxyIndex = index;
        }
    }

    InputProxy& getActiveProxy()
    {
        return *mInputProxies[mActiveProxyIndex];
    }

    const InputProxy& getActiveProxy() const
    {
        return *mInputProxies[mActiveProxyIndex];
    }

    void updateAll(const sf::Event& event)
    {
        for (auto& proxy : mInputProxies)
        {
            proxy->update(event);
        }
    }

    void processFrameAll()
    {
        for (auto& proxy : mInputProxies)
        {
            proxy->processFrame();
        }
    }
};