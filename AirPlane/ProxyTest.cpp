// 测试程序：演示Qt代理模式在SFML游戏中的应用
// 这个程序不需要SFML图形界面，可以直接编译运行
// sodiasome@163.com

#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <functional>
#include <string>
#include <chrono>
#include <thread>

// 模拟SFML的基础类型（用于测试）
namespace TestSFML {
    struct Vector2u { 
        unsigned int x, y; 
        Vector2u(unsigned int x = 0, unsigned int y = 0) : x(x), y(y) {}
    };
    
    struct Color { 
        unsigned char r, g, b, a; 
        Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255) 
            : r(r), g(g), b(b), a(a) {}
        static const Color Black;
    };
    const Color Color::Black(0, 0, 0, 255);
    
    class Clock {
        std::chrono::steady_clock::time_point start;
    public:
        Clock() : start(std::chrono::steady_clock::now()) {}
        void restart() { start = std::chrono::steady_clock::now(); }
        float getElapsedTime() const {
            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
            return static_cast<float>(duration.count());
        }
    };
    
    class MockTexture {
        std::string filename;
        bool loaded;
        Vector2u size;
    public:
        MockTexture() : loaded(false), size(0, 0) {}
        bool loadFromFile(const std::string& file) {
            filename = file;
            loaded = true;
            size = Vector2u(64, 64); // 模拟纹理尺寸
            std::cout << "  [MockTexture] Loaded: " << file << " (64x64)\n";
            return true;
        }
        Vector2u getSize() const { return size; }
        bool isLoaded() const { return loaded; }
        const std::string& getFilename() const { return filename; }
    };

    // 模拟键盘按键
    namespace Keyboard {
        enum Key { A, D, W, S, Space, P, Escape };
        static bool isKeyPressed(Key /*key*/) { return false; } // 模拟实现
    }
}

// 使用测试版本的SFML类型
using MockTexture = TestSFML::MockTexture;
using MockClock = TestSFML::Clock;
namespace MockKeyboard = TestSFML::Keyboard;

// 简化的资源代理实现（用于测试）
template<typename Resource>
class SimpleResourceProxy
{
private:
    std::string mFilename;
    std::unique_ptr<Resource> mResource;
    mutable int mAccessCount;
    mutable MockClock mLastAccess;
    bool mLoaded;

public:
    SimpleResourceProxy(const std::string& filename) 
        : mFilename(filename), mAccessCount(0), mLoaded(false)
    {
        std::cout << "ResourceProxy created for: " << filename << "\n";
    }

    Resource& getResource()
    {
        mAccessCount++;
        mLastAccess.restart();
        
        if (!mLoaded)
        {
            std::cout << "Lazy loading resource: " << mFilename << "\n";
            mResource = std::unique_ptr<Resource>(new Resource());
            mResource->loadFromFile(mFilename);
            mLoaded = true;
        }
        
        return *mResource;
    }

    bool isLoaded() const { return mLoaded; }
    int getAccessCount() const { return mAccessCount; }
    float getTimeSinceLastAccess() const { return mLastAccess.getElapsedTime(); }
    
    void printStats() const
    {
        std::cout << "  Resource: " << mFilename << "\n";
        std::cout << "  Loaded: " << (mLoaded ? "Yes" : "No") << "\n";
        std::cout << "  Access Count: " << mAccessCount << "\n";
        std::cout << "  Time since last access: " << getTimeSinceLastAccess() << "ms\n";
    }
};

// 简化的输入代理实现（用于测试）
enum class SimpleGameAction { MoveLeft, MoveRight, MoveUp, MoveDown, Fire, Pause, Exit };

class SimpleInputProxy
{
private:
    std::map<SimpleGameAction, int> mActionCounts;
    std::function<bool(SimpleGameAction)> mFilter;
    bool mEnabled;

public:
    SimpleInputProxy() : mEnabled(true) {}

    void setInputFilter(std::function<bool(SimpleGameAction)> filter)
    {
        mFilter = filter;
    }

    void setEnabled(bool enabled) { mEnabled = enabled; }

    bool simulateAction(SimpleGameAction action)
    {
        if (!mEnabled) return false;
        
        if (mFilter && !mFilter(action))
        {
            std::cout << "Action filtered: " << static_cast<int>(action) << "\n";
            return false;
        }

        mActionCounts[action]++;
        std::cout << "Action executed: " << static_cast<int>(action) 
                  << " (count: " << mActionCounts[action] << ")\n";
        return true;
    }

    int getActionCount(SimpleGameAction action) const
    {
        auto it = mActionCounts.find(action);
        return it != mActionCounts.end() ? it->second : 0;
    }

    void printStats() const
    {
        std::cout << "Input Proxy Statistics:\n";
        for (const auto& pair : mActionCounts)
        {
            std::cout << "  Action " << static_cast<int>(pair.first) 
                      << ": " << pair.second << " times\n";
        }
        std::cout << "  Input Enabled: " << (mEnabled ? "Yes" : "No") << "\n";
    }
};

// 简化的渲染代理实现（用于测试）
class SimpleRenderProxy
{
private:
    int mDrawCalls;
    int mQueuedCalls;
    bool mBatchingEnabled;
    std::vector<std::string> mRenderQueue;

public:
    SimpleRenderProxy() : mDrawCalls(0), mQueuedCalls(0), mBatchingEnabled(true) {}

    void setBatchingEnabled(bool enabled) 
    { 
        if (!enabled && mBatchingEnabled)
        {
            flushQueue();
        }
        mBatchingEnabled = enabled; 
    }

    void draw(const std::string& objectName)
    {
        mDrawCalls++;
        
        if (mBatchingEnabled)
        {
            mRenderQueue.push_back(objectName);
            mQueuedCalls++;
            std::cout << "Queued for rendering: " << objectName << "\n";
        }
        else
        {
            std::cout << "Immediately rendered: " << objectName << "\n";
        }
    }

    void flushQueue()
    {
        if (!mRenderQueue.empty())
        {
            std::cout << "Flushing render queue (" << mRenderQueue.size() << " items):\n";
            for (const auto& item : mRenderQueue)
            {
                std::cout << "  Rendering: " << item << "\n";
            }
            mRenderQueue.clear();
        }
    }

    void printStats() const
    {
        std::cout << "Render Proxy Statistics:\n";
        std::cout << "  Total draw calls: " << mDrawCalls << "\n";
        std::cout << "  Queued calls: " << mQueuedCalls << "\n";
        std::cout << "  Queue size: " << mRenderQueue.size() << "\n";
        std::cout << "  Batching enabled: " << (mBatchingEnabled ? "Yes" : "No") << "\n";
    }
};

// 测试函数
void testResourceProxy()
{
    std::cout << "\n=== Testing Resource Proxy ===\n";
    
    SimpleResourceProxy<MockTexture> textureProxy("player.png");
    
    std::cout << "1. Initial state:\n";
    textureProxy.printStats();
    
    std::cout << "\n2. First access (triggers loading):\n";
    textureProxy.getResource();
    textureProxy.printStats();
    
    std::cout << "\n3. Second access (uses cached resource):\n";
    textureProxy.getResource();
    textureProxy.printStats();
    
    // 模拟时间流逝
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    std::cout << "\n4. After time delay:\n";
    textureProxy.printStats();
}

void testInputProxy()
{
    std::cout << "\n=== Testing Input Proxy ===\n";
    
    SimpleInputProxy inputProxy;
    
    std::cout << "1. Normal input processing:\n";
    inputProxy.simulateAction(SimpleGameAction::MoveLeft);
    inputProxy.simulateAction(SimpleGameAction::MoveRight);
    inputProxy.simulateAction(SimpleGameAction::Fire);
    
    std::cout << "\n2. Setting input filter (blocks Fire action):\n";
    inputProxy.setInputFilter([](SimpleGameAction action) {
        return action != SimpleGameAction::Fire;
    });
    
    inputProxy.simulateAction(SimpleGameAction::MoveLeft);
    inputProxy.simulateAction(SimpleGameAction::Fire); // Should be blocked
    
    std::cout << "\n3. Disabling input:\n";
    inputProxy.setEnabled(false);
    inputProxy.simulateAction(SimpleGameAction::MoveUp); // Should be ignored
    
    std::cout << "\n4. Final statistics:\n";
    inputProxy.printStats();
}

void testRenderProxy()
{
    std::cout << "\n=== Testing Render Proxy ===\n";
    
    SimpleRenderProxy renderProxy;
    
    std::cout << "1. Batched rendering (default):\n";
    renderProxy.draw("Player Sprite");
    renderProxy.draw("Enemy Sprite");
    renderProxy.draw("Background");
    
    std::cout << "\n2. Flushing render queue:\n";
    renderProxy.flushQueue();
    
    std::cout << "\n3. Immediate rendering mode:\n";
    renderProxy.setBatchingEnabled(false);
    renderProxy.draw("UI Element");
    renderProxy.draw("Debug Text");
    
    std::cout << "\n4. Final statistics:\n";
    renderProxy.printStats();
}

void printIntroduction()
{
    std::cout << "Qt代理类(Proxy Classes)演示程序\n";
    std::cout << "================================\n\n";
    std::cout << "什么是代理模式？\n";
    std::cout << "代理模式为另一个对象提供一个替身或占位符，以控制对这个对象的访问。\n\n";
    std::cout << "在Qt中的应用：\n";
    std::cout << "1. QNetworkProxy - 网络代理\n";
    std::cout << "2. QAbstractProxyModel - 数据模型代理\n";
    std::cout << "3. QSortFilterProxyModel - 排序过滤代理\n";
    std::cout << "4. QGraphicsProxyWidget - 图形界面代理\n\n";
    std::cout << "本程序演示的代理类型：\n";
    std::cout << "1. ResourceProxy - 虚代理（延迟加载）\n";
    std::cout << "2. InputProxy - 保护代理（访问控制）\n";
    std::cout << "3. RenderProxy - 缓存代理（性能优化）\n\n";
}

// 主程序
int main()
{
    printIntroduction();
    
    try
    {
        testResourceProxy();
        testInputProxy();
        testRenderProxy();
        
        std::cout << "\n=== 演示完成 ===\n";
        std::cout << "所有代理模式测试完成！\n";
        std::cout << "查看源代码以了解详细实现。\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}