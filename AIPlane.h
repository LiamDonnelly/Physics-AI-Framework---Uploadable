#pragma once
#include "Plane.h"
#include "PlaneParticleModel.h"
#include "GameObject.h"

#include <directxmath.h>
#include <d3d11_1.h>

class AIPlane : public Plane
{
public:
	AIPlane(GameObject* _planeBody, vector < GameObject* > _planeWheels);
	~AIPlane();

	void Update(float t);
	void Fly();
	void Wander(float t );
	void GoToTarget();

private:
	XMFLOAT3 targetPos;
	float timer;
	GameObject* obj;
};

