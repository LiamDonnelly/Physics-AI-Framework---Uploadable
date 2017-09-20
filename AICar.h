#pragma once

#include "Plane.h"
#include "GameObject.h"

#include <directxmath.h>
#include <d3d11_1.h>

class AICar : public Plane
{
public:
	AICar(GameObject* _carBody, vector < GameObject* > _carWheels, vector < GameObject* > _waypoints);
	~AICar();

	void Update(float t);

	// ----------------- PATH FINDING -------------------- //
	void Pathfinding();

	// --------------- STEERING BEHAVIOUR ---------------- //
	void GoToTarget();

private:
	// Path Finding
	vector < GameObject* > waypoints;

	bool waypointReached;

	XMFLOAT3 targetPosition;
	int targetIndex;
};

