#pragma once
#include "GameObject.h"

class Particle
{
public:
	Particle(Appearance* appearance);
	void Init();
	void Update(float t);
	void Release();
	void Draw(ID3D11DeviceContext* pImmediateContext);
	~Particle();

	void UpdateRotation();

	XMFLOAT3 GetPos() { return pos; }
	void SetPos(XMFLOAT3 value) { pos = value; }

	XMFLOAT3 GetVel() { return vel; }
	void SetVel(XMFLOAT3 value) { vel = value; }

	XMFLOAT3 GetRot() { return rot; }
	void SetRot(XMFLOAT3 value) { rot = value; }

	XMFLOAT3 GetCamPos() { return cam; }
	void SetCamPos(XMFLOAT3 value) { cam = value; }

	float GetAge() { return age; }
	void SetAge(float value) { age = value; }

	bool GetAlive() { return alive; }
	void SetAlive(float value) { alive = value; }

	float GetSize() { return size; }
	void SetSize(float value) { size = value; }

	Appearance* GetAppearance() const { return appearance; };
	Transform* GetTransform() const { return transform; };
	ParticleModel* GetParticleModel() { return particleModel; };
private:
	XMFLOAT3 pos;
	XMFLOAT3 vel;
	XMFLOAT3 rot;
	XMFLOAT3 cam;

	float age;
	bool alive;
	float size;

	string type;
	Appearance* appearance;
	Transform* transform;
	ParticleModel* particleModel;
};

