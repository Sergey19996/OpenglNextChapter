#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader& shader,unsigned int widthX, unsigned int height,unsigned int cellSize)
{
	this->shader = shader;
	this->initRendererData(widthX,height,cellSize);
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
	this->shader.use();


	glm::mat4 trans = glm::mat4(1.0f);
	glm::vec3 scale = { 32.0f,32.0f,1.0 };

	trans = glm::translate(trans, glm::vec3(position, 1.0f));

	trans = glm::translate(trans, glm::vec3(size.x * 0.5f, size.y * 0.5f, 0.0f)); // move origin in center
	trans = glm::rotate(trans, (3.1415f * 0.5f) * rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::translate(trans, glm::vec3(size.x * -0.5f, size.y * -0.5f, 0.0f)); //move origin back
	trans = glm::scale(trans, glm::vec3(size,1.0f));
	shader.setMat4("model", trans);


	glActiveTexture(GL_TEXTURE0);  // ��� �� ��������� ������ � �������� 
	texture.Bind();

	glBindVertexArray(this->quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 6 indices
	glBindVertexArray(0);
}

void SpriteRenderer::initRendererData(unsigned int widthX, unsigned int height,unsigned int CellSize)
{
	
	 

	////  VAO/VBO
	float vertices[] = {
	// vrtx         UV
   1.0f,  1.0f,		1,1,		 // top right    0
   1.0f,  0.0f,		1,0,			// bottom right 1
   0.0f,  1.0f,		0,1,			// top left     2
   // vrtx           UV
   0.0f, 0.0f, 		0,0,			// bottom left  3
	};
	unsigned int indices[] = {
		0,3,2,
		1,3,0
	};



	unsigned int  VBO, EBO;
	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// 1. bind Vertex Array Object
	glBindVertexArray(this->quadVAO);


	// // 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // layout = 0  // ������� ��� ������ ������ � VBO
	glEnableVertexAttribArray(0);
	// ������������� VAO (�������������, �� ������� ��������)
	glBindVertexArray(0);
	//


}
