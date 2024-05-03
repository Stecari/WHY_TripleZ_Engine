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

		//������ɢ
		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);	//lerp���Բ�ֵ ������ڸ���������ֵ֮��ĳ����������ֵ
		float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);			//��  ǰһ��ֵΪ5�� ��һ��ֵΪ55�� ������ֵΪ 0.5�� �򷵻أ�55 - 5��* 0.5 + 5
	
		TribleZ::Renderer2D::DrawRotationQuad(particle.position, glm::radians(particle.rotation), { size, size }, color);
	}
	TribleZ::Renderer2D::SceneEnd();
}

void ParticleSystem::Emit(const ParticleProps& particle_props)
{
	//���ӻ�����������
	Particle& emit_particle = ParticlePool[ParticleIndex];
	emit_particle.Active = true;										//�״̬���
	emit_particle.position = particle_props.Position;					//λ��
	emit_particle.rotation = Random::Float() * 2.0f * glm::pi<float>();	//����һ��0 ~ 2أ ֮��Ļ��ȣ�

	//�ٶ�����
	emit_particle.velocity = particle_props.Velocity;
	emit_particle.velocity.x += particle_props.VelocityVariation.x * (Random::Float() - 0.5f);
	emit_particle.velocity.y += particle_props.VelocityVariation.y * (Random::Float() - 0.5f);

	//��ɫ����
	emit_particle.colorBegin = particle_props.ColorBegin;
	emit_particle.colorEnd = particle_props.ColorEnd;

	//�ߴ��������������
	emit_particle.sizeBegin = particle_props.SizeBegin + particle_props.SizeVariation * (Random::Float() - 0.5f);
	emit_particle.sizeEnd = particle_props.SizeEnd;
	emit_particle.LifeTime = particle_props.LifeTime;
	emit_particle.LifeRemaining = particle_props.LifeTime;

	ParticleIndex = --ParticleIndex % ParticlePool.size();
}