#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(Appearance* _apperance)
{
	apperance = _apperance; 
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Init()
{
	for (int i = 0; i < maxParticles; i++)
	{
		
		float  x = ((float)rand()) / RAND_MAX * 2.0 - 1.0; x = x / 100;
		float  z = ((float)rand()) / RAND_MAX * 2.0 - 1.0; z = z / 100;
		float  y = ((float)rand()) / RAND_MAX;			   y = y / 100;


		Particle* particle = new Particle(apperance);
		particle->SetPos(startPos);
		particle->SetAge(age);
		particle->SetSize(2.0f);
		particle->SetVel(XMFLOAT3(x,y,z));
		particle->SetCamPos(camPos);
		particles.push_back(particle);
		particle->Init();
	}
}

void ParticleSystem::Release()
{
}

void ParticleSystem::Update(float t)
{
	for each (Particle* var in particles)
	{
		if (var->GetAge() > 0)
		{
			var->SetCamPos(camPos);
			var->Update(t);
		}
		else
		{
			Reset(var);
		}
	}
}

void ParticleSystem::Reset(Particle* var)
{
	var->GetTransform()->SetPosition(startPos);
	var->SetAge(age);
}

void ParticleSystem::ResetAll()
{
	for each (Particle* var in particles)
	{
			Reset(var);
	}
}
