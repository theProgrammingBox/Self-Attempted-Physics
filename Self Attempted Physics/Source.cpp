#define OLC_PGE_APPLICATION
#include "Ball.h"

class Example : public olc::PixelGameEngine
{
private:
	const float FPS = 1.0 / 60.0;
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

	void Controls()
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
				selectedBall->SetPosition(GetMousePos());
				selectedBall->SetVelocity(vf2d(0, 0));
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
			}
		}
		if (GetMouse(1).bReleased)
		{
			if (selectedBall != NULL)
			{
				selectedBall->SetVelocity((GetMousePos() - selectedBall->position) / (200 * FPS));
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
					vf2d dPos = balls[i].position - balls[j].position;
					float distanceSquared = dPos.mag2();
					float totalThickness = balls[i].radius + balls[j].radius;
					float iffyOverlap = distanceSquared - totalThickness * totalThickness;
					if (iffyOverlap <= 0.0f)
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
						float impulse = (distance - totalThickness) / (balls[i].inverseMass + balls[j].inverseMass);
						vf2d impulseVector = collisionNormal * impulse;
						balls[i].AddDisplacement(-impulseVector);
						balls[j].AddDisplacement(impulseVector);
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
				vf2d dPos = balls[i].position - balls[j].position;
				float distanceSquared = dPos.mag2();
				float totalThickness = balls[i].radius + balls[j].radius;
				if (distanceSquared <= totalThickness * totalThickness)
				{
					float elasticity = (balls[i].elasticity + balls[j].elasticity) * 0.5f + 1.0f;
					vf2d collisionNormal = dPos / sqrt(distanceSquared);
					vf2d dVel = balls[i].velocity - balls[j].velocity;
					float normalVelocity = dVel.dot(collisionNormal);
					float impulse = normalVelocity / (balls[i].inverseMass + balls[j].inverseMass);
					vf2d impulseVector = collisionNormal * impulse * elasticity;
					balls[i].AddForce(-impulseVector);
					balls[j].AddForce(impulseVector);
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
		Controls();
		SeperateAllBalls();
		StimulateTimestep(FPS);

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