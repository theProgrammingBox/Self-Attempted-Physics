#define OLC_PGE_APPLICATION
#include "Ball.h"

class Example : public olc::PixelGameEngine
{
private:
	Random random;
	vector<Ball> balls;
	Ball* selectedBall = nullptr;
	bool paused;

	struct collision
	{
		Ball& ball1;
		Ball& ball2;
	};

	Pixel mapToRainbow(float d) { // 0 - 1
		d *= 6.0;
		const float r = (d > 4.0f) ? max(0.0f, min(1.0f, 6.0f - d)) : max(0.0f, min(1.0f, d - 2.0f));
		const float g = (d > 2.0f) ? max(0.0f, min(1.0f, 4.0f - d)) : max(0.0f, min(1.0f, d));
		const float b = (d > 3.0f) ? max(0.0f, min(1.0f, d - 4.0f)) : max(0.0f, min(1.0f, 2.0f - d));

		return Pixel(r * 0xff, g * 0xff, b * 0xff);
	}

	void Render()
	{
		Clear(olc::BLACK);
		DrawString(10, 10, "Ball Count: " + std::to_string(balls.size()), olc::WHITE);

		for (Ball& ball : balls)
		{
			DrawCircle(ball.position, ball.radius, ball.color);
			DrawLine(ball.position, ball.position + ball.normal * ball.radius, ball.color);
			//DrawLine(ball.position, ball.position + ball.velocity, ball.color);
			//DrawLine(GetWindowSize() / 2.0f, ball.position, ball.color);
		}
	}

	void Controls(float dt)
	{
		if (GetMouse(0).bPressed || GetMouse(1).bPressed && balls.size() != 0)
		{
			selectedBall = nullptr;
			float closest = INFINITY;
			float distanceSquared;
			float thicknessSquared;
			for (Ball& ball : balls)
			{
				distanceSquared = (ball.position - GetMousePos()).mag2();
				thicknessSquared = ball.radius * ball.radius;
				if (distanceSquared < closest && distanceSquared < thicknessSquared)
				{
					closest = distanceSquared;
					selectedBall = &ball;
				}
			}
		}
		if (GetMouse(0).bHeld)
		{
			if (selectedBall != NULL)
			{
				selectedBall->SetVelocity((-selectedBall->position + GetMousePos()) / dt);
			}
		}
		if (GetMouse(0).bReleased)
		{
			if (selectedBall != NULL)
			{
				selectedBall = nullptr;
			}
		}
		if (GetMouse(1).bHeld)
		{
			if (selectedBall != NULL)
			{
				DrawLine(selectedBall->position, GetMousePos(), selectedBall->color);
				DrawLine(selectedBall->position, GetMousePos(), selectedBall->color);
			}
		}
		if (GetMouse(1).bReleased)
		{
			if (selectedBall != NULL)
			{
				selectedBall->SetVelocity((-selectedBall->position + GetMousePos()) / (1000 * dt));
				selectedBall = NULL;
			}
		}
		if (GetKey(olc::Key::B).bPressed)
		{
			Ball ball;
			ball.SetPosition(GetMousePos());
			ball.SetRadius(random.UDoubleRandom() * 6 + 4);
			ball.SetElasticity(random.UDoubleRandom());
			ball.SetFriction(random.UDoubleRandom());
			ball.SetColor(mapToRainbow(random.UDoubleRandom()));
			balls.push_back(ball);
		}
		if (GetKey(olc::Key::R).bPressed)
		{
			balls.clear();
			selectedBall = nullptr;
		}
		if (GetKey(olc::Key::SPACE).bPressed)
		{
			paused = !paused;
		}
	}

	void ApplyAccelerations(float dt)
	{
		for (Ball& ball : balls)
		{
			ball.ApplyAcceleration((GetWindowSize() / 2.0f - ball.position) * 10, dt);
			//ball.ApplyAngularAcceleration(0, dt);
		}
	}

	void Update(float dt)
	{
		for (Ball& ball : balls)
		{
			ball.Update(dt);
		}
	}

	void CollideBalls(Ball& ball1, Ball& ball2)
	{
		float elasticity = (ball1.elasticity + ball2.elasticity) * 0.5f + 1.0f;
		float friction = ball1.friction * ball2.friction;

		vf2d collisionNormal = (ball1.position - ball2.position).norm();
		vf2d collisionTangent = collisionNormal.perp();

		vf2d pointOfContact1 = ball1.position - collisionNormal * ball1.radius;
		vf2d pointOfContact2 = ball2.position + collisionNormal * ball2.radius;

		vf2d radiusVector1 = pointOfContact1 - ball1.position;
		vf2d radiusVector2 = pointOfContact2 - ball2.position;

		vf2d vel1 = ball1.velocity + radiusVector1.perp() * ball1.angularVelocity;
		vf2d vel2 = ball2.velocity + radiusVector2.perp() * ball2.angularVelocity;
		vf2d relativeVel = vel1 - vel2;

		float normalCrossProduct1 = collisionNormal.cross(radiusVector1);
		float normalCrossProduct2 = collisionNormal.cross(radiusVector2);
		float normalImpulse = collisionNormal.dot(relativeVel) / (
			(ball1.inverseMass + ball2.inverseMass) +
			(normalCrossProduct1 * normalCrossProduct1 * ball1.inverseInertia) +
			(normalCrossProduct2 * normalCrossProduct2 * ball2.inverseInertia));

		float frictionCrossProduct1 = collisionTangent.cross(radiusVector1);
		float frictionCrossProduct2 = collisionTangent.cross(radiusVector2);
		float frictionImpulse = collisionTangent.dot(relativeVel) / (
			(ball1.inverseMass + ball2.inverseMass) +
			(frictionCrossProduct1 * frictionCrossProduct1 * ball1.inverseInertia) +
			(frictionCrossProduct2 * frictionCrossProduct2 * ball2.inverseInertia));

		if (fabs(frictionImpulse) > fabs(normalImpulse * friction))
		{
			frictionImpulse = (frictionImpulse > 0 ? normalImpulse : -normalImpulse) * friction;
		}

		vf2d impulse = (collisionNormal * normalImpulse - collisionTangent * frictionImpulse) * elasticity;
		ball1.ApplyForce(-impulse);
		ball2.ApplyForce(impulse);
		ball1.ApplyTorque(radiusVector1.cross(impulse));
		ball2.ApplyTorque(radiusVector2.cross(-impulse));
	}

	void StimulateTimestep(float dt)
	{
		float remainingDt = dt;
		while (remainingDt != 0.0f)
		{
			Update(remainingDt);
			ApplyAccelerations(remainingDt);
			float dtGlobalOffset = 0.0f;
			Ball* ball1 = nullptr;
			Ball* ball2 = nullptr;
			vector<collision> collisions;
			for (int i = 0; i < balls.size(); i++)
			{
				for (int j = i + 1; j < balls.size(); j++)
				{
					vf2d dPos = balls[i].position - balls[j].position;
					float distanceSquared = dPos.mag2();
					float totalRadius = balls[i].radius + balls[j].radius;
					float iffyOverlap = distanceSquared - totalRadius * totalRadius;
					if (iffyOverlap < 0)
					{
						vf2d dVel = balls[i].velocity - balls[j].velocity;
						float a = dVel.mag2();
						if (a != 0)
						{
							float b = dPos.dot(dVel);
							if (b < 0)
							{
								float dtOffset = (-sqrt(b * b - a * iffyOverlap) - b) / a;
								if (dtOffset > -remainingDt)
								{
									if (dtOffset < dtGlobalOffset)
									{
										dtGlobalOffset = dtOffset;
										ball1 = &balls[i];
										ball2 = &balls[j];
									}
								}
								else
								{
									collisions.push_back(collision{ balls[i], balls[j] });
								}
							}
						}
					}
				}
			}
			Update(dtGlobalOffset);
			ApplyAccelerations(dtGlobalOffset);
			if (ball1 != nullptr)
			{
				CollideBalls(*ball1, *ball2);
			}
			for (collision& c : collisions)
			{
				CollideBalls(c.ball1, c.ball2);
			}
			remainingDt = -dtGlobalOffset;
		}
	}

public:
	Example()
	{
		sAppName = "Physics";
	}

	bool OnUserCreate() override
	{
		paused = false;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Render();
		Controls(0.001);
		if (!paused)
		{
			StimulateTimestep(0.001);
		}

		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(1000, 1000, 1, 1))
		demo.Start();

	return 0;
}