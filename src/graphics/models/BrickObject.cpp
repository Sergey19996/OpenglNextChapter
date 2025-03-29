#include "BrickObject.h"

BrickObject::BrickObject() : GameObject()
{
}

BrickObject::BrickObject(glm::vec2 pos, glm::vec2 size, float angle, Texture* texture, int TextureIdx) : GameObject(pos,size,angle,texture,TextureIdx)
{
}
