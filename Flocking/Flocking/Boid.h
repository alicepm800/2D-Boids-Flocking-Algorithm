#pragma once
#include "Play.h"
#include <random>


class Boid
{
public:
	//Constructor to create a boid
	Boid(Vector2D displayDimensions, Vector2D pos, Vector2D velocity, Vector2D acceleration) : m_pos(pos), m_velocity(velocity), m_acceleration(acceleration)
	{ 
		//Assign sprite to Boid
		m_spriteId = PlayGraphics::Instance().GetSpriteId("Boid"); 

		//Recentre sprite's origin
		PlayGraphics::Instance().CentreSpriteOrigin(m_spriteId);

		m_screenDimensions = displayDimensions;

		//========================================================================================
		//Generate a random velocity and position for this boid
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float>disX(-2.5f, 2.5f);
		std::uniform_real_distribution<float>disY(-2.5f, 2.5f);
		std::uniform_real_distribution<float>randomXPos(0, m_screenDimensions.x);
		std::uniform_real_distribution<float>randomYPos(0, m_screenDimensions.y);
		
		//Assign boid a random velocity
		m_velocity = Vector2D(disX(gen), disY(gen));

		//Place boid in a random position around the screen
		m_pos = Vector2D(randomXPos(gen), randomYPos(gen));
		//=========================================================================================
	}
	
	void UpdateBoid();

	void RotateBoid();

	void BoidEdgeDetection();

	//Define lerp functions
	Vector2D Lerp(Vector2D a, Vector2D b, float t);
	float Lerp(float a, float b, float t);
	

	Vector2D m_screenDimensions;
	Vector2D m_pos;
	Vector2D m_velocity;
	Vector2D m_acceleration;
	

	//Rotation is measured in radians
	float m_rotation{ 0 };
	float m_maxVelocity;
	int m_spriteId;
	float m_wanderAngle;

private:
	

	
};