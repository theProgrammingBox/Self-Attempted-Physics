#include "Line.h"

Line::Line()
{
	position = vf2d(0, 0);
	velocity = vf2d(0, 0);

	angle = 0;
	angularVelocity = 0;

	thickness = 10;
	length = 0;

	density = 1;
	elasticity = 1;
	friction = 1;
	color = WHITE;

	float massRectangle = 4.0 * length * thickness * density;
	float massCircle = PI * thickness * thickness * density;

	inverseMass = 1.0 / (massRectangle + massCircle);
	inverseInertia = 1.0 / (
		(1.0 / 12.0) * massRectangle * length * length +
		(1.0 / 2.0) * massCircle * thickness * thickness +
		(1.0 / 4.0) * massCircle * length * length
		);

	normal = vf2d(1, 0);
	point1 = position + normal * length;
	point2 = position - normal * length;
}

void Line::SetPosition(vf2d positon)
{
	position = positon;
}

void Line::SetVelocity(vf2d velocity)
{
	this->velocity = velocity;
}

void Line::SetAngle(float angle)
{
	this->angle = angle;
	normal = vf2d(cos(angle), sin(angle));
	point1 = position + normal * length;
	point2 = position - normal * length;
}

void Line::SetAngularVelocity(float angularVelocity)
{
	this->angularVelocity = angularVelocity;
}

void Line::SetThickness(float thickness)
{
	this->thickness = thickness;

	float massRectangle = 4.0 * length * thickness * density;
	float massCircle = PI * thickness * thickness * density;

	inverseMass = 1.0 / (massRectangle + massCircle);
	inverseInertia = 1.0 / (
		(1.0 / 12.0) * massRectangle * length * length +
		(1.0 / 2.0) * massCircle * thickness * thickness +
		(1.0 / 4.0) * massCircle * length * length
		);
}

void Line::SetLength(float length)
{
	this->length = length;

	float massRectangle = 4.0 * length * thickness * density;
	float massCircle = PI * thickness * thickness * density;

	inverseMass = 1.0 / (massRectangle + massCircle);
	inverseInertia = 1.0 / (
		(1.0 / 12.0) * massRectangle * length * length +
		(1.0 / 2.0) * massCircle * thickness * thickness +
		(1.0 / 4.0) * massCircle * length * length
		);
}

void Line::SetDensity(float density)
{
	this->density = density;

	float massRectangle = 4.0 * length * thickness * density;
	float massCircle = PI * thickness * thickness * density;

	inverseMass = 1.0 / (massRectangle + massCircle);
	inverseInertia = 1.0 / (
		(1.0 / 12.0) * massRectangle * length * length +
		(1.0 / 2.0) * massCircle * thickness * thickness +
		(1.0 / 4.0) * massCircle * length * length
		);
}

void Line::SetElasticity(float elasticity)
{
	this->elasticity = elasticity;
}

void Line::SetFriction(float friction)
{
	this->friction = friction;
}

void Line::SetColor(Pixel color)
{
	this->color = color;
}

void Line::ApplyForce(vf2d force)
{
	velocity += force * inverseMass;
}

void Line::ApplyAngularForce(float angularForce)
{
	angularVelocity += angularForce * inverseInertia;
}

void Line::ApplyAcceleration(vf2d acceleration, float dt)
{
	velocity += acceleration * dt;
}

void Line::ApplyAngularAcceleration(float angularAcceleration, float dt)
{
	angularVelocity += angularAcceleration * dt;
}

void Line::Update(float dt)
{
	position += velocity * dt;
	angle += angularVelocity * dt;
	normal = vf2d(cos(angle), sin(angle));
	point1 = position + normal * length;
	point2 = position - normal * length;
}

vf2d Line::GetPosition() const
{
	return position;
}

vf2d Line::GetVelocity() const
{
	return velocity;
}

float Line::GetAngle() const
{
	return angle;
}

float Line::GetAngularVelocity() const
{
	return angularVelocity;
}

float Line::GetThickness() const
{
	return thickness;
}

float Line::GetLength() const
{
	return length;
}

float Line::GetFriction() const
{
	return friction;
}

float Line::GetElasticity() const
{
	return elasticity;
}

Pixel Line::GetColor() const
{
	return color;
}

float Line::GetInverseMass() const
{
	return inverseMass;
}

float Line::GetInverseInertia() const
{
	return inverseInertia;
}

vf2d Line::GetNormal() const
{
	return normal;
}

vf2d Line::GetPoint1() const
{
	return point1;
}

vf2d Line::GetPoint2() const
{
	return point2;
}