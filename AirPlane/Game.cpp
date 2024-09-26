
#include "Game.h"



Game::Game()
	: mWindow(sf::VideoMode(640, 480), "AirPlane")
	, mPlayer()
{
	mPlayer.setSize(sf::Vector2f(50.f,50.f));
	mPlayer.setFillColor(sf::Color::Yellow);
	mPlayer.setPosition(100.f,100.f);
}

Game::~Game()
{
}

void Game::Run()
{
	while (mWindow.isOpen())
	{
		//处理事件
		ProcessEvent();

		//更新
		Update();

		//渲染
		Render();	
	}
}

void Game::ProcessEvent()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Game::Update()
{
	static double dX = 0;
	static double dY = 0;
	mPlayer.setPosition(dX, dY);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		--dX;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		++dX;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		--dY;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		++dY;
}

void Game::Render()
{	
	mWindow.clear();
	mWindow.draw(mPlayer);
	mWindow.display();
}
