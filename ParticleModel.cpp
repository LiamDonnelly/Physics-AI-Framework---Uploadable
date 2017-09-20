#include "ParticleModel.h"

const float ParticleModel::_gravity = -9.81f;

ParticleModel::ParticleModel(Transform* transform, float mass)
{
	_transform = transform;
	_acceleration = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_velocity = XMFLOAT3(0.0f, 0.0f, -0.1f);
	_netForce = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_slidingForce = XMFLOAT3(0.0f, 0.0f, 0.0f);

	_dragFactor = 1.0f;
	_forceMag = 0.0f;
	_mass = mass;

	_slidingOn = false;

	_useConstAcc = true;
	_laminar = true;

	_collisionRadius = 1.5f;

	_theta = 90.0f;
}

ParticleModel::~ParticleModel()
{

}

// ----------------------- Movement ------------------------- //
void ParticleModel::MoveConstAcc(float t)
{
	t /= 1000.0f;

	XMFLOAT3 position = _transform->GetPosition();

	_constAccel.x = _velocity.x * t + 0.5f * _acceleration.x * t * t;
	_constAccel.y = _velocity.y * t + 0.5f * _acceleration.y * t * t;
	_constAccel.z = _velocity.z * t + 0.5f * _acceleration.z * t * t;

	// Update object position
	position.x += _constAccel.x;
	position.y += _constAccel.y;
	position.z += _constAccel.z;

	// Set velocity
	_velocity.x += (_acceleration.x * 1.4f) * t;
	_velocity.y += (_acceleration.y * 1.4f) * t;
	_velocity.z += (_acceleration.z * 1.4f) * t;

	_transform->SetPosition(position);
}

void ParticleModel::MoveConstVel(float t)
{
	XMFLOAT3 prevPosition = _transform->GetPosition();

	XMFLOAT3 position;
	position.x = prevPosition.x + _velocity.x * t;
	position.y = prevPosition.y + _velocity.y * t;
	position.z = prevPosition.z + _velocity.z * t;

	_transform->SetPosition(position);
}

// -------------------- Sliding -------------------- //

void ParticleModel::SlidingForce(float theta, float frCoef)
{
	_forceMag = _mass * _gravity * (sin(theta) - frCoef * cos(theta));

	_slidingForce.x = _forceMag * cos(theta);
	_slidingForce.y = _forceMag * sin(theta);
	_slidingForce.z = _forceMag * cos(theta);

	if (_slidingForce.y > 0.0f)
	{
		_slidingForce.x = 0.0f;
		_slidingForce.y = 0.0f;
	}
}

// ------------------- Drag Force ------------------ //

void ParticleModel::DragForce()
{
	if (_laminar)
	{
		// Calculate drag force for laminar flow
		DragLamFlow();
	}
	else
	{
		// Calculate drag force for turbulent flow
		TurbulantFlow();
	}
}

void ParticleModel::DragLamFlow()
{
	_dragForce.x = -_dragFactor * _velocity.x;
	_dragForce.y = -_dragFactor * _velocity.y;
	_dragForce.z = -_dragFactor * _velocity.z;
}

void ParticleModel::MotionInFluid(float t)
{
	DragForce();
	MoveConstAcc(t);
}

void ParticleModel::TurbulantFlow()
{
	float velMag;
	// Calculate magnitude of velocity
	velMag = sqrt((_velocity.x * _velocity.x) + (_velocity.y * _velocity.y) + (_velocity.y * _velocity.y));

	// Calculate unit vector of velocity
	XMFLOAT3 unitVel;
	unitVel.x = (_velocity.x / velMag);
	unitVel.y = (_velocity.y / velMag);
	unitVel.z = (_velocity.z / velMag);

	// Calculate magnitude of drag force
	float dragMag;
	dragMag = _dragFactor * velMag * velMag;

	// Calculate of x- and y-components of drag force
	_dragForce.x = -dragMag * unitVel.x;
	_dragForce.y = -dragMag * unitVel.y;
}

// ----------------- Collision Detection -------------- //

bool ParticleModel::BaseCollisionCheck(XMFLOAT3 floorPos)
{
	XMFLOAT3 objectPos = _transform->GetPosition();

	XMFLOAT3 difference;
	difference.y = objectPos.y - floorPos.y;

	float differenceMag = sqrt(difference.y * difference.y);
	//	float velocityMag = sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y) + (velocity.z * velocity.z));

	if (differenceMag <= _collisionRadius)
	{
		_velocity.y = -_velocity.y / _mass;
		//_slidingOn = true;
		return true;
	}
	else
	{
		_slidingOn = false;
	}
	return false;
}

bool ParticleModel::CollisionCheck(XMFLOAT3 object2Pos, float object2Radius)
{
	XMFLOAT3 object1Pos = _transform->GetPosition();

	XMFLOAT3 difference;
	difference.x = object1Pos.x - object2Pos.x;
	difference.y = object1Pos.y - object2Pos.y;
	difference.z = object1Pos.z - object2Pos.z;

	float differenceMag = sqrt((difference.x * difference.x) + (difference.y * difference.y) + (difference.z * difference.z));

	float radiusDistance = _collisionRadius + object2Radius;

	if (differenceMag <= radiusDistance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float ParticleModel::ResolveCollision(ParticleModel* object2)
{
	// Resolve Interpenertrations
	XMFLOAT3 obj1PrevPos = _transform->GetPrevPosition();
	XMFLOAT3 obj2PrevPos = object2->_transform->GetPrevPosition();

	XMFLOAT3 obj1Pos = _transform->GetPosition();
	XMFLOAT3 obj2Pos = object2->_transform->GetPosition();

	_transform->SetPosition(obj1PrevPos);
	object2->_transform->SetPosition(obj2PrevPos);

	// Resulting velocities
	float object1Mass = _mass;
	float object2Mass = object2->GetMass();

	XMFLOAT3 object1Vel = _velocity;
	XMFLOAT3 object2Vel = object2->GetVelocity();

	float cR = 1.0f;

	XMFLOAT3 object1NewVel;
	object1NewVel.x = (((object1Mass * object1Vel.x) + (object2Mass * object2Vel.x) + (object2Mass * cR * (object2Vel.x - object1Vel.x))) / (object1Mass + object2Mass));
	object1NewVel.y = (((object1Mass * object1Vel.y) + (object2Mass * object2Vel.y) + (object2Mass * cR * (object2Vel.y - object1Vel.y))) / (object1Mass + object2Mass));
	object1NewVel.z = (((object1Mass * object1Vel.z) + (object2Mass * object2Vel.z) + (object2Mass * cR * (object2Vel.z - object1Vel.z))) / (object1Mass + object2Mass));

	XMFLOAT3 object2NewVel;
	object2NewVel.x = (((object2Mass * object2Vel.x) + (object1Mass * object1Vel.x) + (object1Mass * cR * (object1Vel.x - object2Vel.x))) / (object1Mass + object2Mass));
	object2NewVel.y = (((object2Mass * object2Vel.y) + (object1Mass * object1Vel.y) + (object1Mass * cR * (object1Vel.y - object2Vel.y))) / (object1Mass + object2Mass));
	object2NewVel.z = (((object2Mass * object2Vel.z) + (object1Mass * object1Vel.z) + (object1Mass * cR * (object1Vel.z - object2Vel.z))) / (object1Mass + object2Mass));

	_velocity = object1NewVel;
	object2->SetVelocity(object2NewVel);

	float mag = sqrt((object1Vel.x * object1Vel.x) + (object1Vel.y * object1Vel.y) + (object1Vel.z * object1Vel.z));
	float kEnergy = ((0.5*object1Mass) * (mag * mag));
	return kEnergy;
}

// --------------------- Update ----------------------- //

void ParticleModel::UpdateAccel()
{
	_acceleration.x = _netForce.x / _mass;
	_acceleration.y = _netForce.y / _mass;
	_acceleration.z = _netForce.z / _mass;
}

void ParticleModel::UpdateNetForce()
{
	_netForce.x = _dragForce.x + _slidingForce.x;
	_netForce.y = _dragForce.y + _slidingForce.y;
	_netForce.z = _dragForce.z + _slidingForce.z;
}

void ParticleModel::UpdateState()
{
	UpdateNetForce();
	UpdateAccel();
}

void ParticleModel::Update(float t)
{
	if (b_gravity)
	{
		if (_useConstAcc)
		{
			if (_slidingOn)
			{
				SlidingForce(XMConvertToRadians(_theta), 0.2f);
			}
			else if (!_slidingOn)
			{
				_slidingForce.x = 0.0f;
				_slidingForce.y = _mass * _gravity;
				_slidingForce.z = 0.0f;
			}

			DragForce();
			UpdateState();
			MoveConstAcc(t);
		}
		else
		{
			MoveConstVel(t);
		}
	}

	else
	{
		MoveConstVel(t);
	}
}