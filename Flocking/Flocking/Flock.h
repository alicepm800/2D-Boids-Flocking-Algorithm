#pragma once
#include "Boid.h"
#include "Play.h"

enum FlockAttributes
{
	COHESION = 0,
	ALIGNMENT,
	SEPARATION
};

class Flock
{
public:

	//Add a boid to this flock
	void AddBoid(const Boid& boid);

	//Samples a feature (e.g. velocity) of all boids that surround a particular feature
	Vector2D SampleSurroundingBoids(FlockAttributes attribute, int boidIndex);

	//Find the average of a feature of group of boids e.g. velocity, control how quickly the boids flock together, and set speed of flock
	Vector2D SetFlockSpecifications(Vector2D sum, float numberOfBoidsSampled, FlockAttributes attribute, int boidIndex);

	//Steer the boid towards the average velocity of the neighboring boids
	Vector2D Alignment(int boidIndex);

	//Steer the boid towards the average position of the neighboring boids 
	Vector2D Cohesion(int boidIndex);
	
	//Steer the boid to avoid crowding with neighboring boids (e.g. so boids do not overlap with each other)
	Vector2D Separation(int boidIndex);

	//Update flock
	void UpdateFlock();

	//Normalise
	void Normalise(Vector2D &vector);

	//Find length of a vector
	float Magnitude(Vector2D &vector);

	//Retrieve a flock
	std::vector<Boid>GetFlock() const{ return m_vFlock; }

private:
	std::vector<Boid>m_vFlock;
	//m_maxForce represents how much you want the boids to align
	float m_maxFlockingForce{ 1.5f };

	float m_flockSpeed{ 3.5f };

	//m_boidMaxPerception represents how far a boid can perceive neighboring boids
	//A boid is not neighboring if its distance from a boid exceeds boidMaxPerception
	float m_boidMaxPerception{ 75.0f };
};