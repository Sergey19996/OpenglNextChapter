#include "indicesGenerator.h"
#include <cstdlib>  // для rand() и srand*()
#include <ctime>  //для time()
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
		} while (FirstRow == escapingNumber); // повторяется пока == escaping

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
		 indices.resize(width * line);  //хранит значения Uint8_t где 255 макс число 

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
	indices.resize(width * height);  //хранит значения Uint8_t где 255 макс число 

	std::vector<unsigned int> lakesRoots(LakesCount);
	AlgorithmBasicGround(width, height);   // заполняем уровень землей

	for (int i = 0; i < lakesRoots.size(); i++)
	{
		int lakeRoot = rand() % (width * height);  //создаём случайный индекс
		//int lakeRoot = (5 - i ) * width + 2 + i;


		lakesRoots[i] = lakeRoot;
		indices[lakeRoot] = WATERIDX;   //  по случайному индексу сажу root





		int RootY = lakeRoot / width;   //переводим в 2д
		int RootX = lakeRoot % width;
		std::cout << "Root POS : " << RootX << '\t' << RootY << std::endl;
		for (int y = RootY - 1; y < RootY + 2; y++)     // бежим вокруг корня 
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
					int nx = x + i, ny = y + j;
					return (nx >= 0 && nx < width && ny >= 0 && ny < indices.size() / width) && indices[ny * width + nx] == WATERIDX;
					 // вернет тру кода "море"
					};

				//r(-1, -1);  // LeftUp
				//r(0, -1);   // Up
				//r(1, -1);   //RightUp
				//r(-1, 0);   //left
				//r(1, 0);    //right
				//r(-1, 1);   //leftDown
				//r(0, 1);    //Down 
				//r(1, 1);    //rightDown
				std::cout  <<x << '\t' <<y << std::endl;

				
				
					//leftUp        UP           R/Up          Left         Right    L/Down        Down        Righ/Down
				//1
				if (r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 27;
				if (              r(0, -1) &&              !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 25;
				if (!r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 24;
				if (!r(-1, -1) && !r(0, -1) &&             !r(-1, 0) && r(1, 0) && !r(-1, 1) && !r(0, 1)             ) indices[y * width + x] = 8;
				if (!r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 0;
				if (!r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) &&              r(0, 1)            ) indices[y * width + x] = 1;
				if (!r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 3;
				if (              !r(0, -1) && !r(1, -1) && r(-1, 0) && !r(1, 0) &&              !r(0, 1) && !r(1, 1)) indices[y * width + x] = 11;
				// 2 roots
				if (r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 15;
				if (!r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 23;
				if (!r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 9;
				if (r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 43;
				if (!r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 35;
				if (r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 51;
				if (!r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 59;
				if (              !r(0, -1)             && r(-1, 0) &&  r(1, 0)               && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 12;
				if (              r(0, -1)              && !r(-1, 0) && !r(1, 0) &&               r(0, 1)            ) indices[y * width + x] = 29;
				if (r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1)   && !r(1, 1)) indices[y * width + x] = 10;
				if (r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 18;
				if (!r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 34;
				if (r(-1, -1) && !r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 42;
				if (r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 50;
				if (r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0) && !r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 58;
				if (!r(-1, -1) && r(0, -1) && !r(1, -1) && !r(-1, 0) && !r(1, 0) && r(-1, 1) && !r(0, 1) && r(1, 1)) indices[y * width + x] = 36;
				if (r(-1, -1) && !r(0, -1) && r(1, -1) && !r(-1, 0) && !r(1, 0)              &&  r(0, 1)           ) indices[y * width + x] = 44;
				if (r(-1, -1)&& !r(0, -1)              && !r(-1, 0) && r(1, 0) && r(-1, 1) &&   !r(0, 1)           ) indices[y * width + x] = 52;
				if (            !r(0, -1) && r(1, -1)  &&   r(-1, 0) && !r(1, 0)              && !r(0, 1) &&  r(1, 1)) indices[y * width + x] = 60;
				if (!r(-1, -1) && !r(0, -1)            && !r(-1, 0)  && r(1, 0)              && r(0, 1)              ) indices[y * width + x] = 62;
				if (              !r(0, -1) && !r(1, -1) && r(-1, 0) && !r(1, 0)             && r(0, 1)            ) indices[y * width + x] = 54;
				if (              r(0, -1)              && r(-1, 0) && !r(1, 0)               && !r(0, 1) && !r(1, 1)) indices[y * width + x] = 46;
				if (             r(0, -1) &&             !r(-1, 0) && r(1, 0) && !r(-1, 1) && !r(0, 1)             ) indices[y * width + x] = 38;

				// 4 roots
				if (            !r(0, -1) && !r(1, -1) && r(-1, 0) &&  r(1, 0) && r(-1, 1) && r(0, 1) && r(1, 1)) indices[y * width + x] = 37;
				if (r(-1, -1) && r(0, -1) && r(1, -1) && r(-1, 0) && r(1, 0) &&              !r(0, 1)           ) indices[y * width + x] = 45;
				if (r(-1, -1) && r(0, -1)             && r(-1, 0) && !r(1, 0) && r(-1, 1) && r(0, 1)            ) indices[y * width + x] = 53;
				if (             r(0, -1) && r(1, -1) && !r(-1, 0) && r(1, 0)             && r(0, 1) && r(1, 1)) indices[y * width + x] = 61;
				if (r(-1, -1) && r(0, -1) && r(1, -1) && r(-1, 0) && r(1, 0) && r(-1, 1) && r(0, 1) && r(1, 1)) indices[y * width + x] = 21;
				//glm::vec2 distance = { x - RootX,  y - RootY };   // БЕРём вектор указывающий на клетку 


				//DefiniteCell(distance,indices[y * width + x]);
				


				

			}

		}

	}

	return indices;
}

Direction indicesGenerator::CheckDirection(glm::vec2 vector)
{
	glm::vec2 compass[] = {
		glm::vec2(-0.7f,-0.7f),  // LeftUp
		glm::vec2(0.0f,-1.0f),  // Up
		glm::vec2(0.7f,-0.7f),  // RighttUp
		glm::vec2(1.0f,0.0f),  // right
		glm::vec2(0.7f,0.7f),  // RightDown
		glm::vec2(0.0f,1.0f), // Down
		glm::vec2(-0.7f,0.7f),  // LeftDown
		glm::vec2(-1.0f,0.0f)  // Left
	};
	float max = 0.0f; // cos 0 - перпендикуляр в dot
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 8; i++) {

		float dot_product = glm::dot(glm::normalize(vector), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}


	return (Direction)best_match; // на какую сторону проекция дала ближе к 1, то и вернёт

	
}

void indicesGenerator::DefiniteCell(const glm::vec2& distance , uint8_t& currIndex)
{



	switch (CheckDirection(distance))    // в зависимости от клетки мы назначаем соотвествующую логику 
	{
	case Direction::LEFTUP:


		currIndex = 0;
		break;
	case Direction::UP:

		currIndex = 1;
		break;
	case Direction::RIGHTUP:

		currIndex = 3;
		break;
	case Direction::RIGHT:

		currIndex = 11;
		break;
	case Direction::RIGHTDOWN:

		currIndex = 27;
		break;
	case Direction::DOWN:

		currIndex  = 25;
		break;
	case Direction::LEFTDOWN:

		currIndex = 24;
		break;
	case Direction::LEFT:


		currIndex = 8;
		break;
	default:
		break;
	}

}
