#include "Table.h"
#include <Windows.h>
#include <iostream>
void Table::draw(sf::RenderTarget & renderTarget, sf::RenderStates states) const
{

	renderTarget.draw(m_sprite, states); 

	for (const auto& ball : m_Balls)
		renderTarget.draw(ball, states);

	for (const auto& hole : m_Holes)
		renderTarget.draw(hole, states);


}

Table::Table(int width, int height, float scale)
	: m_Width(width), m_Height(height)
{
	
	m_Scale = glm::vec2(scale);
	m_position = glm::vec2(m_Width - m_Width * m_Scale.x, 0);

	m_texture.loadFromFile("graphics/Background.png");
	
	m_sprite.setTexture(m_texture); 
	
	m_sprite.setPosition(m_position.x, m_position.y);
	m_sprite.scale(m_Scale.x, m_Scale.y);


	_initHoles();
	_initBalls();
}

void Table::update(const sf::Window& window)
{
	for (auto& ball : m_Balls)
	{
		
		_checkBounds(ball);
		ball.setVelocity(_calculateVel(ball));
		if (GetAsyncKeyState(VK_SPACE))
		{
			sf::Mouse mouse;
			sf::Vector2i lol = mouse.getPosition(window);
			m_Balls.back().setVelocity(glm::normalize(glm::vec2(lol.x, lol.y) - m_Balls.back().getPosition()) * 25.0f);
		}
		_move(ball);
	}

	
}

Table::~Table()
{

}

void Table::_initHoles()
{
	int startX = m_Width - (m_Width * m_Scale.x);

	m_Holes.push_back(Hole(glm::vec2(startX + EDGE_WIDTH,						0 + EDGE_WIDTH)));
	m_Holes.push_back(Hole(glm::vec2(startX + EDGE_WIDTH,						360 * m_Scale.y)));
	m_Holes.push_back(Hole(glm::vec2(startX + EDGE_WIDTH,						720 * m_Scale.y - EDGE_WIDTH)));
	m_Holes.push_back(Hole(glm::vec2(startX + (650 * m_Scale.x),					720 * m_Scale.y - EDGE_WIDTH)));
	m_Holes.push_back(Hole(glm::vec2(startX + (1300 * m_Scale.x) - EDGE_WIDTH,	720 * m_Scale.y - EDGE_WIDTH)));
	m_Holes.push_back(Hole(glm::vec2(startX + (1300 * m_Scale.x) - EDGE_WIDTH,	360 * m_Scale.y)));
	m_Holes.push_back(Hole(glm::vec2(startX + (1300 * m_Scale.x) - EDGE_WIDTH,	0 + EDGE_WIDTH)));
	m_Holes.push_back(Hole(glm::vec2(startX + (650 * m_Scale.x),					0 + EDGE_WIDTH)));

}

void Table::_initBalls()
{
	glm::vec2 dir(1, -1);
	glm::vec2 dir2(1, 1);
	int startX = m_Width - (m_Width * m_Scale.x * 0.85f);
	int startY = m_Height - (m_Height * m_Scale.y * 0.5f);
	for (int i = 0; i < 5; i++)
	{
		
		glm::vec2 start(dir * float(i) * 40.0f);

		for (int j = 0; j < 5 - i; j++)
		{
			glm::vec2 start2(dir2 * float(j) * 40.0f);
			glm::vec2 direction = (start + start2);
			direction.x += startX;
			direction.y += startY;
			m_Balls.push_back(Ball(direction));
		}
	}
}

void Table::_checkBounds(Ball & ball)
{
	
	int startX = m_Width - m_Width * m_Scale.x;
	int endY = m_Height * m_Scale.y;

	if ((ball.getPosition().x + ball.getRadius()) > m_Width - 40 || (ball.getPosition().x - ball.getRadius()) < startX + EDGE_WIDTH)
	{
		ball.setVelocity(glm::vec2(ball.getVelocity().x * -1, ball.getVelocity().y));
	}


	if ((ball.getPosition().y + ball.getRadius()) > endY - 40 || ((ball.getPosition().y - ball.getRadius() < EDGE_WIDTH)))
	{
		ball.setVelocity(glm::vec2(ball.getVelocity().x, ball.getVelocity().y * -1));
	}
}

void Table::_move(Ball & ball)
{

	int collisionId = -1;
	glm::vec2 currentPos = ball.getPosition();

	glm::vec2 nextPos = ball.getPosition() + ball.getVelocity();

	float distance = 1;// glm::length(nextPos - currentPos);

	float precision = 10.0f;

	for (float i = distance / precision; i <= distance && collisionId == -1; i += distance / precision)
	{

		glm::vec2 pos = ball.getPosition() + ((ball.getVelocity() / precision) * i);

		for (int j = 0; j < m_Balls.size(); j++)
		{
			if (m_Balls[j].getPosition() == ball.getPosition()) continue;

			float distanceBet = glm::length(m_Balls[j].getPosition() - pos);

			if (distanceBet < ball.getRadius() + m_Balls[j].getRadius())
			{
				_collision(ball, m_Balls[j]);
				collisionId = j;
			}
		}
		if (collisionId == -1)
		{
			ball.setPosition(pos);
		}

	}

}

glm::vec2 Table::_calculateVel(Ball & ball)
{
	glm::vec2 initialVel = glm::vec2(ball.getVelocity().x, ball.getVelocity().y);
	glm::vec2 frictionForce = -(initialVel*-(0.2f * -(ball.getMass()*9.82f * 0.01f)));
	//frictionForce = glm::vec2(0.0f, 0.0f);
	glm::vec3 temp = glm::vec3((pow(M_PI, 2) * pow(ball.getRadius(), 3) * density) * ball.getAngleVelocity().x, (pow(M_PI, 2) * pow(ball.getRadius(), 3) * density) * ball.getAngleVelocity().y, (pow(M_PI, 2) * pow(ball.getRadius(), 3) * density) * ball.getAngleVelocity().z);
	glm::vec3 magnusEffectForce = glm::cross(temp, glm::vec3(initialVel,0));
	glm::vec2 finalForce = glm::vec2(frictionForce.x + magnusEffectForce.x, frictionForce.y + magnusEffectForce.y);
	glm::vec2 finalVel = ball.getVelocity() + (finalForce / ball.getMass());
	return finalVel;
}

void Table::_collision(Ball & ball1, Ball & ball2)
{
	float e = 1;

	glm::vec2 gWhiteDir = (glm::vec2(ball1.getVelocity().x, ball1.getVelocity().y));
	glm::vec2 gYellowDir = (glm::vec2(ball2.getVelocity().x, ball2.getVelocity().y));

	glm::vec2 gWhitePos = glm::vec2(ball1.getPosition().x, ball1.getPosition().y);
	glm::vec2 gYellowPos = glm::vec2(ball2.getPosition().x, ball2.getPosition().y);

	glm::vec2 d = glm::normalize(gWhitePos - gYellowPos);

	glm::vec2 WhiteV0 = glm::dot(gWhiteDir, d) * d;
	glm::vec2 YellowV0 = glm::dot(gYellowDir, d) * d;

	float WhiteMass = ball1.getMass();
	float YellowMass = ball2.getMass();

	glm::vec2 WhiteV1 = (WhiteV0*(WhiteMass - (YellowMass*e)) + (YellowMass * YellowV0 *(1 + e))) / (WhiteMass + YellowMass);
	glm::vec2 YellowV1 = (YellowV0*(YellowMass - (WhiteMass*e)) + (WhiteMass*WhiteV0*(1 + e))) / (WhiteMass + YellowMass);

	glm::vec2 WhiteFinal = gWhiteDir - WhiteV0 + WhiteV1;
	glm::vec2 YellowFinal = gYellowDir - YellowV0 + YellowV1;

	ball2.setAngleVelocity(-ball1.getAngleVelocity());

	ball1.setVelocity(WhiteFinal);
	ball2.setVelocity(YellowFinal);

}
