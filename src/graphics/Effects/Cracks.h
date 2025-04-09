#ifndef CRACKS_H
#define CRACKS_H
#include "Particle.h"
#include "../SpriteRenderer.h"

class CracksGenerator : public ParticleGenerator {
public:
	CracksGenerator(Shader shader, Texture tex, unsigned int amount,const unsigned int frames = 3,const float lifeTime = 0.5f);
	void Update(float dt) override;

	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f)) override;
	void Draw(SpriteRenderer& renderer);


private:
	float frames;
	int currFrame;
	glm::vec2 SpriteScale;
};



#endif // !CRACKS_H
