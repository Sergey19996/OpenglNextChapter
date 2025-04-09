#ifndef  PARTICLE_H
#define PARTICLE_H
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include "../rendering/Shader.h"
#include "../rendering/Texture.h"
#include "../models/GameObject.h"

struct Particle
{
	glm::vec2 Pos;
	glm::vec2 Velocity;
	glm::vec2 InitialVel;
	glm::vec2 Coord;
	glm::vec4 Color;
		float Life;
		float InitialLife;
		float Scale;
		float InitialScale;


		Particle() : Pos(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f),Coord(0.0f),Scale(1.0f),InitialLife(0.0f),InitialScale(0.0f),InitialVel(0.0f) {}

};



class ParticleGenerator {
public:
	ParticleGenerator(Shader shader, Texture texture, unsigned int amount, bool isRandCoord = true);
	
	virtual void Update(float dt);

	void Draw();
	void Spawn(GameObject& object, unsigned int newParticles, glm::vec2 offset);
	void reset( bool isRandCoord);

protected:
	std::vector<Particle> particles;
	unsigned int amount;

	Shader shader;
	Texture texture;
	unsigned int VAO;

	void init(bool& isRandCoord);

	 int firstUnusedParticle();

	virtual void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));


};






#endif // ! PARTICLE_H
