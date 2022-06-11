#define OLC_PGE_APPLICATION
#include "Ball.h"

class Example : public olc::PixelGameEngine
{
private:
	vf2d screen;
	vf2d mouse;
	vector<Ball> balls;
	const float FPS = 1.0 / 60.0;
	Ball* selectedBall = nullptr;

	Pixel mapToRainbow(float d) { // 0 - 1
		d *= 6.0;
		const float r = (d > 4) ? max(0.0, min(1.0, 6.0 - d)) : max(0.0, min(1.0, d - 2.0));
		const float g = (d > 2) ? max(0.0, min(1.0, 4.0 - d)) : max(0.0, min(1.0, (double)d));
		const float b = (d > 3) ? max(0.0, min(1.0, d - 4.0)) : max(0.0, min(1.0, 2.0 - d));

		return Pixel(r * 0xff, g * 0xff, b * 0xff);
	}

	void Controls()
	{
		mouse = vf2d(GetMouseX(), GetMouseY());
		if (GetMouse(0).bPressed || GetMouse(1).bPressed && balls.size() != 0)
		{
			selectedBall = nullptr;
			float closest = INFINITY;
			float distanceSquared;
			float thicknessSquared;
			for (Ball& ball : balls)
			{
				distanceSquared = (ball.GetPosition() - mouse).mag2();
				thicknessSquared = ball.GetThickness() * ball.GetThickness();
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
				selectedBall->SetVelocity((mouse - selectedBall->GetPosition()) / FPS);
			}
		}
		if (GetMouse(0).bReleased)
		{
			if (selectedBall != NULL)
			{
				selectedBall->SetVelocity(vf2d(0, 0));
				selectedBall = nullptr;
			}
		}
		if (GetMouse(1).bHeld)
		{
			if (selectedBall != NULL)
			{
				DrawLine(selectedBall->GetPosition(), mouse, selectedBall->GetColor());
			}
		}
		if (GetMouse(1).bReleased)
		{
			if (selectedBall != NULL)
			{
				selectedBall->SetVelocity((mouse - selectedBall->GetPosition()) / (100 * FPS));
				selectedBall = NULL;
			}
		}
		if (GetKey(olc::Key::B).bPressed)
		{
			Ball ball;
			ball.SetPosition(mouse);
			balls.push_back(ball);
		}
		if (GetKey(olc::Key::R).bPressed)
		{
			balls.clear();
			selectedBall = nullptr;
		}
	}

	bool HasCollision(float& dt, Ball*& ball1, Ball*& ball2)
	{
		bool hasCollision = false;
		dt = INFINITY;
		for (int i = 0; i < balls.size(); i++)
		{
			for (int j = i + 1; j < balls.size(); j++)
			{
				const vf2d dPos = balls[i].GetPosition() - balls[j].GetPosition();
				const float distanceSquared = dPos.mag2();
				const float thicknessSquared =
					(balls[i].GetThickness() + balls[j].GetThickness()) *
					(balls[i].GetThickness() + balls[j].GetThickness());
				if (distanceSquared < thicknessSquared)
				{
					/*Need to fix the "bug" where neither balls have a velocity.
					This algorithm only works if the starting state has no collisions.
					If it does, move back time until the collision never occured.
					But if there is no velocity, both objects just sit there
					overlaping indefinitely.*/
					hasCollision = true;
					const vf2d dVel = balls[i].GetVelocity() - balls[j].GetVelocity();
					const float a = dVel.mag2();
					const float b = dPos.dot(dVel);
					const float c = distanceSquared - thicknessSquared - 0.001f;
					float d = (-sqrt(b * b - a * c) - b) / a;
					if (d < dt)
					{
						dt = d;
						ball1 = &balls[i];
						ball2 = &balls[j];
					}
				}
			}
		}
		return hasCollision;
	}

	void Render()
	{
		Clear(olc::BLACK);

		for (Ball& ball : balls)
		{
			DrawCircle(ball.GetPosition(), ball.GetThickness(), ball.GetColor());
			DrawLine(ball.GetPosition(), ball.GetPosition() + ball.GetVelocity(), ball.GetColor());
			DrawLine(ball.GetPosition(), ball.GetPosition() + ball.GetNormal() * ball.GetThickness(), ball.GetColor());
		}
	}

	void Update(float dt)
	{
		for (Ball& ball : balls)
		{
			ball.ApplyAcceleration(vf2d(0.0f, 0.0f), dt);
			ball.ApplyAngularAcceleration(0.0f, dt);
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
		screen = vf2d(ScreenWidth(), ScreenHeight());
		mouse = vf2d(GetMouseX(), GetMouseY());
		balls.push_back(Ball(
			vf2d(100, 500),
			vf2d(10.0f, 0.0f),
			0.0f,
			0.0f,
			10.0f,
			100000.0f,
			1.0f,
			0.0f,
			olc::RED));
		balls.push_back(Ball(
			vf2d(200, 500),
			vf2d(0.0f, 0.0f),
			0.0f,
			0.0f,
			10.0f,
			1.0f,
			1.0f,
			0.0f,
			olc::RED));
		balls.push_back(Ball(
			vf2d(300, 500),
			vf2d(0.0f, 0.0f),
			0.0f,
			0.0f,
			10.0f,
			100000.0f,
			1.0f,
			0.0f,
			olc::RED));

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Render();
		Controls();
		bool hasCollision;
		float totalDT = 0;
		float dt;
		Ball* ball1;
		Ball* ball2;
		while (totalDT < FPS)
		{
			ball1 = nullptr;
			ball2 = nullptr;
			Update(FPS - totalDT);
			totalDT = FPS;
			hasCollision = HasCollision(dt, ball1, ball2);
			while (hasCollision)
			{
				Update(dt);
				hasCollision = HasCollision(dt, ball1, ball2);
			}
			if (ball1 != nullptr && ball2 != nullptr)
			{
				float elasticity = (ball1->GetElasticity() + ball2->GetElasticity()) / 2.0f + 1.0f;
				vf2d dPos = ball2->GetPosition() - ball1->GetPosition();
				vf2d collisionNormal = dPos.norm();
				vf2d dVel = ball1->GetVelocity() - ball2->GetVelocity();
				double normalVelocity = dVel.dot(collisionNormal);
				double impulse = normalVelocity / (ball1->GetInverseMass() + ball2->GetInverseMass());
				vf2d impulseVector = collisionNormal * impulse * elasticity;
				ball1->ApplyForce(-impulseVector);
				ball2->ApplyForce(impulseVector);
			}
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