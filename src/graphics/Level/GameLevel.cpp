#include "GameLevel.h"

#define SQUERE_X  384
#define SQUERE_Y  256
indicesGenerator* Generator;

GameLevel::GameLevel()
{
	Generator = new indicesGenerator();
}

GameLevel::~GameLevel()
{
	delete Generator;
}

void GameLevel::Generate(const unsigned int& SCREENWIDTH, const unsigned int& SCREENHEIGHT,Texture* texture) //этап когда мы уже генерируем числа в массив и отправляем на инициализацию уровня
{
	Bricks.clear();

	int line = SQUERE_Y / BRICK_SIZE;
	int row = SQUERE_X / BRICK_SIZE;

	//std::vector <uint8_t>& indices = Generator->AlgorithmBasicGround(line, row);

		//init(SCREENWIDTH, SCREENHEIGHT, Generator->AlgorithmBasicGround(line, row), row, line, texture);
	init(SCREENWIDTH, SCREENHEIGHT, Generator->AlgorithmLakeGround(row, line,5), row, line, texture);

}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (GameObject& brick : Bricks) {
		ResourceManager::GetShader("sprite").setInt("tileIndex", brick.TextureIdx);
		brick.Draw(renderer);
	}
}

void GameLevel::init(const unsigned int& SCREENWIDTH, const unsigned int& SCREENHEIGHT, std::vector<uint8_t>& indices,const int& row, const int& line, Texture* texture) // этап загрузки в будущий уровень рандомных чисел 
{
	

	unsigned int offsetX = SCREENWIDTH / 2 - SQUERE_X / 2;
	unsigned int offsetY = SCREENHEIGHT / 2 - SQUERE_Y / 2;

	//unsigned int SQUERE_CenterX = SQUERE_X / 2;
	//unsigned int SQUERE_CenterY = SQUERE_Y / 2;


	for (unsigned int x = 0; x < row; x++)
	{
		for (unsigned int y = 0; y < line; y++)
		{
		
			Bricks.push_back(BrickObject(glm::vec2(x * BRICK_SIZE + offsetX, y * BRICK_SIZE + offsetY), glm::vec2(BRICK_SIZE, BRICK_SIZE), 0, texture, indices[row * y + x]));
		}
	}

}
