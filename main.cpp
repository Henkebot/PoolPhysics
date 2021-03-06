
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include <chrono>
#include "Ball.h"
#include "Table.h"


#include <glm.hpp>
#define WIDTH 1300.0f
#define HEIGHT 720.0f
#define UPDATE_RATE 60.0f
bool checkKeys(int keys[], bool keyPressed[])
{
	for (int i = 0; i < 4; i++)
	{
		keyPressed[i] = GetAsyncKeyState(keys[i]);
		
		if(keyPressed[i])
			return true;
	}

	return false;
}

int main()
{
	int keys[] =
	{
		int('1'),
		int('2'),
		int('3'),
		int('4')
	};
	bool keyPressed[4] = { 0 };

	Table background(WIDTH,HEIGHT, 1);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

	using namespace std::chrono;
	auto lastTime = steady_clock::now();
	int updates = 0;
	int fpsCounter = 0;
	float freq = 1000000000.0f / UPDATE_RATE;
	float unprocessed = 0;
	auto timer = steady_clock::now();
	int currentMenu = 0;
	bool runSimulation = true;
	float t2 = 0.0f;
    while (window.isOpen())
    {
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{

			system("cls");
			std::cout << "Menu\n1. Edit current ball.\n2. Change physic values.\n3. Change cue angle.\n4. Run Simulation.\nEnter value: ";
			currentMenu = 0;
			std::cin >> currentMenu;
			switch (currentMenu)
			{
			case 1:
			{
				int selectIndex = 0;

				Ball* currentBall = &background.getCurrentBall();
				glm::vec2 pos(0.0f);
				glm::vec2 vel(0.0f);
				float tempValue = 0;
				while (selectIndex != 5)
				{
					system("cls");

					std::cout << "Edit ball\n";
					std::cout << "1. Position:\t(" << currentBall->getPosition().x << "," << currentBall->getPosition().y << ")\n";
					std::cout << "2. Velocity:\t(" << currentBall->getVelocity().x << "," << currentBall->getVelocity().y << "), " << glm::length(currentBall->getVelocity()) << " ms/s\n";
					std::cout << "3. Mass:\t" << currentBall->getMass() << " Kg\n";
					std::cout << "4. Mass:\t" << currentBall->getRadius() << " m\n";
					std::cout << "5. Back\nValue: ";
					std::cin >> selectIndex;

					switch (selectIndex)
					{
					case 1:

						std::cout << "Position.x = ";
						std::cin >> tempValue;
						pos.x = tempValue;
						std::cout << "Position.y = ";
						std::cin >> tempValue;
						pos.y = tempValue;
						std::cin.ignore();
						currentBall->setPosition(pos);
						break;
					case 2:

						std::cout << "Veloctiy.x = ";
						std::cin >> tempValue;
						vel.x = tempValue;
						std::cout << "Veloctiy.y = ";
						std::cin >> tempValue;
						vel.y = tempValue;
						std::cin.ignore();
						currentBall->setVelocity(vel);
						break;
					case 3:
						std::cout << "Mass = ";
						std::cin >> tempValue;
						vel.y = tempValue;
						std::cin.ignore();
						currentBall->setMass(vel.y);
						break;
					case 4:
						std::cout << "Radius = ";
						std::cin >> tempValue;
						vel.y = tempValue;
						std::cin.ignore();
						currentBall->setRadius(vel.y);
						break;
					}
				}



			}
			break;
			case 2:
			{
				int selectIndex = 0;

				float eValue = background.getE();
				float frictionValue = background.getFriction();
				float edgeFriciton = background.getEdgeFriciton();
				float tempValue = 0.0f;
				while (selectIndex != 4)
				{
					system("cls");
					std::cout << "Physics\n";
					std::cout << "1. Elastic constant: " << eValue << "\n";
					std::cout << "2. Friciton constant: " << frictionValue << "\n";
					std::cout << "3. Edge Friciton constant: " << edgeFriciton << "\n";
					std::cout << "4. Back\n";
					std::cout << "Value: ";
					std::cin >> selectIndex;
					std::cin.ignore();
						switch (selectIndex)
						{
						case 1:
							std::cout << "Elastic constant = ";
							std::cin >> tempValue;
							background.setE(tempValue);
							eValue = background.getE();
							break;
						case 2:
							std::cout << "Friciton constant = ";
							std::cin >> tempValue;
							background.setFriction(tempValue);
							frictionValue = background.getFriction();
							break;
						case 3:
							std::cout << "Edge Friction constant = ";
							std::cin >> tempValue;
							background.setEdgeFriciton(tempValue);
							edgeFriciton = background.getEdgeFriciton();
							break;
						}
				}
			}
			break;
			case 3:
			{
				int selectIndex = 0;
				float tempValue = 0.0f;
				Ball* curBall = &background.getCurrentBall();
				while (selectIndex != 3)
				{
					system("cls");
					std::cout << "Spin!\n";
					std::cout << "1. Left spin\n";
					std::cout << "2. Right spin\n";
					std::cout << "3. Back.\n\n";
					std::cout << "Value: ";
					std::cin >> selectIndex;
					switch (selectIndex)
					{
					case 1:
					{
						std::cout << "0-10?\n";
						std::cout << "Value: ";
						std::cin >> tempValue;
						tempValue = glm::clamp(tempValue, 0.0f, 10.0f);
						curBall->setAngleVelocity(glm::vec3(0, 0, -tempValue * 0.001f));					}
					break;
					case 2:
						std::cout << "1-10?\n";
						std::cout << "Value: ";
						std::cin >> tempValue;
						tempValue = glm::clamp(tempValue, 0.0f, 10.0f);
						curBall->setAngleVelocity(glm::vec3(0, 0, tempValue * 0.001f));
					break;
					}
				}
				
			}
			break;
			case 4:
				int value = 0;
				std::cout << "Run Simulation!\n";
				std::cout << "1. Euler\n";
				std::cout << "2. RK4\n";
				std::cout << "3. Back\n";
				std::cout << "Value: ";
				std::cin >> value;
				
				background.runSimulation((value == 1) ? Table::StepType::Euler : Table::StepType::RK4);
				t2 = 0.0f;
				break;
			
			}
			lastTime = steady_clock::now();
		}
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		auto current_time = steady_clock::now();
		auto dt = duration_cast<nanoseconds>(current_time - lastTime).count();
		unprocessed += dt / freq;
		lastTime = current_time;


		while (unprocessed > 1)
		{
			updates++;
			unprocessed -= 1;
			float dt2 =30.0f / UPDATE_RATE;


			t2 += dt2;

			if(background.isSimulating())
				if (t2 > 6.0f) background.stopSimulation();

			background.update(window,t2, dt2);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				background.setCurrentBall(mousePos.x, mousePos.y);
			}
			
		}
        window.clear();
		
		fpsCounter++;
		window.draw(background);
		
		if (duration_cast<milliseconds>(steady_clock::now() - timer).count() > 1000)
		{
			printf("\rFPS: %d TICK: %d", fpsCounter, updates);
			updates = 0;
			fpsCounter = 0;
			timer += milliseconds(1000);
		}

        window.display();
    }

    return 0;
}