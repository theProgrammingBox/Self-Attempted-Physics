#pragma once
#include "Header.h"

class Ball
{
private:
	bool positionallyStatic;
	vf2d position;
	vf2d positiontionalVelocity;
	vf2d positiontionalAcceleration;
	vf2d netForce;
	float mass;

	bool angularlyStatic;
	float angle;
	float angularVelocity;
	float angularAcceleration;
	float netTorque;
	float inertia;

	float radius;
	float density;
	float elasticity;
	float friction;
	Pixel color;

public:
	Ball();
	~Ball();

	void SetPositionallyStatic(bool notPositionallyStatic);
	void SetPosition(vf2d position);
	void SetPositiontionalVelocity(vf2d positiontionalVelocity);
	void SetPositiontionalAcceleration(vf2d positiontionalAcceleration);
	void SetNetForce(vf2d netForce);

	void SetAngularlyStatic(bool rotationalStatic);
	void SetAngle(float angle);
	void SetAngularVelocity(float angularVelocity);
	void SetAngularAcceleration(float angularAcceleration);
	void SetNetTorque(float netTorque);

	void SetRadius(float radius);
	void SetDensity(float density);
	void SetElasticity(float elasticity);
	void SetFriction(float friction);
	void SetColor(Pixel color);

	void TogglePositionallyStatic();
	void ToggleAngularlyStatic();

	void ApplyForce(vf2d force);
	void ApplyTorque(float torque);

	void Update(float deltaTime);
};