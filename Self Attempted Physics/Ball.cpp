#include "Ball.h"

void Ball::ApplyForces()
{
	vf2d avgForces = vf2d(0, 0);
	if (forces.size() != 0)
	{
		vf2d sumForces = vf2d(0, 0);
		for (vf2d force : forces)
		{
			sumForces += force;
		}
		avgForces = sumForces / forces.size();
		forces.clear();
	}
	float avgTorques = 0;
	if (torques.size() != 0)
	{
		float sumTorques = 0;
		for (float torque : torques)
		{
			sumTorques += torque;
		}
		avgTorques = sumTorques / torques.size();
		torques.clear();
	}
	velocity += avgForces * inverseMass;
	angularVelocity += avgTorques * inverseInertia;
}

void Ball::ApplyAccelerations(float dt)
{
	velocity += acceleration * dt;
	angularVelocity += angularAcceleration * dt;
	acceleration = vf2d(0, 0);
	angularAcceleration = 0;
}

void Ball::Update(float dt)
{
	/*position += velocity * dt;
	angle += angularVelocity * dt;
	normal = vf2d(cos(angle), sin(angle));*/

	vf2d avgForces = vf2d(0, 0);
	if (forces.size() != 0)
	{
		vf2d sumForces = vf2d(0, 0);
		for (vf2d force : forces)
		{
			sumForces += force;
		}
		avgForces = sumForces / forces.size();
		forces.clear();
	}

	float avgTorques = 0;
	if (torques.size() != 0)
	{
		float sumTorques = 0;
		for (float torque : torques)
		{
			sumTorques += torque;
		}
		avgTorques = sumTorques / torques.size();
		torques.clear();
	}

	velocity += avgForces * inverseMass;
	angularVelocity += avgTorques * inverseInertia;
	position += velocity * dt;
	angle += angularVelocity * dt;

	velocity += acceleration * dt;
	angularVelocity += angularAcceleration * dt;

	normal = vf2d(cos(angle), sin(angle));
	acceleration = vf2d(0, 0);
	angularAcceleration = 0;
}