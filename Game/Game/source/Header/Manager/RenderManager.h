#pragma once
#include "ManagerBase.h"
class RenderManager : public ManagerBase
{
public :
	RenderManager();
	~RenderManager();
	virtual bool Terminate()override;
	void AddInput(void* value)override;
	void DeleteName(std::string name)override;
	void DeleteInput(void* value)override;
	void DelAll()override;
	void Sort();

	bool UpdateInit()override;
	bool Update()override;
	bool Draw()override;
	
protected:
	std::vector<class ModelBase*> _modelList;
	std::vector<class ModelBase*> _addModelList;
	std::vector<class ModelBase*> _delModelList;
	std::vector<std::string> _delModelNameList;
};

