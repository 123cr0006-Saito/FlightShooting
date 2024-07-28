#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <utility>
#include "ManagerBase.h"

class SuperManager 
{
public:
	SuperManager();
	~SuperManager();
	bool Add(std::string name, int id,ManagerBase* manager);
	bool DeleteName(std::string);
	bool DelAll();
	ManagerBase* GetManager(std::string name);
	bool Update();
	bool Draw();

	void Sort();
	static SuperManager* GetInstance() { return _instance;}
	void Skip() { _isProcessSkip = true; }
protected:
	static SuperManager* _instance;
	bool _isSort;
	std::vector<std::tuple<std::string,int,ManagerBase*>> _superManager;
	std::vector<std::string> _delSuperManager;
	bool _isProcessSkip;
};

