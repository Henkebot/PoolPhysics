#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <Windows.h>

#include <glm.hpp>
#define WIDTH 1280.0f
#define HEIGHT 720.0f
#define Radius 50
#define numberOfBalls 3

class Ball
{
private:
	sf::CircleShape shape;
	float mass;
	glm::vec2 velocity;
	glm::vec2 position;
	glm::vec2 force;
	glm::vec2 accelertion;

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
		accelertion = (velocity - this->velocity) / 0.001f;

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

	glm::vec2 getForce()
	{
		return mass * accelertion;
	}
};

void collisionMove(Ball& ball1, Ball& ball2)
{
	float e = 1;
	glm::vec2 gWhiteDir = (glm::vec2(ball1.getVelocity().x, ball1.getVelocity().y));
	glm::vec2 gYellowDir = (glm::vec2(ball2.getVelocity().x, ball2.getVelocity().y));

	glm::vec2 gWhitePos = glm::vec2(ball1.getPos().x, ball1.getPos().y);
	glm::vec2 gYellowPos = glm::vec2(ball2.getPos().x, ball2.getPos().y);

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

int move(Ball& ball, std::vector<Ball>& balls)
{
	int collisionId = -1;
	glm::vec2 currentPos = ball.getPos();

	glm::vec2 nextPos = ball.getPos() + ball.getVelocity();

	float distance = glm::length(nextPos - currentPos);

	float precision = 10.0f;

	for (float i = distance / precision; i <= distance && collisionId == -1; i += distance / precision)
	{
		glm::vec2 pos = ball.getPos() + ((ball.getVelocity() / precision) * i) ;
		
		for (int j = 0; j < numberOfBalls; j++)
		{
			if (balls[j].getPos() == ball.getPos()) continue;

			float distanceBet = glm::length(balls[j].getPos() - pos);
			
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




int main()
{
	std::vector <Ball> balls;
	

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
		balls.push_back(Ball(glm::vec2( (i* 3 * Radius) + Radius , HEIGHT / 2.0)));
		balls.back().setVelocity(glm::vec2(1,-1));
		balls.back().setMass(50);
		balls.back().setColor(colors[i % 6]);
	}
	/*int index = numberOfBalls - 1;
	balls.push_back(Ball(glm::vec2(75, 75)));
	balls.back().setColor(sf::Color::White);
	balls.back().setMass(50);
	balls.back().setVelocity(glm::vec2(0, 1));
*/
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
		

		for (int i = 0; i < numberOfBalls; i++)
		{
			checkBounds(balls[i]);
			glm::vec2 initialVel = balls[i].getVelocity();
			glm::vec2 finalVelocity = initialVel - ((initialVel*-(0.2f * -(balls[i].getMass()*9.82f) * 0.0001f)) / balls[i].getMass());
			balls[i].setVelocity(finalVelocity);
			move(balls[i], balls);
		}
	

        window.clear();
		for (auto& ball : balls)
		{
			window.draw(ball.getShape());
			
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(ball.getPos().x, ball.getPos().y)),
				sf::Vertex(sf::Vector2f(ball.getPos().x + (ball.getVelocity().x * Radius), ball.getPos().y + (ball.getVelocity().y * Radius)))
			};

			window.draw(line, 2, sf::Lines);

		}
		window.display();
    }

    return 0;
}