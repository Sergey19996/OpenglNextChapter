#include "GameLevel.h"
#include "../Effects/Particle.h"
#include "../Effects/Cracks.h"

indicesGenerator* Generator;
ParticleGenerator* particle;
CracksGenerator* Cracks;
GameLevel::GameLevel()
{
	Generator = new indicesGenerator();
	particle = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("Stones"), 100);
	Cracks = new CracksGenerator(ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("Cracks"), 100);
}

GameLevel::~GameLevel()
{
	delete Generator;
	delete particle;
	delete Cracks;
}

void GameLevel::Generate(Texture* texture,const int& BRICK_SIZE, const glm::vec2& offset,const glm::ivec2& SQUERE_SIZE,unsigned int WaterRoots ) //���� ����� �� ��� ���������� ����� � ������ � ���������� �� ������������� ������
{
	Bricks.clear();
	MarkedBricks.clear();
	Pixels_UV_Coords.clear();

	int width = SQUERE_SIZE.x / BRICK_SIZE;
	int height = SQUERE_SIZE.y / BRICK_SIZE;

	//std::vector <uint8_t>& indices = Generator->AlgorithmBasicGround(line, row);

		//init(SCREENWIDTH, SCREENHEIGHT, Generator->AlgorithmBasicGround(line, row), row, line, texture);
	init(Generator->AlgorithmLakeGround(width, height,WaterRoots), width, height, texture,offset,BRICK_SIZE);

	MarkedBricks.reserve(Bricks.size() / 4);
	Pixels_UV_Coords.reserve(width * height);
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	particle->Draw();
	Cracks->Draw(renderer);

	int count = 0;
	Shader& Shader = ResourceManager::GetShader("sprite");
	Shader.use();
	float TexWidth = ResourceManager::GetTexture("plates").Width;
	float TexHeight = ResourceManager::GetTexture("plates").Height;
	Shader.setvec2("spriteScale", { 128.0f / TexWidth,128.0f / TexHeight });

	for (GameObject& brick : Bricks) {
		 // Pixels_Uv_coords � ��������� �� 0 �� ���� width � height ��������
		Shader.setvec2("uv", { Pixels_UV_Coords[count].x / TexWidth,Pixels_UV_Coords[count].y / TexHeight });
		Shader.setvec4("color", brick.color);
		if(!brick.bDestroyed)
		brick.Draw(renderer);
		count++;
	}
}

void GameLevel::Update(float dt)
{
	particle->Update(dt);
	Cracks->Update(dt);
	if (MarkedBricks.size() > 0) {


		MarkedBricks.erase(std::remove_if(MarkedBricks.begin(), MarkedBricks.end(), [](GameObject* obj) { return obj->bDestroyed; }), MarkedBricks.end());



		for (GameObject* obj : MarkedBricks) {

			if (obj->color.g > 0)
			{
			obj->color = { obj->color.r,obj->color.g - dt ,obj->color.b - dt ,obj->color.a };

			}
			else
			{
				obj->bDestroyed = true;
				
				
				particle->Spawn(*obj, 1, { obj->size.x / 2.0f - 5,obj->size.y / 2.0f - 5  });  //center - 5 half of particle tex (10)
				particle->Spawn(*obj, 1, { 0 ,0}); // left up
				particle->Spawn(*obj, 1, { obj->size.x - 10 ,obj->size.y  });// right up
				particle->Spawn(*obj, 1, { obj->size.x ,obj->size.y- 10 });// right dow
				particle->Spawn(*obj, 1, { 0 ,obj->size.y - 10 });// left up

				Cracks->Spawn(*obj, 1, { 0.0f,0.0f });

				
			
			}

		}
	}
}

bool GameLevel::IsCompleted()
{
	for (GameObject& brick : Bricks) {



		if (!brick.prepared && !brick.IsSolid && !brick.IsDanger)
			return false;

		/*if (!brick.bDestroyed)
			return false;*/
	}



	return true;
}

void GameLevel::PreparateDestroy(GameObject* Brick)
{
	MarkedBricks.push_back(Brick);
	Brick->prepared = true;

}

void GameLevel::init( std::vector<uint8_t>& indices,const int& row, const int& line, Texture* texture,const glm::ivec2& offset,const unsigned int& BRICK_SIZE) // ���� �������� � ������� ������� ��������� ����� 
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
			float ux = indices[row * y + x] % TextureRow; // ������� (����� �� X)
			float uy = indices[row * y + x] / TextureRow; //�������  (����� �� Y)

			float pxlX = ux * 128.0f;
			float pxlY = uy * 128.0f;
			
			//float uvX = pxlX / TextureSize;
			//float uvY = pxlY / TextureSize;
			Pixels_UV_Coords.push_back(glm::vec2(pxlX, pxlY));
			//
			GameObject Box(glm::vec2(x * BRICK_SIZE + offset.x, y * BRICK_SIZE + offset.y), glm::vec2(BRICK_SIZE, BRICK_SIZE), 0, texture);
			
			if (indices[row * y + x] == 7) //water index
			{
				Box.IsDanger = true;
			}

			Bricks.push_back(Box);
		}
	}

}
