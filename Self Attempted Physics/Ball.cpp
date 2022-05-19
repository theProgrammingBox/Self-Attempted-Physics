#include "Ball.h"

Ball::Ball()
{
	positionallyStatic = true;
	position = vf2d(0.0f, 0.0f);
	positiontionalVelocity = vf2d(0.0f, 0.0f);
	positiontionalAcceleration = vf2d(0.0f, 0.0f);
	netForce = vf2d(0.0f, 0.0f);
	mass = 1.0f * 10.0f * 10.0f * 3.14159265359f;	// PI

	angularlyStatic = true;
	angle = 0.0f;
	angularVelocity = 0.0f;
	angularAcceleration = 0.0f;
	netTorque = 0.0f;
	inertia = 1.0f * 10.0f * 10.0f * 10.0f * 10.0f * 0.785398163397f;	// PI/4

	radius = 10.0f;
	density = 1.0f;
	elasticity = 1.0f;
	friction = 1.0f;
	color = Pixel(255, 255, 255, 255);
}

Ball::~Ball()
{
}

void Ball::SetPositionallyStatic(bool positionallyStatic)
{
	this->positionallyStatic = positionallyStatic;
	if (positionallyStatic)
	{
		positiontionalVelocity = vf2d(0.0f, 0.0f);
		positiontionalAcceleration = vf2d(0.0f, 0.0f);
	}
}

void Ball::SetPosition(vf2d position)
{
	this->position = position;
}

void Ball::SetPositiontionalVelocity(vf2d positiontionalVelocity)
{
	this->positiontionalVelocity = positiontionalVelocity;
}

void Ball::SetPositiontionalAcceleration(vf2d positiontionalAcceleration)
{
	this->positiontionalAcceleration = positiontionalAcceleration;
}

void Ball::SetNetForce(vf2d netForce)
{
	this->netForce = netForce;
}

void Ball::SetAngularlyStatic(bool angularlyStatic)
{
	this->angularlyStatic = angularlyStatic;
	if (angularlyStatic)
	{
		angularVelocity = 0.0f;
		angularAcceleration = 0.0f;
	}
}

void Ball::SetAngle(float angle)
{
	this->angle = angle;
}

void Ball::SetAngularVelocity(float angularVelocity)
{
	this->angularVelocity = angularVelocity;
}

void Ball::SetAngularAcceleration(float angularAcceleration)
{
	this->angularAcceleration = angularAcceleration;
}

void Ball::SetNetTorque(float netTorque)
{
	this->netTorque = netTorque;
}

void Ball::SetRadius(float radius)
{
	this->radius = radius;
	mass = density * radius * radius * 3.14159265359f;	// PI
	inertia = density * radius * radius * radius * radius * 0.785398163397f;	// PI/4
}

void Ball::SetDensity(float density)
{
	this->density = density;
	mass = density * radius * radius * 3.14159265359f;	// PI
	inertia = density * radius * radius * radius * radius * 0.785398163397f;	// PI/4
}

void Ball::SetElasticity(float elasticity)
{
	this->elasticity = elasticity;
}

void Ball::SetFriction(float friction)
{
	this->friction = friction;
}

void Ball::SetColor(Pixel color)
{
	this->color = color;
}

void Ball::TogglePositionallyStatic()
{
	SetPositionallyStatic(!positionallyStatic);
}

void Ball::ToggleAngularlyStatic()
{
	SetAngularlyStatic(!angularlyStatic);
}

void Ball::ApplyForce(vf2d force)
{
	netForce += force;
}

void Ball::ApplyTorque(float torque)
{
	netTorque += torque;
}

void Ball::Update(float deltaTime)
{
	if (!positionallyStatic)
	{
		positiontionalVelocity += positiontionalAcceleration * deltaTime;
		position += positiontionalVelocity * deltaTime;
	}

	if (!angularlyStatic)
	{
		angularVelocity += angularAcceleration * deltaTime;
		angle += angularVelocity * deltaTime;
	}
}