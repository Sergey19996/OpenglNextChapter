#include "GameLevel.h"


indicesGenerator* Generator;

GameLevel::GameLevel()
{
	Generator = new indicesGenerator();
}

GameLevel::~GameLevel()
{
	delete Generator;
}

void GameLevel::Generate(Texture* texture,const int& BRICK_SIZE, const glm::vec2& offset,const glm::ivec2& SQUERE_SIZE ) //этап когда мы уже генерируем числа в массив и отправляем на инициализацию уровня
{
	Bricks.clear();

	int width = SQUERE_SIZE.x / BRICK_SIZE;
	int height = SQUERE_SIZE.y / BRICK_SIZE;

	//std::vector <uint8_t>& indices = Generator->AlgorithmBasicGround(line, row);

		//init(SCREENWIDTH, SCREENHEIGHT, Generator->AlgorithmBasicGround(line, row), row, line, texture);
	init(Generator->AlgorithmLakeGround(width, height,5), width, height, texture,offset,BRICK_SIZE);

}

void GameLevel::Draw(SpriteRenderer& renderer)
{

	int count = 0;
	Shader& Shader = ResourceManager::GetShader("sprite");
	float TexWidth = ResourceManager::GetTexture("plates").Width;
	float TexHeight = ResourceManager::GetTexture("plates").Height;
	Shader.setvec2("spriteScale", { 128.0f / TexWidth,128.0f / TexHeight });

	for (GameObject& brick : Bricks) {
		 // Pixels_Uv_coords в диапозоне от 0 до макс width и height тектсруы
		Shader.setvec2("uv", { Pixels_UV_Coords[count].x / TexWidth,Pixels_UV_Coords[count].y / TexHeight });


		brick.Draw(renderer);
		count++;
	}
}

void GameLevel::init( std::vector<uint8_t>& indices,const int& row, const int& line, Texture* texture,const glm::ivec2& offset,const unsigned int& BRICK_SIZE) // этап загрузки в будущий уровень рандомных чисел 
{
	

	

	//unsigned int SQUERE_CenterX = SQUERE_X / 2;
	//unsigned int SQUERE_CenterY = SQUERE_Y / 2;

			float TextureSize = ResourceManager::GetTexture("plates").Width;
			int TextureRow = TextureSize / 128.0f;

	for (unsigned int x = 0; x < row; x++)
	{
		for (unsigned int y = 0; y < line; y++)
		{
			

			//brick index calc
			float ux = indices[row * y + x] % TextureRow; // Колонка (номер по X)
			float uy = indices[row * y + x] / TextureRow; //колонка  (номер по Y)

			float pxlX = ux * 128.0f;
			float pxlY = uy * 128.0f;
			
			//float uvX = pxlX / TextureSize;
			//float uvY = pxlY / TextureSize;
			Pixels_UV_Coords.push_back(glm::vec2(pxlX, pxlY));
			//
			//
			Bricks.push_back(BrickObject(glm::vec2(x * BRICK_SIZE + offset.x, y * BRICK_SIZE + offset.y), glm::vec2(BRICK_SIZE, BRICK_SIZE), 0, texture));
		}
	}

}
