#include "Ball.h"
int Ball::ballTotalIndex = 0;
sf::Color Ball::colors[] = {
	sf::Color::Red,
	sf::Color::Yellow,
	sf::Color::Blue,
	sf::Color::Green,
	sf::Color::Magenta,
	sf::Color::Cyan
};

void Ball::draw(sf::RenderTarget & renderTarget, sf::RenderStates states) const
{
	renderTarget.draw(m_shape);

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(m_position.x, m_position.y)),
		sf::Vertex(sf::Vector2f(m_position.x + (m_velocity.x * m_radius), m_position.y + (m_velocity.y * m_radius)))
	};

	renderTarget.draw(line, 2, sf::Lines);
}

Ball::Ball(glm::vec2 position)
{

		m_radius = 25; 
		m_mass = 50.0f;

		m_shape = sf::CircleShape(m_radius);
		m_shape.setOrigin(m_radius, m_radius);
		m_shape.setPosition(sf::Vector2f(position.x, position.y));
		setColor(colors[ballTotalIndex++ % 6]);
		
		m_position = position;

		m_velocity = glm::vec2(0, 0);

		m_angleVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
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

void Ball::setAcceleration(glm::vec3 acceleration)
{
	m_acceleration = acceleration;
}


float Ball::getMass() 
{
	return m_mass;
}

float Ball::getRadius() const
{
	return m_radius;
}

sf::CircleShape& Ball::getShape()
{
	return m_shape;
}

glm::vec3 Ball::getAngleVelocity() const
{
	return m_angleVelocity; 
}

glm::vec3 Ball::getAcceleration() const
{
	return m_acceleration;
}

