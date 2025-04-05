#include "indicesGenerator.h"
#include <cstdlib>  // дл€ rand() и srand*()
#include <ctime>  //дл€ time()
#include <iostream>


#define WATERIDX 7
indicesGenerator::indicesGenerator()
{
	std::srand(std::time(nullptr));
}

uint8_t indicesGenerator::GenerateGroundIndex(uint8_t escapingNumber)
{
	int FirstLine = 32;
	int SecondLine = 40;
	int ThirdLine = 48;
	int FourLine = 56;
	uint8_t FirstRow = FirstLine + rand() % 2;   // generate 4 random numbers equal idx in sprite
	uint8_t SecondRow = SecondLine + rand() % 2;
	uint8_t ThirdRow = ThirdLine + rand() % 2;
	uint8_t FourRow = FourLine + rand() % 2;



	if (escapingNumber != 0)
	{
		int availableNumbers[4] = { FirstLine, SecondLine, ThirdLine, FourLine };


		do {
			FirstRow = availableNumbers[rand() % 4] + rand() % 2;
		} while (FirstRow == escapingNumber); // повтор€етс€ пока == escaping

		return FirstRow;

	}
	else
	{

		int randRow = rand() % 4;
		switch (randRow)     // take module from idx
		{   //depends from the module we write number
		case 0:
			return  FirstRow;
			break;
		case 1:
			return  SecondRow;
			break;
		case 2:
			return  ThirdRow;
			break;
		case 3:
			return  FourRow;
			break;
		}

	}

	return 0;
}

std::vector<uint8_t>& indicesGenerator::AlgorithmBasicGround(unsigned int width, unsigned int line)
{
		 indices.clear();
		 indices.resize(width * line);  //хранит значени€ Uint8_t где 255 макс число 

	for (unsigned int idx = 0; idx < width * line; idx++)
	{


		indices[idx] = GenerateGroundIndex();

	}

	for (unsigned int x = 0; x < width - 1; x++)    // checking on repitable tile
	{
		for (unsigned int y = 0; y < line - 1; y++)
		{
			if (indices[width * y + x] == indices[width * (y + 1) + x])
			{
				indices[width * (y + 1) + x] = GenerateGroundIndex(indices[width * (y + 1) + x]);
			}
			if (indices[width * y + x] == indices[width * (y) + x + 1])
			{
				indices[width * (y)+x + 1] = GenerateGroundIndex(indices[width * (y)+x + 1]);
			}
			if (indices[width * y + x] == indices[width * (y + 1)+x + 1])
			{
				indices[width * (y + 1) + x + 1] = GenerateGroundIndex(indices[width * (y + 1) + x + 1]);
			}
		}
	}

	return indices;
}

std::vector<uint8_t>& indicesGenerator::AlgorithmLakeGround(unsigned int width, unsigned int height,unsigned int LakesCount)
{
	indices.clear();
	indices.resize(width * height);  //хранит значени€ Uint8_t где 255 макс число 


	//std::vector<unsigned int> lakesRoots(7);
	std::vector<unsigned int> lakesRoots(LakesCount);
	AlgorithmBasicGround(width, height);   // заполн€ем уровень землей

	/*lakesRoots[0] = width +  5 % width;
	lakesRoots[1] = width +  6 % width;
	lakesRoots[2] = width +  7 % width;
	lakesRoots[4] = 7 % width;
	lakesRoots[3] =  5 % width;
	lakesRoots[5] = 2 * width + 6 % width;
	lakesRoots[6] = 2 *width + 7 % width;
	lakesRoots[7] = 3 * width + 7 % width;
	lakesRoots[8] = 4 * width + 7 % width;
	lakesRoots[9] = 2 * width + 8 % width;
	lakesRoots[10] = 2 * width + 9 % width;*/
	/*lakesRoots[0] = 0 * width + 10 % width;
	lakesRoots[1] = 2 * width + 5 % width;
	lakesRoots[2] = 2 * width + 4 % width;
	lakesRoots[3] = 4 * width +2 % width;
	lakesRoots[4] = 5 * width + 3 % width;
	lakesRoots[5] = 5 * width + 5 % width;
	lakesRoots[6] = 5 * width + 7 % width;*/

	for (int i = 0; i < lakesRoots.size(); i++)
	{
		//int lakeRoot = rand() % (width * height);  //создаЄм случайный индекс (позици€ рута) в 1д
		//int lakeRoot = (5 - i ) * width + 2 + i;


		do {
			
			lakesRoots[i] = rand() % (width * height);;       // «аписываю его в вектор 


		} while (indices[lakesRoots[i]] == WATERIDX);  //  по случайному индексу сажу root 
			
			indices[lakesRoots[i]] = WATERIDX;   //  по случайному индексу сажу root 	
	

	};

	for (int i = 0; i < lakesRoots.size(); i++)
	{


		int RootY = lakesRoots[i] / width;   //переводим в 2д
		int RootX = lakesRoots[i] % width;
	
		for (int y = RootY - 1; y < RootY + 2; y++)     // бежим вокруг корн€ 
		{
			if (y < 0) {

				continue;
			}
			if (y >= height)
				continue;

			for (int x = RootX - 1; x < RootX + 2; x++) {

				if (x < 0) {

					continue;
				}
				if (x >= width)
					continue;

				if (x == RootX && y == RootY)
					continue;
				if (indices[y * width + x] == WATERIDX)
					continue;

				auto r = [&](int i, int j) {
					int nx = x + i, ny = y + j;  // добавл€ет оффсет 
					

					if (nx < 0 || nx >= width || ny < 0 || ny >= height)
						return false; // если вышли за границы, считаем, что воды нет

					return  indices[ny * width + nx] == WATERIDX;
					 // вернет тру кода "море" и когда nx и ny больше нул€ и меньше ширин и высоту
					};

				//r(-1, -1);  // LeftUp
				//r(0, -1);   // Up
				//r(1, -1);   //RightUp
				//r(-1, 0);   //left
				//r(1, 0);    //right
				//r(-1, 1);   //leftDown
				//r(0, 1);    //Down 
				//r(1, 1);    //rightDown
			

				
				
					//leftUp        UP           R/Up          Left         Right    L/Down        Down        Righ/Down
				//1
				
				if (              r(0, -1)              && !r(-1, 0) && !r(1, 0) &&               r(0, 1)            ) indices[y * width + x] = 29;
				if (             r(0, -1) &&             !r(-1, 0) && r(1, 0) && !r(-1, 1) && !r(0, 1)             ) indices[y * width + x] = 38;
				if (              r(0, -1)              && r(-1, 0) && !r(1, 0)               && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 46;
				if (!r(-1, -1) && !r(0, -1)            && !r(-1, 0)  && r(1, 0)              && r(0, 1)              ) indices[y * width + x] = 62;
				if (             r(0, -1)             && r(-1, 0)    && !r(1, 0)             && r(0, 1)             ) indices[y * width + x] = 53;

				if (              !r(0, -1) && !r(1, -1) && r(-1, 0) && !r(1, 0)             && r(0, 1)            ) indices[y * width + x] = 54;
				if (              !r(0, -1)             && r(-1, 0) &&  r(1, 0)               && !r(0, 1)          ) indices[y * width + x] = 12;
				if (              !r(0, -1) && !r(1, -1) && r(-1, 0) && !r(1, 0) &&              !r(0, 1) && !r(1, 1)) indices[y * width + x] = 11;
				if (              r(0, -1) &&              !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 25;
				if (!r(-1, -1) && !r(0, -1) &&             !r(-1, 0) && r(1, 0) && !r(-1, 1) && !r(0, 1)             ) indices[y * width + x] = 8;
				if (!r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) &&              r(0, 1)            ) indices[y * width + x] = 1;
				if (r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0)              &&  r(0, 1)           ) indices[y * width + x] = 44;
				if (r(-1, -1)&& !r(0, -1)              && !r(-1, 0) && r(1, 0) && r(-1, 1) &&   !r(0, 1)           ) indices[y * width + x] = 52;
				if (            !r(0, -1) && r(1, -1)  &&   r(-1, 0) && !r(1, 0)              && !r(0, 1) &&  r(1, 1)) indices[y * width + x] = 60;
				if (!r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0)              && r(0, 1)             ) indices[y * width + x] = 9;
				if (              r(0, -1)              && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 10;
				if (              !r(0, -1) && r(1, -1) && r(-1, 0) && !r(1, 0) &&             !r(0, 1) && !r(1, 1)) indices[y * width + x] = 18;
				if (               r(0, -1)             && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 20;
				if (r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0)              && r(0, 1)            ) indices[y * width + x] = 22;
				if (             !r(0, -1) && !r(1, -1) && r(-1, 0) && !r(1, 0) &&              !r(0, 1) && r(1, 1)) indices[y * width + x] = 14;
				if (r(-1, -1) && !r(0, -1) &&             !r(-1, 0) && r(1, 0) && !r(-1, 1) && !r(0, 1)            ) indices[y * width + x] = 16;
				if (             !r(0, -1) && r(1, -1) && r(-1, 0) && !r(1, 0) &&              r(0, 1)              ) indices[y * width + x] = 28;
				if (              r(0, -1) &&              r(-1, 0) && !r(1, 0) &&            !r(0, 1) &&    r(1, 1)) indices[y * width + x] = 4;
				if (             r(0, -1) &&             !r(-1, 0) && r(1, 0) && r(-1, 1) && !r(0, 1)             ) indices[y * width + x] = 6;
				if (r(-1, -1) && !r(0, -1) &&             !r(-1, 0) && r(1, 0) &&             r(0, 1)              ) indices[y * width + x] = 30;
				if (            !r(0, -1)             && r(-1, 0) &&  r(1, 0) &&              r(0, 1)              ) indices[y * width + x] = 37;
				if (             r(0, -1) &&             r(-1, 0) && r(1, 0) &&              !r(0, 1)           ) indices[y * width + x] = 45;
				if (             r(0, -1) && r(1, -1) && !r(-1, 0) && r(1, 0)             && r(0, 1) && r(1, 1)) indices[y * width + x] = 61;
				if (!r(-1, -1) && !r(0, -1) &&             !r(-1, 0) && r(1, 0) && r(-1, 1) && !r(0, 1)            ) indices[y * width + x] = 17;
				if (              r(0, -1)            && !r(-1, 0) && r(1, 0) &&             r(0, 1)               ) indices[y * width + x] = 31;
				if (              r(0, -1) &&            !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 36;
				
				// 2 roots
				
			

				
				// 4 roots
				if (              r(0, -1) &&              r(-1, 0) && r(1, 0) &&            r(0, 1)                ) indices[y * width + x] = 21;

				if (r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 15;
				if (!r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 23;
				
				if (r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 43;
				if (!r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 35;
				if (r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 51;
				if (!r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 59;
				if (!r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 0;
				if (r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 27;
				if (!r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 24;
				if (!r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 3;
				
				
				if (!r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 34;
				if (r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 42;
				if (r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 50;
				if (r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 58;
			
				if (r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 13;
				//glm::vec2 distance = { x - RootX,  y - RootY };   // Ѕ≈–Єм вектор указывающий на клетку 


				//DefiniteCell(distance,indices[y * width + x]);
				

				

			}

		}

	}

	return indices;
}