#pragma once

#include <iostream>
#include "stdafx.h"
#include "Boid.h"

class BoidsSim
{

public:
	BoidsSim();
	
	void updateBoids();
	void renderBoids();
	void update();
	void render();
	
	const sf::RenderWindow& getWindow() const;

	~BoidsSim();

private:
	sf::RenderWindow window;
	sf::Event event;

	int windowWidth = 800;
	int windowHeight = 800;
	int maxFramerate = 60;

	int numBoids = 500;
	Boid* boids;

	float protectedRadius = 2.0f;
	float visualRadius = 20.0f;
	float centeringFactor = 0.0005f;
	float matchingFactor = 0.05f;
	float avoidFactor = 0.05f;
	float turnFactor = 0.2f;
	float maxSpeed = 10.0f;
	float minSpeed = 1.0f;

	int boundingBoxMargin = 10;
	sf::RectangleShape boundingBox;

	void initWindow();
	void initBoids();
};
