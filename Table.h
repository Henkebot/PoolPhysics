#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <SFML\Graphics.hpp>
#include <glm.hpp>
#define _USE_MATH_DEFINES
#define M_PI 3.141592f
#include <vector>
#include "Hole.h"
#include "Ball.h"
#include <fstream>

#define EDGE_WIDTH 40
#define density 1637

class Table : public sf::Drawable
{
private:
	glm::vec2 m_position; 
	sf::Texture m_texture; 
	sf::Sprite m_sprite;  


	std::ofstream m_dataOut; 

	// TABLE DIMENSIONS
	int m_Width;
	int m_Height;
	glm::vec2 m_Scale;

	std::vector<Hole> m_Holes;
	std::vector<Ball> m_Balls;

	int currentBallIndex;
	enum StepType
	{
		Euler,
		RK4
	};
	
	StepType m_stepFunction;

	float m_E;
	float m_Friction;


public:
	Table(int width, int height, float scale);

	void update(const sf::Window& window, double t, float dt);

	Ball& getCurrentBall();
	void setCurrentBall(int windowX, int windowY);
	void setStepFunction(StepType stepFunction); 

	StepType getStepFunction(); 
	void setE(float e);
	void setFriction(float friction);
	float getE() const;
	float getFriction() const;

	~Table(); 

private:
	void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const; 


	void _initHoles();
	void _initBalls();
	
	void _checkBounds(Ball& ball);
	int _move(Ball& ball, float dt);
	glm::vec2 _calculateVel(Ball& ball, float dt);
	void _collision(Ball& ball1, Ball& ball2);

	struct Derivative
	{
		glm::vec2 dx;      // dx/dt = velocity
		glm::vec2 dv;      // dv/dt = acceleration
	};

	Derivative evaluate(Ball& ball, double t, float dt, const Derivative &d);
	glm::vec2 acceleration(Ball& ball, double t);
	int integrate(Ball& ball, double t, float dt);
};
#endif 

