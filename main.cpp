#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include <glm.hpp>
#define WIDTH 1280
#define HEIGHT 720

void checkBounds(sf::CircleShape &shape, sf::Vector2f& pos, sf::Vector2f& direction)
{
	if ((pos.x + shape.getRadius()) > WIDTH || (pos.x - shape.getRadius()) < 0)
	{
		direction.x *= -1;
	}


	if ((pos.y + shape.getRadius()) > HEIGHT || (pos.y - shape.getRadius() < 0))
	{
		direction.y *= -1;
	}
}

bool checkCollison(sf::CircleShape& shape1, sf::CircleShape& shape2)
{
	float s1X = shape1.getPosition().x;
	float s1Y = shape1.getPosition().y;

	float s2X = shape2.getPosition().x;
	float s2Y = shape2.getPosition().y;

	float distanceBetween = sqrt(((s2X - s1X) * (s2X - s1X)) + ((s2Y - s1Y) * (s2Y - s1Y)));
	float combinedRadius = shape1.getRadius() + shape2.getRadius();
	
	return distanceBetween <= combinedRadius;
}
const float G = 0.00982f;
int main()
{
	static float WhiteMass = 10;
	static float YellowMass = 20;
	bool Collision = false;
	float my = -0.2f;
	float WhiteAcceleration = (my * G);
	float YellowAcceleration = (my * G);
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

    sf::CircleShape whiteBall(50.f);

	whiteBall.setOrigin(sf::Vector2f(whiteBall.getRadius(), whiteBall.getRadius()));

	sf::Vector2f whitePos(WIDTH/2,HEIGHT/2);
	sf::Vector2f whiteDir(5.0f, 0.0f);

	whiteBall.setPosition(whitePos);
    whiteBall.setFillColor(sf::Color::White);

	sf::CircleShape yellowBall(50.0f);

	yellowBall.setOrigin(sf::Vector2f(yellowBall.getRadius(), yellowBall.getRadius()));

	sf::Vector2f yellowPos(80.0f,( HEIGHT/2) + 50.0f);
	sf::Vector2f yellowDir(0.0f, 0.0f);

	yellowBall.setPosition(yellowPos);
	yellowBall.setFillColor(sf::Color::Yellow);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		
		checkBounds(whiteBall, whitePos, whiteDir);
		checkBounds(yellowBall, yellowPos, yellowDir);
		/*if (whiteDir.x > -WhiteAcceleration)
		{
			whiteDir.x += WhiteAcceleration;
		}
		else if(whiteDir.x < WhiteAcceleration)
		{
			whiteDir.x -= WhiteAcceleration;
		}
		else
		{
			whiteDir.x = 0;
		}
		if (whiteDir.y > -WhiteAcceleration)
		{
			whiteDir.y += WhiteAcceleration;
		}
		else if (whiteDir.y < WhiteAcceleration)
		{
			whiteDir.y -= WhiteAcceleration;
		}
		else
		{
			whiteDir.y = 0;
		}*/
		//yellowDir.x += YellowAcceleration;
		//yellowDir.y += YellowAcceleration;

		Collision = false;
		
		for(int i = 0; i < whiteDir.x && Collision == false || i < whiteDir.y && Collision == false;i++)
		{
			if (i < whiteDir.x)
			{
				whitePos.x += 1;
			}
			if (i < whiteDir.y)
			{
				whitePos.y += 1;
			}
			whiteBall.setPosition(whitePos);
			Collision = checkCollison(whiteBall, yellowBall);
		}
		for (int i = 0; i < yellowDir.x && Collision == false || i < yellowDir.y && Collision == false; i++)
		{
			if (i < yellowDir.x)
			{
				yellowPos.x += 1;
			}
			if (i < yellowDir.y)
			{
				yellowPos.y += 1;
			}
			yellowBall.setPosition(yellowPos);
			Collision = checkCollison(whiteBall, yellowBall);
		}
		for (int i = 0; i > whiteDir.x && Collision == false || i > whiteDir.y && Collision == false; i--)
		{
			if (i > whiteDir.x)
			{
				whitePos.x -= 1;
			}
			if (i > whiteDir.y)
			{
				whitePos.y -= 1;
			}
			whiteBall.setPosition(whitePos);
			Collision = checkCollison(whiteBall, yellowBall);
		}
		for (int i = 0; i > yellowDir.x && Collision == false || i > yellowDir.y && Collision == false; i--)
		{
			if (i > yellowDir.x)
			{
				yellowPos.x -= 1;
			}
			if (i > yellowDir.y)
			{
				yellowPos.y -= 1;
			}
			Collision = checkCollison(whiteBall, yellowBall);
			yellowBall.setPosition(yellowPos);
		}

		if (Collision)
		{
			static float e = 0.2;


			glm::vec2 gWhiteDir = (glm::vec2(whiteDir.x, whiteDir.y));
			glm::vec2 gYellowDir = (glm::vec2(yellowDir.x, yellowDir.y));

			glm::vec2 gWhitePos = glm::vec2(whitePos.x, whitePos.y);
			glm::vec2 gYellowPos = glm::vec2(yellowPos.x, yellowPos.y);

			glm::vec2 d = gWhitePos - gYellowPos;

			glm::vec2 WhiteV0 = glm::dot(gWhiteDir, d) * glm::normalize(d);
			glm::vec2 YellowV0 = glm::dot(gYellowDir, d) * glm::normalize(d);

			glm::vec2 WhiteV1 = (WhiteV0*(WhiteMass - (YellowMass*e)) + (YellowMass * YellowV0 *(1 + e))) / (WhiteMass + YellowMass);
			glm::vec2 YellowV1 = (YellowV0*(YellowMass - (WhiteMass*e)) + (WhiteMass*WhiteV0*(1 + e))) / (WhiteMass + YellowMass);

			glm::vec2 WhiteFinal = gWhiteDir - WhiteV0 + WhiteV1;
			glm::vec2 YellowFinal = gYellowDir - YellowV0 + YellowV1;
			WhiteFinal = glm::normalize(WhiteFinal);
			YellowFinal = glm::normalize(YellowFinal);

			std::cout << "WhiteBall Direction: before: (" << gWhiteDir.x << "," << gWhiteDir.y << ")" << " After: (" << WhiteFinal.x << "," << WhiteFinal.y << ")\n";
			std::cout << "WhiteBall Position: (" << whitePos.x << "," << whitePos.y << ")" << std::endl;

			std::cout << "\nYellowBall Direction: before: (" << gYellowDir.x << "," << gYellowDir.y << ")" << " After: (" << YellowFinal.x << "," << YellowFinal.y << ")\n";
			std::cout << "YellowBall Position: (" << yellowPos.x << "," << yellowPos.y << ")" << std::endl;

			whiteDir = sf::Vector2f(WhiteFinal.x, WhiteFinal.y);
			yellowDir = sf::Vector2f(YellowFinal.x, YellowFinal.y);



		}
		

		


        window.clear();
		window.draw(yellowBall);
        window.draw(whiteBall);
        window.display();
    }

    return 0;
}