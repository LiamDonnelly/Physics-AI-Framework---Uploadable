#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <vector>
#include "Particle.h"
#include "ParticleModel.h"
#include "Transform.h"
#include "ParticleSystem.h"
using namespace DirectX;
class ParticleSystem
{
public:
	ParticleSystem(Appearance* _apperance);
	~ParticleSystem();

	void Init();
	void Release();
	void Update(float t);

	void SetMaxParticles(int value) { maxParticles = value; }
	void SetStartPos(XMFLOAT3 value) { startPos = value; }
	XMFLOAT3 GetCamPos() { return camPos; }
	void SetCamPos(XMFLOAT3 value) { camPos = value; }
	void SetAge(float value) { age = value; }
	float GetAge() { return age; }
	vector<Particle*> GetParticalSystems() { return particles; }
	void Reset(Particle* var);
	void ResetAll();
private:

	std::vector<Particle*> particles;
	Appearance* apperance;
	XMFLOAT3 startPos;
	XMFLOAT3 camPos;
	int maxParticles;
	float age;

};

