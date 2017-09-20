#include "MaterialManager.h"



MaterialManager::MaterialManager()
{
	// SkyBox Material
	skyBoxMat.ambient = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	skyBoxMat.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	skyBoxMat.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	skyBoxMat.specularPower = 10.0f;

	// Shiny Material
	shinyMaterial.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	shinyMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	shinyMaterial.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	shinyMaterial.specularPower = 10.0f;

	// No Specular Material
	noSpecMaterial.ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	noSpecMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	noSpecMaterial.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	noSpecMaterial.specularPower = 0.0f;
}


MaterialManager::~MaterialManager()
{
}
