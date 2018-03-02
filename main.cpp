#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include "Ball.h"
#include "Background.h"
#include "Hole.h"

#include <glm.hpp>
#define WIDTH 1300.0f
#define HEIGHT 720.0f
#define Radius 25
#define numberOfBalls 1
#define density 1637
#define _USE_MATH_DEFINES
#include <math.h>

void collisionMove(Ball& ball1, Ball& ball2)
{
	float e = 1;
	glm::vec2 gWhiteDir = (glm::vec2(ball1.getVelocity().x, ball1.getVelocity().y));
	glm::vec2 gYellowDir = (glm::vec2(ball2.getVelocity().x, ball2.getVelocity().y));

	glm::vec2 gWhitePos = glm::vec2(ball1.getPosition().x, ball1.getPosition().y);
	glm::vec2 gYellowPos = glm::vec2(ball2.getPosition().x, ball2.getPosition().y);

	glm::vec2 d = glm::normalize(gWhitePos - gYellowPos);
	if (glm::length(d))
	{
		glm::vec2 WhiteV0 = glm::dot(gWhiteDir, d) * d;
		glm::vec2 YellowV0 = glm::dot(gYellowDir, d) * d;

		float WhiteMass = ball1.getMass();
		float YellowMass = ball2.getMass();

		glm::vec2 WhiteV1 = (WhiteV0*(WhiteMass - (YellowMass*e)) + (YellowMass * YellowV0 *(1 + e))) / (WhiteMass + YellowMass);
		glm::vec2 YellowV1 = (YellowV0*(YellowMass - (WhiteMass*e)) + (WhiteMass*WhiteV0*(1 + e))) / (WhiteMass + YellowMass);

		glm::vec2 WhiteFinal = gWhiteDir - WhiteV0 + WhiteV1;
		glm::vec2 YellowFinal = gYellowDir - YellowV0 + YellowV1;
		WhiteFinal = (WhiteFinal);
		YellowFinal = (YellowFinal);

		ball1.setVelocity(WhiteFinal);
		ball2.setVelocity(YellowFinal);
	}

	


}

void checkBounds(Ball& ball)
{

	if ((ball.getPosition().x + Radius) > WIDTH - 40 || (ball.getPosition().x - Radius) < 40)
	{
		ball.setVelocity(glm::vec2(ball.getVelocity().x * -1, ball.getVelocity().y));
	}


	if ((ball.getPosition().y + Radius) > HEIGHT - 40|| ((ball.getPosition().y - Radius < 40)))
	{
		ball.setVelocity(glm::vec2(ball.getVelocity().x, ball.getVelocity().y * -1));
	}
}

bool checkCollison(Ball ball1, Ball ball2)
{

	float distance = glm::length(ball1.getPosition() - ball2.getPosition());

	return distance <= Radius*2;
}

int move(Ball& ball, std::vector<Ball>& balls)
{

	int collisionId = -1;
	glm::vec2 currentPos = ball.getPosition();

	glm::vec2 nextPos = ball.getPosition() + ball.getVelocity();

	float distance = glm::length(nextPos - currentPos);

	float precision = 10.0f;

	for (float i = distance / precision; i <= distance && collisionId == -1; i += distance / precision)
	{

		glm::vec2 pos = ball.getPosition() + ((ball.getVelocity() / precision) * i) ;
		
		for (int j = 0; j < balls.size(); j++)
		{
			if (balls[j].getPosition() == ball.getPosition()) continue;

			float distanceBet = glm::length(balls[j].getPosition() - pos);
			
			if (distanceBet < Radius * 2)
			{
				collisionMove(ball, balls[j]);
				collisionId = j;
			}
		}
		if (collisionId == -1)
		{
			

			ball.setPosition(pos);

		}

	}

	return collisionId;
}


glm::vec2 calcNewVelocity(Ball& ball)
{
	glm::vec3 initialVel = glm::vec3(ball.getVelocity().x, ball.getVelocity().y,0);
	glm::vec2 frictionForce = -(initialVel*-(0.2f * -(ball.getMass()*9.82f) * 0.0001f));
	//frictionForce = glm::vec2(0.0f, 0.0f);
	glm::vec3 temp = glm::vec3((pow(M_PI, 2) * pow(Radius, 3) * density) * ball.getAngleVelocity().x, (pow(M_PI, 2) * pow(Radius, 3) * density) * ball.getAngleVelocity().y, (pow(M_PI, 2) * pow(Radius, 3) * density) * ball.getAngleVelocity().z);
	glm::vec3 magnusEffectForce = glm::cross(temp, initialVel); 
	glm::vec2 finalForce = glm::vec2(frictionForce.x + magnusEffectForce.x, frictionForce.y + magnusEffectForce.y);
	glm::vec2 finalVel = ball.getVelocity() + (finalForce / ball.getMass());
	return finalVel; 
}

int main()
{
	Hole holeArray[8]; 
	std::vector <Ball> balls;
	holeArray[0].setPosition(glm::vec2(0 + 40, 0 + 40)); 
	holeArray[1].setPosition(glm::vec2(0 + 40, 360));
	holeArray[2].setPosition(glm::vec2(0 + 40 , 720 - 40));
	holeArray[3].setPosition(glm::vec2(650, 720 - 40));
	holeArray[4].setPosition(glm::vec2(1300 -  40, 720 - 40));
	holeArray[5].setPosition(glm::vec2(1300 - 40, 360));
	holeArray[6].setPosition(glm::vec2(1300 - 40, 0 + 40));
	holeArray[7].setPosition(glm::vec2(650, 0 + 40));

	Background background = Background();
	

	sf::Color colors[] =
	{
		sf::Color::Red,
		sf::Color::Yellow,
		sf::Color::Blue,
		sf::Color::Green,
		sf::Color::Magenta,
		sf::Color::Cyan
	};

	for (int i = 0; i < numberOfBalls; i++)
	{
		balls.push_back(Ball(glm::vec2( ((i+1)* 3 * Radius) + Radius , HEIGHT / 2.0 + 40.0f)));
		balls.back().setVelocity(glm::vec2(0.5,0.1f));
		balls.back().setMass(50);
		balls.back().setColor(colors[i % 6]);
	}
	
	bool Collision = false;

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
		

		for (int i = 0; i < balls.size(); i++)
		{
			
			checkBounds(balls[i]);
			
			glm::vec2 finalVelocity = calcNewVelocity(balls[i]);
			balls[i].setVelocity(finalVelocity);


			move(balls[i], balls);
			
		}
	
	
        window.clear();
		window.draw(background);

		for (int i = 0; i < 8; i++)
		{
			window.draw(holeArray[i]); 
		}

		for (auto& ball : balls)
		{
			window.draw(ball.getShape());

			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(ball.getPosition().x, ball.getPosition().y)),
				sf::Vertex(sf::Vector2f(ball.getPosition().x + (ball.getVelocity().x * Radius), ball.getPosition().y + (ball.getVelocity().y * Radius)))
			};

			window.draw(line, 2, sf::Lines);

		}
		/*int removeIndex = -1;
		for (int i = 0; i < balls.size(); i++)
		{
			for (auto& hole : holeArray)
				if (hole.collision(balls[i]))
					//removeIndex = i;

		}
		if(removeIndex != -1)balls.erase(balls.begin() + removeIndex);
		*/
        window.display();
    }

    return 0;
}