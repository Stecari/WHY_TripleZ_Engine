#include "tzpch.h"

#include "ParticleSystem.h"

#include "glm/gtc/constants.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/compatibility.hpp"

#include "Random.h"

ParticleSystem::ParticleSystem(uint32_t MaxParticle)
	:ParticleIndex(MaxParticle - 1)
{
	ParticlePool.resize(MaxParticle);
}

void ParticleSystem::OnUpdata(TribleZ::TimeStep time_step)
{
	for (auto& particle : ParticlePool)
	{
		if (!particle.Active)
		{
			continue;
		}
		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= time_step;
		particle.position += particle.velocity * (float)time_step;
		particle.rotation += 90.0f * time_step;
	}
}

void ParticleSystem::OnRender(TribleZ::OrthoGraphicCamera& camera)
{
	TribleZ::Renderer2D::SceneBegin(camera);
	for (auto& particle : ParticlePool)
	{
		if (!particle.Active)
		{
			continue;
		}

		//粒子消散
		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);	//lerp线性插值 大概是在给定的两个值之间某个比例处的值
		float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);			//如  前一个值为5， 后一个值为55， 第三个值为 0.5， 则返回（55 - 5）* 0.5 + 5
	
		TribleZ::Renderer2D::DrawRotationQuad(particle.position, glm::radians(particle.rotation), { size, size }, color);
	}
	TribleZ::Renderer2D::SceneEnd();
}

void ParticleSystem::Emit(const ParticleProps& particle_props)
{
	//粒子基础属性设置
	Particle& emit_particle = ParticlePool[ParticleIndex];
	emit_particle.Active = true;										//活动状态：活动
	emit_particle.position = particle_props.Position;					//位置
	emit_particle.rotation = Random::Float() * 2.0f * glm::pi<float>();	//产生一个0 ~ 2兀 之间的弧度；

	//速度设置
	emit_particle.velocity = particle_props.Velocity;
	emit_particle.velocity.x += particle_props.VelocityVariation.x * (Random::Float() - 0.5f);
	emit_particle.velocity.y += particle_props.VelocityVariation.y * (Random::Float() - 0.5f);

	//颜色设置
	emit_particle.colorBegin = particle_props.ColorBegin;
	emit_particle.colorEnd = particle_props.ColorEnd;

	//尺寸和生存周期设置
	emit_particle.sizeBegin = particle_props.SizeBegin + particle_props.SizeVariation * (Random::Float() - 0.5f);
	emit_particle.sizeEnd = particle_props.SizeEnd;
	emit_particle.LifeTime = particle_props.LifeTime;
	emit_particle.LifeRemaining = particle_props.LifeTime;

	ParticleIndex = --ParticleIndex % ParticlePool.size();
}