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
	struct collisionDetails
	{
		Ball* ball1;
		Ball* ball2;
		vf2d dPos;
		vf2d dVel;
		float distanceSquared;
	};

	Pixel mapToRainbow(float d) { // 0 - 1
		d *= 6.0;
		const float r = (d > 4.0f) ? max(0.0f, min(1.0f, 6.0f - d)) : max(0.0f, min(1.0f, d - 2.0f));
		const float g = (d > 2.0f) ? max(0.0f, min(1.0f, 4.0f - d)) : max(0.0f, min(1.0f, d));
		const float b = (d > 3.0f) ? max(0.0f, min(1.0f, d - 4.0f)) : max(0.0f, min(1.0f, 2.0f - d));

		return Pixel(r * 0xff, g * 0xff, b * 0xff);
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

	bool SameState()
	{
		for (Ball& ball : balls)
		{
			if (!ball.SameState())
			{
				return false;
			}
		}
		return true;
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
				//set pos to mouse
				selectedBall->SetPosition(mouse);
				selectedBall->SetVelocity(vf2d(0, 0));
				//selectedBall->SetVelocity((mouse - selectedBall->GetPosition()) / FPS);
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

	void SeperateAllBalls()
	{
		vector<collisionDetails> collisions;
		int counter;
		int i, j;
		vf2d dPos;
		float distanceSquared;
		float totalThickness;
		vf2d collisionNormal;
		float distance;
		float impulse;
		vf2d impulseVector;

		counter = 0;
		while (counter != balls.size())
		{
			for (i = 0; i < balls.size(); i++)
			{
				counter++;
				for (j = i + 1; j < balls.size(); j++)
				{
					dPos = balls[i].GetPosition() - balls[j].GetPosition();
					distanceSquared = dPos.mag2();
					totalThickness = balls[i].GetThickness() + balls[j].GetThickness();
					if (distanceSquared < totalThickness * totalThickness)
					{
						counter = 0;
						distance = sqrt(distanceSquared);
						if (distance == 0.0f)
						{
							collisionNormal = vf2d(1.0f, 0.0f);
						}
						else
						{
							collisionNormal = dPos / distance;
						}
						impulse = (distance - totalThickness - 0.1f) / (balls[i].GetInverseMass() + balls[j].GetInverseMass());
						impulseVector = collisionNormal * impulse;
						balls[i].SetPosition(balls[i].GetPosition() - impulseVector * balls[i].GetInverseMass());
						balls[j].SetPosition(balls[j].GetPosition() + impulseVector * balls[j].GetInverseMass());
						/*Theres a bug where an object sandwitched between two objects of enourmous mass will not be able
						to move either one resulting in infinite loop because it jitters between the two objects without
						moving them*/
					}
				}
				if (counter == balls.size())
				{
					return;
				}
			}
		}
	}

	void StimulateTimestep(float dt)
	{
		vector<collisionDetails> collisions;
		vector<collisionDetails> prevCollisions;
		int i, j;
		float totalDt;
		float remainingDt;
		float dtOffset;
		float a, b, c;
		float offset;

		vf2d dPos;
		float distanceSquared;
		float totalThickness;
		vf2d dVel;
		float elasticity;
		vf2d collisionNormal;
		float normalVelocity;
		float impulse;
		vf2d impulseVector;

		totalDt = 0.0f;
		/*while (totalDt < dt)
		{*/
		remainingDt = dt - totalDt;
		Update(remainingDt);
		totalDt += remainingDt;
		prevCollisions.clear();
		/*do
		{*/
		dtOffset = 0.0f;
		prevCollisions = collisions;
		collisions.clear();
		for (i = 0; i < balls.size(); i++)
		{
			for (j = i + 1; j < balls.size(); j++)
			{
				dPos = balls[i].GetPosition() - balls[j].GetPosition();
				distanceSquared = dPos.mag2();
				totalThickness = balls[i].GetThickness() + balls[j].GetThickness();
				if (distanceSquared < totalThickness * totalThickness)
				{
					dVel = balls[i].GetVelocity() - balls[j].GetVelocity();
					a = dVel.mag2();
					b = dVel.dot(dPos);
					c = distanceSquared - (totalThickness + 1.1f) * (totalThickness + 1.1f);
					offset = -(b + sqrt(b * b - a * c)) / a;
					if (offset < dtOffset)
					{
						dtOffset = offset < -totalDt ? -totalDt : offset;
						//dtOffset = offset;
					}
					/*cout << "Distance from ball " << i << " to " << j << " is " << sqrt(distanceSquared) << endl;
					cout << "It should be " << totalThickness << endl;
					cout << "Therefore the offset is " << offset << endl;*/
					collisionDetails collision;
					collision.ball1 = &balls[i];
					collision.ball2 = &balls[j];
					collision.dPos = dPos;
					collision.dVel = dVel;
					collision.distanceSquared = distanceSquared;
					collisions.push_back(collision);
				}
			}
		}
		//cout << dtOffset << endl;
		/*if (dtOffset != 0.0f)
		{*/
		Update(dtOffset);
		totalDt += dtOffset;
		//}
	/*} while (dtOffset != 0.0f);*/
		for (collisionDetails& collision : collisions)
		{
			elasticity = (collision.ball1->GetElasticity() + collision.ball2->GetElasticity()) * 0.5f + 1.0f;
			collisionNormal = collision.dPos / sqrt(collision.distanceSquared);
			normalVelocity = collision.dVel.dot(collisionNormal);
			impulse = normalVelocity / (collision.ball1->GetInverseMass() + collision.ball2->GetInverseMass());
			impulseVector = collisionNormal * impulse * elasticity;
			collision.ball1->ApplyForce(-impulseVector);
			collision.ball2->ApplyForce(impulseVector);
		}
		//cout << "Ball 1 velocity is " << balls[0].GetVelocity().x << " " << balls[0].GetVelocity().y << endl;
		/*cout << "Ball 2 velocity is " << balls[1].GetVelocity().x << " " << balls[1].GetVelocity().y << endl;*/
		//cout << "Ball 3 velocity is " << balls[2].GetVelocity().x << " " << balls[2].GetVelocity().y << endl;
	//}
	}

	void Render()
	{
		Clear(olc::BLACK);
		DrawString(10, 10, "Ball Count: " + std::to_string(balls.size()), olc::WHITE);

		for (Ball& ball : balls)
		{
			DrawCircle(ball.GetPosition(), ball.GetThickness(), ball.GetColor());
			DrawLine(ball.GetPosition(), ball.GetPosition() + ball.GetVelocity(), ball.GetColor());
			DrawLine(ball.GetPosition(), ball.GetPosition() + ball.GetNormal() * ball.GetThickness(), ball.GetColor());
			//DrawLine(screen / 2.0f, ball.GetPosition(), ball.GetColor());
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
		/*balls.push_back(Ball(vf2d(400, 500), vf2d(20.0f, 0.0f), 0.0f, 0.0f, 10.0f, 1000.0f, 0.0f, 0.0f, WHITE));
		balls.push_back(Ball(vf2d(450, 500), vf2d(100.0f, 0.0f), 0.0f, 0.0f, 10.0f, 1.0f, 0.0f, 0.0f, RED));*/
		balls.push_back(Ball(vf2d(500, 500), vf2d(0.0f, 0.0f), 0.0f, 0.0f, 10.0f, 1000.0f, 1.0f, 0.0f, GREEN));

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Render();
		Controls();
		SeperateAllBalls();
		StimulateTimestep(FPS * 1.0f);
		//Update(FPS);

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