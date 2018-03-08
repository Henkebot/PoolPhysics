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
public:
	enum StepType
	{
		Euler,
		RK4
	};
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
	

	struct State
	{
		glm::vec2 x;
		glm::vec2 v;
	};
	
	StepType m_stepFunction;

	float m_E;
	float m_EdgeElastic;
	float m_Friction;

	bool m_RunSimulation;


public:
	Table(int width, int height, float scale);

	void update(const sf::Window& window, double t, float dt);

	Ball& getCurrentBall();
	void setCurrentBall(int windowX, int windowY);
	void setStepFunction(StepType stepFunction); 
	void runSimulation(StepType type);
	void stopSimulation();

	StepType getStepFunction();

	void setE(float e);
	void setFriction(float friction);
	void setEdgeFriciton(float edgeElastic);

	float getE() const;
	float getFriction() const;
	float getEdgeFriciton() const;

	bool isSimulating() const;

	~Table(); 

private:
	void draw(sf::RenderTarget& renderTarget, sf::RenderStates states) const; 


	void _initHoles();
	void _initBalls();
	
	void _checkBounds(Ball& ball);
	int _move(Ball & ball, float t, float dt);
	glm::vec2 _calculateVel(Ball& ball, float dt);
	void _collision(Ball& ball1, Ball& ball2);

	struct Derivative
	{
		glm::vec2 dx;      // dx/dt = velocity
		glm::vec2 dv;      // dv/dt = acceleration
	};

	Derivative _evaluate(Ball& initial, float t, float dt, Derivative& d);
	glm::vec2 _acceleration(Ball& ball, float t);
	glm::vec2 _acceleration(Ball ball, glm::vec2 x, glm::vec2 v, float t);
	int _integrate(Ball& ball, float t, float dt);
};
#endif 

