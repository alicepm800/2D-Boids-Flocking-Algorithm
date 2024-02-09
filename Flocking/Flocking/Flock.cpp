#include "Flock.h"

#define PI 3.141592635

void Flock::AddBoid(const Boid& boid)
{
	m_vFlock.push_back(boid);
}

void Flock::UpdateFlock()
{
	Vector2D boidAlignment = Vector2D(0.0f,0.0f);
	Vector2D boidCohesion = Vector2D(0.0f, 0.0f);
	Vector2D boidSeparation = Vector2D(0.0f, 0.0f);
	
	for (int i = 0; i < m_vFlock.size(); i++)
	{
		//Steer the boid towards the average velocity and position of its neighboring boids
		boidAlignment = Alignment(i);
		boidCohesion = Cohesion(i);
		boidSeparation = Separation(i);
		
		m_vFlock[i].m_acceleration += boidAlignment;
		float cohesionWeakener = 1.5f;
		m_vFlock[i].m_acceleration += boidCohesion / cohesionWeakener;
		m_vFlock[i].m_acceleration += boidSeparation;
		
		//Allow boids to detect screen edges so they do not disappear
		m_vFlock[i].BoidEdgeDetection();

		m_vFlock[i].RotateBoid();

		m_vFlock[i].UpdateBoid();
		
		PlayGraphics::Instance().DrawRotated(m_vFlock[i].m_spriteId, m_vFlock[i].m_pos, 0.0f, m_vFlock[i].m_rotation);
	}
}

Vector2D Flock::Alignment(int boidIndex)
{
	//averageVelocity represents the velocity I want to steer this boid towards
	Vector2D averageVelocity = Vector2D(0.0f, 0.0f);

	//Find the average velocity of neighboring boids
	averageVelocity = SampleSurroundingBoids(ALIGNMENT, boidIndex);

	return averageVelocity;
}

Vector2D Flock::Cohesion(int boidIndex)
{
	//averagePosition represents the position I want to steer this boid towards
	Vector2D averagePosition = Vector2D(0.0f, 0.0f);

	//Find the average position of neighboring boids
	averagePosition = SampleSurroundingBoids(COHESION, boidIndex);

	return averagePosition;
}

Vector2D Flock::Separation(int boidIndex)
{
	Vector2D average = Vector2D(0.0f, 0.0f);
	Vector2D sum = Vector2D(0.0f, 0.0f);
	int totalBoidsSampled = 0;
	float desiredSeparation = 20.0f;

	for (int i = 0; i < m_vFlock.size(); i++)
	{
		//Go through the whole flock except from the current boid
		if (boidIndex == i)
		{
			continue;
		}
		//Find the distance between this boid and the rest of the boids in the flock
		float distance = sqrt(pow(m_vFlock[boidIndex].m_pos.x - m_vFlock[i].m_pos.x, 2) + pow(m_vFlock[boidIndex].m_pos.y - m_vFlock[i].m_pos.y, 2));

		//If distance of other boid is within the current boid's perception, sample the other boid's attribute
		if ((distance > 0) && (distance < desiredSeparation))
		{
			Vector2D positionDifference = Vector2D(0, 0);
			//Find the difference between the positions of the current boid and another boid
			//Resulting vector goes from current boid to other boid
			positionDifference = m_vFlock[boidIndex].m_pos - m_vFlock[i].m_pos;
			//Normalise
			Normalise(positionDifference);
			//Make effect of separation stronger when distance between boids is smaller
			positionDifference /=  pow(distance, 2);

			//Sum up the separation forces required for neigboring boids 
			sum += positionDifference;
			
			totalBoidsSampled++;
		}
	}

	average = SetFlockSpecifications(sum, totalBoidsSampled, SEPARATION, boidIndex);
	return Vector2D(average.x, average.y);
}



Vector2D Flock::SampleSurroundingBoids(FlockAttributes attribute, int boidIndex)
{
	Vector2D average = Vector2D(0.0f, 0.0f);
	Vector2D sum = Vector2D(0.0f, 0.0f);
	int totalBoidsSampled = 0;
	float desiredSeparation = 20.0f;

	//Go through the entire flock to:
	//1) Find other boid's distances from the current boid
	//2) If other boid's distance is within the current boid's perception, their attribute is sampled to find the average for that attribute e.g. velocity
	for (int i = 0; i < m_vFlock.size(); i++)
	{
		//Go through the whole flock except from the current boid
		if (boidIndex == i)
		{
			continue;
		}
		//Find the distance between this boid and the rest of the boids in the flock
		float distance = sqrt(pow(m_vFlock[boidIndex].m_pos.x - m_vFlock[i].m_pos.x, 2) + pow(m_vFlock[boidIndex].m_pos.y - m_vFlock[i].m_pos.y, 2));

		//If distance of other boid is within the current boid's perception, sample the other boid's attribute
		if ((distance > 0) && (distance < m_boidMaxPerception))
		{
			//Set what attribute of the other boid you are sampling (e.g. velocity)
			Vector2D otherBoidAttribute;
			switch (attribute)
			{
			case(ALIGNMENT):
				otherBoidAttribute = m_vFlock[i].m_velocity;
				break;

			case(COHESION):
				otherBoidAttribute = m_vFlock[i].m_pos;
				break;	
			}
			sum += otherBoidAttribute;
			totalBoidsSampled++;
		}
	}
	average = SetFlockSpecifications(sum, totalBoidsSampled, attribute, boidIndex);
	return Vector2D(average.x, average.y);
}

Vector2D Flock::SetFlockSpecifications(Vector2D sum, float numberOfBoidsSampled, FlockAttributes attribute, int boidIndex)
{
	Vector2D average = Vector2D(0, 0);
	if (numberOfBoidsSampled > 0)
	{
		average = sum / numberOfBoidsSampled;

		if (attribute == COHESION)
		{
			average -= m_vFlock[boidIndex].m_pos;
		}

		//Normalise the vector so each boid can be set to the same speed (so each vector has a unit of 1 length)
		Normalise(average);

		//Set speed of flock
		average.x *= m_flockSpeed;
		average.y *= m_flockSpeed;

		average -= m_vFlock[boidIndex].m_velocity;

		float magnitude = Magnitude(average);
		//Control the speed of the flocking (how quickly do the boids join together into a flock?)
		//This also limits how quickly the flock can flock together
		if (magnitude > m_maxFlockingForce)
		{
			average = Vector2D(average.x / magnitude, average.y / magnitude);
		}
	}

	return Vector2D(average.x, average.y);
}

void Flock::Normalise(Vector2D &vector)
{
	//Find length of the vector
	float magnitude = Magnitude(vector);
	//Find the unit vector by dividing the vector by its length
	vector /= magnitude;
}

float Flock::Magnitude(Vector2D &vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}