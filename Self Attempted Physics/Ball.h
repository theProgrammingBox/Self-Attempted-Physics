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
	float density;
	float elasticity;
	float friction;
	Pixel color;
	float inverseMass;
	float inverseInertia;
	vf2d normal;
	vector<vf2d> forces;
	vector<float> torques;
	vector<vf2d> displacements;

	Ball() : position(0, 0), velocity(0, 0), angle(0), angularVelocity(0), radius(10), density(1), elasticity(1), friction(1), color(WHITE),
		inverseMass(1 / (density * PI * radius * radius)), inverseInertia(2.0f * inverseMass / (radius * radius)), normal(cos(angle), sin(angle)), forces(0), torques(0), displacements(0) {}

	Ball(vf2d position, vf2d velocity, float angle, float angularVelocity, float radius, float density, float elasticity, float friction, Pixel color) :
		position(position), velocity(velocity), angle(angle), angularVelocity(angularVelocity), radius(radius), density(density), elasticity(elasticity), friction(friction), color(color),
		inverseMass(1 / (density * PI * radius * radius)), inverseInertia(2.0f * inverseMass / (radius * radius)), normal(cos(angle), sin(angle)), forces(0), torques(0), displacements(0) {}

	void SetPosition(vf2d position) { this->position = position; }
	void SetVelocity(vf2d velocity) { this->velocity = velocity; }
	void SetAngle(float angle) { this->angle = angle; normal = vf2d(cos(angle), sin(angle)); }
	void SetAngleVelocity(float angularVelocity) { this->angularVelocity = angularVelocity; }
	void SetRadius(float radius) { this->radius = radius; inverseMass = 1 / (density * PI * radius * radius); inverseInertia = 2.0f * inverseMass / (radius * radius); }
	void SetDensity(float density) { this->density = density; inverseMass = 1 / (density * PI * radius * radius); inverseInertia = 2.0f * inverseMass / (radius * radius); }
	void SetElasticity(float elasticity) { this->elasticity = elasticity; }
	void SetFriction(float friction) { this->friction = friction; }
	void SetColor(Pixel color) { this->color = color; }
	void SetInverseMass(float inverseMass) { this->inverseMass = inverseMass; this->inverseInertia = 2.0f * inverseMass / (radius * radius); }
	void AddForce(vf2d force) { forces.push_back(force); }
	void AddTorque(float torque) { torques.push_back(torque); }
	void AddDisplacement(vf2d displacement) { displacements.push_back(displacement); }

	void UpdateOverlaps();
	void Update(float dt, vf2d pos);
};