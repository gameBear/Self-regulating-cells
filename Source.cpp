#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "warrior.h"
int wrldW = 800, wrldH = 800;
float reds = 0, greens = 0, xPos = 0;
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}
public:
	olc::Sprite* pulse;
	std::vector<warrior*> Units;
	bool OnUserCreate() override
	{
		int r = 10;
		for (int i = 0; i < r; i++) {
			Units.push_back(new warrior(rand() % 800 + 100, rand() % 800 + 50));
		}
		
		pulse = new olc::Sprite(200,100);
		SetDrawTarget(pulse);
		Clear(olc::VERY_DARK_GREEN);
		SetDrawTarget(nullptr);
		return true;
		
	}
	bool OnUserUpdate(float fElapsedTime) override
	{

		static float time = 0;
		time += fElapsedTime;
		for (int i = 0; i < Units.size(); i++) {
			Units.at(i)->move(10.0, fElapsedTime);
		/*	if (Units.at(i)->x > 805) Units.at(i)->x -= 810;
			if (Units.at(i)->y > 805) Units.at(i)->y -= 810;
			if (Units.at(i)->x < -5) Units.at(i)->x += 810;
			if (Units.at(i)->y < -5) Units.at(i)->y += 810;*/

			if (Units.at(i)->x > 805) Units.at(i)->x -= 810;
			if (Units.at(i)->y > 805) Units.at(i)->y -= 810;
			if (Units.at(i)->x < -5) Units.at(i)->x += 810;
			if (Units.at(i)->y < -5) Units.at(i)->y += 810;
			
		}

		for (int i = 0; i < Units.size(); i++) {
			if (i < Units.size() - 1) for (int j = i + 1; j < Units.size(); j++) Units.at(i)->collide(*Units.at(j));
			Units.at(i)->rotate();
		}
		for (int i = 0; i < Units.size(); i++) {
			if ((Units.at(i)->lifeTime > 12 && Units.at(i)->aggresive) || Units.at(i)->lifeTime > 25) {
				Units.push_back(new warrior(Units.at(i)->x + 10, Units.at(i)->y));
				if(Units.at(i)->aggresive && (rand()% 20 < 18)) Units.back()->aggresive = true;
				Units.at(i)->passive = 2;
				Units.at(i)->lifeTime = 2;
				Units.back()->passive = 2;
				Units.at(i)->kids++;
			}
			else if (Units.at(i)->lifeTime < 0 || Units.at(i)->kids > 4 || (Units.at(i)->kids >= 2 && Units.at(i)->aggresive)) {
				Units.erase(Units.begin() + i);
			}
		}
		Units.shrink_to_fit();
		if (time > 0.05) {
			float allSpeed = 0;
			SetPixelMode(olc::Pixel::ALPHA);
			SetDrawTarget(pulse);
			Draw(xPos, reds, olc::RED);
			Draw(xPos, greens, olc::WHITE);
			xPos++;
			DrawLine(xPos, 0, xPos, 99, olc::Pixel(5,50,5,150));
			DrawLine(xPos+1, 0, xPos+1, 99, olc::Pixel(255, 150, 200, 40));
			DrawRect(0, 0, 199, 99, olc::DARK_GREY);
			if (xPos >= 200) xPos = 0;
			SetDrawTarget(nullptr);
			
			
			FillRect(0,0,ScreenWidth(), ScreenHeight(), olc::Pixel(10,10,150,80));
			DrawSprite(5, 5, pulse);
			SetPixelMode(olc::Pixel::NORMAL);
			reds = 0;
			greens = 0;
			float sum = 0;
			for (int i = 0; i < Units.size(); i++) {
				sum++;
				if (Units.at(i)->aggresive) reds++;
				else greens++;
				FillCircle(Units.at(i)->x, Units.at(i)->y, Units.at(i)->radius, olc::Pixel(20, 50, 255 - Units.at(i)->heat()));

				FillCircle(Units.at(i)->x, Units.at(i)->y, Units.at(i)->radius * 0.5, olc::Pixel(50, Units.at(i)->color, 50));
				if(Units.at(i)->aggresive) DrawCircle(Units.at(i)->x, Units.at(i)->y, Units.at(i)->radius, olc::Pixel(50 * Units.at(i)->hunger, 0,0));
				else DrawCircle(Units.at(i)->x, Units.at(i)->y, Units.at(i)->radius, olc::GREEN);
				DrawLine(Units.at(i)->x, Units.at(i)->y, Units.at(i)->lineX(), Units.at(i)->lineY());

			}
			if (reds == 0 && greens == 0) Units.push_back(new warrior(rand() % 800 + 100, rand() % 800 + 50));
			sum = 100 / (sum * 1.2);
			greens *= sum;
			greens = 100 - greens - 5;
			reds *= sum;
			reds = 100 - reds - 5;
			time = 0;
		}
		return true;
	}
};
int main()
{
	srand(1);
	Example demo;
	if (demo.Construct(wrldW, wrldH, 1, 1,false))
		demo.Start();
	return 0;
}