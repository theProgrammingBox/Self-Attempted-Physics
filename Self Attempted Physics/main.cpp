#define OLC_PGE_APPLICATION
#include "Ball.h"

class Example : public olc::PixelGameEngine
{
private:
	Random random;
	vector<Ball> balls;
	Ball* selectedBall = nullptr;
	bool paused;

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
			DrawLine(ball.position, ball.position + ball.velocity, ball.color);
			DrawLine(ball.position, ball.position + ball.normal * ball.radius, ball.color);
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

		if (GetKey(olc::Key::SPACE).bPressed)
		{
			paused = !paused;
		}
	}

	void ApplyAccelerations(float dt)
	{
		for (Ball& ball : balls)
		{
			//ball.AddAcceleration(GetWindowSize() / 2.0f - ball.position);
			ball.AddAngularAcceleration(0);
			ball.ApplyAccelerations(dt);
		}
	}

	void ApplyForces()
	{
		for (Ball& ball : balls)
		{
			ball.ApplyForces();
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
		vf2d dPos = ball1.position - ball2.position;
		vf2d collisionNormal = dPos.norm();

		vf2d pa = ball1.position + collisionNormal * ball1.radius;
		vf2d pb = ball2.position - collisionNormal * ball2.radius;
		vf2d ra = pa - ball1.position;
		vf2d rb = pb - ball2.position;
		vf2d va = ball1.velocity + ra.perp() * ball1.angularVelocity;
		vf2d vb = ball2.velocity - rb.perp() * ball2.angularVelocity;
		vf2d v = va - vb;
		vf2d vt = v - (v.dot(collisionNormal) * collisionNormal);
		vf2d nc = collisionNormal;
		float jc = nc.dot(v) / (
			(ball1.inverseMass + ball2.inverseMass) +
			(ra.cross(collisionNormal) * ra.cross(collisionNormal) * ball1.inverseInertia) +
			(rb.cross(collisionNormal) * rb.cross(collisionNormal) * ball2.inverseInertia));
		vf2d nf = -vt.norm();
		float jf = nf.dot(v) / (
			(ball1.inverseMass + ball2.inverseMass) +
			(ra.cross(nf) * ra.cross(nf) * ball1.inverseInertia) +
			(rb.cross(nf) * rb.cross(nf) * ball2.inverseInertia));

		if (fabs(jf) > fabs(jc * friction))
		{
			jf = (jf > 0 ? jc : -jc) * friction;
		}
		vf2d impulse = nc * (jc * -elasticity) + nf * -jf;
		ball1.AddForce(impulse);
		ball2.AddForce(-impulse);
		ball1.AddTorque(ra.cross(impulse));
		ball2.AddTorque(-rb.cross(impulse));
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
								if (dtOffset <= -remainingDt)
								{
									CollideBalls(balls[i], balls[j]);
								}
								else if (dtOffset < dtGlobalOffset)
								{
									dtGlobalOffset = dtOffset;
									ball1 = &balls[i];
									ball2 = &balls[j];
								}
							}
						}
					}
				}
			}
			if (ball1 != nullptr)
			{
				CollideBalls(*ball1, *ball2);
			}
			Update(dtGlobalOffset);
			ApplyAccelerations(dtGlobalOffset);
			ApplyForces();
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
		/*balls.push_back(
			Ball(vf2d(100, 500), vf2d(100.0f, 0.0f), 0.0f, 0.0f, 10.0f, 1000000.0f, 1.0f, 0.0f, WHITE));
		balls.push_back(
			Ball(vf2d(150, 500), vf2d(0.0f, 0.0f), 0.0f, 0.0f, 10.0f, 1.0f, 0.7f, 0.0f, WHITE));
		balls.push_back(
			Ball(vf2d(200, 500), vf2d(0.0f, 0.0f), 0.0f, 0.0f, 10.0f, 1000000.0f, 1.0f, 0.0f, WHITE));*/


		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Render();
		Controls(fElapsedTime);
		if (!paused)
		{
			StimulateTimestep(fElapsedTime);
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