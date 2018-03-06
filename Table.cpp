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

	m_stepFunction = RK4;
	m_dataOut.open("Results.txt");
	m_dataOut << "T\tPosition\t\t\tVelocity\tAngleVelocity\tCollision\n";


	_initHoles();
	_initBalls();
}

void Table::update(const sf::Window& window, double t, float dt)
{
	int collision = -1;

	for (auto& ball : m_Balls)
	{
		
		_checkBounds(ball);
		if (m_stepFunction == Euler)
		{
			_calculateVel(ball, dt);
		}

		if (GetAsyncKeyState(VK_SPACE))
		{
			sf::Mouse mouse;
			sf::Vector2i lol = mouse.getPosition(window);
			m_Balls[currentBallIndex].setVelocity((glm::vec2(lol.x, lol.y) - m_Balls.back().getPosition()) / 10.0f);
		//	m_Balls[currentBallIndex].setAngleVelocity(glm::vec3(0,0,0.001f));
		}
		if (m_stepFunction == Euler)
		{
			collision = _move(ball, dt);
		}
		else
		{
			collision = integrate(ball, t, dt);
		}
	}


	if (m_dataOut.is_open())
	{
		m_dataOut << t << "\t";
		m_dataOut << glm::length(m_Balls[0].getVelocity()) << "\t";
	}
	if (collision == -1)
		m_dataOut << std::endl;
	else
		m_dataOut << "1" << std::endl;
	
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

void Table::setStepFunction(StepType stepFunction)
{
	m_stepFunction = stepFunction; 
}

Table::StepType Table::getStepFunction()
{
	return m_stepFunction; 
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
	/*glm::vec2 dir(1, -1);
	glm::vec2 dir2(1, 1);
	int startX = m_Width - (m_Width * m_Scale.x * 0.85f);
	int startY = m_Height - (m_Height * m_Scale.y * 0.5f);
	for (int i = 0; i < 1; i++)
	{
		
		glm::vec2 start(dir * float(i) * 50.0f);

		for (int j = 0; j < 2 - i; j++)
		{
			glm::vec2 start2(dir2 * float(j) * 50.0f);
			glm::vec2 direction = (start + start2);
			direction.x += startX;
			direction.y += startY;
			m_Balls.push_back(Ball(direction));
		}
	}*/
	
	m_Balls.push_back(Ball(glm::vec2(m_Width * 0.1f, m_Height * 0.5f)));
	m_Balls[0].setVelocity(glm::vec2(50, 0));
	m_Balls[0].setAngleVelocity(glm::vec3(0, 0,0.001f));
	m_Balls.push_back(Ball(glm::vec2(m_Width * 0.9f, m_Height * 0.5f)));

	currentBallIndex = m_Balls.size() - 1;
	m_Balls[currentBallIndex].getShape().setOutlineColor(sf::Color::Black);
	m_Balls[currentBallIndex].getShape().setOutlineThickness(5.0f);
	
	//m_Balls[0].setVelocity(glm::vec2(1,1));

	
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

int Table::_move(Ball & ball, float dt)
{

	int collisionId = -1;
	glm::vec2 currentPos = ball.getPosition();

	glm::vec2 nextPos = ball.getPosition() + ball.getVelocity();

	float distance = 1;// glm::length(nextPos - currentPos);

	float precision = 10.0f;

	for (float i = distance / precision; i <= distance && collisionId == -1; i += distance / precision)
	{

		glm::vec2 pos = ball.getPosition() + (((ball.getVelocity() / precision) * i)) * dt;

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

	return collisionId;
}
#define MU_SLIDE 0.1f
glm::vec2 Table::_calculateVel(Ball & ball, float dt)
{

	glm::vec2 frictionForce = -(ball.getVelocity()*-(0.2f * -(ball.getMass()*9.82f * 0.01f)));
	
	//glm::vec3 temp = glm::vec3((pow(M_PI, 2) * pow(ball.getRadius(), 3) * density) * ball.getAngleVelocity().x, (pow(M_PI, 2) * pow(ball.getRadius(), 3) * density) * ball.getAngleVelocity().y, (pow(M_PI, 2) * pow(ball.getRadius(), 3) * density) * ball.getAngleVelocity().z);
	//glm::vec3 magnusEffectForce = glm::cross(temp, glm::vec3(ball.getVelocity(), 0));

	glm::vec3 magnusEffect = glm::cross((0.1f * 0.5f * M_PI * ball.getRadius() *ball.getRadius() * glm::length(ball.getVelocity()) * 0.5f) * ball.getAngleVelocity(), glm::vec3(ball.getVelocity(),0.0f));

 	glm::vec2 finalForce = glm::vec2(frictionForce.x + magnusEffect.x, frictionForce.y + magnusEffect.y);

	glm::vec2 finalVel = ball.getVelocity() + (finalForce / ball.getMass()) * dt ;
	
	
	ball.setVelocity(finalVel);

	return finalVel;
}
#define MU_BALL 0.1f
void Table::_collision(Ball & ball1, Ball & ball2)
{
	float e = 1;

	// Källa Formelblad - Icke roterande objekt

	//glm::vec2 n = glm::normalize(ball1.getPosition() - ball2.getPosition());
	//float nom = -(e + 1)*glm::dot(ball1.getVelocity() - ball2.getVelocity(), n);
	//float denom = (1 / ball1.getMass()) + (1 / ball2.getMass());
	//float j = nom / denom;

	//glm::vec2 v1 = ball1.getVelocity() + (j / ball1.getMass())*n;
	//glm::vec2 v2 = ball2.getVelocity() - (j / ball2.getMass())*n;

	//ball1.setVelocity(v1);
	//ball2.setVelocity(v2);

	// Källa Formelblad - Roterande objekt
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

	// Källa http://archive.ncsa.illinois.edu/Classes/MATH198/townsend/public/illiPool.c
	/*glm::vec2 normVec = glm::normalize(ball1.getPosition() - ball2.getPosition());

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
	ball2.setAngleVelocity(ball2.getAngleVelocity() + dw2);*/

	// Källa Elastiska kollisioner (Formelblad) + lite ändrat
	//glm::vec2 gWhiteDir = (glm::vec2(ball1.getVelocity().x, ball1.getVelocity().y));
	//glm::vec2 gYellowDir = (glm::vec2(ball2.getVelocity().x, ball2.getVelocity().y));

	//glm::vec2 gWhitePos = glm::vec2(ball1.getPosition().x, ball1.getPosition().y);
	//glm::vec2 gYellowPos = glm::vec2(ball2.getPosition().x, ball2.getPosition().y);

	//glm::vec2 d = glm::normalize(gWhitePos - gYellowPos);

	//glm::vec2 WhiteV0 = glm::dot(gWhiteDir, d) * d;
	//glm::vec2 YellowV0 = glm::dot(gYellowDir, d) * d;

	//float WhiteMass = ball1.getMass();
	//float YellowMass = ball2.getMass();

	//glm::vec2 WhiteV1 = (WhiteV0*(WhiteMass - (YellowMass*e)) + (YellowMass * YellowV0 *(1 + e))) / (WhiteMass + YellowMass);
	//glm::vec2 YellowV1 = (YellowV0*(YellowMass - (WhiteMass*e)) + (WhiteMass*WhiteV0*(1 + e))) / (WhiteMass + YellowMass);

	//glm::vec2 WhiteFinal = gWhiteDir - WhiteV0 + WhiteV1;
	//glm::vec2 YellowFinal = gYellowDir - YellowV0 + YellowV1;

	//// Bara överför vinkelhastigheterna
	//ball1.setAngleVelocity(-ball2.getAngleVelocity());
	//ball2.setAngleVelocity(-ball1.getAngleVelocity());

	//ball1.setVelocity(WhiteFinal);
	//ball2.setVelocity(YellowFinal);
	

}

Table::Derivative Table::evaluate(Ball & ball, double t, float dt, const Derivative & d)
{
	Derivative output;
	output.dx = ball.getVelocity();
	output.dv = acceleration(ball, t + dt);
	return output;
}

glm::vec2 Table::acceleration(Ball & ball, double t)
{
	glm::vec2 frictionForce = -(ball.getVelocity()*-(0.2f * -(ball.getMass()*9.82f * 0.01f)));

	glm::vec3 magnusEffect = glm::cross((0.1f * 0.5f * M_PI * ball.getRadius() *ball.getRadius() * glm::length(ball.getVelocity()) * 0.5f) * ball.getAngleVelocity(), glm::vec3(ball.getVelocity(), 0.0f));

	glm::vec2 finalForce = glm::vec2(frictionForce.x + magnusEffect.x, frictionForce.y + magnusEffect.y);   
	glm::vec2 acceleration = finalForce / ball.getMass();

	return acceleration;
}

int Table::integrate(Ball & ball, double t, float dt)
{
	Derivative a, b, c, d;

	a = evaluate(ball, t, 0.0f, Derivative());
	b = evaluate(ball, t, dt*0.5f, a);
	c = evaluate(ball, t, dt*0.5f, b);
	d = evaluate(ball, t, dt, c);

	glm::vec2 dxdt = glm::vec2(1.0f / 6.0f *
		(a.dx.x + 2.0f * (b.dx.x + c.dx.x) + d.dx.x), 1.0f / 6.0f *
		(a.dx.y + 2.0f * (b.dx.y + c.dx.y) + d.dx.y));

	glm::vec2 dvdt = glm::vec2(1.0f / 6.0f *
		(a.dv.x + 2.0f * (b.dv.x + c.dv.x) + d.dv.x), 1.0f / 6.0f *
		(a.dv.y + 2.0f * (b.dv.y + c.dv.y) + d.dv.y));

	glm::vec2 oldPos = ball.getPosition();

	glm::vec2 newPos = ball.getPosition() + dxdt*dt;
	glm::vec2 newVel = ball.getVelocity() + dvdt*dt;

	bool collision = false;
	float distance = glm::length(newPos - oldPos);

	float precision = 0.0f;

	for (float i = precision; i <= 1 && collision == false; i += 0.001f)
	{
		glm::vec2 tempPos = glm::mix(oldPos, newPos, i);// oldPos + (((newVel / precision) * i));

		for (int j = 0; j < m_Balls.size(); j++)
		{
			if (m_Balls[j].getPosition() == ball.getPosition()) continue;

			float distanceBet = glm::length(m_Balls[j].getPosition() - tempPos);

			if (distanceBet < ball.getRadius() + m_Balls[j].getRadius())
			{
				_collision(ball, m_Balls[j]);
				collision = true;
			}
		}

	}
	
 	if (!collision)
	{
		ball.setPosition(newPos);
		ball.setVelocity(newVel);
	}


	return collision ? 0 : -1;
}
