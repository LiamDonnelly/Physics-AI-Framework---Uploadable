#include "AIPlane.h"



AIPlane::AIPlane(GameObject* _planeBody, vector < GameObject* > _planeWheels) : Plane(_planeBody, _planeWheels)
{
	targetPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	obj = _planeBody;
	PlaneParticleModel* planeBodyModel = (PlaneParticleModel*)GetPlaneBody()->GetParticleModel();
	planeBodyModel->SetEngineSpeedLimit(0.2f);
	planeBodyModel->SetEngineSpeed(0.2f);
	planeBodyModel->SetWindSpeed(10.0f);
	timer = -1.0f;
}


AIPlane::~AIPlane()
{
}

void AIPlane::Update(float t)
{
	Fly();
	if(Plane::isFlying())
		Wander(t);
	GoToTarget();
	Plane::Update(t);
}

void AIPlane::Fly()
{
	PlaneParticleModel* planeBodyModel = (PlaneParticleModel*)GetPlaneBody()->GetParticleModel();

	if (GetPlaneBody()->GetTransform()->GetPosition().y < 50.0f)
	{
		planeBodyModel->SetWingFlapAngle(1.0f);
		
	}
	else
	{
		planeBodyModel->ZeroFlapAngle();
		planeBodyModel->SetWindSpeed(0.0f);
	}
}
void AIPlane::Wander(float t)
{
	if (timer < 0.0f)
	{
		timer = 5000;

		XMVECTOR vcircleCenter = XMLoadFloat3(&GetPlaneBody()->GetParticleModel()->GetVelocity());
		XMVector3Normalize(vcircleCenter);
		XMVectorScale(vcircleCenter, 10.0f);

		XMFLOAT3 cirleCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMStoreFloat3(&cirleCenter, vcircleCenter);

		float angle = rand() % 360;
		targetPos = XMFLOAT3(cirleCenter.x + 15.0f * cos(XMConvertToRadians(angle)), 0.0f, cirleCenter.z + 15.0f * sin(XMConvertToRadians(angle)));
	}
	timer -= t;
	

}

void AIPlane::GoToTarget()
{
	XMFLOAT3 planeForwardVector = GetForwardVector();
	XMFLOAT3 planePos = GetPlaneBody()->GetTransform()->GetPosition();

	XMFLOAT3 upDirection = XMFLOAT3(planePos.x, planePos.y + 10.0f, planePos.z);
	upDirection.x = upDirection.x - planePos.x;
	upDirection.y = upDirection.y - planePos.y;
	upDirection.z = upDirection.z - planePos.z;

	XMFLOAT3 forwardDirection;
	forwardDirection.x = planePos.x - (planePos.x + planeForwardVector.x);
	forwardDirection.y = planePos.y - (planePos.y + planeForwardVector.y);
	forwardDirection.z = planePos.z - (planePos.z + planeForwardVector.z);

	XMFLOAT3 targetDirection;
	targetDirection.x = targetPos.x - planePos.x;
	targetDirection.y = targetPos.y - planePos.y;
	targetDirection.z = targetPos.z - planePos.z;

	XMFLOAT3 crossProduct;
	crossProduct.x = (forwardDirection.y * targetDirection.z) - (forwardDirection.z * targetDirection.y);
	crossProduct.y = (forwardDirection.z * targetDirection.x) - (forwardDirection.x * targetDirection.z);
	crossProduct.z = (forwardDirection.x * targetDirection.y) - (forwardDirection.y * targetDirection.x);

	float dotProduct = (crossProduct.x + upDirection.x) + (crossProduct.y + upDirection.y) + (crossProduct.z + upDirection.z);

	PlaneParticleModel* planeBodyTemp = (PlaneParticleModel*)GetPlaneBody()->GetParticleModel();
	float engineSpeed = planeBodyTemp->GetEngineSpeed();

	if (dotProduct < 5.0f && dotProduct > -5.0f)
	{
		planeBodyTemp->AddEngineSpeed(0.0008f);
		SetPlaneRearFlapRotation(0.0f);
	}
	else if (dotProduct > 5.0f)
	{
		float  rot = ((float)rand()) / RAND_MAX;
		SetPlaneRearFlapRotation(-rot);

		if (engineSpeed < 0.1)
		{
			planeBodyTemp->AddEngineSpeed(0.0002f);
		}
		else if (engineSpeed > 0.1)
		{
			planeBodyTemp->AddEngineSpeed(-0.0002f);
		}
	}
	else if (dotProduct < 5.0f)
	{
		float  rot = ((float)rand()) / RAND_MAX  ;
		SetPlaneRearFlapRotation(rot);

		if (engineSpeed < 0.1)
		{
			planeBodyTemp->AddEngineSpeed(0.0002f);
		}
		else if (engineSpeed > 0.1)
		{
			planeBodyTemp->AddEngineSpeed(-0.0002f);
		}
	}

	engineSpeed = planeBodyTemp->GetEngineSpeed();

	GameObject* planetemp = GetPlaneBody();
	float planeRotation = GetPlaneBody()->GetTransform()->GetRotation().y;
	float planeWheelRotation = GetPlaneWheelRotation();
	float planeRotationSpeed = 0.1f;
}
