#include "../../Header/Model/RimLightModel.h"
#include "../AppFrame/source/System/Header/Resource/ResourceServer.h"
RimLightModel::RimLightModel(std::string name, int layer, int handle) :
	ModelBase(name,layer,handle)
{
	_pixelShader = ResourceServer::LoadPixelShader("RimLightPS","Shader/PS/MV1_Rim_PS.pso");
	_vertexShader = ResourceServer::LoadVertexShader("RimLightVS","Shader/VS/MV1_VS.vso");
	_rimLightColor = CreateShaderConstantBuffer(sizeof(float) * 4);
};

RimLightModel::~RimLightModel(){
	DeleteShaderConstantBuffer(_rimLightColor);
};

void RimLightModel::SetRimColor(float r, float g, float b){
	FLOAT4* color = (FLOAT4*)GetBufferShaderConstantBuffer(_rimLightColor);
	color->x = r;
	color->y = g;
	color->z = b;
	color->w = 1.0f;
	UpdateShaderConstantBuffer(_rimLightColor);
	SetShaderConstantBuffer(_rimLightColor, DX_SHADERTYPE_PIXEL, 10);
};
