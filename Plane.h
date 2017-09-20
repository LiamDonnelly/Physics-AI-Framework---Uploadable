#pragma once

#include "PlaneParticleModel.h"
#include "GameObject.h"

class Plane
{
public:
	Plane(GameObject* _planeBody, vector < GameObject* > _planeWheels);
	~Plane();

	virtual void Update(float t);
	void Draw(ID3D11DeviceContext* _pImmediateContext);

	// Plane Input
	void Input();

	// Get Plane Body and Wheels
	GameObject* GetPlaneBody() const { return planeBody; };
	vector< GameObject* > GetPlaneWheels() const { planeWheels; };

	// Get/Set Plane Position
	XMFLOAT3 GetPlanePosition() const { return planePos; };
	void SetPlanePosition(XMFLOAT3 _planePos) { planePos = _planePos; };
	void SetPlanePosition(float x, float y, float z) { planePos.x = x, planePos.y = y, planePos.z = z; };

	// Get/Calculate Plane Forward Vector
	XMFLOAT3 GetForwardVector() const { return planeForwardVector; };
	void CalculateForwardVector();

	bool isFlying() const { return flying; };
	void setFlying(bool value) { flying = value; };

	bool isExploaded() const { return exploaded; };
	void setExploaded(bool value) { exploaded = value; };

	// Get/Set Plane Rotation
	float GetPlaneRotation() const { return planeYawRotation; };
	void SetPlaneRotation(float _planeRotation) {
		planeYawRotation = _planeRotation; planeYawRotation = planeYawRotation * (XM_PI / 180);
		planeBody->GetTransform()->SetRotation(0.0f, planeYawRotation, 0.0f);
	};
	void AddPlaneRotation(float _planeRotation) {
		planeYawRotation = _planeRotation; planeYawRotation = planeYawRotation * (XM_PI / 180);
		planeBody->GetTransform()->SetRotation(0.0f, planeYawRotation, 0.0f);
	};

	// Get/Set Plane Rotation Speed
//	float GetPlaneRotationSpeed() const { return planeRotationSpeed; };
//	void SetPlaneRotationSpeed(float _planeRotationSpeed) { planeRotationSpeed = planeRotationSpeed; };

	// Get/Set Plane Wheel Rotation
	float GetPlaneWheelRotation() const { return planeWheelRotation; };
	void SetPlaneWheelRotation(float _planeWheelRotation) { planeWheelRotation = _planeWheelRotation; };
	void AddPlaneWheelRotation(float _planeWheelRotation) { planeWheelRotation += _planeWheelRotation; };

	// Get/Set Plane Wheel Rotation
	float GetPlaneRearFlapRotation() const { return planeRearFlap; };
	void SetPlaneRearFlapRotation(float _planeflapRotation) { planeRearFlap = _planeflapRotation; };
	void AddPlaneRearFlapRotation(float _planeflapRotation) { planeRearFlap += _planeflapRotation; };
	void SetPlaneYawRotation(float _planeYawRotation) { planeYawRotation = _planeYawRotation; };
private:
	// Plane Object
	GameObject* planeBody;
	vector < GameObject* > planeWheels;

	// Plane Properties
	XMFLOAT3 planePos;
	XMFLOAT3 planeForwardVector;

	float planeYawRotation;
	float planeRotationSpeed;
	float planeWheelRotation;
	float planeRearFlap;
	float planePitchRotation;
	float planeRollRotation;
	bool flying;
	bool exploaded;

	float engineSpeedAdd;
};