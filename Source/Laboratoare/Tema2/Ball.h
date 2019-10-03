#pragma once
#include <string>
#include <Core/Engine.h>

class Ball
{
	public:
		Ball();
		Ball(std::string newType);
		Ball(std::string newType, float newX, float newZ);
		~Ball();
		
		void SetPosition(float newX, float newZ);
		void SetX(float newX);
		void SetZ(float newZ);

		float x;
		float z;
		std::string type;
		//float speed = 0;
		//glm::vec2 direction = glm::vec2(0,0);
		float vx = 0;
		float vz = 0;

		bool alive = true;
};
