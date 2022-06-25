#define OLC_PGE_APPLICATION
#include "Ball.h"

class Example : public olc::PixelGameEngine
{
private:
	Random random;
	vector<Ball> balls;
	Ball* selectedBall = nullptr;
	float elapsedTime;

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
			//IDK();
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
			ball.SetRadius(random.UDoubleRandom() * 8 + 10);
			ball.SetElasticity(random.UDoubleRandom());
			ball.SetColor(mapToRainbow(random.UDoubleRandom()));
			balls.push_back(ball);
		}
		if (GetKey(olc::Key::R).bPressed)
		{
			balls.clear();
			selectedBall = nullptr;
		}
		/*if (GetKey(olc::Key::UP).bHeld)
		{
			elapsedTime += 0.000001;
		}
		if (GetKey(olc::Key::DOWN).bHeld)
		{
			elapsedTime -= 0.000001;
		}*/
		//elapsedTime = (GetWindowSize().y / 2.0f - GetMousePos().y) * 0.000001;
	}

	/*void SeperateAllBalls()
	{
		for (int iterations = 0; iterations < 100; iterations++)
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

	void Collision()
	{
		for (int i = 0; i < balls.size(); i++)
		{
			for (int j = i + 1; j < balls.size(); j++)
			{
				vf2d dPos = balls[i].position - balls[j].position;
				float distanceSquared = dPos.mag2();
				float totalRadius = balls[i].radius + balls[j].radius;
				if (distanceSquared < totalRadius * totalRadius)
				{
					vf2d collisionNormal = dPos / sqrt(distanceSquared);
					vf2d radiusNormal1 = (-collisionNormal.perp()) * balls[i].radius;
					vf2d radiusNormal2 = collisionNormal.perp() * balls[j].radius;
					vf2d dVel =
						(balls[i].velocity + radiusNormal1 * balls[i].angularVelocity) -
						(balls[j].velocity + radiusNormal2 * balls[j].angularVelocity);
					float normalVelocity = collisionNormal.dot(dVel);
					if (normalVelocity < 0.0f)
					{
						float elasticity = (balls[i].elasticity + balls[j].elasticity) * 0.5f + 1.0f;
						float collisionImpulse = normalVelocity / (balls[i].inverseMass + balls[j].inverseMass);
						vf2d totalForce = collisionNormal * collisionImpulse * elasticity;
						balls[i].AddForce(-totalForce);
						balls[j].AddForce(totalForce);
					}
				}
			}
		}
	}

	void SaveState()
	{
		for (Ball& ball : balls)
		{
			ball.SaveState();
		}
	}

	void RestoreState()
	{
		for (Ball& ball : balls)
		{
			ball.RestoreState();
		}
	}*/

	void StimulateTimestep(float dt)
	{
		//SaveState();
		int ball1Index, ball2Index;
		float remainingDt = dt;
		while (remainingDt != 0.0f)
		{
			//RestoreState();
			ball1Index = -1;
			ball2Index = -1;
			Update(remainingDt);
			float dtGlobalOffset = 0.0f;
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
								if (dtOffset < dtGlobalOffset)
								{
									ball1Index = i;
									ball2Index = j;
									dtGlobalOffset = dtOffset < -dt ? -dt : dtOffset;
								}
							}
						}
					}
				}
			}
			Update(dtGlobalOffset);
			remainingDt = -dtGlobalOffset;
			if (ball1Index != -1)
			{
				vf2d dPos = balls[ball1Index].position - balls[ball2Index].position;
				vf2d collisionNormal = dPos.norm();
				vf2d dVel = balls[ball1Index].velocity - balls[ball2Index].velocity;
				float normalVelocity = collisionNormal.dot(dVel);
				float elasticity = (balls[ball1Index].elasticity + balls[ball2Index].elasticity) * 0.5f + 1.0f;
				float collisionImpulse = normalVelocity / (balls[ball1Index].inverseMass + balls[ball2Index].inverseMass);
				vf2d totalForce = collisionNormal * collisionImpulse * elasticity;
				balls[ball1Index].AddForce(-totalForce);
				balls[ball2Index].AddForce(totalForce);
			}
		}
	}

	void Render()
	{
		Clear(olc::BLACK);
		DrawString(10, 10, "Ball Count: " + std::to_string(balls.size()), olc::WHITE);
		DrawString(10, 30, "elapsedTime: " + std::to_string(elapsedTime), olc::WHITE);

		for (Ball& ball : balls)
		{
			DrawCircle(ball.position, ball.radius, ball.color);
			DrawLine(ball.position, ball.position + ball.velocity, ball.color);
			//DrawLine(ball.position, ball.position + ball.normal * ball.radius, ball.color);
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

	/*void IDK()
	{
		for (Ball& ball : balls)
		{
			ball.velocity = vf2d(0, 0);
		}
	}*/

	void Update(float dt)
	{
		for (Ball& ball : balls)
		{
			ball.AddAcceleration(GetWindowSize() / 2.0f - ball.position);
			ball.AddAngularAcceleration(0);
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
		elapsedTime = 0.002;
		balls.clear();
		balls.push_back(
			Ball(vf2d(100, 500), vf2d(100.0f, 0.0f), 0.0f, 0.0f, 10.0f, 1000000.0f, 1.0f, 0.0f, WHITE));
		/*balls.push_back(
			Ball(vf2d(150, 500), vf2d(0.0f, 0.0f), 0.0f, 0.0f, 10.0f, 1.0f, 0.8f, 0.0f, WHITE));
		balls.push_back(
			Ball(vf2d(200, 500), vf2d(0.0f, 0.0f), 0.0f, 0.0f, 10.0f, 1000000.0f, 1.0f, 0.0f, WHITE));
		*/

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Render();
		Controls(elapsedTime);
		//SeperateAllBalls();
		StimulateTimestep(elapsedTime);
		//Update(1.0f / 60.0f);
		//Collision();

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