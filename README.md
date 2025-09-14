# SFMLGame

一个基于SFML的飞机游戏项目，集成了Qt风格的代理模式演示。

## 项目特色

### 原游戏功能
- 基于SFML的2D飞机游戏
- 场景图管理系统
- 资源管理系统
- 实体系统

### Qt代理模式实现
本项目新增了Qt风格的代理类实现，用于演示代理模式在游戏开发中的应用：

1. **ResourceProxy** - 资源代理
   - 延迟加载游戏资源
   - 资源访问统计
   - 自动垃圾回收
   - 访问权限控制

2. **InputProxy** - 输入代理
   - 输入事件抽象
   - 键位映射配置
   - 输入过滤和权限控制
   - 输入统计分析

3. **RenderProxy** - 渲染代理
   - 批处理渲染优化
   - 渲染队列管理
   - 多层渲染支持
   - 渲染统计信息

## 编译和运行

### 代理模式演示程序
```bash
cd AirPlane
make run
```

这将编译并运行代理模式演示程序，展示：
- 虚代理（延迟加载）
- 保护代理（访问控制）
- 缓存代理（性能优化）

### 原游戏（需要SFML）
游戏需要SFML库支持，请先安装SFML开发环境。

## 文件结构

- `AirPlane/` - 主要游戏代码
  - `ResourceProxy.h` - 资源代理类实现
  - `InputProxy.h` - 输入代理类实现
  - `RenderProxy.h` - 渲染代理类实现
  - `ProxyTest.cpp` - 代理模式演示程序
  - `GameWithProxies.h` - 集成代理的游戏类
  - `Makefile` - 编译脚本
- `Qt_Proxy_Classes_README.md` - 代理模式详细文档

## 学习资源

查看 `Qt_Proxy_Classes_README.md` 了解：
- Qt代理类的概念和应用
- 代理模式的实现细节
- 在游戏开发中的实际应用