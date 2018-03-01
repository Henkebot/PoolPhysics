#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <SFML\Graphics.hpp>
#include <glm.hpp>

class Background : public sf::Drawable
{
private:
	glm::vec2 m_position; 
	sf::Texture m_texture; 
	sf::Sprite m_sprite;  

	void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const; 

public:
	Background();
	~Background(); 
};

#endif 

