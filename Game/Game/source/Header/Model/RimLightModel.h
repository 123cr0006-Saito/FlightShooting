#pragma once
#include "Base/ModelBase.h"
class RimLightModel : public ModelBase
{
public:
	RimLightModel(std::string name,int layer, int handle);
	virtual ~RimLightModel();
	void SetRimColor(float r,float g,float b);
protected:
	int _rimLightColor;
};

