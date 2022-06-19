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
		//cout << "forces.size() = " << forces.size() << endl;
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
		//cout << "torques.size() = " << torques.size() << endl;
		torques.clear();
	}

	/*cout << "Velocity1: " << velocity << endl;
	cout << "Angular Velocity1: " << angularVelocity << endl;
	cout << "avgForces: " << avgForces << endl;
	cout << "avgTorques: " << avgTorques << endl;
	cout << "inverseMass: " << inverseMass << endl;
	cout << "inverseInertia: " << inverseInertia << endl;*/

	velocity += avgForces * inverseMass;
	angularVelocity += avgTorques * inverseInertia;
	position += velocity * dt;
	angle += angularVelocity * dt;
	normal = vf2d(cos(angle), sin(angle));

	/*cout << "Velocity2: " << velocity << endl;
	cout << "Angular Velocity2: " << angularVelocity << endl;*/
}