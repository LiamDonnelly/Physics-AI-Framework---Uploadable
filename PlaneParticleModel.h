#pragma once

#include "ParticleModel.h"

class PlaneParticleModel : public ParticleModel
{
public:
	PlaneParticleModel(Transform* transform, float mass);
	~PlaneParticleModel();

	void CalculateWheelSpeed();
	void CalculateThrust(float t);
	void CalculateVelocity();
	void CalculateUpThrust();
	void CalculateWindSpeed();
	void TruncateSpeed();

	void Update(float t);


	// --------------- Get/Set ------------------ //

	// Get/Set plane Direction
	XMFLOAT3 GetPlaneDirection() const { return _planeDirection; }
	void SetPlaneDirection(XMFLOAT3 planeDirection) { _planeDirection = planeDirection; }

	// Get/Set plane Velocity
	XMFLOAT3 GetPlaneVelocity() const { return _planeVelocity; }
	void SetEngineSpeed(XMFLOAT3 planeVelocity) { _planeVelocity = planeVelocity; }

	// Get/Set/Add Engine Speed
	float GetEngineSpeed() const { return _engineSpeed; }
	void SetEngineSpeed(float engineSpeed) { _engineSpeed = engineSpeed; }
	void AddEngineSpeed(float engineSpeed);

	// Get/Set/Add Engine Speed Limit
	float GetEngineSpeedLimit() const { return _engineSpeedLimit; }
	void SetEngineSpeedLimit(float engineSpeedLimit) { _engineSpeedLimit = engineSpeedLimit; }

	float GetWingFlapAngle() const { return _wingFlapAngle; }
	void SetWingFlapAngle(float angle) { _wingFlapAngle += angle; }
	void ZeroFlapAngle() { _wingFlapAngle = 0.0f; }

	float GetWingSpeed() const { return _windSpeed; }
	void SetWindSpeed(float value) { _windSpeed = value; }
private:
	float _wheelRadius;
	float _wingResistance;
	float _jetSpeed;
	float _windSpeed;
	float _wingFlapAngle;
	int	_gearRatio;

	float _engineSpeed;
	float _engineSpeedLimit;

	float _thrust;

	bool _gravity;
	XMFLOAT3 _planeVelocity;
	XMFLOAT3 _planeDirection;
};