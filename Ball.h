#ifndef BALL_H	
#define BALL_H
#include <glm.hpp>
#include <SFML\Graphics.hpp>

class Ball : public sf::Drawable
{
private:
	
	static int ballTotalIndex;
	static sf::Color colors[];
	

	sf::CircleShape m_shape; 
	float m_radius; 
	float m_mass; 
	glm::vec2 m_position;
	glm::vec2 m_velocity; 
	glm::vec3 m_angleVelocity; 


	void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const;

public:
	Ball(glm::vec2 position); 
	~Ball(); 

	void setColor(sf::Color color); 
	void setPosition(glm::vec2 position); 
	void setVelocity(glm::vec2 velocity); 
	void setMass(float mass); 
	void setAngleVelocity(glm::vec3 angleVelocity);


	glm::vec2 getVelocity(); 
	glm::vec2 getPosition() const; 
	float getMass(); 
	float getRadius() const;
	sf::CircleShape& getShape(); 
	glm::vec3 getAngleVelocity() const;
 

};


#endif
