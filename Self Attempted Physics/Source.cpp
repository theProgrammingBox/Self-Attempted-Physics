#define OLC_PGE_APPLICATION
#include "Ball.h"

class Example : public olc::PixelGameEngine
{
private:
	vf2d screen;
	vector<Ball> balls;
	const int subInterval = 10;
	double initialEnergy;

	Pixel mapToRainbow(double d) { // 0 - 1
		d *= 6;
		const double r = (d > 4) ? max(0.0, min(1.0, 6 - d)) : max(0.0, min(1.0, d - 2));
		const double g = (d > 2) ? max(0.0, min(1.0, 4 - d)) : max(0.0, min(1.0, d));
		const double b = (d > 3) ? max(0.0, min(1.0, d - 4)) : max(0.0, min(1.0, 2 - d));

		return Pixel(r * 0xff, g * 0xff, b * 0xff);
	}

public:
	Example()
	{
		sAppName = "Circle Physics";
	}

	void ApplyAccerleration()
	{
		for (Ball& ball : balls)
		{
			ball.ApplyAcceleration(vf2d(0.0f, 0.0f));
			ball.ApplyAngularAcceleration(0.0f);
		}
	}

	void Collisions()
	{
		for (int i = 0; i < balls.size(); i++)
		{
			Ball& ball1 = balls[i];

			if (ball1.GetPosition().x + ball1.GetRadius() > screen.x)
			{
				vf2d collisionNormal = vf2d(1, 0);
				vf2d tangentalNormal = collisionNormal.perp();
				vf2d relativeVelocity = ball1.GetVelocity() + ball1.GetRadius() * tangentalNormal * ball1.GetAngularVelocity();
				double normalVelocity = relativeVelocity.dot(collisionNormal);

				if (normalVelocity > 0)
				{
					double elasticity = 1.0f + ball1.GetElasticity();
					double tangentalVelocity = relativeVelocity.dot(tangentalNormal);
					double collisionImpulse = normalVelocity / ball1.GetInverseMass();
					double frictionImpulse = tangentalVelocity / (ball1.GetInverseMass() + ball1.GetRadius() * ball1.GetRadius() * ball1.GetInverseInertia());

					/*if (fabs(frictionImpulse) > fabs(collisionImpulse * ball1.GetFriction()))
					{
						frictionImpulse = (frictionImpulse > 0 ? collisionImpulse : -collisionImpulse) * ball1.GetFriction();
					}*/
					frictionImpulse *= ball1.GetFriction();

					vf2d totalForce = (collisionImpulse * collisionNormal + frictionImpulse * tangentalNormal) * elasticity;
					double angularForce = frictionImpulse * elasticity;
					ball1.ApplyForce(-totalForce);
					ball1.ApplyAngularForce(ball1.GetRadius() * -angularForce);
				}
			}

			if (ball1.GetPosition().y + ball1.GetRadius() > screen.y)
			{
				vf2d collisionNormal = vf2d(0, 1);
				vf2d tangentalNormal = collisionNormal.perp();
				vf2d relativeVelocity = ball1.GetVelocity() + ball1.GetRadius() * tangentalNormal * ball1.GetAngularVelocity();
				double normalVelocity = relativeVelocity.dot(collisionNormal);

				if (normalVelocity > 0)
				{
					double elasticity = 1.0f + ball1.GetElasticity();
					double tangentalVelocity = relativeVelocity.dot(tangentalNormal);
					double collisionImpulse = normalVelocity / ball1.GetInverseMass();
					double frictionImpulse = tangentalVelocity / (ball1.GetInverseMass() + ball1.GetRadius() * ball1.GetRadius() * ball1.GetInverseInertia());

					/*if (fabs(frictionImpulse) > fabs(collisionImpulse * ball1.GetFriction()))
					{
						frictionImpulse = (frictionImpulse > 0 ? collisionImpulse : -collisionImpulse) * ball1.GetFriction();
					}*/
					frictionImpulse *= ball1.GetFriction();

					vf2d totalForce = (collisionImpulse * collisionNormal + frictionImpulse * tangentalNormal) * elasticity;
					double angularForce = frictionImpulse * elasticity;
					ball1.ApplyForce(-totalForce);
					ball1.ApplyAngularForce(ball1.GetRadius() * -angularForce);
				}
			}

			if (ball1.GetPosition().x - ball1.GetRadius() < 0)
			{
				vf2d collisionNormal = vf2d(-1, 0);
				vf2d tangentalNormal = collisionNormal.perp();
				vf2d relativeVelocity = ball1.GetVelocity() + ball1.GetRadius() * tangentalNormal * ball1.GetAngularVelocity();
				double normalVelocity = relativeVelocity.dot(collisionNormal);

				if (normalVelocity > 0)
				{
					double elasticity = 1.0f + ball1.GetElasticity();
					double tangentalVelocity = relativeVelocity.dot(tangentalNormal);
					double collisionImpulse = normalVelocity / ball1.GetInverseMass();
					double frictionImpulse = tangentalVelocity / (ball1.GetInverseMass() + ball1.GetRadius() * ball1.GetRadius() * ball1.GetInverseInertia());

					/*if (fabs(frictionImpulse) > fabs(collisionImpulse * ball1.GetFriction()))
					{
						frictionImpulse = (frictionImpulse > 0 ? collisionImpulse : -collisionImpulse) * ball1.GetFriction();
					}*/
					frictionImpulse *= ball1.GetFriction();

					vf2d totalForce = (collisionImpulse * collisionNormal + frictionImpulse * tangentalNormal) * elasticity;
					double angularForce = frictionImpulse * elasticity;
					ball1.ApplyForce(-totalForce);
					ball1.ApplyAngularForce(ball1.GetRadius() * -angularForce);
				}
			}

			if (ball1.GetPosition().y - ball1.GetRadius() < 0)
			{
				vf2d collisionNormal = vf2d(0, -1);
				vf2d tangentalNormal = collisionNormal.perp();
				vf2d relativeVelocity = ball1.GetVelocity() + ball1.GetRadius() * tangentalNormal * ball1.GetAngularVelocity();
				double normalVelocity = relativeVelocity.dot(collisionNormal);

				if (normalVelocity > 0)
				{
					double elasticity = 1.0f + ball1.GetElasticity();
					double tangentalVelocity = relativeVelocity.dot(tangentalNormal);
					double collisionImpulse = normalVelocity / ball1.GetInverseMass();
					double frictionImpulse = tangentalVelocity / (ball1.GetInverseMass() + ball1.GetRadius() * ball1.GetRadius() * ball1.GetInverseInertia());

					/*if (fabs(frictionImpulse) > fabs(collisionImpulse * ball1.GetFriction()))
					{
						frictionImpulse = (frictionImpulse > 0 ? collisionImpulse : -collisionImpulse) * ball1.GetFriction();
					}*/
					frictionImpulse *= ball1.GetFriction();

					vf2d totalForce = (collisionImpulse * collisionNormal + frictionImpulse * tangentalNormal) * elasticity;
					double angularForce = frictionImpulse * elasticity;
					ball1.ApplyForce(-totalForce);
					ball1.ApplyAngularForce(ball1.GetRadius() * -angularForce);
				}
			}

			for (int j = i + 1; j < balls.size(); j++)
			{
				Ball& ball2 = balls[j];

				vf2d toTarget = ball2.GetPosition() - ball1.GetPosition();
				double totalRadius = ball1.GetRadius() + ball2.GetRadius();
				double distanceSquared = toTarget.mag2();

				if (distanceSquared < totalRadius * totalRadius)
				{
					vf2d collisionNormal = toTarget / sqrt(distanceSquared);
					vf2d tangentalNormal = collisionNormal.perp();
					vf2d velocity1 = ball1.GetVelocity() + ball1.GetRadius() * tangentalNormal * ball1.GetAngularVelocity();
					vf2d velocity2 = ball2.GetVelocity() + ball2.GetRadius() * tangentalNormal * ball2.GetAngularVelocity();
					vf2d relativeVelocity = velocity1 - velocity2;
					double normalVelocity = relativeVelocity.dot(collisionNormal);

					if (normalVelocity > 0)
					{
						double elasticity = 1.0f + ball1.GetElasticity() * ball2.GetElasticity();
						double friction = ball1.GetFriction() * ball2.GetFriction();
						double tangentalVelocity = relativeVelocity.dot(tangentalNormal);
						double collisionImpulse = normalVelocity / (ball1.GetInverseMass() + ball2.GetInverseMass());
						double frictionImpulse = tangentalVelocity / (
							ball1.GetInverseMass() + ball1.GetRadius() * ball1.GetRadius() * ball1.GetInverseInertia() +
							ball2.GetInverseMass() + ball2.GetRadius() * ball2.GetRadius() * ball2.GetInverseInertia());

						/*if (fabs(frictionImpulse) > fabs(collisionImpulse * friction))
						{
							frictionImpulse = (frictionImpulse > 0 ? collisionImpulse : -collisionImpulse) * friction;
						}*/
						frictionImpulse *= friction;

						vf2d totalForce = (collisionImpulse * collisionNormal + frictionImpulse * tangentalNormal) * elasticity;
						double angularForce = frictionImpulse * elasticity;
						ball1.ApplyForce(-totalForce);
						ball1.ApplyAngularForce(ball1.GetRadius() * -angularForce);
						ball2.ApplyForce(totalForce);
						ball2.ApplyAngularForce(ball2.GetRadius() * angularForce);
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

	void Update(double dt)
	{
		for (Ball& ball : balls)
		{
			ball.Update(dt);
		}
	}

	double GetTotalEnergy()
	{
		double totalEnergy = 0.0f;
		for (Ball& ball : balls)
		{
			totalEnergy += ball.GetEnergy();
		}
		return totalEnergy;
	}

	void Render()
	{
		Clear(olc::BLACK);

		for (Ball& ball : balls)
		{
			DrawCircle(ball.GetPosition(), ball.GetRadius(), ball.GetColor());
			DrawLine(ball.GetPosition(), ball.GetPosition() + vf2d(cos(ball.GetAngle()), sin(ball.GetAngle())) * ball.GetRadius(), ball.GetColor());
		}

		DrawString(5, 5, "Energy Error: " + to_string(GetTotalEnergy() - initialEnergy), olc::WHITE);
	}

	bool OnUserCreate() override
	{
		screen = vf2d(ScreenWidth(), ScreenHeight());
		Random random;

		for (int i = 0; i < 10; i++)
		{
			Ball ball;
			double x = random.UDoubleRandom() * 0.8 + 0.1;
			double y = random.UDoubleRandom() * 0.8 + 0.1;
			double vx = random.DoubleRandom() * random.DoubleRandom() * 100;
			double vy = random.DoubleRandom() * random.DoubleRandom() * 100;
			ball.SetPosition(vf2d(x * screen.x, y * screen.y));
			ball.SetVelocity(vf2d(vx, vy));
			ball.SetElasticity(1);
			ball.SetFriction(1);
			balls.push_back(ball);
		}
		/*Ball ball;
		ball.SetPosition(vf2d(10, 10));
		ball.SetVelocity(vf2d(100, 100));
		ball.SetElasticity(1);
		ball.SetFriction(0);
		balls.push_back(ball);
		ball.SetPosition(vf2d(40, 10));
		ball.SetVelocity(vf2d(-10, 10));
		ball.SetElasticity(1);
		ball.SetFriction(0);
		balls.push_back(ball);
		ball.SetPosition(vf2d(10, 40));
		ball.SetVelocity(vf2d(10, -10));
		ball.SetElasticity(1);
		ball.SetFriction(0);
		balls.push_back(ball);
		ball.SetPosition(vf2d(40, 40));
		ball.SetVelocity(vf2d(-100, -100));
		ball.SetElasticity(1);
		ball.SetFriction(0);
		balls.push_back(ball);*/

		initialEnergy = GetTotalEnergy();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		ApplyAccerleration();
		Collisions();
		Update(1.0f / 60.0f);
		Render();

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