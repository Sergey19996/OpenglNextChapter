#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H
#include "rendering/Shader.h"
#include "rendering/Texture.h"


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class SpriteRenderer {
public:

	SpriteRenderer(Shader& shader,unsigned int SizeX, unsigned int SizeY,unsigned int CellSize);
	~SpriteRenderer();


	void DrawSprite(Texture& texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(32.0f, 32.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));




private:

	Shader shader;
	unsigned quadVAO;  // хранит вертексы квадрата


	void initRendererData(unsigned int SizeX, unsigned int SizeY,unsigned int CellSize);



};


#endif // !SPRITE_RENDERER_H
