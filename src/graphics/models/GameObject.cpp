#include "GameObject.h"

GameObject::GameObject() : pos(0.0,0.0),size(32.0f,32.0f),rotate(0),texture(nullptr),TextureIdx(0)
{

}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, float angle, Texture* texture,unsigned int TextureIndex) : pos(pos), size(size),rotate(angle), texture(texture),TextureIdx(TextureIndex)
{

}

void GameObject::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(*texture, pos, size, rotate);
}
