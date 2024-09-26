#include "Game.h"
#include <SFML/Graphics.hpp>

using namespace sf;

Game::Game()
{
}

Game::~Game()
{
}

void Game::Run()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "AirPlane");
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.display();

	}
}

void Game::Update()
{
}

void Game::Render()
{
}
