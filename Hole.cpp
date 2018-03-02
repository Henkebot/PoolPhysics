#include "Hole.h"

sf::Texture Hole::m_texture;

void Hole::draw(sf::RenderTarget & renderTarget, sf::RenderStates states) const
{
	renderTarget.draw(m_sprite, states); 
}

Hole::Hole(glm::vec2 position)
{
	m_position = position; 
	m_radius = 35; 
	m_texture.loadFromFile("graphics/Hole.png"); 
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(35, 35); 
	m_sprite.setPosition(position.x, position.y);
}

Hole::~Hole()
{
}

void Hole::setPosition(glm::vec2 position)
{
	m_position.x = position.x;
	m_position.y = position.y; 
	m_sprite.setPosition(position.x, position.y);
}

bool Hole::collision(const Ball & ball) const
{
	float distance = glm::length(ball.getPosition() - m_position);
	return distance < (2 * m_radius)/2;
}
