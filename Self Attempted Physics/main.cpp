#define OLC_PGE_APPLICATION
#include "Ball.h"

class Example : public olc::PixelGameEngine
{
private:
	const int INTERVALS = 10;
	Random random;
	vector<Ball> balls;
	Ball* selectedBall = nullptr;

	Pixel mapToRainbow(float d) { // 0 - 1
		d *= 6.0;
		const float r = (d > 4.0f) ? max(0.0f, min(1.0f, 6.0f - d)) : max(0.0f, min(1.0f, d - 2.0f));
		const float g = (d > 2.0f) ? max(0.0f, min(1.0f, 4.0f - d)) : max(0.0f, min(1.0f, d));
		const float b = (d > 3.0f) ? max(0.0f, min(1.0f, d - 4.0f)) : max(0.0f, min(1.0f, 2.0f - d));

		return Pixel(r * 0xff, g * 0xff, b * 0xff);
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
				//selectedBall->SetVelocity(vf2d(0, 0));
				selectedBall = nullptr;
			}
		}
		if (GetMouse(1).bHeld)
		{
			if (selectedBall != NULL)
			{
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
			//ball.SetRadius(random.UDoubleRandom() * 8 + 4);
			ball.SetRadius(10);
			ball.SetColor(mapToRainbow(random.UDoubleRandom()));
			balls.push_back(ball);
		}
		if (GetKey(olc::Key::R).bPressed)
		{
			balls.clear();
			selectedBall = nullptr;
		}
	}

	void SeperateAllBalls()
	{
		for (int iterations = 0; iterations < 1; iterations++)
		{
			for (int i = 0; i < balls.size(); i++)
			{
				for (int j = i + 1; j < balls.size(); j++)
				{
					vf2d dPos = balls[j].position - balls[i].position;
					float distanceSquared = dPos.mag2();
					float totalRadius = balls[i].radius + balls[j].radius;
					if (distanceSquared <= totalRadius * totalRadius)
					{
						vf2d collisionNormal;
						float distance = sqrt(distanceSquared);
						if (distance == 0.0f)
						{
							collisionNormal = vf2d(1.0f, 0.0f);
						}
						else
						{
							collisionNormal = dPos / distance;
						}
						float impulse = (distance - totalRadius) / (balls[i].inverseMass + balls[j].inverseMass);
						vf2d impulseVector = collisionNormal * impulse;
						balls[i].AddDisplacement(impulseVector);
						balls[j].AddDisplacement(-impulseVector);
					}
				}
			}
			UpdateOverlaps();
		}
	}

	void StimulateTimestep(float dt)
	{
		Update(dt);
		for (int i = 0; i < balls.size(); i++)
		{
			for (int j = i + 1; j < balls.size(); j++)
			{
				vf2d dPos = balls[j].position - balls[i].position;
				float distanceSquared = dPos.mag2();
				float totalRadius = balls[i].radius + balls[j].radius;
				if (distanceSquared <= totalRadius * totalRadius)
				{
					vf2d collisionNormal = dPos / sqrt(distanceSquared);
					vf2d tangentalNormal = collisionNormal.perp();
					vf2d dVel =
						(balls[j].velocity + balls[j].radius * tangentalNormal * balls[j].angularVelocity) -
						(balls[i].velocity + balls[i].radius * tangentalNormal * balls[i].angularVelocity);
					float normalVelocity = collisionNormal.dot(dVel);
					if (normalVelocity < 0)
					{
						float elasticity = (balls[i].elasticity + balls[j].elasticity) * 0.5f + 1.0f;
						double friction = balls[i].friction * balls[j].friction;
						double tangentalVelocity = tangentalNormal.dot(dVel);
						float collisionImpulse = normalVelocity / (balls[i].inverseMass + balls[j].inverseMass);
						double frictionImpulse = tangentalVelocity / (
							balls[i].inverseMass + balls[i].radius * balls[i].radius * balls[i].inverseInertia +
							balls[j].inverseMass + balls[j].radius * balls[j].radius * balls[j].inverseInertia);
						if (fabs(frictionImpulse) > fabs(collisionImpulse * friction))
						{
							frictionImpulse = (frictionImpulse > 0 ? collisionImpulse : -collisionImpulse) * friction;
						}
						vf2d totalForce = (collisionNormal * collisionImpulse + tangentalNormal * frictionImpulse) * elasticity;
						balls[i].AddForce(totalForce);
						balls[i].AddTorque(balls[i].radius * frictionImpulse * elasticity);
						balls[j].AddForce(-totalForce);
						balls[j].AddTorque(-balls[j].radius * frictionImpulse * elasticity);
					}
				}
			}
		}
	}

	void Render()
	{
		Clear(olc::BLACK);
		DrawString(10, 10, "Ball Count: " + std::to_string(balls.size()), olc::WHITE);

		for (Ball& ball : balls)
		{
			DrawCircle(ball.position, ball.radius, ball.color);
			DrawLine(ball.position, ball.position + ball.velocity, ball.color);
			DrawLine(ball.position, ball.position + ball.normal * ball.radius, ball.color);
			//DrawLine(GetWindowSize() / 2.0f, ball.position, ball.color);
		}
	}

	void UpdateOverlaps()
	{
		for (Ball& ball : balls)
		{
			ball.UpdateOverlaps();
		}
	}

	void Update(float dt)
	{
		for (Ball& ball : balls)
		{
			ball.Update(dt);
		}
	}

public:
	Example()
	{
		sAppName = "Physics";
	}

	bool OnUserCreate() override
	{
		balls.clear();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Render();
		Controls(fElapsedTime);
		for (int i = 0; i < INTERVALS; i++)
		{
			SeperateAllBalls();
			StimulateTimestep(fElapsedTime / INTERVALS);
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