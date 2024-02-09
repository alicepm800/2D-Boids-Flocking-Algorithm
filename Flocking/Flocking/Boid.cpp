#include "Boid.h"

#define PI 3.141592635

void Boid::UpdateBoid()
{
	m_pos += m_velocity;
	Vector2D targetVelocity = Vector2D(0, 0);
	targetVelocity = m_velocity + m_acceleration;
	//Use linear interpolation to reduce jitteriness of movement
	m_velocity = Lerp(m_velocity, targetVelocity, 0.05f);

	//Reset acceleration on every frame
	m_acceleration = Vector2D(0, 0);	
}

void Boid::RotateBoid()
{
	float targetRotation = 0.0f;
	targetRotation = std::atan2(m_velocity.y, m_velocity.x) + PI/2;

	//============================================================================================
	//Make rotation speed relative to how much rotation is required to reach the target rotation
	//e.g. rotation speed slows for a larger angle difference between current rotation and target rotation
	
	//Calculation the difference in angles needed for rotation
	float angleDifference = targetRotation - m_rotation;

	//Set range of angle difference to [-PI, PI]
	while (angleDifference > PI)
	{
		angleDifference -= 2 * PI;
	}
	while (angleDifference < -PI)
	{
		angleDifference += 2 * PI;
	}

	// Calculate interpolation factor based on the magnitude of angle difference
	float rotationSpeed = 0.1f; 
	if (std::abs(angleDifference) > PI / 2) {
		//If the angle difference is larger than PI/2, reduce the interpolation factor
		rotationSpeed *= 0.5f; 
	}

	//============================================================================================
	//Use linear interpolation to reduce jitterniness of rotation
	m_rotation = Lerp(m_rotation, targetRotation, rotationSpeed);
}

void Boid::BoidEdgeDetection()
{
	//Screen edge detection so boids do not go off screen
	if (m_pos.x > m_screenDimensions.x)
	{
		m_pos.x = 0;
	}
	else if (m_pos.x < 0)
	{
		m_pos.x = m_screenDimensions.x;
	}

	if (m_pos.y > m_screenDimensions.y)
	{
		m_pos.y = 0;
	}
	else if (m_pos.y < 0)
	{
		m_pos.y = m_screenDimensions.y;
	}
}

//Linear interpolation functions to reduce jitteriness of movement
//The lower lerpStrength is, the smoother the movement will be
float Boid::Lerp(float currentValue, float targetValue, float lerpStrength)
{
	return currentValue + lerpStrength * (targetValue - currentValue);
}

Vector2D Boid::Lerp(Vector2D currentValue, Vector2D targetValue, float lerpStrength)
{
	return Vector2D(Lerp(currentValue.x, targetValue.x, lerpStrength), Lerp(currentValue.y, targetValue.y, lerpStrength));
}

