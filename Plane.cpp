#include "Plane.h"
#include <iostream>

Plane::Plane(GameObject* _planeBody, vector < GameObject* > _planeWheels)
{
	planeBody = _planeBody;
	planeWheels = _planeWheels;
	planePos = planeBody->GetTransform()->GetPosition();

	planeRotationSpeed = 0.001f;
	planePitchRotation = 0.0f;
	planeYawRotation = 0.0f;
	planeRollRotation = 0.0f;
	planeWheelRotation = 0.0f;
	planeRearFlap = 0.0f;
	engineSpeedAdd = 0.0008f;

	flying = false;
	exploaded = false;
}

Plane::~Plane()
{

}

void Plane::Input()
{
	PlaneParticleModel* planeBodyModel = (PlaneParticleModel*)planeBody->GetParticleModel();
	float engineSpeed = planeBodyModel->GetEngineSpeed();

	planePos = planeBody->GetTransform()->GetPosition();

	if (GetAsyncKeyState('B'))
	{
		planePos = XMFLOAT3(planePos.x, 20.0f, planePos.z);
		planeBody->GetTransform()->SetPosition(planePos);
	}

	if (GetAsyncKeyState('W'))
	{
		// Set plane Engine Speed
		planeBodyModel->AddEngineSpeed((engineSpeedAdd));
	}
	else if (GetAsyncKeyState('S'))
	{
		if (engineSpeed > 0)
		{
			planeBodyModel->AddEngineSpeed(-engineSpeedAdd * 2.0f);
		}
		else
		{
			planeBodyModel->AddEngineSpeed((-engineSpeedAdd / 1.5f));
		}
	}
	else
	{
		float engineSpeed = planeBodyModel->GetEngineSpeed();

		if (engineSpeed > 0)
		{
			planeBodyModel->AddEngineSpeed(-engineSpeedAdd);

			if (engineSpeed < 0.0008f && engineSpeed != 0.0f)
			{
				planeBodyModel->SetEngineSpeed(0.0f);
			}
		}
		else if (engineSpeed < 0)
		{
			planeBodyModel->AddEngineSpeed(engineSpeedAdd);
		}
	}

	// plane Rotation Check
	if (GetAsyncKeyState('A'))
	{
		if (planeRearFlap > -0.05f)
			planeRearFlap -= 0.001f;
		else
			planeRearFlap = -0.05f;


		if (flying)
		{
			planeRollRotation += 0.1f;
			if (planeRollRotation > 5.0f)
			{
				planeRollRotation = 5.0f;
			}

		}
	}
	else if (GetAsyncKeyState('D'))
	{
		if (planeRearFlap < 0.05f)
			planeRearFlap += 0.001f;
		else
			planeRearFlap = 0.05f;

		if (flying)
		{
			planeRollRotation -= 0.1f;

			if (planeRollRotation < -5.0f)
			{
				planeRollRotation = -5.0f;
			}
		}
	}
	else
	{
		if (planeRearFlap < 0)
		{
			planeRearFlap += 0.02f;
		}
		else if (planeRearFlap > 0)
		{
			planeRearFlap -= 0.02f;
		}

		if (planeRearFlap < 0.01f && planeRearFlap > -0.01f)
		{
			planeRearFlap = 0;
		}

		if (planeRollRotation > 0.0f)
		{
			planeRollRotation -= 0.01f;
		}
		else if (planeRollRotation < 0.0f)
		{
			planeRollRotation += 0.01f;
		}
	}

	if (GetAsyncKeyState('R'))
	{
		planeBodyModel->SetWingFlapAngle(1.0f);
	}
	else if (GetAsyncKeyState('F'))
	{
		planeBodyModel->SetWingFlapAngle(-1.0f);
	}
	else
	{
		if (planeBodyModel->GetWingFlapAngle() > 0.0f)
		{
			planeBodyModel->SetWingFlapAngle(-1.0f);
		}
		else if (planeBodyModel->GetWingFlapAngle() < 0.0f)
		{
			planeBodyModel->SetWingFlapAngle(1.0f);
		}
	}
}

void Plane::CalculateForwardVector()
{
	planeBody->GetTransform()->GetRotation();

	planeForwardVector.x = sin((planeYawRotation / 17.425f) * (XM_PI / 180.0f));
	planeForwardVector.y = 0.0f;
	planeForwardVector.z = cos((planeYawRotation / 17.425f) * (XM_PI / 180.0f));

	//Cross product to find direction
	float planeDirectionMag = sqrt((planeForwardVector.x * planeForwardVector.x) + (planeForwardVector.y * planeForwardVector.y) + (planeForwardVector.z * planeForwardVector.z));
	//Normalise to find magnitude
	planeForwardVector = XMFLOAT3((planeForwardVector.x / planeDirectionMag), (planeForwardVector.y / planeDirectionMag), (planeForwardVector.z / planeDirectionMag));

	PlaneParticleModel* planeBodyModel = (PlaneParticleModel*)planeBody->GetParticleModel();
	planeBodyModel->SetPlaneDirection(planeForwardVector);

	// Reset Plane Rotation if over 360 or 0 degrees
	if (planeYawRotation <= -6247.0f || planeYawRotation >= 6247.0f)
	{
		planeYawRotation = 0.0f;
	}
}


void Plane::Update(float t)
{
	CalculateForwardVector();
	PlaneParticleModel* planeBodyModel = (PlaneParticleModel*)planeBody->GetParticleModel();
	float engineSpeed = planeBodyModel->GetEngineSpeed();
	float windSpeed = planeBodyModel->GetWingSpeed();

	

	// Update plane Wheels Rotation
	planeBody->GetChildren().at(0)->GetTransform()->SetRotation(engineSpeed * 50, 0.0f, 0.0f);
	planeBody->GetChildren().at(1)->GetTransform()->SetRotation(engineSpeed * 50, 0.0f, 0.0f);

	// Check Plane type --- you will want to add an AI controlled plane
	string type = planeBody->GetType();

	if (type == "Plane")
	{
		XMFLOAT3 velTemp = planeBody->GetParticleModel()->GetVelocity();
		XMFLOAT3 planeVelTemp = planeBodyModel->GetPlaneVelocity();

		velTemp.x += planeVelTemp.x;
		velTemp.y += planeVelTemp.y;
		velTemp.z += planeVelTemp.z;

		planeBodyModel->SetVelocity(velTemp);

		// Limit Wheel Rotation
		float planeFlapLimit = 0.1f;

		if (planeRearFlap <= -planeFlapLimit)
		{
			planeRearFlap = -planeFlapLimit;
		}
		else if (planeRearFlap >= planeFlapLimit)
		{
			planeRearFlap = planeFlapLimit;
		}
	}
	else
	{
		planePos = planeBody->GetTransform()->GetPosition();
		XMFLOAT3 velTemp = planeBody->GetParticleModel()->GetVelocity();

		// AI positioning
		XMFLOAT3 planeVelocity = planeBodyModel->GetPlaneVelocity();
		planePos = XMFLOAT3((planePos.x + planeVelocity.x), (planePos.y + planeVelocity.y), (planePos.z + planeVelocity.z));
		planeBody->GetTransform()->SetPosition(planePos);

		// Limit Wheel Rotation
		float planeFlapLimit = 0.1f;

		if (planeRearFlap <= -planeFlapLimit)
		{
			planeRearFlap = -planeFlapLimit;
		}
		else if (planeRearFlap >= planeFlapLimit)
		{
			planeRearFlap = planeFlapLimit;
		}
	}


	// Rotate plane in consideration to the Wheels Rotation
	if (engineSpeed > 0)
	{
		planeYawRotation += (planeRearFlap * 10000) * (windSpeed);
	}
	else if (engineSpeed < 0)
	{
		engineSpeed *= -1;
		planeYawRotation -= (planeRearFlap * 10000) * (-windSpeed);
	}




	float upVelocity = planeBody->GetParticleModel()->GetVelocity().y;
	float xVelocity = planeBody->GetParticleModel()->GetVelocity().x;
	if (upVelocity > 1.0f)
	{
		flying = true;
	}
	planePitchRotation = -upVelocity;
	//PlaneRoll Not using local coords
	planeBody->GetTransform()->SetRotation(planePitchRotation * 0.005f, planeYawRotation * planeRotationSpeed, planeRollRotation * planeRotationSpeed); //2 //3
// Update Transform
	planeBody->Update(t);
	// Update Particle Model
	planeBody->GetParticleModel()->Update(t);
}

void Plane::Draw(ID3D11DeviceContext* _pImmediateContext)
{
	planeBody->Draw(_pImmediateContext);
}
