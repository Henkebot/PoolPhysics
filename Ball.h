#ifndef BALL_H	
#define BALL_H
#include <glm.hpp>
#include <SFML\Graphics.hpp>
class Ball
{
private:
	
	sf::CircleShape m_shape; 
	float m_radius; 
	float m_mass; 
	glm::vec2 m_position;
	glm::vec2 m_velocity; 


public:
	Ball(glm::vec2 position); 
	~Ball(); 

	void setColor(sf::Color color); 
	void setPosition(glm::vec2 position); 
	void setVelocity(glm::vec2 velocity); 
	void setMass(float mass); 

	glm::vec2 getVelocity(); 
	glm::vec2 getPosition(); 
	float getMass(); 
	sf::CircleShape getShape(); 


};


#endif
