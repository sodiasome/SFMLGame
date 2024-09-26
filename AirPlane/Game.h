#pragma once

// 游戏主逻辑
// sodiasome@163.com

#include <SFML/Graphics.hpp>

using namespace sf;

class Game
{
public:
	Game();
	~Game();
	void Run();					//游戏主循环
private:
	void Update();				//游戏操作更新
	void Render();				//游戏渲染
private:
	sf::RenderWindow mWindow;	//游戏窗口
	sf::RectangleShape mPlayer;		//角色
};

