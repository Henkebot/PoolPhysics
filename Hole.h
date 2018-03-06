#ifndef HOLE_H
#define HOLE_H
#include <SFML\Graphics.hpp>
#include <glm.hpp>
#include "Ball.h"
class Hole : public sf::Drawable
{
private:
	glm::vec2 m_position; 

	static sf::Texture m_texture;

	sf::Sprite m_sprite; 
	float m_radius; 

	void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const; 

public:
	Hole(glm::vec2 position = glm::vec2(0,0)); 
	~Hole(); 

	void setPosition(glm::vec2 position); 
	bool collision(const Ball& ball) const;

};
#endif

