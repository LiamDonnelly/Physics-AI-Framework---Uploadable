#pragma once

#include <directxmath.h>
#include <d3d11_1.h>

#include "Transform.h"

using namespace DirectX;
using namespace std;

class ParticleModel
{
public:
	ParticleModel(Transform* transform, float mass);
	~ParticleModel();

	// --------------- Movement ----------------

	void MoveConstVel(float t);
	void MoveConstAcc(float t);
	
	// -------------- Drag Force ---------------- //

	void DragForce();
	void DragLamFlow();
	void MotionInFluid(float t);
	void TurbulantFlow();

	// --------------- Sliding ------------------ //

	void SlidingMotion();
	void SlidingForce(float theta, float frCoef);

	// ------------ Collision Check ------------- //

	bool BaseCollisionCheck(XMFLOAT3 floorPos);
	bool CollisionCheck(XMFLOAT3 object2Pos, float object2Radius);
	float ResolveCollision(ParticleModel* object2);

	// --------------- Updates ------------------ //

	void UpdateNetForce();
	void UpdateAccel();
	void UpdateState();
	virtual void Update(float t);

	// --------------- Get/Set ------------------ //

	// Get/Set Collision Radius
	float GetCollisionRadius() const { return _collisionRadius; }
	void SetCollisionRadius(float collisionRadius) { _collisionRadius = collisionRadius; }

	// Get/Set Mass
	float GetMass() const { return _mass; };
	void SetMass(float mass) { _mass = mass; }

	// Get/Set Mass
	bool GetGravity() const { return b_gravity; };
	void SetGravity(float bol) { b_gravity = bol; }

	// Get/Set Velocity
	XMFLOAT3 GetVelocity() const { return _velocity; }
	void SetVelocity(XMFLOAT3 velocity) { _velocity = velocity; }

	XMFLOAT3 GetAccl() const { return _acceleration; }
	void SetAccl(XMFLOAT3 accel) { _acceleration = accel; }

	XMFLOAT3 GetcAccl() const { return _constAccel; }
	void SetcAccl(XMFLOAT3 accel) { _constAccel = accel; }

	XMFLOAT3 GetDragForce() const { return _dragForce; }
	void SetDragForce(XMFLOAT3 force) { _dragForce = force; }

	XMFLOAT3 GetNetForce() const { return _netForce; }
	void SetNetForce(XMFLOAT3 force) { _netForce = force; }

	void SetTheta(float theta) { _theta = theta; }

protected:
	Transform* _transform;

private:
	// Movement
	XMFLOAT3 _acceleration;
	XMFLOAT3 _velocity;

	XMFLOAT3 _constAccel;

	bool _useConstAcc;

	// Forces
	XMFLOAT3 _netForce;
	float _forceMag;

	// Drag Force
	XMFLOAT3 _dragForce;
	float _dragFactor;
	bool _laminar;

	// Sliding
	XMFLOAT3 _slidingForce;
	bool _slidingOn;

	// Collision Check
	float _collisionRadius;

	// Other
	static const float _gravity;
	bool b_gravity = true;
	float _mass;

	float _theta;
};