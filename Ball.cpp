#include "Ball.h"

Ball::Ball(glm::vec2 position)
{
		m_radius = 25; 
		m_shape = sf::CircleShape(m_radius);
		m_shape.setOrigin(m_radius, m_radius);
		m_shape.setPosition(sf::Vector2f(position.x, position.y));
		
		m_position = position;
		m_velocity = glm::vec2(0, 0);
		m_mass = 50;
		m_angleVelocity = glm::vec3(0.000000000050f, 0.000000000010f, 0.000000000010f);
}

Ball::~Ball()
{
}

void Ball::setColor(sf::Color color)
{
	m_shape.setFillColor(color); 
}

void Ball::setPosition(glm::vec2 position)
{
	m_position = position;
	m_shape.setPosition(position.x, position.y);
}

glm::vec2 Ball::getVelocity()
{
	return m_velocity; 
}

void Ball::setVelocity(glm::vec2 velocity)
{
	m_velocity = velocity;
}

glm::vec2 Ball::getPosition() const
{
	return m_position;
}

void Ball::setMass(float mass) 
{
	m_mass = mass;
}

void Ball::setAngleVelocity(glm::vec3 angleVelocity)
{
	m_angleVelocity = angleVelocity; 
}

float Ball::getMass() 
{
	return m_mass;
}

sf::CircleShape Ball::getShape()
{
	return m_shape;
}

glm::vec3 Ball::getAngleVelocity() const
{
	return m_angleVelocity; 
}
