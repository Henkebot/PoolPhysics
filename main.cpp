
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include <chrono>
#include "Ball.h"
#include "Table.h"


#include <glm.hpp>
#define WIDTH 1300.0f
#define HEIGHT 720.0f
#define UPDATE_RATE 60.0f

int main()
{
	
	Table background(WIDTH,HEIGHT, 1);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

	using namespace std::chrono;
	auto lastTime = steady_clock::now();
	int updates = 0;
	int fpsCounter = 0;
	float freq = 1000000000.0f / UPDATE_RATE;
	float unprocessed = 0;
	auto timer = steady_clock::now();
    while (window.isOpen())
    {
		
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		auto current_time = steady_clock::now();
		auto dt = duration_cast<nanoseconds>(current_time - lastTime).count();
		unprocessed += dt / freq;
		lastTime = current_time;

		while (unprocessed > 1)
		{
			updates++;
			unprocessed -= 1;
			background.update(window);
		}
        window.clear();
		
		fpsCounter++;
		window.draw(background);
		
		if (duration_cast<milliseconds>(steady_clock::now() - timer).count() > 1000)
		{
			printf("\rFPS: %d TICK: %d", fpsCounter, updates);
			updates = 0;
			fpsCounter = 0;
			timer += milliseconds(1000);
		}

        window.display();
    }

    return 0;
}