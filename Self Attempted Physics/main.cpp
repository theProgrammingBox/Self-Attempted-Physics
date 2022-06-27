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
		//elapsedTime = (GetWindowSize().y / 2.0f - GetMousePos().y) * 0.00001;
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
	}

	bool ResolveContacts()
	{
		bool contact = false;
		for (int i = 0; i < balls.size(); i++)
		{
			//balls[i].contacts.clear();
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
							contact = true;
							//balls[i].contacts.push_back(&balls[j]);

							vf2d collisionNormal = dPos / sqrt(distanceSquared);
							float normalVelocity = collisionNormal.dot(dVel);
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
		return contact;
	}

	struct collisionPair
	{
		Ball* ball1;
		Ball* ball2;
	};

	void StimulateTimestep(float dt)
	{
		collisionPair collision;
		//vector<collisionPair> collisions;
		float remainingDt = dt;
		while (remainingDt != 0.0f)
		{
			if (!ResolveContacts())
			{
				//bool maxed = false;
				Update(remainingDt);
				float dtGlobalOffset = 0.0f;
				collision.ball1 = nullptr;
				collision.ball2 = nullptr;
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
										collision.ball1 = &balls[i];
										collision.ball2 = &balls[j];
										if (!maxed)
										{
											//if (dtOffset < -remainingDt)
											if (dtOffset < -dt)
											{
												maxed = true;
												//dtGlobalOffset = -remainingDt;
												dtGlobalOffset = -dt;
												collisions.push_back(collision);
											}
											else
											{
												dtGlobalOffset = dtOffset;
											}
										}
										else
										{
											collisions.push_back(collision);
										}
									}
								}
							}
						}
					}
				}
			}
			Update(dtGlobalOffset);
			remainingDt = -dtGlobalOffset;
			if (collision.ball1 != nullptr)
			{
				if (!maxed)
				{
					vf2d dPos = collision.ball1->position - collision.ball2->position;
					vf2d collisionNormal = dPos.norm();
					vf2d dVel = collision.ball1->velocity - collision.ball2->velocity;
					float normalVelocity = collisionNormal.dot(dVel);
					float elasticity = (collision.ball1->elasticity + collision.ball2->elasticity) * 0.5f + 1.0f;
					float collisionImpulse = normalVelocity / (collision.ball1->inverseMass + collision.ball2->inverseMass);
					vf2d totalForce = collisionNormal * collisionImpulse * elasticity;
					collision.ball1->AddForce(-totalForce);
					collision.ball2->AddForce(totalForce);
				}
				else
				{
					for (collisionPair& c : collisions)
					{
						vf2d dPos = c.ball1->position - c.ball2->position;
						vf2d collisionNormal = dPos.norm();
						vf2d dVel = c.ball1->velocity - c.ball2->velocity;
						float normalVelocity = collisionNormal.dot(dVel);
						float elasticity = (c.ball1->elasticity + c.ball2->elasticity) * 0.5f + 1.0f;
						float collisionImpulse = normalVelocity / (c.ball1->inverseMass + c.ball2->inverseMass);
						vf2d totalForce = collisionNormal * collisionImpulse * elasticity;
						c.ball1->AddForce(-totalForce);
						c.ball2->AddForce(totalForce);
					}
				}
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
		StimulateTimestep(elapsedTime);

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