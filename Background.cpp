#include "Background.h"

void Background::draw(sf::RenderTarget & renderTarget, sf::RenderStates states) const
{
	renderTarget.draw(m_sprite, states); 
}

Background::Background()
{
	
	m_position = glm::vec2(0, 0); 
	m_texture.loadFromFile("graphics/Background.png");
	m_sprite.setTexture(m_texture); 
}

Background::~Background()
{
}
