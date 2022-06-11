#pragma once
#include "Header.h"

class Ball
{
private:
	vf2d position;
	vf2d velocity;

	float angle;
	float angularVelocity;

	float thickness;

	float density;
	float elasticity;
	float friction;
	Pixel color;

	float inverseMass;
	float inverseInertia;

	vf2d normal;

public:
	Ball() :
		position(0.0f, 0.0f),
		velocity(0.0f, 0.0f),
		angle(0.0f),
		angularVelocity(0.0f),
		thickness(10.0f),
		density(1.0f),
		elasticity(0.8f),
		friction(0.8f),
		color(WHITE),
		inverseMass(1.0f / (density * thickness * thickness * PI)),
		inverseInertia(2.0f * inverseMass / (thickness * thickness)),
		normal(cos(angle), sin(angle)) {};

	Ball(
		vf2d position,
		vf2d velocity,
		float angle,
		float angularVelocity,
		float thickness,
		float density,
		float elasticity,
		float friction,
		Pixel color) :
		position(position),
		velocity(velocity),
		angle(angle),
		angularVelocity(angularVelocity),
		thickness(thickness),
		density(density),
		elasticity(elasticity),
		friction(friction),
		color(color),
		inverseMass(1.0f / (density * thickness * thickness * PI)),
		inverseInertia(2.0f * inverseMass / (thickness * thickness)),
		normal(cos(angle), sin(angle)) {};

	vf2d GetPosition() const { return position; };
	vf2d GetVelocity() const { return velocity; };
	float GetAngle() const { return angle; };
	float GetAngularVelocity() const { return angularVelocity; };
	float GetThickness() const { return thickness; };
	float GetDensity() const { return density; };
	float GetElasticity() const { return elasticity; };
	float GetFriction() const { return friction; };
	Pixel GetColor() const { return color; };
	float GetInverseMass() const { return inverseMass; };
	float GetInverseInertia() const { return inverseInertia; };
	vf2d GetNormal() const { return normal; };

	void SetPosition(vf2d positon);
	void SetVelocity(vf2d velocity);
	void SetAngle(float angle);
	void SetAngularVelocity(float angularVelocity);
	void SetThickness(float thickness);
	void SetDensity(float density);
	void SetElasticity(float elasticity);
	void SetFriction(float friction);
	void SetColor(Pixel color);

	void ApplyForce(vf2d force);
	void ApplyAngularForce(float angularForce);
	void ApplyAcceleration(vf2d acceleration, float dt);
	void ApplyAngularAcceleration(float angularAcceleration, float dt);
	void Update(float dt);
};

void Ball::SetPosition(vf2d positon) { this->position = positon; }
void Ball::SetVelocity(vf2d velocity) { this->velocity = velocity; }
void Ball::SetAngle(float angle)
{
	this->angle = angle;
	normal = vf2d(cos(angle), sin(angle));
}
void Ball::SetAngularVelocity(float angularVelocity) { this->angularVelocity = angularVelocity; }
void Ball::SetThickness(float thickness)
{
	this->thickness = thickness;
	inverseMass = 1.0f / (density * thickness * thickness * PI);
	inverseInertia = 2.0f * inverseMass / (thickness * thickness);
}
void Ball::SetDensity(float density)
{
	this->density = density;
	inverseMass = 1.0f / (density * thickness * thickness * PI);
	inverseInertia = 2.0f * inverseMass / (thickness * thickness);
}
void Ball::SetElasticity(float elasticity) { this->elasticity = elasticity; }
void Ball::SetFriction(float friction) { this->friction = friction; }
void Ball::SetColor(Pixel color) { this->color = color; }

void Ball::ApplyForce(vf2d force) { velocity += force * inverseMass; }
void Ball::ApplyAngularForce(float angularForce) { angularVelocity += angularForce * inverseInertia; }
void Ball::ApplyAcceleration(vf2d acceleration, float dt) { velocity += acceleration * dt; }
void Ball::ApplyAngularAcceleration(float angularAcceleration, float dt) { angularVelocity += angularAcceleration * dt; }
void Ball::Update(float dt)
{
	position += velocity * dt;
	angle += angularVelocity * dt;
	normal = vf2d(cos(angle), sin(angle));
}