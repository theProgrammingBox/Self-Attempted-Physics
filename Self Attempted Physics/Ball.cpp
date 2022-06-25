#include "Ball.h"

void Ball::UpdateOverlaps()
{
	vf2d avgDisplacements = vf2d(0, 0);
	if (displacements.size() != 0)
	{
		vf2d sumDisplacements = vf2d(0, 0);
		for (vf2d displacement : displacements)
		{
			sumDisplacements += displacement;
		}
		avgDisplacements = sumDisplacements / displacements.size();
		displacements.clear();
	}

	position += avgDisplacements * inverseMass;
}

void Ball::Update(float dt)
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
	position += velocity * dt;
	angle += angularVelocity * dt;

	velocity += acceleration * dt;
	angularVelocity += angularAcceleration * dt;

	normal = vf2d(cos(angle), sin(angle));
	acceleration = vf2d(0, 0);
	angularAcceleration = 0;
}