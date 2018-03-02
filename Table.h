#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <SFML\Graphics.hpp>
#include <glm.hpp>
#include <vector>
#include "Hole.h"
#include "Ball.h"

#define EDGE_WIDTH 40

class Table : public sf::Drawable
{
private:
	glm::vec2 m_position; 
	sf::Texture m_texture; 
	sf::Sprite m_sprite;  

	// TABLE DIMENSIONS
	int m_Width;
	int m_Height;
	glm::vec2 m_Scale;


	
	std::vector<Hole> m_Holes;
	std::vector<Ball> m_Balls;

public:
	Table(int width, int height, float scale);

	void update();

	~Table(); 

private:
	void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const; 


	void _initHoles();
	void _initBalls();
	
	void _checkBounds(Ball& ball);
	void _move(Ball& ball);
	void _collision(Ball& ball1, Ball& ball2);
};

#endif 

