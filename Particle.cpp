#include "Particle.h"


Particle::Particle(Appearance* _apperance)
{
	appearance = _apperance;
	
}

void Particle::Init()
{
	transform = new Transform();
	transform->SetPosition(pos);
	transform->SetScale(size, size, size);
	transform->SetRotation(0.0f, 0.0f, 0.0f);
	particleModel = new ParticleModel(transform, 1.0f);
	particleModel->SetVelocity(vel);
	particleModel->SetGravity(false);
}

void Particle::Update(float t)
{

	age -= t;

	UpdateRotation();
	transform->Update(t);
	particleModel->Update(t);
}

void Particle::Release()
{
}

void Particle::Draw(ID3D11DeviceContext * pImmediateContext)
{
	appearance->Draw(pImmediateContext);
}


Particle::~Particle()
{
}

void Particle::UpdateRotation()
{
	float angle = atan2(cam.x - transform->GetPosition().x, cam.z - transform->GetPosition().z) * (180 / XM_PI);
	//float angle = atan2(transform->GetPosition().x - cam.x, transform->GetPosition().z - cam.z) * (180 / XM_PI);
	float rotation = (float)angle * 0.0174532925f;
	transform->SetRotation(XMFLOAT3(transform->GetRotation().x, rotation, transform->GetRotation().z));
}
