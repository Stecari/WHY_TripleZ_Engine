#pragma once

#include "TribleZ.h"

struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
};


class ParticleSystem
{
private:
	struct Particle
	{
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec4 colorBegin, colorEnd;
		float rotation;
		float sizeBegin, sizeEnd;
		
		float LifeTime = 1.0f;			//生存周期
		float LifeRemaining = 0.0f;		//持续参数

		bool Active = false;
	};

	std::vector<Particle> ParticlePool;
	uint32_t ParticleIndex = 999;
public:
	ParticleSystem(uint32_t MaxParticle = 5000);

	void OnUpdata(TribleZ::TimeStep time_step);
	void OnRender(TribleZ::OrthoGraphicCamera& camera);
	
	void Emit(const ParticleProps& particle_props);
};