
#include "Game.h"



Game::Game()
	: mWindow(sf::VideoMode(640, 480), "AirPlane")
	, mPlayer()
	, mTimeFame(sf::seconds(1.f / 60.f))
	, mSpeedFame(50.f)
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
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		//处理事件
		ProcessEvent();

		timeSinceLastUpdate += clock.restart();
		if (timeSinceLastUpdate > mTimeFame)
		{
			timeSinceLastUpdate -= mTimeFame;
			//处理事件
			ProcessEvent();

			//更新
			Update();
		}
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
	sf::Vector2f movePoint(0.f,0.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		movePoint.x -= mSpeedFame;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		movePoint.x += mSpeedFame;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		movePoint.y -= mSpeedFame;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		movePoint.y += mSpeedFame;

	mPlayer.move(movePoint * mTimeFame.asSeconds());
}

void Game::Render()
{	
	mWindow.clear();
	mWindow.draw(mPlayer);
	mWindow.display();
}
