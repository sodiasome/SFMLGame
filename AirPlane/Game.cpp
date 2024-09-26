
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
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mWindow.close();
		}

		//渲染
		Render();	
	}
}

void Game::Update()
{
}

void Game::Render()
{
	mWindow.clear();
	mWindow.draw(mPlayer);
	mWindow.display();
}
