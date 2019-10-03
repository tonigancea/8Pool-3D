#include "Ball.h"
#include <iostream>

#include <string>

Ball::Ball()
{
}

Ball::Ball(std::string newType)
{
	type = newType;
}

Ball::Ball(std::string newType, float newX, float newZ)
{
	x = newX;
	z = newZ;
	type = newType;
}

Ball::~Ball()
{
}

void Ball::SetPosition(float newX, float newZ)
{
	x = newX;
	z = newZ;
}

void Ball::SetX(float newX)
{
	x = newX;
}

void Ball::SetZ(float newZ)
{
	z = newZ;
}
