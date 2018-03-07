#include "Table.h"
#include <Windows.h>
#include <iostream>
#include <locale>
template <class charT, charT sep>
class punct_facet : public std::numpunct<charT>
{
protected:
	charT do_decimal_point() const { return sep; }
};

void Table::draw(sf::RenderTarget & renderTarget, sf::RenderStates states) const
{

	renderTarget.draw(m_sprite, states); 


	for (const auto& hole : m_Holes)
		renderTarget.draw(hole, states);
	for(const auto& ball : m_Balls)
		renderTarget.draw(ball, states);
}

Table::Table(int width, int height, float scale)
	: m_Width(width), m_Height(height)
{
	m_E = 1.0f;
	m_Friction = 0.4f;
	m_EdgeElastic = 0.8f;
	m_RunSimulation = false;

	m_Scale = glm::vec2(scale);
	m_position = glm::vec2(m_Width - m_Width * m_Scale.x, 0);

	m_texture.loadFromFile("graphics/Background.png");
	
	m_sprite.setTexture(m_texture); 
	
	m_sprite.setPosition(m_position.x, m_position.y);
	m_sprite.scale(m_Scale.x, m_Scale.y);

	m_stepFunction = Euler;

	_initHoles();
	_initBalls();
}

void Table::update(const sf::Window& window, double t, float dt)
{
	for (int i = 0; i < m_Balls.size(); i++)
	{

		_checkBounds(m_Balls[i]);
		
		if (GetAsyncKeyState(VK_SPACE))
		{
			sf::Mouse mouse;
			sf::Vector2i lol = mouse.getPosition(window);
			if(currentBallIndex >= 0)m_Balls[currentBallIndex].setVelocity((glm::vec2(lol.x, lol.y) - m_Balls[currentBallIndex].getPosition()) / 10.0f);
		}
		if (m_stepFunction == Euler)
		{
			if (m_RunSimulation)
			{
				_move(m_Balls[i], t + dt);
			}
			else
			{
				_move(m_Balls[i], dt);

			}
		}
		else
		{
			if (m_RunSimulation)
			{
				_integrate(m_Balls[i],t, dt);
			}
			else
			{
				_integrate(m_Balls[i], t, dt);
			}
		}
		if (!m_RunSimulation)
		{
			
			for (auto& hole : m_Holes)
			{
				if (hole.collision(m_Balls[i]))
				{
					m_Balls.erase(m_Balls.begin() + i);
					
					if(--currentBallIndex >= 0)
					{
						m_Balls[currentBallIndex].getShape().setOutlineColor(sf::Color::Black);
						m_Balls[currentBallIndex].getShape().setOutlineThickness(5.0f);
					}
					
				}
				
			}
			
			
		}
		
	}
	
	if (m_dataOut.is_open())
	{
		m_dataOut.imbue(std::locale(m_dataOut.getloc(), new punct_facet<char, ','>));
		m_dataOut << t + dt << "\t";
		m_dataOut << glm::length(m_Balls[0].getVelocity()) << "\t";
		m_dataOut << glm::length(m_Balls[0].getAngleVelocity()) << "\t";
		m_dataOut << glm::length(m_Balls[0].getAcceleration()) << "\t";

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
	if (currentBallIndex >= 0)
	{
		m_Balls[currentBallIndex].getShape().setOutlineColor(sf::Color::Black);
		m_Balls[currentBallIndex].getShape().setOutlineThickness(5.0f);

	}
}

void Table::setStepFunction(StepType stepFunction)
{
	m_stepFunction = stepFunction; 
}

void Table::runSimulation(StepType type)
{
	m_Balls.clear();
	m_Balls.push_back(Ball(glm::vec2(m_Width * 0.1f, m_Height * 0.5f)));
	m_Balls[0].setVelocity(glm::vec2(50, 0));
	m_Balls[0].setAngleVelocity(glm::vec3(0, 0,0.001f));
	m_Balls.push_back(Ball(glm::vec2(m_Width * 0.9f, m_Height * 0.5f)));

	currentBallIndex = 0;
	m_stepFunction = type;


	m_dataOut.open("Results.txt");
	const char* method = (m_stepFunction == Euler) ? "Euler\n" : "RK4\n";
	m_dataOut << method;
	m_dataOut << "T\tVelocity\tAngleVelocity\tAcceleration\n";

	m_RunSimulation = true;
	
}

void Table::stopSimulation()
{
	m_Balls.clear();
	_initBalls();
	m_RunSimulation = false;
}

Table::StepType Table::getStepFunction()
{
	return m_stepFunction; 
}

void Table::setE(float e)
{
	m_E = glm::clamp(e , 0.0f, 1.0f);
}

void Table::setFriction(float friction)
{
	m_Friction = glm::clamp(friction, 0.0f, 1.0f);
}

void Table::setEdgeFriciton(float edgeElastic)
{
	m_EdgeElastic = edgeElastic;
}

float Table::getE() const
{
	return m_E;
}

float Table::getFriction() const
{
	return m_Friction;
}

float Table::getEdgeFriciton() const
{
	return m_EdgeElastic;
}

bool Table::isSimulating() const
{
	return m_RunSimulation;
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
	int startX = m_Width - (m_Width * m_Scale.x * 0.35f);
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

	m_Balls.push_back(Ball(glm::vec2(m_Width * 0.3f, m_Height * 0.5f)));
	
	
	currentBallIndex = m_Balls.size() - 1;
	m_Balls[currentBallIndex].getShape().setOutlineColor(sf::Color::Black);
	m_Balls[currentBallIndex].getShape().setOutlineThickness(5.0f);
	
	//m_Balls[0].setVelocity(glm::vec2(1,1));

	
}

void Table::_checkBounds(Ball & ball)
{
	
	int startX = m_Width - m_Width * m_Scale.x;
	int endY = m_Height * m_Scale.y;

	glm::vec2 collided(0,0);

	if ((ball.getPosition().x + ball.getRadius()) > m_Width - EDGE_WIDTH && ball.getVelocity().x > 0)
	{
		collided = glm::vec2(-1, 0);
	}
	else if ((ball.getPosition().x - ball.getRadius()) < startX + EDGE_WIDTH && ball.getVelocity().x < 0)
	{
		collided = glm::vec2(1, 0);
	}

	if ((ball.getPosition().y + ball.getRadius()) > m_Height - 40 && ball.getVelocity().y > 0)
	{
		
		collided = glm::vec2(0, -1);
	}
	else if ((ball.getPosition().y - ball.getRadius() < EDGE_WIDTH) && ball.getVelocity().y < 0)
	{
		collided = glm::vec2(0, 1);
	}

	
	if (glm::length(collided) > 0)
	{

		glm::vec2 velocity = ball.getVelocity();
		glm::vec2 relfectVec = velocity - 2.0f * glm::dot(collided, velocity) * collided;
		relfectVec *= m_EdgeElastic;
		ball.setVelocity(relfectVec);
	}

}

int Table::_move(Ball & ball, float dt)
{

	int collisionId = -1;

	glm::vec2 nextVel = ball.getVelocity() + _acceleration(ball,dt);
	glm::vec2 nextPos = ball.getPosition() + nextVel * dt;
			
	for (int j = 0; j < m_Balls.size(); j++)
	{
		if (m_Balls[j].getPosition() == ball.getPosition()) continue;

		float distanceBet = glm::length(m_Balls[j].getPosition() - nextPos) - (ball.getRadius() + m_Balls[j].getRadius());

		if (distanceBet < 0.0f)
		{
			glm::vec2 colliisonVector = glm::normalize(m_Balls[j].getPosition() - nextPos) * (distanceBet/2.0f);
			
			ball.setPosition(colliisonVector + ball.getPosition());
			m_Balls[j].setPosition(-colliisonVector + m_Balls[j].getPosition());

			_collision(ball, m_Balls[j]);
			
			collisionId = j;
		}
	}
	if (collisionId == -1)
	{
		ball.setPosition(nextPos);
		ball.setVelocity(nextVel);
	}

	return collisionId;
}

glm::vec2 Table::_calculateVel(Ball & ball, float dt)
{	
	glm::vec2  frictionForce = m_Friction * -ball.getVelocity();
	
	glm::vec3 magnusEffect = glm::cross((0.1f * 0.5f * M_PI * ball.getRadius() *ball.getRadius() * glm::length(ball.getVelocity()) * 0.5f) * ball.getAngleVelocity(), glm::vec3(ball.getVelocity(),0.0f));

 	glm::vec2 finalForce = glm::vec2(frictionForce.x + magnusEffect.x, frictionForce.y + magnusEffect.y);

	glm::vec2 acceleration = (finalForce / ball.getMass()) * dt;

	ball.setAcceleration(glm::vec3(acceleration, 0.0f));

	return acceleration;
}
#define MU_BALL 0.1f
void Table::_collision(Ball & ball1, Ball & ball2)
{
	float e = m_E;

	glm::vec2 n = glm::normalize(ball1.getPosition() - ball2.getPosition());
	glm::vec3 n3 = glm::vec3(n, 0.0f);
	glm::vec3 b1p3 = glm::vec3(0, 0, 1);
	glm::vec3 b2p3 = glm::vec3(0, 0, 1);
	glm::vec3 b1l;
	b1l.x = 0.25f*ball1.getMass()* ball1.getRadius() *ball1.getRadius();
	b1l.y = 0.25f*ball1.getMass()* ball1.getRadius() *ball1.getRadius();
	b1l.z = 0.5 * ball1.getMass() * ball1.getRadius() *ball1.getRadius();
	
	glm::vec3 b2l;
	b2l.x = 0.25f*ball2.getMass()* ball2.getRadius() *ball2.getRadius();
	b2l.y = 0.25f*ball2.getMass()* ball2.getRadius() *ball2.getRadius();
	b2l.z = (5.0f/2.0f) * ball2.getMass() * ball2.getRadius() *ball2.getRadius();
	float vr = glm::dot(ball1.getVelocity() - ball2.getVelocity(), n);

	float nom = (e + 1) * vr;
	
	float rot1 = glm::dot(n3, glm::cross(glm::cross(b1p3, n3) / b1l, b1p3));
	float rot2 = glm::dot(n3, glm::cross(glm::cross(b2p3, n3) / b2l, b2p3));
	float mr = (1 / ball1.getMass()) + (1 / ball2.getMass());
	float denom = mr + rot1 + rot2;

	float j = -(nom / denom);


	glm::vec2 v1 = ball1.getVelocity() + (j / ball1.getMass())*n;
	glm::vec2 v2 = ball2.getVelocity() - (j / ball2.getMass())*n;
	glm::vec3 w1 = ball1.getAngleVelocity() + (glm::cross(b1p3, j*n3) / b1l.z);
	glm::vec3 w2 = ball2.getAngleVelocity() + (glm::cross(b2p3, j*n3) / b2l.z);

	ball1.setVelocity(v1);
	ball2.setVelocity(v2);

	ball1.setAngleVelocity(w1);
	ball1.setAngleVelocity(w2);
}

Table::Derivative Table::_evaluate(Ball& initial, float t, float dt)
{
	Ball ball = initial;

	Derivative output;
	output.dx = ball.getVelocity() * ((m_RunSimulation) ? t+dt : 1.0f);
	output.dv = _acceleration(ball, t + dt);

	return output;
}

glm::vec2 Table::_acceleration(Ball & ball, float t)
{

	glm::vec2 frictionForce = m_Friction * -ball.getVelocity();

	glm::vec3 magnusEffect = glm::cross((0.1f * 0.5f * M_PI * ball.getRadius() *ball.getRadius() * glm::length(ball.getVelocity()) * 0.5f) * ball.getAngleVelocity(), glm::vec3(ball.getVelocity(), 0.0f));

	glm::vec2 finalForce = glm::vec2(frictionForce.x + magnusEffect.x, frictionForce.y + magnusEffect.y);

	glm::vec2 acceleration = (finalForce / ball.getMass()) * ((m_RunSimulation) ? t : 1.0f);

	return acceleration;
}

int Table::_integrate(Ball & ball, float t, float dt)
{

	Derivative a, b, c, d;

	a = _evaluate(ball, t, 0.0f);
	b = _evaluate(ball, t, dt*0.2f);
	c = _evaluate(ball, t, dt*0.7f);
	d = _evaluate(ball, t, dt);

	glm::vec2 dxdt = glm::vec2(1.0f / 6.0f *
		(a.dx + 2.0f * (b.dx + c.dx) + d.dx));

	glm::vec2 dvdt = glm::vec2(1.0f / 6.0f *
		(a.dv.x + 2.0f * (b.dv.x + c.dv.x) + d.dv.x), 1.0f / 6.0f *
		(a.dv.y + 2.0f * (b.dv.y + c.dv.y) + d.dv.y));

	glm::vec2 oldVel = ball.getVelocity();

	glm::vec2 newPos = ball.getPosition() + dxdt * dt;
	glm::vec2 newVel = ball.getVelocity() + dvdt * dt;

	bool collision = false;

	for (int j = 0; j < m_Balls.size(); j++)
	{
		if (m_Balls[j].getPosition() == ball.getPosition()) continue;

		float distanceBet = glm::length(m_Balls[j].getPosition() - newPos) - (ball.getRadius() + m_Balls[j].getRadius());

		if (distanceBet < 0)
		{
			glm::vec2 colliisonVector = glm::normalize(m_Balls[j].getPosition() - newPos) * (distanceBet/2.0f);
			ball.setPosition(colliisonVector + ball.getPosition());
			m_Balls[j].setPosition(-colliisonVector + m_Balls[j].getPosition());

			_collision(ball, m_Balls[j]);
			collision = true;
		}
	}
		
		

	
	if (!collision)
	{
		ball.setPosition(newPos);
		ball.setVelocity(newVel);
		ball.setAcceleration(glm::vec3(dvdt, 0.0f));
	}
	


	return collision ? 0 : -1;
}
