#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <Windows.h>

#include <glm.hpp>
#define WIDTH 1280.0f
#define HEIGHT 720.0f
#define Radius 50

class Ball
{
private:
	sf::CircleShape shape;
	float mass;
	glm::vec2 velocity;
	glm::vec2 position;

public:

	Ball(glm::vec2 pos)
	{
		this->shape = sf::CircleShape(Radius);
		shape.setOrigin(Radius, Radius);
		shape.setPosition(sf::Vector2f(pos.x, pos.y));
		
		position = pos;
		velocity = glm::vec2(0, 0);
		mass = 50;
	}

	void setColor(sf::Color color)
	{
		shape.setFillColor(color);
	}

	void setPosition(glm::vec2 position)
	{
		this->position = position;
		shape.setPosition(position.x, position.y);
	}

	glm::vec2 getVelocity()
	{
		return velocity;
	}

	void setVelocity(glm::vec2 velocity)
	{
		this->velocity = velocity;
	}

	glm::vec2 getPos()
	{
		return position;
	}

	void setMass(float mass)
	{
		this->mass = mass;
	}
	float getMass()
	{
		return mass;
	}

	sf::CircleShape getShape()
	{
		return shape;
	}
};


void checkBounds(Ball& ball)
{
	if (GetAsyncKeyState(int('C')))
		__debugbreak();
	if ((ball.getPos().x + Radius) > WIDTH || (ball.getPos().x - Radius) < 0)
	{
		ball.setVelocity(glm::vec2(ball.getVelocity().x * -1, ball.getVelocity().y));
	}


	if ((ball.getPos().y + Radius) > HEIGHT || ((ball.getPos().y - Radius < 0)))
	{
		ball.setVelocity(glm::vec2(ball.getVelocity().x, ball.getVelocity().y * -1));
	}
}

bool checkCollison(Ball ball1, Ball ball2)
{

	float distance = glm::length(ball1.getPos() - ball2.getPos());

	return distance <= Radius*2;
}

bool move(Ball& ball, Ball& ball2)
{
	bool Collision = false;
	glm::vec2 currentPos = ball.getPos();

	glm::vec2 nextPos = ball.getPos() + ball.getVelocity();

	float distance = glm::length(nextPos - currentPos);

	float precision = 10.0f;

	for (float i = distance / precision; i < distance && !Collision; i += distance / precision)
	{
		glm::vec2 pos = ball.getPos() + ((ball.getVelocity() / precision) * i);

		float distanceBet = glm::length(ball2.getPos() - pos);
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
			white.setVelocity(glm::vec2(1, 0));
			yellow.setVelocity(glm::vec2(0, 1));
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

			glm::vec2 gWhitePos = glm::vec2(white.getPos().x, white.getPos().y);
			glm::vec2 gYellowPos = glm::vec2(yellow.getPos().x, yellow.getPos().y);

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
		window.draw(white.getShape());
        window.draw(yellow.getShape());
        window.display();
    }

    return 0;
}