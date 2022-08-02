#include "BoidsSim.h"

BoidsSim::BoidsSim()
{
	initWindow();
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
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			window.close();
	}

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
	
	window.display();
}

const sf::RenderWindow & BoidsSim::getWindow() const
{
	return window;
}

BoidsSim::~BoidsSim()
{
}
