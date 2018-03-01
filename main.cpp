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
#define Radius 50

void checkBounds(Ball& ball)
{
	if (GetAsyncKeyState(int('C')))
		__debugbreak();
	if ((ball.getPosition().x + Radius) > WIDTH || (ball.getPosition().x - Radius) < 0)
	{
		ball.setVelocity(glm::vec2(ball.getVelocity().x * -1, ball.getVelocity().y));
	}


	if ((ball.getPosition().y + Radius) > HEIGHT || ((ball.getPosition().y - Radius < 0)))
	{
		ball.setVelocity(glm::vec2(ball.getVelocity().x, ball.getVelocity().y * -1));
	}
}

bool checkCollison(Ball ball1, Ball ball2)
{

	float distance = glm::length(ball1.getPosition() - ball2.getPosition());

	return distance <= Radius*2;
}

bool move(Ball& ball, Ball& ball2)
{
	bool Collision = false;
	glm::vec2 currentPos = ball.getPosition();

	glm::vec2 nextPos = ball.getPosition() + ball.getVelocity();

	float distance = glm::length(nextPos - currentPos);

	float precision = 10.0f;

	for (float i = distance / precision; i < distance && !Collision; i += distance / precision)
	{
		glm::vec2 pos = ball.getPosition() + ((ball.getVelocity() / precision) * i);

		float distanceBet = glm::length(ball2.getPosition() - pos);
		Collision = distanceBet < Radius * 2;
		if (!Collision)
		{
			ball.setPosition(pos);
		}

	}

	return Collision;
}

const float G = 0.00982f;
int main()
{
	Hole holeArray[8]; 

	holeArray[0].setPosition(glm::vec2(0 + 40, 0 + 40)); 
	holeArray[1].setPosition(glm::vec2(0 + 40, 360));
	holeArray[2].setPosition(glm::vec2(0 + 40 , 720 - 40));
	holeArray[3].setPosition(glm::vec2(650, 720 - 40));
	holeArray[4].setPosition(glm::vec2(1300 -  40, 720 - 40));
	holeArray[5].setPosition(glm::vec2(1300 - 40, 360));
	holeArray[6].setPosition(glm::vec2(1300 - 40, 0 + 40));
	holeArray[7].setPosition(glm::vec2(650, 0 + 40));

	Background background = Background();

	Ball white(glm::vec2(WIDTH / 2.0f, HEIGHT / 2.0f));
    white.setColor(sf::Color::White);
	white.setVelocity(glm::vec2(1, 0));
	white.setMass(25);
	

	Ball yellow(glm::vec2(80.0f, (HEIGHT / 2) + 50.0f));
	yellow.setColor(sf::Color::Yellow);
	yellow.setVelocity(glm::vec2(0,1));
	yellow.setMass(120.0f);

	bool Collision = false;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");


    while (window.isOpen())
    {
		if (GetAsyncKeyState(VK_SPACE))
		{
			white.setVelocity(glm::normalize(white.getVelocity()));
			yellow.setVelocity(glm::normalize(yellow.getVelocity()));
		}
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		
		checkBounds(white);
		checkBounds(yellow);

		Collision = false;
		
		Collision = move(white, yellow);
		Collision = move(yellow, white);


		//for(int i = 0; i < white.getVelocity().x && Collision == false || i < white.getVelocity().y && Collision == false;i++)
		//{
		//	if (i < white.getVelocity().x)
		//	{
		//		
		//		whitePos.x += 1;
		//	}
		//	if (i < whiteDir.y)
		//	{
		//		whitePos.y += 1;
		//	}
		//	sf::Vector2f oldPos = whiteBall.getPosition();
		//	
		//	Collision = checkCollison(whiteBall, yellowBall);
		//	if(Collision)
		//		whiteBall.setPosition(oldPos);
		//}
		//for (int i = 0; i < yellowDir.x && Collision == false || i < yellowDir.y && Collision == false; i++)
		//{
		//	if (i < yellowDir.x)
		//	{
		//		yellowPos.x += 1;
		//	}
		//	if (i < yellowDir.y)
		//	{
		//		yellowPos.y += 1;
		//	}
		//	sf::Vector2f oldPos = yellowBall.getPosition();
		//	yellowBall.setPosition(yellowPos);
		//	Collision = checkCollison(whiteBall, yellowBall);
		//	if (Collision)
		//		yellowBall.setPosition(oldPos);
		//}
		//for (int i = 0; i > whiteDir.x && Collision == false || i > whiteDir.y && Collision == false; i--)
		//{
		//	if (i > whiteDir.x)
		//	{
		//		whitePos.x -= 1;
		//	}
		//	if (i > whiteDir.y)
		//	{
		//		whitePos.y -= 1;
		//	}
		//	sf::Vector2f oldPos = whiteBall.getPosition();
		//	whiteBall.setPosition(whitePos);
		//	Collision = checkCollison(whiteBall, yellowBall);
		//	if (Collision)
		//		whiteBall.setPosition(oldPos);
		//}
		//for (int i = 0; i > yellowDir.x && Collision == false || i > yellowDir.y && Collision == false; i--)
		//{
		//	if (i > yellowDir.x)
		//	{
		//		yellowPos.x -= 1;
		//	}
		//	if (i > yellowDir.y)
		//	{
		//		yellowPos.y -= 1;
		//	}
		//	sf::Vector2f oldPos = whiteBall.getPosition();
		//	yellowBall.setPosition(yellowPos);
		//	Collision = checkCollison(whiteBall, yellowBall);
		//	if (Collision)
		//		yellowBall.setPosition(oldPos);
		//}

		if (Collision)
		{
			static float e = 0.5;


			glm::vec2 gWhiteDir = (glm::vec2(white.getVelocity().x, white.getVelocity().y));
			glm::vec2 gYellowDir = (glm::vec2(yellow.getVelocity().x, yellow.getVelocity().y));

			glm::vec2 gWhitePos = glm::vec2(white.getPosition().x, white.getPosition().y);
			glm::vec2 gYellowPos = glm::vec2(yellow.getPosition().x, yellow.getPosition().y);

			glm::vec2 d = glm::normalize(gWhitePos - gYellowPos);

			glm::vec2 WhiteV0 = glm::dot(gWhiteDir, d) * d;
			glm::vec2 YellowV0 = glm::dot(gYellowDir, d) * d;

			float WhiteMass = white.getMass();
			float YellowMass = yellow.getMass();

			glm::vec2 WhiteV1 = (WhiteV0*(WhiteMass - (YellowMass*e)) + (YellowMass * YellowV0 *(1 + e))) / (WhiteMass + YellowMass);
			glm::vec2 YellowV1 = (YellowV0*(YellowMass - (WhiteMass*e)) + (WhiteMass*WhiteV0*(1 + e))) / (WhiteMass + YellowMass);

			glm::vec2 WhiteFinal = gWhiteDir - WhiteV0 + WhiteV1;
			glm::vec2 YellowFinal = gYellowDir - YellowV0 + YellowV1;
			WhiteFinal = (WhiteFinal);
			YellowFinal = (YellowFinal);

			/*std::cout << "WhiteBall Direction: before: (" << gWhiteDir.x << "," << gWhiteDir.y << ")" << " After: (" << WhiteFinal.x << "," << WhiteFinal.y << ")\n";
			std::cout << "WhiteBall Position: (" << whitePos.x << "," << whitePos.y << ")" << std::endl;

			std::cout << "\nYellowBall Direction: before: (" << gYellowDir.x << "," << gYellowDir.y << ")" << " After: (" << YellowFinal.x << "," << YellowFinal.y << ")\n";
			std::cout << "YellowBall Position: (" << yellowPos.x << "," << yellowPos.y << ")" << std::endl;*/

			white.setVelocity(WhiteFinal);
			yellow.setVelocity(YellowFinal);
			




		}
	
        window.clear();
		window.draw(background);

		for (int i = 0; i < 8; i++)
		{
			window.draw(holeArray[i]); 
		}

		window.draw(white.getShape());
        window.draw(yellow.getShape());
        window.display();
    }

    return 0;
}