#pragma once
#include <memory>
#include <string>
#include <map>
#include <functional>
#include <SFML/Graphics.hpp>

// 资源代理类 - Qt风格的代理模式实现
// 提供延迟加载、缓存管理和访问控制功能
// sodiasome@163.com

// 抽象资源接口
template<typename Resource>
class IResourceSubject
{
public:
    virtual ~IResourceSubject() = default;
    virtual Resource& getResource() = 0;
    virtual bool isLoaded() const = 0;
    virtual void load() = 0;
};

// 真实资源类
template<typename Resource>
class RealResource : public IResourceSubject<Resource>
{
private:
    std::unique_ptr<Resource> mResource;
    std::string mFilename;
    bool mIsLoaded;

public:
    RealResource(const std::string& filename)
        : mFilename(filename), mIsLoaded(false)
    {
    }

    Resource& getResource() override
    {
        if (!mIsLoaded)
        {
            load();
        }
        return *mResource;
    }

    bool isLoaded() const override
    {
        return mIsLoaded;
    }

    void load() override
    {
        if (!mIsLoaded)
        {
            mResource = std::make_unique<Resource>();
            if (mResource->loadFromFile(mFilename))
            {
                mIsLoaded = true;
            }
        }
    }
};

// 资源代理类
template<typename Resource>
class ResourceProxy : public IResourceSubject<Resource>
{
private:
    std::unique_ptr<RealResource<Resource>> mRealResource;
    std::string mFilename;
    mutable int mAccessCount; // 访问计数
    mutable sf::Clock mLastAccess; // 最后访问时间

    // 访问权限检查函数（可选）
    std::function<bool()> mAccessChecker;

public:
    ResourceProxy(const std::string& filename)
        : mFilename(filename), mAccessCount(0), mAccessChecker(nullptr)
    {
        // 延迟创建真实对象
    }

    // 设置访问权限检查器
    void setAccessChecker(std::function<bool()> checker)
    {
        mAccessChecker = checker;
    }

    Resource& getResource() override
    {
        // 权限检查
        if (mAccessChecker && !mAccessChecker())
        {
            throw std::runtime_error("Access denied to resource: " + mFilename);
        }

        // 延迟创建真实对象
        if (!mRealResource)
        {
            mRealResource = std::make_unique<RealResource<Resource>>(mFilename);
        }

        // 更新访问统计
        mAccessCount++;
        mLastAccess.restart();

        return mRealResource->getResource();
    }

    bool isLoaded() const override
    {
        return mRealResource && mRealResource->isLoaded();
    }

    void load() override
    {
        if (!mRealResource)
        {
            mRealResource = std::make_unique<RealResource<Resource>>(mFilename);
        }
        mRealResource->load();
    }

    // 代理特有的功能
    int getAccessCount() const { return mAccessCount; }
    float getTimeSinceLastAccess() const { return mLastAccess.getElapsedTime().asSeconds(); }
    
    // 预加载功能
    void preload()
    {
        load();
    }

    // 卸载资源（内存管理）
    void unload()
    {
        mRealResource.reset();
        mAccessCount = 0;
    }
};

// 资源代理管理器
template<typename Resource, typename Identifier>
class ResourceProxyManager
{
private:
    std::map<Identifier, std::unique_ptr<ResourceProxy<Resource>>> mProxies;

public:
    void loadResource(Identifier id, const std::string& filename)
    {
        mProxies[id] = std::make_unique<ResourceProxy<Resource>>(filename);
    }

    Resource& getResource(Identifier id)
    {
        auto it = mProxies.find(id);
        if (it == mProxies.end())
        {
            throw std::runtime_error("Resource not found");
        }
        return it->second->getResource();
    }

    // 预加载指定资源
    void preloadResource(Identifier id)
    {
        auto it = mProxies.find(id);
        if (it != mProxies.end())
        {
            it->second->preload();
        }
    }

    // 卸载长时间未访问的资源
    void garbageCollect(float maxIdleTime = 60.0f)
    {
        for (auto& pair : mProxies)
        {
            if (pair.second->isLoaded() && 
                pair.second->getTimeSinceLastAccess() > maxIdleTime)
            {
                pair.second->unload();
            }
        }
    }

    // 获取资源访问统计
    void printResourceStats() const
    {
        for (const auto& pair : mProxies)
        {
            std::cout << "Resource ID: " << static_cast<int>(pair.first)
                      << ", Access Count: " << pair.second->getAccessCount()
                      << ", Loaded: " << (pair.second->isLoaded() ? "Yes" : "No")
                      << std::endl;
        }
    }
};

// 类型定义，方便使用
typedef ResourceProxy<sf::Texture> TextureProxy;
typedef ResourceProxyManager<sf::Texture, int> TextureProxyManager;