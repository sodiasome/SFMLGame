

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    sf::RenderWindow window(sf::VideoMode(480,640),"hello sfml!");
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


