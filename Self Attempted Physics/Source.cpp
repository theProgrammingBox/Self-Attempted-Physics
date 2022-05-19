#include "Header.h"

class Example : public olc::PixelGameEngine
{
private:
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

	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
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