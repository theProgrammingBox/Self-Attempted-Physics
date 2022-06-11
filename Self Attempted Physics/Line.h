#pragma once
#include "Header.h"

class Line
{
private:
	vf2d position;
	vf2d velocity;

	float angle;
	float angularVelocity;

	float thickness;
	float length;

	float density;
	float elasticity;
	float friction;
	Pixel color;

	float inverseMass;
	float inverseInertia;

	vf2d normal;
	vf2d point1;
	vf2d point2;

public:
	Line();

	void SetPosition(vf2d positon);
	void SetVelocity(vf2d velocity);
	void SetAngle(float angle);
	void SetAngularVelocity(float angularVelocity);
	void SetThickness(float thickness);
	void SetLength(float length);
	void SetDensity(float density);
	void SetElasticity(float elasticity);
	void SetFriction(float friction);
	void SetColor(Pixel color);

	void ApplyForce(vf2d force);
	void ApplyAngularForce(float angularForce);
	void ApplyAcceleration(vf2d acceleration, float dt);
	void ApplyAngularAcceleration(float angularAcceleration, float dt);
	void Update(float dt);

	vf2d GetPosition() const;
	vf2d GetVelocity() const;
	float GetAngle() const;
	float GetAngularVelocity() const;
	float GetThickness() const;
	float GetLength() const;
	float GetFriction() const;
	float GetElasticity() const;
	Pixel GetColor() const;
	float GetInverseMass() const;
	float GetInverseInertia() const;
	vf2d GetNormal() const;
	vf2d GetPoint1() const;
	vf2d GetPoint2() const;
};