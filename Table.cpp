#include "Table.h"
#include <Windows.h>
#include <iostream>
void Table::draw(sf::RenderTarget & renderTarget, sf::RenderStates states) const
{

	renderTarget.draw(m_sprite, states); 

	for(const auto& ball : m_Balls)
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

void Table::update(const sf::Window& window, float dt)
{
	

	for (auto& ball : m_Balls)
	{
		
		_checkBounds(ball);
		_calculateVel(ball, dt);

		if (GetAsyncKeyState(VK_SPACE))
		{
			sf::Mouse mouse;
			sf::Vector2i lol = mouse.getPosition(window);
			m_Balls.back().setVelocity(glm::normalize(glm::vec2(lol.x, lol.y) - m_Balls.back().getPosition()) * 25.0f);
			m_Balls.back().setAngleVelocity(glm::vec3(20, 020, 2));
		}
		_move(ball);
	}

	
}

Ball & Table::getCurrentBall()
{
	return m_Balls[currentBallIndex];
}

void Table::setCurrentBall(int windowX, int windowY)
{
	for (int i = 0; i <  m_Balls.size(); i++)
	{
		glm::vec2 mousePos(windowX, windowY);
		float mouseToBallLength = glm::length(m_Balls[i].getPosition() - mousePos);
		if (mouseToBallLength <= m_Balls[i].getRadius())
		{
			m_Balls[currentBallIndex].getShape().setOutlineThickness(0.0f);
			currentBallIndex = i;
		}

	}

	m_Balls[currentBallIndex].getShape().setOutlineColor(sf::Color::Black);
	m_Balls[currentBallIndex].getShape().setOutlineThickness(5.0f);
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
	currentBallIndex = m_Balls.size() - 1;
	m_Balls[currentBallIndex].getShape().setOutlineColor(sf::Color::Black);
	m_Balls[currentBallIndex].getShape().setOutlineThickness(5.0f);
	
}

void Table::_checkBounds(Ball & ball)
{
	
	int startX = m_Width - m_Width * m_Scale.x;
	int endY = m_Height * m_Scale.y;

	if ((ball.getPosition().x + ball.getRadius()) > m_Width - EDGE_WIDTH && ball.getVelocity().x > 0)
	{
		ball.setVelocity(glm::vec2(ball.getVelocity().x * -1, ball.getVelocity().y));
	}
	else if ((ball.getPosition().x - ball.getRadius()) < startX + EDGE_WIDTH && ball.getVelocity().x < 0)
	{
		ball.setVelocity(glm::vec2(ball.getVelocity().x * -1, ball.getVelocity().y));
	}

	if ((ball.getPosition().y + ball.getRadius()) > m_Height - 40 && ball.getVelocity().y > 0)
	{
		ball.setVelocity(glm::vec2(ball.getVelocity().x, ball.getVelocity().y * -1));
	}
	else if ((ball.getPosition().y - ball.getRadius() < EDGE_WIDTH) && ball.getVelocity().y < 0)
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
#define MU_SLIDE 0.1f
glm::vec2 Table::_calculateVel(Ball & ball, float dt)
{

	glm::vec2 initialVel = glm::vec2(ball.getVelocity().x, ball.getVelocity().y);
	glm::vec2 frictionForce = -(initialVel*-(0.2f * -(ball.getMass()*9.82f * 0.01f)));
	glm::vec2 frictionForceAngular = -(glm::vec2(ball.getAngleVelocity())*-(0.2f * -(ball.getMass()*9.82f * 0.01f)));
	//frictionForce = glm::vec2(0.0f, 0.0f);
	glm::vec3 temp = glm::vec3((pow(M_PI, 2) * pow(ball.getRadius(), 3) * density) * ball.getAngleVelocity().x, (pow(M_PI, 2) * pow(ball.getRadius(), 3) * density) * ball.getAngleVelocity().y, (pow(M_PI, 2) * pow(ball.getRadius(), 3) * density) * ball.getAngleVelocity().z);
	glm::vec3 magnusEffectForce = glm::cross(temp, glm::vec3(initialVel,0));
	glm::vec3 angular = ball.getAngleVelocity() * ball.getRadius();
	glm::vec2 finalForce = glm::vec2(frictionForce + frictionForceAngular);

	/*if (glm::length(ball.getVelocity()) > 0)
		__debugbreak();
*/
	glm::vec2 spin = glm::vec2(ball.getAngleVelocity()) * ball.getRadius();
	glm::vec2 finalVel = ball.getVelocity() + (finalForce / ball.getMass());
	
	ball.setVelocity(finalVel);

	
	return finalVel;
}
#define MU_BALL 0.1f
void Table::_collision(Ball & ball1, Ball & ball2)
{
	float e = 1;

	glm::vec2 normVec = glm::normalize(ball1.getPosition() - ball2.getPosition());

	glm::vec2 an = glm::dot(ball1.getVelocity(), -normVec) * -normVec;
	glm::vec2 at = ball1.getVelocity() - an;
	glm::vec2 bn = glm::dot(ball2.getVelocity(), normVec) * normVec;
	glm::vec2 bt = ball2.getVelocity() - bn;

	ball1.setVelocity(at + bn);
	ball2.setVelocity(bt + an);

	glm::vec3 vp1 = glm::cross(ball1.getRadius() * -glm::vec3(normVec, 0.0f), glm::vec3(ball1.getVelocity(), 0.0));
	glm::vec3 vp2 = glm::cross(ball2.getRadius() * glm::vec3(normVec, 0.0f), glm::vec3(ball2.getVelocity(), 0.0));

	glm::vec3 vpr1 = vp1 - vp2;
	glm::vec3 vpr2 = vp2 - vp1;

	glm::vec3 ff1 = -MU_BALL*ball1.getMass() * glm::length(an - bn) * glm::normalize(vpr1 + glm::vec3(at,0.0f));
	glm::vec3 ff2 = -MU_BALL*ball2.getMass() * glm::length(an - bn) * glm::normalize(vpr2 + glm::vec3(bt, 0.0f));

	glm::vec3 dw1 = 5 * 0.05f / (2 * ball1.getMass()*ball1.getRadius()*ball1.getRadius()) * glm::cross(ball1.getRadius()* -glm::vec3(normVec,0.0f), ff1);
	glm::vec3 dw2 = 5 * 0.05f / (2 * ball2.getMass()*ball2.getRadius()*ball2.getRadius()) * glm::cross(ball2.getRadius()* glm::vec3(normVec, 0.0f), ff2);

	ball1.setAngleVelocity(ball1.getAngleVelocity() + dw1);
	ball2.setAngleVelocity(ball2.getAngleVelocity() + dw2);


	/*glm::vec2 gWhiteDir = (glm::vec2(ball1.getVelocity().x, ball1.getVelocity().y));
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
	ball2.setVelocity(YellowFinal);*/
	

}
