#pragma once
#include "stdafx.h"

class Boid
{
public:
	Boid();
	~Boid();

	float getX();
	float getY();
	float getXVel();
	float getYVel();

	void setXVel(float _xVel);
	void setYVel(float _yVel);

	void updatePosition();

	void render(sf::RenderTarget& target);

private:
	sf::CircleShape sprite;

	float xVel;
	float yVel;
};

