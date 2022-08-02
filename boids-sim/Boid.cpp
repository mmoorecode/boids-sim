#include "stdafx.h"
#include "Boid.h"

Boid::Boid()
{
	float initX = rand() % 200 + 350.0f;
	float initY = rand() % 200 + 350.0f;

	xVel = 1.5f;
	yVel = 1.5f;

	sprite.setPosition(initX, initY);
	sprite.setFillColor(sf::Color::Red);
	sprite.setRadius(1.0f);
}

Boid::~Boid()
{

}

void Boid::updatePosition()
{
	float currentX = sprite.getPosition().x;
	float currentY = sprite.getPosition().y;
	sprite.setPosition(sf::Vector2f(currentX + xVel, currentY + yVel));
}

float Boid::getX()
{
	return sprite.getPosition().x;
}

float Boid::getY()
{
	return sprite.getPosition().y;
}

float Boid::getXVel()
{
	return xVel;
}

float Boid::getYVel()
{
	return yVel;
}

void Boid::setXVel(float _xVel)
{
	xVel = _xVel;
}

void Boid::setYVel(float _yVel)
{
	yVel = _yVel;
}

void Boid::render(sf::RenderTarget & target)
{
	target.draw(sprite);
}
