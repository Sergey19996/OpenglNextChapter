#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <glm/glm.hpp>
#include "../SpriteRenderer.h"

class GameObject {
public:

	glm::vec2 pos;
	glm::vec2 size;
	float rotate;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, float angle,Texture* texture);

	Texture* texture;
	virtual void Draw(SpriteRenderer& renderer);


};
#endif // !GAME_OBJECT_J
