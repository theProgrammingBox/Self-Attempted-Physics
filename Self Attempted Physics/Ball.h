#pragma once
#include "Header.h"

class Ball
{
private:
	vf2d position;
	vf2d velocity;
	vf2d acceleration;
	vf2d force;

	double angle;
	double angularVelocity;
	double angularAcceleration;
	double angularForce;

	vf2d savedPosition;
	vf2d savedVelocity;
	vf2d accumulatedvelocity;

	double savedAngle;
	double savedAngularVelocity;
	double accumulatedAngularVelocity;

	double radius;
	double density;
	double elasticity;
	double friction;
	Pixel color;

	double inverseMass;
	double inverseInertia;

public:
	Ball();

	void SetPosition(vf2d positon);
	void SetVelocity(vf2d velocity);
	void SetAngle(double angle);
	void SetAngularVelocity(double angularVelocity);
	void SetRadius(double radius);
	void SetDensity(double density);
	void SetElasticity(double elasticity);
	void SetFriction(double friction);
	void SetColor(Pixel color);

	void ApplyForce(vf2d force);
	void ApplyAngularForce(double angularForce);
	void ApplyAcceleration(vf2d acceleration);
	void ApplyAngularAcceleration(double angularAcceleration);
	void Update(double dt);
	void SaveState();
	void RestoreState();

	vf2d GetPosition() const;
	vf2d GetVelocity() const;
	double GetAngle() const;
	double GetAngularVelocity() const;
	double GetRadius() const;
	double GetFriction() const;
	double GetElasticity() const;
	Pixel GetColor() const;
	double GetInverseMass() const;
	double GetInverseInertia() const;
	double GetEnergy() const;
};

Ball::Ball()
{
	position = vf2d(0, 0);
	velocity = vf2d(0, 0);
	acceleration = vf2d(0, 0);
	force = vf2d(0, 0);
	savedPosition = vf2d(0, 0);
	savedVelocity = vf2d(0, 0);
	accumulatedvelocity = vf2d(0, 0);

	angle = 0;
	angularVelocity = 0;
	angularAcceleration = 0;
	angularForce = 0;
	savedAngle = 0;
	savedAngularVelocity = 0;
	accumulatedAngularVelocity = 0;

	radius = 10;
	density = 1;
	elasticity = 1;
	friction = 1;
	color = WHITE;
	inverseMass = 0.00318309886184f;
	inverseInertia = 0.000127323954474f;
}

void Ball::SetPosition(vf2d positon)
{
	position = positon;
}

void Ball::SetVelocity(vf2d velocity)
{
	this->velocity = velocity;
}

void Ball::SetAngle(double angle)
{
	this->angle = angle;
}

void Ball::SetAngularVelocity(double angularVelocity)
{
	this->angularVelocity = angularVelocity;
}

void Ball::SetRadius(double radius) // > 0
{
	this->radius = radius;
	inverseMass = 1.0f / (3.14159265359 * radius * radius * density);
	inverseInertia = 1.0f / (0.785398163397 * radius * radius * radius * radius * density);
}

void Ball::SetDensity(double density) // > 0
{
	this->density = density;
	inverseMass = 1.0f / (3.14159265359 * radius * radius * density);
	inverseInertia = 1.0f / (0.785398163397 * radius * radius * radius * radius * density);
}

void Ball::SetElasticity(double elasticity) // 0 - 1
{
	this->elasticity = elasticity;
}

void Ball::SetFriction(double friction) // 0 - 1
{
	this->friction = friction;
}

void Ball::SetColor(Pixel color)
{
	this->color = color;
}

void Ball::ApplyForce(vf2d force)
{
	//this->force += force;
	velocity += force * inverseMass;
}

void Ball::ApplyAngularForce(double angularForce)
{
	//this->angularForce += angularForce;
	angularVelocity += angularForce * inverseInertia;
}

void Ball::ApplyAcceleration(vf2d acceleration)
{
	this->acceleration += acceleration;
}

void Ball::ApplyAngularAcceleration(double angularAcceleration)
{
	this->angularAcceleration += angularAcceleration;
}

void Ball::Update(double dt)
{
	velocity += acceleration * dt;
	position += velocity * dt;
	acceleration = vf2d(0, 0);
	force = vf2d(0, 0);

	angularVelocity += angularAcceleration * dt;
	angle += angularVelocity * dt;
	angle = angle > 6.28318530718 ? angle - 6.28318530718 : (angle < 0 ? angle + 6.28318530718 : angle);
	angularAcceleration = 0;
	angularForce = 0;
}

void Ball::SaveState()
{
	savedPosition = position;
	savedVelocity = velocity;
	accumulatedvelocity = vf2d(0, 0);
	savedAngle = angle;
	savedAngularVelocity = angularVelocity;
	accumulatedAngularVelocity = 0;
}

void Ball::RestoreState()
{
	velocity = accumulatedvelocity / 6.0f;
	position = savedPosition + velocity;
	angularVelocity = accumulatedAngularVelocity / 6.0f;
	angle = savedAngle + angularVelocity;
}

vf2d Ball::GetPosition() const
{
	return position;
}

vf2d Ball::GetVelocity() const
{
	return velocity;
}

double Ball::GetAngle() const
{
	return angle;
}

double Ball::GetAngularVelocity() const
{
	return angularVelocity;
}

double Ball::GetRadius() const
{
	return radius;
}

double Ball::GetFriction() const
{
	return friction;
}

double Ball::GetElasticity() const
{
	return elasticity;
}

Pixel Ball::GetColor() const
{
	return color;
}

double Ball::GetInverseMass() const
{
	return inverseMass;
}

double Ball::GetInverseInertia() const
{
	return inverseInertia;
}

double Ball::GetEnergy() const
{
	return
		(0.5f / inverseMass * velocity.mag2()) +
		(0.5f / inverseInertia * angularVelocity * angularVelocity);
}