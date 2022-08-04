#include "BoidsSim.h"

#include "imgui\imgui.h"
#include "imgui\imgui-SFML.h"

BoidsSim::BoidsSim()
{
	initWindow();
	initUI();
	initBoids();
}

void BoidsSim::initWindow()
{
	window.create(sf::VideoMode(windowWidth, windowHeight), "Boids");
	window.setFramerateLimit(maxFramerate);
}

void BoidsSim::initBoids()
{
	boids = new Boid[numBoids];
}

void BoidsSim::initUI()
{
	ImGui::SFML::Init(window);
}

void BoidsSim::updateUI()
{
	ImGui::SFML::Update(window, deltaClock.restart());

	ImGui::Begin("Parameters");
	ImGui::SliderFloat("Protected Radius", &protectedRadius, 0.0f, 10.0f);
	ImGui::SliderFloat("Visual Radius", &visualRadius, 10.5f, 100.0f);
	ImGui::SliderFloat("Centering Factor", &centeringFactor, 0.0005f, 1.0f);
	ImGui::SliderFloat("Matching Factor", &matchingFactor, 0.05f, 2.0f);
	ImGui::SliderFloat("Avoid Factor", &avoidFactor, 0.05f, 2.0f);
	ImGui::SliderFloat("Turn Factor", &turnFactor, 0.1f, 2.0f);
	ImGui::SliderFloat("Max Speed", &maxSpeed, 5.5f, 20.0f);
	ImGui::SliderFloat("Min Speed", &minSpeed, 0.5f, 5.0f);

	ImGui::Text("");
	ImGui::End();
}

void BoidsSim::renderUI()
{
	ImGui::SFML::Render(window);
}

void BoidsSim::updateBoids()
{
	for (size_t currentBoid = 0; currentBoid < numBoids; currentBoid++)
	{
		float xPosAvg = 0.0f;
		float yPosAvg = 0.0f;
		float xVelAvg = 0.0f;
		float yVelAvg = 0.0f;
		float closedx = 0.0f; 
		float closedy = 0.0f; 
		float numNeighboringBoids = 0.0f;

		float currentBoidX = boids[currentBoid].getX();
		float currentBoidXVel = boids[currentBoid].getXVel();
		float currentBoidY = boids[currentBoid].getY();
		float currentBoidYVel = boids[currentBoid].getYVel();

		for (size_t otherBoid = 0; otherBoid < numBoids; otherBoid++)
		{
			if (currentBoid == otherBoid)
				break;

			float otherBoidX = boids[otherBoid].getX();
			float otherBoidY = boids[otherBoid].getY();
			float otherBoidVelX = boids[otherBoid].getXVel();
			float otherBoidVelY = boids[otherBoid].getYVel();

			float dx = currentBoidX - otherBoidX;
			float dy = currentBoidY - otherBoidY;

			if (std::abs(dx) < visualRadius && std::abs(dy) < visualRadius)
			{
				float squaredDistance = dx * dx + dy * dy;
				float squaredProtectedRadius = protectedRadius * protectedRadius;
				float squaredVisualRadius = visualRadius * visualRadius;
				if (squaredDistance < squaredProtectedRadius)
				{
					closedx += dx;
					closedy += dy;
				}
				else if (squaredDistance < squaredVisualRadius)
				{
					xPosAvg += otherBoidX;
					yPosAvg += otherBoidY;
					xVelAvg += otherBoidVelX;
					yVelAvg += otherBoidVelY;

					numNeighboringBoids++;
				}
			}
		}

		if (numNeighboringBoids > 0.0f)
		{
			xPosAvg /= numNeighboringBoids;
			yPosAvg /= numNeighboringBoids;
			xVelAvg /= numNeighboringBoids;
			yVelAvg /= numNeighboringBoids;

			// Centering/matching contributions to velocity
			currentBoidXVel = currentBoidXVel
				+ (xPosAvg - currentBoidX) * centeringFactor
				+ (xVelAvg - currentBoidXVel) * matchingFactor;
			currentBoidYVel = currentBoidYVel
				+ (yPosAvg - currentBoidY) * centeringFactor
				+ (yVelAvg - currentBoidYVel) * matchingFactor;
		}

		// Avoidance contribution to velocity
		currentBoidXVel = currentBoidXVel + (closedx * avoidFactor);
		currentBoidYVel = currentBoidYVel + (closedy * avoidFactor);
		
		// Check if close to edge of window
		if (currentBoidY <= 0.0f) // Top
			currentBoidYVel = currentBoidYVel + turnFactor;
		if (currentBoidX >= windowWidth) // Right
			currentBoidXVel = currentBoidXVel - turnFactor;
		if (currentBoidX <= 0.0f) // Left
			currentBoidXVel = currentBoidXVel + turnFactor;
		if (currentBoidY >= windowHeight) // Bottom
			currentBoidYVel = currentBoidYVel - turnFactor;

		float speed = std::sqrt(currentBoidXVel * currentBoidXVel + currentBoidYVel * currentBoidYVel);
		if (speed < minSpeed)
		{
			currentBoidXVel = (currentBoidXVel / speed) * minSpeed;
			currentBoidYVel = (currentBoidYVel / speed) * minSpeed;
		}
		if (speed > maxSpeed)
		{
			currentBoidXVel = (currentBoidXVel / speed) * maxSpeed;
			currentBoidYVel = (currentBoidYVel / speed) * maxSpeed;
		}

		// Assign final velocity
		boids[currentBoid].setXVel(currentBoidXVel);
		boids[currentBoid].setYVel(currentBoidYVel);

		boids[currentBoid].updatePosition();
	}
}

// Gets events and updates variables
void BoidsSim::update()
{
	// Poll events
	ImGui::SFML::ProcessEvent(event);
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			window.close();
	}

	updateUI();
	updateBoids();
}

void BoidsSim::renderBoids()
{
	for (size_t i = 0; i < numBoids; i++)
		boids[i].render(window);
}

// Renders the game objects
void BoidsSim::render()
{
	window.clear();

	renderBoids();
	
	renderUI();

	window.display();
}

const sf::RenderWindow & BoidsSim::getWindow() const
{
	return window;
}

BoidsSim::~BoidsSim()
{
	ImGui::SFML::Shutdown();
}
