#pragma once
#include "Header.h"

class Ball
{
private:

public:
	vf2d position;
	vf2d velocity;
	float angle;
	float angularVelocity;

	float radius;
	float halfLength;
	float density;
	float elasticity;
	float friction;
	Pixel color;

	float inverseMass;
	float inverseInertia;
	vf2d normal;

	Ball() :
		position(0, 0), velocity(0, 0), angle(0), angularVelocity(0),
		radius(10), halfLength(10), density(1), elasticity(1), friction(1), color(WHITE),
		inverseMass(1.0f / (density * (PI * radius * radius + 4.0f * radius * halfLength))),
		inverseInertia(1.0f / (density * (
			(1.0 / 12.0) * 4.0f * radius * halfLength * halfLength * halfLength +
			(1.0 / 2.0) * PI * radius * radius * radius * radius +
			(1.0 / 4.0) * PI * radius * radius * halfLength * halfLength
			))),
		normal(cos(angle), sin(angle)) {};

	Ball(vf2d position, vf2d velocity, float angle, float angularVelocity, float radius, float halfLength, float density, float elasticity, float friction, Pixel color) :
		position(position), velocity(velocity), angle(angle), angularVelocity(angularVelocity),
		radius(radius), halfLength(halfLength), density(density), elasticity(elasticity), friction(friction), color(color),
		inverseMass(1.0f / (density * (PI * radius * radius + 4.0f * radius * halfLength))),
		inverseInertia(1.0f / (density * (
			(1.0 / 12.0) * 4.0f * radius * halfLength * halfLength * halfLength +
			(1.0 / 2.0) * PI * radius * radius * radius * radius +
			(1.0 / 4.0) * PI * radius * radius * halfLength * halfLength
			))),
		normal(cos(angle), sin(angle)) {};

	void SetPosition(vf2d position) { this->position = position; }
	void SetVelocity(vf2d velocity) { this->velocity = velocity; }
	void SetAngle(float angle) { this->angle = angle; normal = vf2d(cos(angle), sin(angle)); }
	void SetAngleVelocity(float angularVelocity) { this->angularVelocity = angularVelocity; }
	void SetRadius(float radius) {
		this->radius = radius;
		inverseMass = 1.0f / (density * (PI * radius * radius + 4.0f * radius * halfLength));
		inverseInertia = 1.0f / (density * (
			(1.0 / 12.0) * 4.0f * radius * halfLength * halfLength * halfLength +
			(1.0 / 2.0) * PI * radius * radius * radius * radius +
			(1.0 / 4.0) * PI * radius * radius * halfLength * halfLength
			));
	}
	void SetHalfLength(float halfLength) {
		this->halfLength = halfLength;
		inverseMass = 1.0f / (density * (PI * radius * radius + 4.0f * radius * halfLength));
		inverseInertia = 1.0f / (density * (
			(1.0 / 12.0) * 4.0f * radius * halfLength * halfLength * halfLength +
			(1.0 / 2.0) * PI * radius * radius * radius * radius +
			(1.0 / 4.0) * PI * radius * radius * halfLength * halfLength
			));
	}
	void SetDensity(float density) {
		this->density = density;
		inverseMass = 1.0f / (density * (PI * radius * radius + 4.0f * radius * halfLength));
		inverseInertia = 1.0f / (density * (
			(1.0 / 12.0) * 4.0f * radius * halfLength * halfLength * halfLength +
			(1.0 / 2.0) * PI * radius * radius * radius * radius +
			(1.0 / 4.0) * PI * radius * radius * halfLength * halfLength
			));
	}
	void SetElasticity(float elasticity) { this->elasticity = elasticity; }
	void SetFriction(float friction) { this->friction = friction; }
	void SetColor(Pixel color) { this->color = color; }
	//void SetInverseMass(float inverseMass) { this->inverseMass = inverseMass; this->inverseInertia = 2.0f * inverseMass / (radius * radius); }

	void ApplyForce(vf2d force) { velocity += force * inverseMass; }
	void ApplyTorque(float torque) { angularVelocity += torque * inverseInertia; }
	void ApplyAcceleration(vf2d acceleration, float dt) { velocity += acceleration * dt; }
	void ApplyAngularAcceleration(float angularAcceleration, float dt) { angularVelocity += angularAcceleration * dt; }

	void Update(float dt) { position += velocity * dt; angle += angularVelocity * dt; normal = vf2d(cos(angle), sin(angle)); }
};