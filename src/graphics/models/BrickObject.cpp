#include "BrickObject.h"

BrickObject::BrickObject() : GameObject()
{
}

BrickObject::BrickObject(glm::vec2 pos, glm::vec2 size, float angle, Texture* texture) : GameObject(pos,size,angle,texture)
{
}
