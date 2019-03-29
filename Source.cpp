#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

//using namespace std::chrono_literals;

class Snake : public olc::PixelGameEngine
{
public:
	Snake()
	{
		sAppName = "Snake";
	}

private:
	//FIELD
	int nFieldWidth = 18;
	int nFieldHeight = 13;
	unsigned char *pField = nullptr;
	olc::Sprite *sBackground;

	//SCREEN
	//olc::Sprite *screen = new olc::Sprite[540 * 480];

	//SNAKE
	int nCurrentX = 10;
	int nCurrentY = 7;
	int nDirX = -1;
	int nDirY = 0;
	std::vector<std::pair<int, int>> vSnake;
	olc::Sprite *sBody = new olc::Sprite(16, 16);

	//FOOD
	int nFoodX;
	int nFoodY;
	olc::Sprite *sFood = new olc::Sprite(16, 16);

	//GAME VARIABLES
	int nScore = 0;
	float fGameCounter;
	double fGameDelay = 50.00;		//For Release: 50.00 || For Debug: 5.00

	std::vector<std::pair<int, int>>::iterator it;

	bool bForceMove = true;
	bool bGameOver = false;

public:
	bool OnUserCreate() override
	{
		//GENERATE SEED
		srand(time(NULL));

		//INITIALIZE STUFF
		sBackground = new olc::Sprite("SnakeGameBackground.png");
		sBody = new olc::Sprite("SnakeBody.png");
		sFood = new olc::Sprite("SnakeFood.png");

		//DRAW PLAYING FIELD
		DrawSprite(0, 0, sBackground, 4);

		//SNAKE STARTING POS
		vSnake.push_back({ nCurrentX, nCurrentY });

		//DRAW FOOD
		spawnFood(rand() % 18, rand() % 13);
		DrawSprite(nFoodX * 64, nFoodY * 64, sFood, 4);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//MOVE SNAKE (HEAD)
		nCurrentX = vSnake[0].first;
		nCurrentY = vSnake[0].second;

		if (GetKey(olc::Key::LEFT).bPressed && nDirX != 1 || GetKey(olc::Key::RIGHT).bPressed && nDirX != -1)
		{
			nDirX = 0;  //So it doesn't double in speed

			nDirX -= (GetKey(olc::Key::LEFT).bPressed) ? 1 : 0;
			nDirX += (GetKey(olc::Key::RIGHT).bPressed) ? 1 : 0;

			nDirY = 0;
		}

		if (GetKey(olc::Key::UP).bPressed && nDirY != 1 || GetKey(olc::Key::DOWN).bPressed && nDirY != -1)
		{
			nDirY = 0; //So it doesn't double in speed

			nDirY -= (GetKey(olc::Key::UP).bPressed) ? 1 : 0;
			nDirY += (GetKey(olc::Key::DOWN).bPressed) ? 1 : 0;

			nDirX = 0;
		}

		fGameCounter += fElapsedTime;

		if (fGameCounter >= fElapsedTime * fGameDelay)
		{
			//UPDATING BODY (INCLUDING HEAD)
			int nTempStorageX = vSnake[0].first;
			int nTempStorageY = vSnake[0].second;

			nTempStorageX += nDirX;
			nTempStorageY += nDirY;

			it = vSnake.begin();

			vSnake.insert(it, { nTempStorageX, nTempStorageY });
			vSnake.pop_back();

			fGameCounter = 0;
		}

		//DID SNAKE EAT FOOD
		if (vSnake[0].first == nFoodX && vSnake[0].second == nFoodY)
		{
			vSnake.push_back({ nCurrentX, nCurrentY });

			spawnFood(rand() % 18, rand() % 13);

			fGameDelay /= 1.01;
		}

		//DRAW PLAYING FIELD
		DrawSprite(0, 0, sBackground, 4);

		//DRAW FOOD
		DrawSprite(nFoodX * 64, nFoodY * 64, sFood, 4);

		//DRAW CURRENT POS
		for (int i = 0; i < vSnake.size(); i++)
		{
			DrawSprite(vSnake[i].first * 64, vSnake[i].second * 64, sBody, 4);
		}

		//CHECK IF GAME OVER
		bGameOver = bDoesSnakeFit(vSnake[0].first, vSnake[0].second);

		//std::cout << "fGameDelay = " << fGameDelay << "\n";

		return bGameOver;
	}

	bool bDoesSnakeFit(int nPosX, int nPosY)
	{
		if (!bIsInSnake(nPosX, nPosY, 1))
			if (nPosX >= 1 && nPosX < nFieldWidth + 1)
				if (nPosY >= 1 && nPosY < nFieldHeight + 1)
					return true;

		return false;
	}

	bool bDoesFoodFit(int nPosX, int nPosY)
	{
		if (!bIsInSnake(nPosX, nPosY, 0))
			if (nPosX >= 2 && nPosX < nFieldWidth)
				if (nPosY >= 2 && nPosY < nFieldHeight)
					return true;

		return false;
	}

	bool bIsInSnake(int nPosX, int nPosY, int nIterator)
	{
		for (nIterator; nIterator < vSnake.size(); nIterator++)
			if (nPosX == vSnake[nIterator].first && nPosY == vSnake[nIterator].second)
				return true;

		return false;
	}

	void spawnFood(int nPosX, int nPosY)
	{
		while (true)
		{
			if (bDoesFoodFit(nPosX, nPosY))
			{
				nFoodX = nPosX;
				nFoodY = nPosY;

				break;
			}

			nPosX = rand() % 18;
			nPosY = rand() % 13;
		}
	}
};

int main()
{
	Snake demo;
	if (demo.Construct(1280, 960, 1, 1))
		demo.Start();

	return 0;
}



//CREATE PLAYING FIELD
		/*
		pField = new unsigned char[nFieldWidth*nFieldHeight];
		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
				pField[y*nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
				*/
				//for (int i = 0; i < nFieldWidth * nFieldHeight; i++) screen[i] = olc::Sprite(16, 16);

				//RENDEROUTPUT
				//FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(112, 128, 144));