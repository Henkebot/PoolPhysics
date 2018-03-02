#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include "Ball.h"
#include "Table.h"


#include <glm.hpp>
#define WIDTH 1300.0f
#define HEIGHT 720.0f
#define Radius 25
#define numberOfBalls 3

void collisionMove(Ball& ball1, Ball& ball2)
{
	
	


}

void checkBounds(Ball& ball)
{

	
}

bool checkCollison(Ball ball1, Ball ball2)
{

	float distance = glm::length(ball1.getPosition() - ball2.getPosition());

	return distance <= Radius*2;
}

int move(Ball& ball, std::vector<Ball>& balls)
{

	return 0;
}

int main()
{

	std::vector <Ball> balls;
	
	Table background(WIDTH,HEIGHT, 0.85);


	for (int i = 0; i < numberOfBalls; i++)
	{
		balls.push_back(Ball(glm::vec2( ((i+1)* 3 * Radius) + Radius , HEIGHT / 2.0)));
	}
	

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");


    while (window.isOpen())
    {
		if (GetAsyncKeyState(VK_SPACE))
		{

			for (auto& ball : balls)
			{
				ball.setVelocity(glm::normalize(ball.getVelocity()) * 1.5f);
			}

		}
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		
		background.update();

		/*for (int i = 0; i < balls.size(); i++)
		{
			
			checkBounds(balls[i]);
			glm::vec2 initialVel = balls[i].getVelocity();
			glm::vec2 finalVelocity = initialVel - ((initialVel*-(0.2f * -(balls[i].getMass()*9.82f) * 0.0001f)) / balls[i].getMass());
			balls[i].setVelocity(finalVelocity);

			move(balls[i], balls);
			
		}
	*/
	
        window.clear();
		window.draw(background);

        window.display();
    }

    return 0;
}