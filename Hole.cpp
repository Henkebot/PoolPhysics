#include "Hole.h"

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
